#ifndef MAIN
#define MAIN

#include <M5Stack.h>
#include <WiFi.h>
#include "secrets.h"
#include "anim5_server.h"
#include "sd_m5_image_manager.h"
#include "aws_mqtt_api.h"

#define AWS_SERVER

extern const char iot_cert_pem_start[]   asm("_binary_credentials_anim5stack_AmazonRootCA1_pem_start");
extern const char iot_cert_pem_end[]   asm("_binary_credentials_anim5stack_AmazonRootCA1_pem_end");

extern const char iot_client_cert_pem_start[]   asm("_binary_credentials_anim5stack_anim5_certificate_pem_crt_start");
extern const char iot_client_cert_pem_end[]   asm("_binary_credentials_anim5stack_anim5_certificate_pem_crt_end");

extern const char iot_key_pem_start[]   asm("_binary_credentials_anim5stack_anim5_private_pem_key_start");
extern const char iot_key_pem_end[]   asm("_binary_credentials_anim5stack_anim5_private_pem_key_end");


#endif