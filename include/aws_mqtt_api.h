/**
 * @file aws_mqtt_api.h
 * @brief Header file for aws_mqtt_api.cpp
 *
 * AWS MQTT API contains functions related to the connection to aws
 * iot core using the mqtt protocol with an additional WiFi connection
 * function
 *
 * @author Juan Neyra
 * @date September 4th 2021
 *
 */
#ifndef AWS_MQTT_SERVER
#define AWS_MQTT_SERVER

#include "Arduino.h"
#include <WiFi.h>
#include <MQTT.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include "sd_utils.h"

extern const char server_sd_file_pem_start[]   asm("_binary_credentials_server_sd_file_pem_start");
extern const char server_sd_file_pem_end[]   asm("_binary_credentials_server_sd_file_pem_end");

#define DEVICE_NAME "anim5"
#define WEB_SERVER "undergroundcuy.com"
#define WEB_PORT 443
#define WEB_URI "/anim5_stack/sd_file/"
#define DEVICE_TOKEN "0k25YEmxDU-xOHRqR6VZH-b0bZmnvCRt"
#define TOPIC_SD_FILE "sd_file"


/**
 * @brief Configures MQTT connection to a secure MQTT Server
 *
 * Given the server, the port and the certificates for connection,
 * a connection to the MQTT server is established
 *
 * @param server Name of mqtt server
 * @param port Name of mqtt port
 * @param iot_cert_pem CA certificate to mqtt server (for AWS Amazon Root CA)
 * @param iot_client_cert_pem Client certificate to mqtt server
 * @param iot_key_pem Key certificate to mqtt server
 *
 */
void configureMqttConnection(const char* server, const uint16_t port,
      const char* iot_cert_pem, const char* iot_client_cert_pem, 
      const char* iot_key_pem);

/**
 * @brief Callback function that executes when message arrives at topic
 *
 * Executes when a message arrives to a subscribed topic. It currently
 * supports topic sd_file
 *
 * @param topic Name of topic
 * @param payload Message
 *
 */
void callback(String &topic, String &payload);

/**
 * @brief Checks connection to WiFi and reconnects if necessary
 *
 * Checks WiFi connection. If the status isn't connected, it retries
 * the connection every 1 second
 *
 */
void connectToWiFi();

/**
 * @brief Reconnects to MQTT Server
 *
 * Checks if connection to mqtt server is on. If it's disconnected,
 * first validates WiFi connection, then reconnects to MQTT server
 *
 */
void reconnect();

/**
 * @brief Reconnects to MQTT Server in a non-blocking way
 *
 * Reconnects to MQTT server when client is disconnected and 
 * the time passed is greater than REC_INTERVAL time, otherwise
 * it keeps the connection going
 *
 */
void non_blocking_check_connection();

/**
 * @brief Publish info on SD card
 *
 * Publishes information on storage to topic m5_sd_info
 *
 */
void publish_sd_info();

#endif