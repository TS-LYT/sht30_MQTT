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
#ifndef _MOSQUITTO_PUB_H
#define _MOSQUITTO_PUB_H

#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <libgen.h>
#include <getopt.h>
#include <string.h>
#include <mosquitto.h>
#include <stdlib.h>

#include "sht30.h"
#include "cJSON.h"


#define  PR_VERSION    "1.0.0"
//#define  HOST          "a1qhRBGEKHE.iot-as-mqtt.cn-shanghai.aliyuncs.com" 
#define  HOST          "192.168.240.134" 
#define  PORT          1883
#define  USERNAME      "sht30&a1qhRBGEKHE"    
#define  PASSWD        "5950db996c7a3281dc112d2d622ab524f1baa6e4"
//#define  CLIENTID      "12345|securemode=3,signmethod=hmacsha1|"
#define  CLIENTID      "12345"
//#define  PUBTOPIC      "/sys/a1qhRBGEKHE/sht30/thing/event/property/post"
//#define  PUBTOPIC      "/a1qhRBGEKHE/sht30/user/update"
//#define  PUBTOPIC      "/a1qhRBGEKHE/sht30/user/test_topic"
#define  PUBTOPIC      "lyt"
#define  PUBQOS        1
#define  KEEPALIVE     60
#define  METHOD        "thing.event.property.post"
#define  ID            "232424235324"
#define  VERSION       "1.0.0"

typedef struct broker_s
{
    char    host[64];  
    int     port;        
    char    username[32]; 
    char    passwd[64];    
    char    clientid[64]; 
    char    pubTopic[64]; 
    int     pubQos;       
    int     keepalive;     
    char    method[32];
    char    id[16];
    char    version[16];
}broker_t;

char *get_temp_hmti_cjson(broker_t *broker);
static void  print_usage( char *progname);
void callback(struct mosquitto *mosq, void *obj, int rc);
int pub_init(struct mosquitto ** mosq, broker_t broker, void *buf);

#endif

