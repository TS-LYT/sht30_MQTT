/*********************************************************************************
 *      Copyright:  (C) 2020 longyongtu<longyongtu13@qq.com>
 *                  All rights reserved.
 *
 *       Filename:  mosquitto_sub.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(16/07/20)
 *         Author:  longyongtu <longyongtu13@qq.com>
 *      ChangeLog:  1, Release initial version on "16/07/20 14:32:00"
 *                 
 ********************************************************************************/
#include "mosquitto_sub.h"



void connect_callback(struct mosquitto *mosq, void *obj, int rc)
{
    broker_t    *broker = (broker_t*)obj;

    if(!rc)
    {
        if( mosquitto_subscribe(mosq, NULL, broker->topic, 0) != MOSQ_ERR_SUCCESS )
        {
            printf("Mosq_subcrible() error: %s\n", strerror(errno));
            return ;
        }
        printf("subicrible topic:%s\n", broker->topic);
        return;
    }

    return;
}

#if 0        
struct mosquitto_message{
    int mid;//消息序号ID
    char *topic; //主题
    void *payload; //主题内容 ，MQTT 中有效载荷
    int payloadlen; //消息的长度，单位是字节
    int qos; //服务质量
    bool retain; //是否保留消息
};
#endif

void message_callback(struct mosquitto *mosq, void* obj, const struct mosquitto_message *message)
{
    cJSON    *buf = NULL;
        
    printf("subcrible topic is %s\n", message->topic);
    printf("message:  \n");
    buf=cJSON_Parse(message->payload);
    printJson(buf);
    cJSON_Delete(buf);
    
    return;
}





int main(int argc, char *argv[])
{
    broker_t            broker;     
    int                 rv = -1;
    struct mosquitto    *mosq = NULL;

    broker_conf(&broker);

    sub_init(&mosq, broker, (void *)&broker);
    
    if( !rv ) 
    {
        return -1;   
    }
    
    /* Set the subscribe callback.  This is called when the broker responds to a subscription request */
    mosquitto_message_callback_set(mosq, message_callback);
    
    /* Set the connect callback.  This is called when the broker sends a CONNACK message in response to a connection */
    mosquitto_connect_callback_set(mosq, connect_callback);

    rv=mosquitto_connect(mosq, broker.host, broker.port, broker.keeplive);
    if( rv != MOSQ_ERR_SUCCESS)
    {
        printf("client connect broker failure\n");
        mosquitto_destroy(mosq);//Use to free memory associated with a mosquitto client instance.
        mosquitto_lib_cleanup();//Call to free resources associated with the library.
        return -1;
    }

    rv=mosquitto_loop_forever(mosq, -1, 1);
    if( rv != MOSQ_ERR_SUCCESS ) 
    {
        printf("%s\n", strerror(errno));
    }

    return 0;

}

int sub_init(struct mosquitto ** mosq, broker_t broker, void *buf)
{
    int  rv=-1;
    //struct mosquitto *mosq1 = NULL;

    /* Must be called before any other mosquitto functions. not thread safe */
    mosquitto_lib_init();

    /* Create a new mosquitto client instance */
    *mosq=mosquitto_new(broker.clientid, true, buf);
    if( !*mosq ) 
    {
        printf("creat a mosquitto instance errno\n");
        mosquitto_lib_cleanup();
        return -1;
    }

    rv=mosquitto_username_pw_set(*mosq, broker.username, broker.password);
    if( rv!=MOSQ_ERR_SUCCESS ) 
    {
        printf("set username ande password failure\n");
        mosquitto_destroy(*mosq);//Use to free memory associated with a mosquitto client instance.
        mosquitto_lib_cleanup();//Call to free resources associated with the library.
        return -1;
    }

    return 0;
}

