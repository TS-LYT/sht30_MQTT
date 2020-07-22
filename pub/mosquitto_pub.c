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
#include "mosquitto_pub.h"


char *get_temp_hmti_cjson(broker_t *broker)
{
    float           temp = 31.3;
    float           hmti = 8;
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
    int     retain = 1;
    broker_t *obj1 = obj;

    printf("hhhhhhh\n");
    buf=get_temp_hmti_cjson(obj1);

    if(!rc)
    {
        if( mosquitto_publish(mosq, &mid, obj1->pubTopic,strlen(buf)+1, buf, obj1->pubQos, retain) != MOSQ_ERR_SUCCESS)

        {
            printf("Mosq_Publish() error: %s\n", strerror(errno));
            return ;
        }
        printf("pubilush topic:%s\n", obj1->pubTopic) ;
    }
    mosquitto_disconnect(mosq);

    free(buf);
}




int main(int argc, char *argv[])
{
    broker_t            broker;     
    char                *progname = NULL;
    int                 rv = -1;
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


    rv=pub_init(&mosq, broker, (void *)&broker);
    if( rv < 0 ) 
    {
        return -1;   
    }
    printf("hdsaf\n");

    mosquitto_connect_callback_set(mosq, callback);

    while(1)
    {
    printf("hdsaf\n");
        /* 连接MQTT代理*/
        if(mosquitto_connect(mosq, broker.host, broker.port, broker.keepalive) != MOSQ_ERR_SUCCESS )
        {
            printf("mosquitto connect server failure:%s\n",strerror(errno));
            continue;
            sleep(5);
        }
        printf("hdsaf\n");
        
        /* 无线阻塞循环调用loop*/
        mosquitto_loop_forever(mosq, 6000, 1 );
        sleep(60);
    }
cleanup:
    printf("program will exit\n");
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();

    return 0;
}

int pub_init(struct mosquitto ** mosq, broker_t broker, void *buf)
{
    int  rv=-1;

    /*  Must be called before any other mosquitto functions. not thread safe */
    mosquitto_lib_init();

    /*  Create a new mosquitto client instance */
    *mosq=mosquitto_new(broker.clientid, true, buf);
    if( !*mosq ) 
    {
        printf("creat a mosquitto instance errno\n");
        mosquitto_lib_cleanup();
        return -1;
    }

#if 0
    //rv=mosquitto_username_pw_set(*mosq, broker.username, broker.passwd);
    rv=mosquitto_username_pw_set(*mosq, NULL, NULL);
    if( rv!=MOSQ_ERR_SUCCESS ) 
    {
        printf("set username ande password failure\n");
        mosquitto_destroy(*mosq);//Use to free memory associated with a mosquitto client instance.
        mosquitto_lib_cleanup();//Call to free resources associated with the library.
        return -1;
    }
#endif

    return 0;
}
