#include "mqtt_publish.h"
#include "stdlib_noniso.h"
#include "wifi_mqtt.h"
#include "mq_7.h"
#include "dust.h"
#include "control.h"
#include "PID_control.h"
#include "state.h"
#include "Touch_sensor.h"
#include <Arduino.h>

// Definisikan semua topic untuk pengiriman data
const char* topic_status_pwm = "Esp/Status/Pwm";
const char* topic_status_relay = "Esp/Status/Relay";
const char* topic_status_pid = "Esp/Status/pid";
const char* topic_pm25 = "Esp/Pm25";
const char* topic_pm10 = "Esp/Pm10";
const char* topic_co = "Esp/co";
const char* topic_adc = "Esp/adc";

extern int touch_cond;

// Variabel untuk timer non-blocking
long lastPublishTime = 0;
const long publishInterval = 1000; 
 
void mqtt_publish() {
// Jangan lakukan apa-apa jika client tidak terhubung
  if (!client.connected()) {
    return;
 }
// Cek apakah sudah waktunya untuk mengirim data
if (millis() - lastPublishTime > publishInterval) {
  lastPublishTime = millis();
  char payload [10];
  
  itoa(pm10, payload, 10);
  client.publish(topic_pm10, payload);

  itoa(pm25, payload, 10);
  client.publish(topic_pm25, payload);

  itoa(adc_mq7, payload, 10);
  client.publish(topic_adc, payload);

  dtostrf(ppm_mq7, 4, 2, payload);
  client.publish(topic_co, payload);

  itoa(pwmValue, payload, 10);
  client.publish(topic_status_pwm, payload);

  client.publish(topic_status_relay, relayState ? "ON" : "OFF");

  dtostrf(Input, 4, 2, payload);
  client.publish("Esp/Pid/Input", payload);

  dtostrf(Output, 4, 2, payload);
  client.publish("Esp/Pid/Output", payload);


  dtostrf(Setpoint, 4, 2, payload);
  client.publish("Esp/Pid/Setpoint", payload);

  client.publish(topic_status_pid, pid_enable ? "ON" : "OFF");

  }
}
