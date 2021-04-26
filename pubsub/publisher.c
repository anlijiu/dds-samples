#include "dds/dds.h"
#include "PubSub.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>


#define MAX_SAMPLES 100

static bool done = false;

static dds_return_t wait_for_reader(dds_entity_t writer, dds_entity_t participant);
static void start_writing(dds_entity_t writer, PubSubModule_DataType *sample,int burstInterval, uint32_t burstSize, int timeOut);
static dds_entity_t prepare_dds(dds_entity_t *writer, const char *partitionName);
static void finalize_dds(dds_entity_t participant, dds_entity_t writer, PubSubModule_DataType sample);

static void sigint (int sig)
{
  (void)sig;
  done = true;
}

int main (int argc, char **argv)
{
  uint32_t payloadSize = 8;
  int burstInterval = 0;
  uint32_t burstSize = 1;
  int timeOut = 0;
  char * partitionName = "PubSub example";
  dds_entity_t participant;
  dds_entity_t writer;
  dds_return_t rc;
  PubSubModule_DataType sample;

  participant = prepare_dds(&writer, partitionName);

  /* Wait until have a reader */
  if (wait_for_reader(writer, participant) == 0) {
    printf ("=== [Publisher]  Did not discover a reader.\n");
    fflush (stdout);
    rc = dds_delete (participant);
    if (rc < 0)
      DDS_FATAL("dds_delete: %s\n", dds_strretcode(-rc));
    return EXIT_FAILURE;
  }

  sample.count = 0;
  //分配 bytes内存
  sample.payload._buffer = dds_alloc (payloadSize);
  sample.payload._length = payloadSize;
  sample.payload._release = true;
  for (uint32_t i = 0; i < payloadSize; i++) {
    sample.payload._buffer[i] = 'a';
  }

  /* handle for Ctrl-C */
  signal (SIGINT, sigint);

  /* Register the sample instance and write samples repeatedly or until time out */
  start_writing(writer, &sample, burstInterval, burstSize, timeOut);

  /* Cleanup */
  finalize_dds(participant, writer, sample);
  return EXIT_SUCCESS;
}

/**
 * 创建DP 和 writer
 */
static dds_entity_t prepare_dds(dds_entity_t *writer, const char *partitionName)
{
  dds_entity_t participant;
  dds_entity_t topic;
  dds_entity_t publisher;
  const char *pubParts[1];
  dds_qos_t *pubQos;
  dds_qos_t *dwQos;

  /* 创建DP */
  participant = dds_create_participant (DDS_DOMAIN_DEFAULT, NULL, NULL);
  if (participant < 0)
    DDS_FATAL("dds_create_participant: %s\n", dds_strretcode(-participant));

  /* 创建topic */
  topic = dds_create_topic (participant, &PubSubModule_DataType_desc, "PubSub", NULL, NULL);
  if (topic < 0)
    DDS_FATAL("dds_create_topic: %s\n", dds_strretcode(-topic));

  /* publisher qos*/
  pubQos = dds_create_qos ();
  pubParts[0] = partitionName;

  /**
   * partition 是另一种控制DataWriter与DataReader 能否配对的方式
   * 当topic 相同，qos也兼容， 但是还想隔离的时候（通常发生在相同topic有多个DataWriter和DataReader）
   */
  dds_qset_partition (pubQos, 1, pubParts);

  /**
   * 虽然可以通过DP直接创建writer， 但是也可以先创建publisher，
   * 再通过publisher 创建DataWriter
   */
  publisher = dds_create_publisher (participant, pubQos, NULL);
  if (publisher < 0)
    DDS_FATAL("dds_create_publisher: %s\n", dds_strretcode(-publisher));
  dds_delete_qos (pubQos);

  /**
   * 可靠性
   * DDS_RELIABILITY_RELIABLE: 可靠
   * DDS_RELIABILITY_BEST_EFFORT: 尽量
   */
  dwQos = dds_create_qos ();
  dds_qset_reliability (dwQos, DDS_RELIABILITY_RELIABLE, DDS_SECS (10));

  /**
   * 保存所有历史，cache大小由下一句MAX_SAMPLES指定
   */
  dds_qset_history (dwQos, DDS_HISTORY_KEEP_ALL, 0);

  /**
   * 见readwrite对dds_qset_resource_limits的解释吧
   */
  dds_qset_resource_limits (dwQos, MAX_SAMPLES, DDS_LENGTH_UNLIMITED, DDS_LENGTH_UNLIMITED);


  /**
   * 根据publisher 和 topic 创建 DataWriter
   */
  *writer = dds_create_writer (publisher, topic, dwQos, NULL);
  if (*writer < 0)
    DDS_FATAL("dds_create_writer: %s\n", dds_strretcode(-*writer));
  dds_delete_qos (dwQos);

  return participant;
}

static dds_return_t wait_for_reader(dds_entity_t writer, dds_entity_t participant)
{
  printf ("\n=== [Publisher]  Waiting for a reader ...\n");
  fflush (stdout);

  dds_return_t rc;
  dds_entity_t waitset;

  rc = dds_set_status_mask(writer, DDS_PUBLICATION_MATCHED_STATUS);
  if (rc < 0)
    DDS_FATAL("dds_set_status_mask: %s\n", dds_strretcode(-rc));

  waitset = dds_create_waitset(participant);
  if (waitset < 0)
    DDS_FATAL("dds_create_waitset: %s\n", dds_strretcode(-waitset));

  rc = dds_waitset_attach(waitset, writer, (dds_attach_t)NULL);
  if (rc < 0)
    DDS_FATAL("dds_waitset_attach: %s\n", dds_strretcode(-rc));

  /* 此处阻塞 ， 直到有match 的 DataReader */
  rc = dds_waitset_wait(waitset, NULL, 0, DDS_SECS(30));
  if (rc < 0)
    DDS_FATAL("dds_waitset_wait: %s\n", dds_strretcode(-rc));

  return rc;
}

static void start_writing(
    dds_entity_t writer,
    PubSubModule_DataType *sample,
    int burstInterval,
    uint32_t burstSize,
    int timeOut)
{
  bool timedOut = false;
  dds_time_t pubStart = dds_time ();
  dds_time_t now;
  dds_time_t deltaTv;
  dds_return_t status;

  if (!done)
  {
    dds_time_t burstStart = pubStart;

    printf ("=== [Publisher]  Writing samples...\n");
    fflush (stdout);

    int i = 100;
    while (!done && !timedOut && i > 0)
    {
      status = dds_write (writer, sample);
      if (status == DDS_RETCODE_TIMEOUT) {
        timedOut = true;
        printf("timeout\n");
      } else if (status < 0) {
        DDS_FATAL("dds_write: %s\n", dds_strretcode(-status));
      } else {
        sample->count++;
        i--;
      }
    }

    printf ("=== [Publisher] %llu samples written.\n", (unsigned long long) sample->count);
    fflush (stdout);
  }
}

static void finalize_dds(dds_entity_t participant, dds_entity_t writer, PubSubModule_DataType sample)
{
  dds_return_t status = dds_dispose (writer, &sample);
  if (status != DDS_RETCODE_TIMEOUT && status < 0)
    DDS_FATAL("dds_dispose: %s\n", dds_strretcode(-status));

  //释放bytes
  dds_free (sample.payload._buffer);
  //退出虚拟全局空间
  status = dds_delete (participant);
  if (status < 0)
    DDS_FATAL("dds_delete: %s\n", dds_strretcode(-status));
}
