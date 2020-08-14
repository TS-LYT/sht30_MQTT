/********************************************************************************
 *      Copyright:  (C) 2020 longyongtu<longyongtu13@qq.com>
 *                  All rights reserved.
 *
 *       Filename:  mosquitto_sub.h
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
#include "emqx_sub_conf.h"


void connect_callback(struct mosquitto *mosq, void *obj, int rc);

int sub_init(struct mosquitto ** mosq, broker_t broker, void *buf);

void message_callback(struct mosquitto *mosq, void* obj, const struct mosquitto_message *message);

#endif

