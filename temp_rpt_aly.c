/*********************************************************************************
 *      Copyright:  (C) 2020 longyongtu<longyongtu13@qq.com>
 *                  All rights reserved.
 *
 *       Filename:  temp_rpt_aly.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(16/07/20)
 *         Author:  longyongtu <longyongtu13@qq.com>
 *      ChangeLog:  1, Release initial version on "16/07/20 14:32:00"
 *                 
 ********************************************************************************/
#include <stdio.h>

#include "sht_temp_hmti.h"
#include "cJSON.h"


#define  PR_VERSION    "1.0.0"
#define  HOST     
#define  PORT    
#define  USERNAME    
#define  PASSWD    
#define  CLIENTID     
#define  PUBTOPIC    
#define  PUBQOS     
#define  KEEPALIVE    
#define  METHOD   
#define  ID  
#define  VERSION       "1.0.0"

{
    char    host[128];  /*   MQTT broker server name   */
    int     port;           /*   MQTT broker listen port   */
    char    username[64];   /*   username */
    char    passwd[64];     /*   password  */
    char    clientid[128];  /*    production ID */
    char    pubTopic[256];  /*   Publisher topic   */
    int     pubQos;         /*   Publisher Qos  */
    int     keepalive;      /*   MQTT broker send PING message to subsciber/publisher keepalive timeout<seconds> */
    char    method[64];
    char    id[16];
    char    version[16];
}broker_t,*broker_p;

int get_temp_hmti_cjson(char *buf, int bufsize, broker_t *broker)
{
    float           temp = 0;
    float           hmti = 0;
    cJSON           *root = cJSON_CreateObject();
    cJSON           *params = cJSON_CreateObject();
    char            *data = NULL;
    int             rv = 0;

    rv=get_temp_hmti(I2C_ADDR, SHT_ADDR, &temp, &hmti);
    if(rv < 0) 
    {
        retrun -1;
    }

    cJSON_AddItemToObject(root, "method", cJSON_CreateString(broker->method));
    cJSON_AddItemToObject(root, "id", cJSON_CreateString(broker->id));
    cJSON_AddItemToObject(params, "CurrentTemperature", cJSON_CreateString(temp));
    cJSON_AddItemToObject(params, "CurrentHumidity", cJSON_CreateString(hmti));
    cJSON_AddItemToObject(root, "params", params);
    cJSON_AddItemToObject(root, "vision", cJSON_CreateString(broker->vision));

    data=cJSON_Print(root);
    printf("%s\n", data);
    free(data);
}   

static void  print_usage( char *progname)
{
    printf("Usage  %s [option]...\n", progname);
    printf("-p (--port):   the port of the server you want to connect\n");
    printf("-h (--host):   the hostname of the server you want to connect\n");
    printf("-u (--user):   the username of the client\n");
    printf("-P (--passwd): the passwd of the client \n");
    printf("-i (--clientid): the clientid of the user\n");
    printf("-t (--topic):  the topic of the client you want to pub\n");
    printf("-H (--help): Display this help information\n");
    printf("-v (--version): Display the program version\n");
    printf("%s  Version %s\n", progname, PR_VERSION);
    return ;
}



int main(int argc, char *argv[])
{
    struct broker_t     broker;     
    char                *progname = NULL;
    int                 rv = -1;
    struct mosquitto    *mosq = NULL;

    broker.host = HOST;
    broker.port = PORT;
    broker.username = USERNAME;
    broker.passwd = PASSWD;
    broker.clientid = CLIENTID;
    broker.pubTopic = PUBTOPIC;
    broker.pubQos = PUBQOS;
    broker.keepalive = KEEPALIVE;
    broker.method = METHOD;
    broker.id = ID;
    broker.version = VERSION;

    struct option long_options[]= {
        {"hostname", required_argument, NULL, 'h'},
        {"port", required_argument, NULL, 'p'},
        {"user", required_argument, NULL, 'u'},
        {"passwd",required_argument, NULL,'P'},
        {"topic", required_argument, NULL, 't'},
        {"clientid", required_argument, NULL, 'i'},
        {"help", no_argument, NULL, 'H'},
        {"version", no_argument, NULL, 'v'},
        {0, 0, 0, 0}
    };

    progname = (char *)basename(argv[0]);

    while( (opt = getopt_long(argc, argv,"h:p:u:P:i:tHv", long_options,NULL)) != -1)
    {
        switch (opt)
        {
            case 'h':
                host = optarg;
                break;
            case 'p':
                port = atoi(optarg);
                break;
            case 'u':
                user = optarg;
                break;
            case 'P':
                passwd = optarg;
                break;
            case 'i':
                clientid = optarg;
            case 't':
                topic = optarg;
                break;
            case 'v':
                printf("%s Version %s\n",progname, PROG_VERSION);
                return 0;
            case 'H':
                print_usage(progname);
                return 0;
            default:
                break;
        }
    }

    mosquitto_lib_init();

    mosq=mosquitto_new(broker.clientid, true, (void *)&broker );
    if(!mosq) 
    {
        return -1;
    }

    if( mosquitto_username_pw_set(mosq, mqtt.username,mqtt.passwd) !=MOSQ_ERR_SUCCESS)
    {
        printf("mosquitto username and passwd failure:%s\n",strerror(errno));
        goto cleanup;
    }

    while(1)
    {
        /* 连接MQTT代理*/
        if(mosquitto_connect(mosq, mqtt.hostname, mqtt.port, mqtt.keepalive) != MOSQ_ERR_SUCCESS )
        {
            printf("mosquitto connect server failure:%s\n",strerror(errno));
            continue;
            sleep(1);
        }

        /* 无线阻塞循环调用loop*/
        mosquitto_loop_forever(mosq, -1, 1 );
        sleep(10);
    }


    return 0;
}
