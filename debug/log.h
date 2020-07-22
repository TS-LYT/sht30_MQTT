/********************************************************************************
 *      Copyright:  (C) 2020 longyongtu<longyongtu13@qq.com>
 *                  All rights reserved.
 *
 *       Filename:  log_fa.h
 *    Description:  This head file 
 *
 *        Version:  1.0.0(2020年05月24日)
 *         Author:  longyongtu <longyongtu13@qq.com>
 *      ChangeLog:  1, Release initial version on "2020年05月24日 18时39分07秒"
 *                 
 ********************************************************************************/

#ifndef _LOG_H_
#define _LOG_H_

#include <time.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdarg.h>
#include <string.h>



/*-----------------------------------------------------------------------------
 *  打印输出级别
 *-----------------------------------------------------------------------------*/
#define DEBUG         5
#define ENTRY         4
#define INFO          3
#define WARN          2
#define ERROR         1
#define NONE          0


/*-----------------------------------------------------------------------------
 *  选择打印输出的位置
 *-----------------------------------------------------------------------------*/
#define STDOUT_ONLY   0
//#define STDOUT_FILE   1
#define FILE_ONLY     2


/*-----------------------------------------------------------------------------
 *  如果在编译阶段不定义PRINT_LEVEL，则默认DEBUG模式打印，如果用-D PRINT_LEVEL=?
 *  则在某个级别模式下打印，如下列：gcc log.c -D PRINT_LEVEL=INFO（或者是对应得数字）
 *  则在INFO模式下打印。
 *-----------------------------------------------------------------------------*/
#ifndef PRINT__LEVEL
#define DEBUG_LEVEL INFO   //默认级别
#else
#define DEBUG_LEVEL PRINT__LEVEL
#endif
 

#define LOG_DEBUG(args,...)\
    do{\
             if (DEBUG_LEVEL >= DEBUG)\
             {\
                        printf("[D] (%s, %d) "args"", __FILE__, __LINE__, ##__VA_ARGS__);\
                    }\
     }while(0)

#define LOG_ENTRY(args,...)\
    do{\
            if (DEBUG_LEVEL >= ENTRY)\
            {\
                        printf("[L] (%s, %d) "args"", __FILE__, __LINE__, ##__VA_ARGS__);\
                    }\
    }while(0)

#define LOG_INFO(args,...)\
    do{\
            if (DEBUG_LEVEL >= INFO)\
            {\
                        printf("[I] (%s, %d) "args"", __FILE__, __LINE__, ##__VA_ARGS__);\
                    }\
    }while(0)

#define LOG_WARN(args,...)\
    do{\
            if (DEBUG_LEVEL >= WARN)\
            {\
                        printf("[W] (%s, %d) "args"", __FILE__, __LINE__, ##__VA_ARGS__);\
                    }\
    }while(0)

#define LOG_ERROR(args,...)\
    do{\
            if (DEBUG_LEVEL >= ERROR)\
            {\
                        printf("[E] (%s, %d) "args"", __FILE__, __LINE__, ##__VA_ARGS__);\
                    }\
    }while(0)

#define LOG_NONE(args,...)\
    do{\
            if (DEBUG_LEVEL >= NONE)\
            {\
                        printf("[N] (%s, %d) "args"", __FILE__, __LINE__, ##__VA_ARGS__);\
                    }\
    }while(0)


/* 
 *  Description:  实现改变打印的输出位置  若不需要使用记得手动关闭日志的文件描述符
 *
 *  \param[int]    tag          打印方式格式如下：
 *  #define STDOUT_ONLY   0     仅标准输出
 * // #define STDOUT_FILE   1     标准输出和文件一起输出   暂未使用
 *  #define FILE_ONLY     2     仅输出到文件
 *
 *  \param[char *] file_name    输出到的文件名
 *  此参数为NULL时就默认为标准输出，而且tag的值不影响 
 *  
 * \return value : 出错返回负数，标准输出返回0，日志输出返回改文件描述符
 *
 */ 
#define RWRWRW  (S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH)//创建文件的权限

int log_model(int tag, char *file_name);



/*! \Brief: rollback log      若不需要使用记得手动关闭日志的文件描述符
 *
 *  Description: 这是一个可变参函数，
 *  可变部分输入对应回滚日志的大小，传入零
 *  时默认不进行回滚
 *
 * \param: [int] fd  日志的文件描述符  
 * \param: [int或long long 都行] ..  可变参数 回滚字节的大小 0时默认不会回滚 
 *
 * \return: 出错返回负数，
 */
int rollback(int fd, ...);

#endif

