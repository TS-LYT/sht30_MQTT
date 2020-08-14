/********************************************************************************
 *      Copyright:  (C) 2020 longyongtu<longyongtu13@qq.com>
 *                  All rights reserved.
 *
 *       Filename:  mosquitto_pub.h
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
#include "iniparser.h"
#include "emqx_pub_conf.h"


void callback(struct mosquitto *mosq, void *obj, int rc);


int pub_init(struct mosquitto ** mosq, broker_t broker, void *buf);

#endif

