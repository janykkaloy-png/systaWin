#include <unistd.h>
#include <stdlib.h>
#include "udp.hpp"
#include "SystaCompact_II.hpp"
#include "mqtt.hpp"

static ssize_t process(char *buffer, ssize_t len);

static Systa *pSysta;

/*!
 *
 */
int main(int argc, char *argv[])
{

   char *mqtt_host = getenv("MQTT_HOST");
   char *mqqt_user = nullptr;
   char *mqqt_passwd = nullptr;
   if (argc > 1)
   {
      mqtt_host = argv[1];
      fprintf(stderr, "Using host %s\n", mqtt_host);
   }
   if (argc > 2)
   {
      mqqt_user = argv[2];
      fprintf(stderr, "Using user %s\n", mqqt_user);
   }
   if (argc > 3)
   {
      mqqt_passwd = argv[3];
      fprintf(stderr, "Using passwd %s\n", mqqt_passwd);
   }

   pSysta = new SystaCompactII();

   udp_create();
   mqtt_create(mqtt_host, mqqt_user, mqqt_passwd);

   while (1)
   {
      udp_receive(&process);
      mqtt_loop();

      usleep(10000); // reduce cpu load
   }

   udp_close();
   mqtt_close();

   return 0;
}

void new_value(const SystaValue &value)
{
   const std::string topic = "systa/" + value.get_name();
   const std::string payload = value.to_string();

   mqtt_publish(topic.c_str(), payload.c_str());
}

/*!
 *
 */
static ssize_t process(char *buffer, ssize_t len)
{
   struct ReceivePacket *pPacket = (struct ReceivePacket *)buffer;

   pSysta->parse_values(pPacket, new_value);

   // TODO generate response packet

   return 0;
}
