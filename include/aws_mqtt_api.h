#ifndef AWS_MQTT_SERVER
#define AWS_MQTT_SERVER

#include "Arduino.h"
#include <WiFi.h>
#include <MQTT.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
//#include <esp_http_client.h>
#include "sd_utils.h"


void configureMqttConnection(const char* server, uint16_t port,
      const char* iot_cert_pem, const char* iot_client_cert_pem, 
      const char* iot_key_pem);
void callback(char* topic, byte* message, unsigned int length);
void connectToWiFi();
void reconnect();
void non_blocking_check_connection();
void publish_sd_info();
void merge_files(String filename, String num_part);
void save_part(String filename, String num_part, 
                  String part, uint16_t total_parts);

#endif