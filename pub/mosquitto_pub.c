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


void callback(struct mosquitto *mosq, void *obj, int rc)
{
    int     mid;
    broker_t *obj1 = obj;

    if(!rc)
    {
        if( mosquitto_publish(mosq, &mid, obj1->topic, strlen(obj1->data)+1, obj1->data, obj1->qos, obj1->retain) != MOSQ_ERR_SUCCESS)
        //if( mosquitto_publish(mosq, &mid, obj1->topic, 0, NULL, obj1->qos, 1) != MOSQ_ERR_SUCCESS)
        {
            printf("Mosq_Publish() error: %s\n", strerror(errno));
            return ;
        }
        printf("pubilush topic:%s\n", obj1->topic) ;
    }
    mosquitto_disconnect(mosq);
    
    return;
}




int main(int argc, char *argv[])
{
    broker_t            broker;     
    char                *progname = NULL;
    int                 rv = -1;
    struct mosquitto    *mosq = NULL;

    broker_conf(&broker);

    rv=pub_init(&mosq, broker, (void *)&broker);
    if( rv < 0 ) 
    {
        return -1;   
    }

    mosquitto_connect_callback_set(mosq, callback);

    while(1)
    {
        /* 连接MQTT代理*/
        if(mosquitto_connect(mosq, broker.host, broker.port, broker.keeplive) != MOSQ_ERR_SUCCESS )
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
