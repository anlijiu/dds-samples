#include "dds/dds.h"
#include "PubSub.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <assert.h>

#define MAX_SAMPLES 1000

static unsigned long long outOfOrder = 0;
static unsigned long long total_bytes = 0;
static unsigned long long total_samples = 0;

static unsigned long payloadSize = 0;

static PubSubModule_DataType data [MAX_SAMPLES];
static void * samples[MAX_SAMPLES];

static dds_entity_t waitSet;

static volatile sig_atomic_t done = false;

static void data_available_handler (dds_entity_t reader, void *arg);
static void process_samples(dds_entity_t reader);
static dds_entity_t prepare_dds(dds_entity_t *reader, const char *partitionName);
static void finalize_dds(dds_entity_t participant);

static void sigint (int sig)
{
  (void) sig;
  done = true;
}

int main (int argc, char **argv)
{
  char *partitionName = "PubSub example";

  dds_entity_t participant;
  dds_entity_t reader;

  printf ("Partition: %s\n", partitionName);
  fflush (stdout);

  participant = prepare_dds(&reader, partitionName);

  printf ("=== [Subscriber] Waiting for samples...\n");
  fflush (stdout);

  signal (SIGINT, sigint);
  process_samples(reader);

  (void) dds_set_status_mask (reader, 0);
  finalize_dds (participant);
  return EXIT_SUCCESS;
}

static int do_take (dds_entity_t reader)
{
  int samples_received;
  dds_sample_info_t *info = NULL;
  dds_instance_handle_t ph = 0;

  info = dds_alloc (sizeof(dds_sample_info_t) * MAX_SAMPLES);

  samples_received = dds_take (reader, samples, info, MAX_SAMPLES, MAX_SAMPLES);
  if (samples_received < 0)
  {
    dds_free( info );
    DDS_FATAL("dds_take: %s\n", dds_strretcode(-samples_received));
  }

  for (int i = 0; !done && i < samples_received; i++)
  {
    if (info[i].valid_data)
    {
      ph = info[i].publication_handle;
      PubSubModule_DataType * this_sample = &data[i];
      printf("receive count: %lu, payload: %s\n", this_sample->count, this_sample->payload._buffer);
    }
  }
  dds_free (info);
  return samples_received;
}

static void data_available_handler (dds_entity_t reader, void *arg)
{
  (void)arg;
  (void) do_take (reader);
}

static void process_samples(dds_entity_t reader)
{
  dds_return_t status;
  unsigned long long prev_bytes = 0;
  unsigned long long prev_samples = 0;
  dds_attach_t wsresults[2];

  while (!done)
  {
      status = dds_waitset_wait (waitSet, wsresults, sizeof(wsresults)/sizeof(wsresults[0]), DDS_MSECS(100));
      if (status < 0)
        DDS_FATAL("dds_waitset_wait: %s\n", dds_strretcode(-status));
  }

  fflush (stdout);
}

static dds_entity_t prepare_dds(dds_entity_t *reader, const char *partitionName)
{
  dds_return_t status;
  dds_entity_t topic;
  dds_entity_t subscriber;
  dds_listener_t *rd_listener;
  dds_entity_t participant;

  int32_t maxSamples = 4000;
  const char *subParts[1];
  dds_qos_t *subQos = dds_create_qos ();
  dds_qos_t *drQos = dds_create_qos ();

  /* A Participant is created for the default domain. */

  participant = dds_create_participant (DDS_DOMAIN_DEFAULT, NULL, NULL);
  if (participant < 0)
    DDS_FATAL("dds_create_participant: %s\n", dds_strretcode(-participant));

  /* A Topic is created for our sample type on the domain participant. */

  topic = dds_create_topic (participant, &PubSubModule_DataType_desc, "PubSub", NULL, NULL);
  if (topic < 0)
    DDS_FATAL("dds_create_topic: %s\n", dds_strretcode(-topic));

  /* A Subscriber is created on the domain participant. */

  subParts[0] = partitionName;
  dds_qset_partition (subQos, 1, subParts);
  subscriber = dds_create_subscriber (participant, subQos, NULL);
  if (subscriber < 0)
    DDS_FATAL("dds_create_subscriber: %s\n", dds_strretcode(-subscriber));
  dds_delete_qos (subQos);

  /* A Reader is created on the Subscriber & Topic with a modified Qos. */

  dds_qset_reliability (drQos, DDS_RELIABILITY_RELIABLE, DDS_SECS (10));
  dds_qset_history (drQos, DDS_HISTORY_KEEP_ALL, 0);
  dds_qset_resource_limits (drQos, maxSamples, DDS_LENGTH_UNLIMITED, DDS_LENGTH_UNLIMITED);

  rd_listener = dds_create_listener(NULL);
  dds_lset_data_available(rd_listener, data_available_handler);

  /* A Read Condition is created which is triggered when data is available to read */
  waitSet = dds_create_waitset (participant);
  if (waitSet < 0)
    DDS_FATAL("dds_create_waitset: %s\n", dds_strretcode(-waitSet));

  status = dds_waitset_attach (waitSet, waitSet, waitSet);
  if (status < 0)
    DDS_FATAL("dds_waitset_attach: %s\n", dds_strretcode(-status));


  memset (data, 0, sizeof (data));
  for (unsigned int i = 0; i < MAX_SAMPLES; i++)
  {
    samples[i] = &data[i];
  }

  *reader = dds_create_reader (subscriber, topic, drQos, rd_listener);
  if (*reader < 0)
    DDS_FATAL("dds_create_reader: %s\n", dds_strretcode(-*reader));

  dds_delete_qos (drQos);
  dds_delete_listener(rd_listener);

  return participant;
}

static void finalize_dds(dds_entity_t participant)
{
  dds_return_t status;

  for (unsigned int i = 0; i < MAX_SAMPLES; i++)
  {
    PubSubModule_DataType_free (&data[i], DDS_FREE_CONTENTS);
  }

  status = dds_waitset_detach (waitSet, waitSet);
  if (status < 0)
    DDS_FATAL("dds_waitset_detach: %s\n", dds_strretcode(-status));
  status = dds_delete (waitSet);
  if (status < 0)
    DDS_FATAL("dds_delete: %s\n", dds_strretcode(-status));
  status = dds_delete (participant);
  if (status < 0)
    DDS_FATAL("dds_delete: %s\n", dds_strretcode(-status));
}
