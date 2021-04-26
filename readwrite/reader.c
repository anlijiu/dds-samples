#include "dds/dds.h"
#include "ReadWriteData.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* 一次性读取数据包最大数量， 如果设置为1 ，则每次只能读取到第一个数据包 */
#define MAX_SAMPLES 3

int main (int argc, char ** argv)
{
  dds_entity_t participant;
  dds_entity_t topic;
  dds_entity_t reader;
  ReadWriteData_Msg *msg;
  void *samples[MAX_SAMPLES];
  dds_sample_info_t infos[MAX_SAMPLES];
  dds_return_t rc;
  dds_qos_t *qos;
  (void)argc;
  (void)argv;

  /**
   * 创建 Domain Participant. DDS_DOMAIN_DEFAULT为该域唯一标识符
   * publisher 也需使用同样的域标识符， 才可以与Subscriber通讯。
   *
   * 用不同的域标识符建立的域彼此不可见，自然也无法通讯。
   */
  participant = dds_create_participant (DDS_DOMAIN_DEFAULT, NULL, NULL);
  if (participant < 0)
    DDS_FATAL("dds_create_participant: %s\n", dds_strretcode(-participant));

  /* 创建topic . */
  topic = dds_create_topic (
    participant, &ReadWriteData_Msg_desc, "ReadWriteData_Msg", NULL, NULL);
  if (topic < 0)
    DDS_FATAL("dds_create_topic: %s\n", dds_strretcode(-topic));

  /* 设置qos */
  qos = dds_create_qos ();

  /* 第二个参数 DDS_RELIABILITY_RELIABLE 为最大保证可靠性
   * 读不到就一直读， 一直到超时
   * 第三个参数为最大block 时间， 这里是 10秒超时*/
  // dds_qset_reliability (qos, DDS_RELIABILITY_RELIABLE, DDS_SECS (10));
  dds_qset_reliability (qos, DDS_RELIABILITY_BEST_EFFORT, DDS_SECS (10));

  /**
   * DDS_HISTORY_KEEP_ALL为保存所有的历史, 如果设置了的话
   * 接下来将会看到读取两轮，每次都会读取到 publisher 发送的所有数据包
   * 第三个参数为保存深度， 仅对DDS_HISTORY_KEEP_LAST有效
   * 设置为1 ，则仅保留最后一个
   */
  // dds_qset_history (qos, DDS_HISTORY_KEEP_ALL, 0);
  dds_qset_history (qos, DDS_HISTORY_KEEP_LAST, 1);

  int32_t maxSamples = 4000;

  /**
   * 第二个参数是如果保存历史的话，最多能保存多少样本.
   * 第三个参数是最多有多少个instance, 这个instance就是用key来区分，
   *     对于我们的这个例子， 就是userID, 最多有多少个userID。如果在idl
   *     中没有定义key的话， 那么这个topic就是只有一个instance.
   *     single instance.
   * 第四个参数是对于每个instance 最多保存多少个样本.
   *
   * 例如下面这样子的话
   * dds_qset_resource_limits(qos, maxSamples, 2, 1);
   * 就是创建2个instance， 每个instance上只保留1个样本
   */
  dds_qset_resource_limits(qos, maxSamples, 2, 1);
  // dds_qset_resource_limits (qos, maxSamples, DDS_LENGTH_UNLIMITED, DDS_LENGTH_UNLIMITED);

  /* 建立针对topic: ReadWriteData_Msg 的 DataReader */
  reader = dds_create_reader (participant, topic, qos, NULL);
  if (reader < 0)
    DDS_FATAL("dds_create_reader: %s\n", dds_strretcode(-reader));
  dds_delete_qos(qos);

  printf ("\n=== [Subscriber] Waiting for a sample ...\n");
  fflush (stdout);

  /* 被封装的初始化动作 */
  samples[0] = ReadWriteData_Msg__alloc ();
  samples[1] = ReadWriteData_Msg__alloc ();
  samples[2] = ReadWriteData_Msg__alloc ();


  //这个例子里读取两遍， 第二遍读取到的是本地的历史数据
  //可以设置 i=200 读取200遍，中途退出publisher ，
  //subscriber仍然可以读取到数据，就是dds中的本地cache
  int i = 2;
  /* Poll until data has been read. */
  while (i > 0)
  {
    // printf("i:%d\n", i);
    /**
     * dds_read 读取(get) reader中的数据，非阻塞  这个函数会立即返回。
     * 具体参数和返回值看头文件
     */
    rc = dds_read (reader, samples, infos, MAX_SAMPLES, MAX_SAMPLES);
    if (rc < 0) {
      DDS_FATAL("dds_read: %s\n", dds_strretcode(-rc));
      printf("dds_read rc:%s\n", dds_strretcode(-rc));
    }

    /* Check if we read some data and it is valid. */
    if ((rc > 0) && (infos[0].valid_data))
    {
      /* Print Message. */
      msg = (ReadWriteData_Msg*) samples[0];
      printf ("=== [Subscriber] total: %d\n", rc);
      printf ("=== [Subscriber] Received : ");
      printf ("Message (%"PRId32", %s)\n", msg->userID, msg->message);
      msg = (ReadWriteData_Msg*) samples[1];
      printf ("=== [Subscriber] Received : ");
      printf ("Message (%"PRId32", %s)\n", msg->userID, msg->message);

      /**
       * 这里我们没有检查读取total是多少
       * 即使只读取2个有效，也仍然打印一下第三个
       */
      msg = (ReadWriteData_Msg*) samples[2];
      printf ("=== [Subscriber] Received : ");
      printf ("Message (%"PRId32", %s)\n", msg->userID, msg->message);
      fflush (stdout);
      --i;//读取成功一次
      memset(samples, 0, sizeof(samples));//清空samples
      // break;
    }
    else
    {
      /* 没读取到有效数据， 睡20ms */
      dds_sleepfor (DDS_MSECS (20));
    }
  }

  /* 释放一下. */
  ReadWriteData_Msg_free (samples[0], DDS_FREE_ALL);
  ReadWriteData_Msg_free (samples[1], DDS_FREE_ALL);
  ReadWriteData_Msg_free (samples[2], DDS_FREE_ALL);

  /* 离开虚拟全局空间 */
  rc = dds_delete (participant);
  if (rc != DDS_RETCODE_OK)
    DDS_FATAL("dds_delete: %s\n", dds_strretcode(-rc));

  return EXIT_SUCCESS;
}
