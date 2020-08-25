/*********************************************************************************
 *      Copyright:  (C) 2020 longyongtu<longyongtu13@qq.com>
 *                  All rights reserved.
 *
 *       Filename:  emqx_sub_conf.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(2020年08月12日)
 *         Author:  longyongtu <longyongtu13@qq.com>
 *      ChangeLog:  1, Release initial version on "2020年08月12日 11时07分03秒"
 *                 
 ********************************************************************************/

#include "emqx_sub_conf.h"


void printJson(cJSON * root)//以递归的方式打印json的最内层键值对
{
    for(int i=0; i<cJSON_GetArraySize(root); i++)   //遍历最外层json键值对
    {
        cJSON * item = cJSON_GetArrayItem(root, i);
        if(cJSON_Object == item->type)      //如果对应键的值仍为cJSON_Object就递归调用printJson
            printJson(item);
        else                                //值不为json对象就直接打印出键和值
        {
            printf("%s:", item->string);
            printf("%s\r\n", cJSON_Print(item));
        }
    }
    return;
}

void broker_conf(broker_t *broker)
{
    dictionary *ini = NULL;

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
    strncpy(broker->username, iniparser_getstring(ini, "username:username2", "null"), sizeof(broker->username));
    printf("%s\n", broker->username);
    strncpy(broker->password, iniparser_getstring(ini, "password:password2", "null"), sizeof(broker->password));
    printf("%s\n", broker->password);
    strncpy(broker->clientid, iniparser_getstring(ini, "clientid:clientid2", "null"), sizeof(broker->clientid));
    printf("%s\n", broker->clientid);
    strncpy(broker->topic, iniparser_getstring(ini, "topic:topic1", "null"), sizeof(broker->topic));
    printf("%s\n", broker->topic);
    broker->qos = atoi(iniparser_getstring(ini, "qos:qos0", "null"));
    printf("%d\n", broker->qos);
    broker->keeplive = atoi(iniparser_getstring(ini, "keeplive:keeplive0", "null"));
    printf("%d\n", broker->keeplive);
    broker->retain = atoi(iniparser_getstring(ini, "retain:retain0", "null"));
    printf("%d\n", broker->retain);

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
