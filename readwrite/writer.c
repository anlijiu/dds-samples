#include "dds/dds.h"
#include "ReadWriteData.h"
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char ** argv)
{
  dds_entity_t participant;
  dds_entity_t topic;
  dds_entity_t writer;
  dds_return_t rc;
  ReadWriteData_Msg msg;
  uint32_t status = 0;
  (void)argc;
  (void)argv;

  /**
   * 创建 Domain Participant. DDS_DOMAIN_DEFAULT为该域唯一标识符
   * subscriber 也需使用同样的域标识符， 才可以与publisher通讯。
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

  /* 创建DataWriter. */
  writer = dds_create_writer (participant, topic, NULL, NULL);
  if (writer < 0)
    DDS_FATAL("dds_create_writer: %s\n", dds_strretcode(-writer));

  printf("=== [Publisher]  Waiting for a reader to be discovered ...\n");
  fflush (stdout);

  /**
   * 设置mask, DDS_PUBLICATION_MATCHED_STATUS 表示 writer 和 reader 
   * 需要拥有相同的topic  和兼容的qos
   */
  rc = dds_set_status_mask(writer, DDS_PUBLICATION_MATCHED_STATUS);
  if (rc != DDS_RETCODE_OK)
    DDS_FATAL("dds_set_status_mask: %s\n", dds_strretcode(-rc));

  /* 根据上面的mask设置， 如果不满足就轮询到满足为止 */
  while(!(status & DDS_PUBLICATION_MATCHED_STATUS))
  {
    rc = dds_get_status_changes (writer, &status);
    if (rc != DDS_RETCODE_OK)
      DDS_FATAL("dds_get_status_changes: %s\n", dds_strretcode(-rc));

    /* 睡20毫秒. */
    dds_sleepfor (DDS_MSECS (20));
  }

  /* 创建一个数据包 */
  msg.userID = 1;
  msg.message = "Hello World";

  printf ("=== [Publisher]  Writing : ");
  printf ("Message (%"PRId32", %s)\n", msg.userID, msg.message);
  fflush (stdout);

  /**
   * 写入全局虚拟空间  到目前我的了解是这个函数是个同步调用
   * 如果我理解错了请立刻纠正我， 十分感谢!
   */
  rc = dds_write (writer, &msg);
  if (rc != DDS_RETCODE_OK)
    DDS_FATAL("dds_write: %s\n", dds_strretcode(-rc));


  msg.userID = 2;
  msg.message = "another Hello World";

  printf ("=== [Publisher]  Writing : ");
  printf ("Message (%"PRId32", %s)\n", msg.userID, msg.message);
  fflush (stdout);
  rc = dds_write (writer, &msg);
  if (rc != DDS_RETCODE_OK) {
    DDS_FATAL("dds_write: %s\n", dds_strretcode(-rc));
  }

  /* 对userID 1 再写入一个数据包*/
  msg.message = "Hello World - 1";
  printf ("=== [Publisher]  Writing : ");
  printf ("Message (%"PRId32", %s)\n", msg.userID, msg.message);
  fflush (stdout);
  rc = dds_write (writer, &msg);
  if (rc != DDS_RETCODE_OK)
    DDS_FATAL("dds_write: %s\n", dds_strretcode(-rc));

  /**
   * 删除DP， 退出全局虚拟空间
   */
  rc = dds_delete (participant);
  if (rc != DDS_RETCODE_OK)
    DDS_FATAL("dds_delete: %s\n", dds_strretcode(-rc));

  return EXIT_SUCCESS;
}
