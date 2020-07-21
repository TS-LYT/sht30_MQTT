/********************************************************************************
 *      Copyright:  (C) 2020 longyongtu<longyongtu13@qq.com>
 *                  All rights reserved.
 *
 *       Filename:  temp_rpt_aly.h
 *    Description:  This head file 
 *
 *        Version:  1.0.0(17/07/20)
 *         Author:  longyongtu <longyongtu13@qq.com>
 *      ChangeLog:  1, Release initial version on "17/07/20 16:53:49"
 *                 
 ********************************************************************************/
#ifndef _MOSQUITTO_SUB_H
#define _MOSQUITTO_SUB_H

#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <libgen.h>
#include <getopt.h>
#include <string.h>
#include <mosquitto.h>
#include <stdlib.h>

#include "cJSON.h"


#define  PR_VERSION    "1.0.0"
#define  HOST          "a1qhRBGEKHE.iot-as-mqtt.cn-shanghai.aliyuncs.com" 
#define  PORT          1883
#define  USERNAME      "sht30&a1qhRBGEKHE"    
#define  PASSWD        "5950db996c7a3281dc112d2d622ab524f1baa6e4"
#define  CLIENTID      "12345|securemode=3,signmethod=hmacsha1|"
#define  SUBTOPIC      "/sys/a1iypOvJTqn/sht30/thing/service/property/set"
//#define  SUBTOPIC      "/sys/a1qhRBGEKHE/sht30/thing/event/property/post"
#define  SUBQOS        0
#define  KEEPALIVE     30

typedef struct broker_sub_s
{
    char    host[64];  
    int     port;        
    char    username[32]; 
    char    passwd[64];    
    char    clientid[64]; 
    char    subTopic[64]; 
    int     subQos;       
    int     keepalive;     
}broker_sub_t;

static void  print_usage( char *progname);
void connect_callback(struct mosquitto *mosq, void *obj, int rc);
//struct mosquitto * sub_init(struct mosquitto * mosq, broker_sub_t broker, void *buf);
int sub_init(struct mosquitto ** mosq, broker_sub_t broker, void *buf);
void printJson(cJSON * root);
void message_callback(struct mosquitto *mosq, void* obj, const struct mosquitto_message *message);

#endif

