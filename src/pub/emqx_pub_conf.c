/*********************************************************************************
 *      Copyright:  (C) 2020 longyongtu<longyongtu13@qq.com>
 *                  All rights reserved.
 *
 *       Filename:  emqx_pub_conf.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(2020年08月12日)
 *         Author:  longyongtu <longyongtu13@qq.com>
 *      ChangeLog:  1, Release initial version on "2020年08月12日 11时07分03秒"
 *                 
 ********************************************************************************/

#include "emqx_pub_conf.h"

char *get_temp_hmti_cjson(broker_t *broker)
{
    float           temp = 31.3;
    float           hmti = 8;
    cJSON           *root = cJSON_CreateObject();
    cJSON           *params = cJSON_CreateObject();
    char            *data = NULL;
    int             rv = 0;

#if 0
    rv=get_temp_hmti(I2C_ADDR, SHT_ADDR, &temp, &hmti);
    if(rv < 0) 
    {
        return NULL;
    }
#endif


    cJSON_AddItemToObject(root, "method", cJSON_CreateString(broker->topic));
    cJSON_AddItemToObject(root, "id", cJSON_CreateString(ID));
    //cJSON_AddItemToObject(params, "CurrentHumidity", cJSON_CreateNumber((int)hmti));
    //cJSON_AddItemToObject(params, "CurrentHumidity", cJSON_CreateNumber(34));
    //cJSON_AddItemToObject(params, "CurrentTemperature", cJSON_CreateNumber(temp));
    cJSON_AddItemToObject(params, "CurrentTemperature", cJSON_CreateNumber(66));
    cJSON_AddItemToObject(root, "params", params);
    cJSON_AddItemToObject(root, "version", cJSON_CreateString(VERSION));

    data=cJSON_Print(root);

    /* data must free after you aren't use it */
    printf("%s\n", data);
    return data;
}



char *get_temp_hmti_cjson1(broker_t *broker)
{
    float           temp = 31.3;
    float           hmti = 8;
    cJSON           *root = cJSON_CreateObject();
    cJSON           *params = cJSON_CreateObject();
    char            *data = NULL;
    int             rv = 0;

#if 0
    rv=get_temp_hmti(I2C_ADDR, SHT_ADDR, &temp, &hmti);
    if(rv < 0) 
    {
        return NULL;
    }
#endif


    cJSON_AddItemToObject(root, "method", cJSON_CreateString(broker->topic));
    cJSON_AddItemToObject(root, "id", cJSON_CreateString(ID));
    //cJSON_AddItemToObject(params, "CurrentHumidity", cJSON_CreateNumber((int)hmti));
    cJSON_AddItemToObject(params, "CurrentHumidity", cJSON_CreateNumber(34));
    //cJSON_AddItemToObject(params, "CurrentTemperature", cJSON_CreateNumber(temp));
    //cJSON_AddItemToObject(params, "CurrentTemperature", cJSON_CreateNumber(66));
    cJSON_AddItemToObject(root, "params", params);
    cJSON_AddItemToObject(root, "version", cJSON_CreateString(VERSION));

    data=cJSON_Print(root);

    /* data must free after you aren't use it */
    printf("%s\n", data);
    return data;
}

void broker_conf(broker_t *broker)
{
    dictionary *ini = NULL;
    char       *data = NULL;
    
    ini=iniparser_load(INI_PATH);
    if(ini == NULL)
    {
        printf("no such file %s\n", INI_PATH);
        return ;
    }

    strncpy(broker->host, iniparser_getstring(ini, "ip_pool:ip1", "null"), sizeof(broker->host));
    printf("%s\n", broker->host);
    broker->port = atoi(iniparser_getstring(ini, "port_pool:port0", "null"));
    printf("%d\n", broker->port);
    strncpy(broker->username, iniparser_getstring(ini, "username:username1", "null"), sizeof(broker->username));
    printf("%s\n", broker->username);
    strncpy(broker->password, iniparser_getstring(ini, "password:password1", "null"), sizeof(broker->password));
    printf("%s\n", broker->password);
    strncpy(broker->clientid, iniparser_getstring(ini, "clientid:clientid1", "null"), sizeof(broker->clientid));
    printf("%s\n", broker->clientid);
    strncpy(broker->topic, iniparser_getstring(ini, "topic:topic1", "null"), sizeof(broker->topic));
    printf("%s\n", broker->topic);
    broker->qos = atoi(iniparser_getstring(ini, "qos:qos0", "null"));
    printf("%d\n", broker->qos);
    broker->keeplive = atoi(iniparser_getstring(ini, "keeplive:keeplive0", "null"));
    printf("%d\n", broker->keeplive);
    broker->retain = atoi(iniparser_getstring(ini, "retain:retain0", "null"));
    printf("%d\n", broker->retain);

    data=get_temp_hmti_cjson(broker);
    strncpy(broker->data, data, sizeof(broker->data));
    free(data);

    iniparser_freedict(ini);

    return;
}


void broker_conf1(broker_t *broker)
{
    dictionary *ini = NULL;
    char       *data = NULL;
    
    ini=iniparser_load(INI_PATH);
    if(ini == NULL)
    {
        printf("no such file %s\n", INI_PATH);
        return ;
    }

    strncpy(broker->host, iniparser_getstring(ini, "ip_pool:ip1", "null"), sizeof(broker->host));
    printf("%s\n", broker->host);
    broker->port = atoi(iniparser_getstring(ini, "port_pool:port0", "null"));
    printf("%d\n", broker->port);
    strncpy(broker->username, iniparser_getstring(ini, "username:username1", "null"), sizeof(broker->username));
    printf("%s\n", broker->username);
    strncpy(broker->password, iniparser_getstring(ini, "password:password1", "null"), sizeof(broker->password));
    printf("%s\n", broker->password);
    strncpy(broker->clientid, iniparser_getstring(ini, "clientid:clientid2", "null"), sizeof(broker->clientid));
    printf("%s\n", broker->clientid);
    strncpy(broker->topic, iniparser_getstring(ini, "topic:topic2", "null"), sizeof(broker->topic));
    printf("%s\n", broker->topic);
    broker->qos = atoi(iniparser_getstring(ini, "qos:qos0", "null"));
    printf("%d\n", broker->qos);
    broker->keeplive = atoi(iniparser_getstring(ini, "keeplive:keeplive0", "null"));
    printf("%d\n", broker->keeplive);
    broker->retain = atoi(iniparser_getstring(ini, "retain:retain0", "null"));
    printf("%d\n", broker->retain);

    data=get_temp_hmti_cjson1(broker);
    strncpy(broker->data, data, sizeof(broker->data));
    free(data);

    iniparser_freedict(ini);

    return;
}

#if 0
int main(int argc, char *argv[])
{
    broker_t  broke;

    broker_conf(&broke);
    return 0;
}
#endif
