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
#include "temp_rpt_aly.h"


char *get_temp_hmti_cjson(broker_t *broker)
{
    float          temp = 31.3;
    float          hmti = 8;
    cJSON           *root = cJSON_CreateObject();
    cJSON           *params = cJSON_CreateObject();
    char            *data = NULL;
    int             rv = 0;

#if 1
    rv=get_temp_hmti(I2C_ADDR, SHT_ADDR, &temp, &hmti);
    if(rv < 0) 
    {
        return NULL;
    }
#endif
    
    
    cJSON_AddItemToObject(root, "method", cJSON_CreateString(broker->method));
    cJSON_AddItemToObject(root, "id", cJSON_CreateString(broker->id));
    //cJSON_AddItemToObject(params, "CurrentTemperature", cJSON_CreateNumber(temp));
    //cJSON_AddItemToObject(params, "DetectDistance", cJSON_CreateNumber(2));
    cJSON_AddItemToObject(params, "CurrentHumidity", cJSON_CreateNumber((int)hmti));
    cJSON_AddItemToObject(params, "CurrentTemperature", cJSON_CreateNumber(temp));
    cJSON_AddItemToObject(root, "params", params);
    cJSON_AddItemToObject(root, "version", cJSON_CreateString(broker->version));

    data=cJSON_Print(root);
    printf("%s\n", data);
    return data;
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

void callback(struct mosquitto *mosq, void *obj, int rc)
{
    char    *buf;
    int     mid;
    int     retain = 0;
    broker_t *obj1 = obj;


    buf=get_temp_hmti_cjson(obj1);
    printf("hhhhhh%s\n", buf);

    if(!rc)
    {
        if( mosquitto_publish(mosq, &mid, obj1->pubTopic,strlen(buf)+1, buf, obj1->pubQos, retain) != MOSQ_ERR_SUCCESS
          )
        {
            printf("Mosq_Publish() error: %s\n", strerror(errno));
            return ;
        }
        printf("pubilush topic:%s\n", obj1->pubTopic) ;
    }
    mosquitto_disconnect(mosq);

    //free(buf);
}




int main(int argc, char *argv[])
{
    broker_t            broker;     
    char                *progname = NULL;
    int                 rv = -1;
    int                 opt;
    struct mosquitto    *mosq = NULL;

    strncpy(broker.host, HOST, sizeof(broker.host));
    broker.port = PORT;
    strncpy(broker.username, USERNAME, sizeof(broker.username));
    strncpy(broker.passwd, PASSWD, sizeof(broker.passwd));
    strncpy(broker.clientid, CLIENTID, sizeof(broker.clientid));
    strncpy(broker.pubTopic, PUBTOPIC, sizeof(broker.pubTopic));
    broker.pubQos = PUBQOS;
    broker.keepalive = KEEPALIVE;
    strncpy(broker.method, METHOD, sizeof(broker.method));
    strncpy(broker.id, ID, sizeof(broker.id));
    strncpy(broker.version, VERSION, sizeof(broker.version));

#if 0
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
                broker.host = optarg;
                break;
            case 'p':
                broker.port = atoi(optarg);
                break;
            case 'u':
                broker.username = optarg;
                break;
            case 'P':
                broker.passwd = optarg;
                break;
            case 'i':
                broker.clientid = optarg;
            case 't':
                broker.pubTopic = optarg;
                break;
            case 'v':
                printf("%s Version %s\n",progname, PR_VERSION);
                return 0;
            case 'H':
                print_usage(progname);
                return 0;
            default:
                break;
        }
    }
#endif

    mosquitto_lib_init();

    mosq=mosquitto_new(broker.clientid, true, (void *)&broker );
    if(!mosq) 
    {
        return -1;
    }

    mosquitto_connect_callback_set(mosq, callback);

    if( mosquitto_username_pw_set(mosq, broker.username, broker.passwd) !=MOSQ_ERR_SUCCESS)
    {
        printf("mosquitto username and passwd failure:%s\n",strerror(errno));
        goto cleanup;
    }

    while(1)
    {
        /* 连接MQTT代理*/
        if(mosquitto_connect(mosq, broker.host, broker.port, broker.keepalive) != MOSQ_ERR_SUCCESS )
        {
            printf("mosquitto connect server failure:%s\n",strerror(errno));
            continue;
            sleep(5);
        }

        /* 无线阻塞循环调用loop*/
        mosquitto_loop_forever(mosq, -1, 1 );
        sleep(60);
    }
cleanup:
        printf("program will exit\n");
        mosquitto_destroy(mosq);
        mosquitto_lib_cleanup();

    return 0;
}
