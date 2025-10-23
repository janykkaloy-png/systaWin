#include <stdlib.h>
#include <stdio.h>
#include <mosquitto.h>
#include <string.h>
#include "mqtt.hpp"

static struct mosquitto *mosq;

int mqtt_create(char *host, char *uname, char *passwd)
{

   mosquitto_lib_init();

   mosq = mosquitto_new(NULL, true, NULL);
   if (!mosq)
   {
      fprintf(stderr, "Error creating Mosquitto instance\n");
      exit(EXIT_FAILURE);
   }

   mosquitto_username_pw_set(mosq, uname, passwd);

   int ret = mosquitto_connect(mosq, host, 1883, 10);
   if (ret != MOSQ_ERR_SUCCESS)
   {
      fprintf(stderr, "Error connecting to broker\n");
      exit(EXIT_FAILURE);
   }

   return 0;
}

int mqtt_loop()
{
   mosquitto_loop(mosq, 0, 1);

   return 0;
}

int mqtt_publish(const char *topic, const char *message)
{
   int ret = mosquitto_publish(mosq, NULL, topic, strlen(message), message, 0, false);
   if (ret != MOSQ_ERR_SUCCESS)
   {
      fprintf(stderr, "Error publishing message\n");
   }

   return 0;
}

int mqtt_close()
{
   mosquitto_disconnect(mosq);
   mosquitto_destroy(mosq);
   mosquitto_lib_cleanup();
   return 0;
}
