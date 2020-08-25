/********************************************************************************
 *      Copyright:  (C) 2020 longyongtu<longyongtu13@qq.com>
 *                  All rights reserved.
 *
 *       Filename:  emqx_pub_conf.h
 *    Description:  This head file 
 *
 *        Version:  1.0.0(2020年08月12日)
 *         Author:  longyongtu <longyongtu13@qq.com>
 *      ChangeLog:  1, Release initial version on "2020年08月12日 11时07分15秒"
 *                 
 ********************************************************************************/

#ifndef  _EMQX_PUB_CONF_H
#define  _EMQX_PUB_CONF_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sht30.h"
#include "cJSON.h"
#include "iniparser.h"


#define  VERSION  "1.00" 
#define  ID       "9876"
#define  INI_PATH "../etc/emqx_pub.conf"


typedef struct broker_s
{   
    char    host[64];  
    int     port;        
    char    username[32]; 
    char    password[64];    
    char    clientid[64]; 
    char    topic[64]; 
    int     qos;       
    int     keeplive;     
    int     retain;
    char    data[1024];
}broker_t;

/*! \brief Brief function description here
 *
 * get temperature and humity in cjson
 *
 * \param  broker_t point
 * \return char *, data you public
 */
char *get_temp_hnti_cjson(broker_t *broker);
char *get_temp_hnti_cjson1(broker_t *broker);

/*! \brief Brief function description here
 *
 *  set some opt of struct broker_t using in connecat with broker  
 *
 * \param  Parameter description
 * \return 
 */
void broker_conf(broker_t *broker);
void broker_conf1(broker_t *broker);
    

#endif
