/* *******************************************************************************
 *      Copyright:  (C) 2020 longyongtu<longyongtu13@qq.com>
 *                  All rights reserved.
 *
 *       Filename:  log.h
 *    Description:  This head file 
 *
 *        Version:  1.0.0(2020年05月24日)
 *         Author:  longyongtu <longyongtu13@qq.com>
 *      ChangeLog:  1, Release initial version on "2020年05月24日 18时42分22秒"
 *                 
 ********************************************************************************/

#include "log.h"


int log_model(int tag, char *file_name)
{
    int            rv = -1;
    int            fd = -1;
    mode_t         mode;
    struct stat    buf;


    if( tag!=STDOUT_ONLY /*|| tag!=STDOUT_FIL*/ && tag!=FILE_ONLY)
    {
        LOG_ERROR("invalid argument:tag, pleaser input: STDOUT_ONLY(0) or STDOUT_FILE(1) or FILE_ONLY(2)\n");
        return -1;
    }
    if( !file_name )//文件名为空 默认标准输出
    {
        LOG_INFO("Only standard output\n");
        return 0;
    }
    
    if( tag = STDOUT_ONLY)//执行标准输出
    {
        LOG_INFO("enter STDOUT_ONLY\n");
        return 0;
    }

    //将日志输入到对应的文件
    rv=access(file_name , F_OK);//判断文件是否存在
    if( rv < 0)
    {
        LOG_INFO("%s doesn't exist, need to create a log file\n", file_name);
        umask(0);//为进程设置文件模式创建屏蔽字, 
        fd=open(file_name, O_RDWR|O_CREAT|O_APPEND, RWRWRW );//屏蔽字为1的位，文件mode中的相应位一定关闭
        if( fd < 0)
        {
            LOG_ERROR("open %s failure\n", file_name);
            return -2;
        }
        LOG_INFO("open %s successful\n", file_name);
    }
    else 
    {   
        LOG_INFO("%s exist\n", file_name);
        fd=open(file_name, O_RDWR|O_CREAT|O_APPEND);//打开文件，设置对应的权限
        if( fd < 0)  
        {            
            LOG_ERROR("open %s failure\n", file_name);
            return -2;
        }            
        LOG_INFO("open %s successful\n", file_name);
    }
    
    /* if( tag = STDOUT_FILE)
    {
        printf("enter STDOUT_FILE\n");
        fd1=dup(fd);
        if( fd1 < 0)
        {
            printf("dup %d of %s failure\n", fd, file_name);
            return -3;
        }
        dup2(STDOUT_FILENO, fd1);
        if( )
        {
            
        }
        
    }*/

    if( tag = FILE_ONLY )
    {
        LOG_INFO("enter FILE_ONLY\n");
        rv=dup2(fd, STDOUT_FILENO);//将标准输出重定向到文件fd
        if( rv < 0)
        {
            LOG_ERROR("dup2 %d to stdout failure\n", fd);
            return -4;
        }
        rv=fstat(fd, &buf);//获取日志文件的信息
        if( rv < 0)
        {
            printf("get last time used failure\n");
        }
        else
        {
            printf("File size:                %lld bytes\n",(long long) buf.st_size);//打印截止到现在的文件大小
            printf("Time of last access:%sTime of last modification:%s\n",ctime((const time_t *)&buf.st_atim), ctime((const time_t *)&buf.st_mtim));//打印上次进入的时间， 打印上次进入且修改日志的时间
        }
        return fd;
    }
}


int rollback(int fd, ...)
{
    int            rv = 0;
    va_list        v1;           //申请存储参数的结构体
    int            size = 0;
    int            offset;

    if( !fd )
    {
        LOG_ERROR("invalid Parameter input\n");
        return -1;
    }
    
    va_start(v1, fd);     //申明最后一个传递给函数的已知的固定参数

    size = va_arg(v1, int);      //这个宏检索函数参数列表中类型为 type(int) 的下一个参数

    /* 这个宏允许使用了 va_start 宏的带有可变参数的函数返回。如果在从函数返回之前没有调用 va_end，则结果为未定义 */
    va_end(v1);

    if( !size )
    {
        LOG_INFO("not rollback\n");
        return 0;
    }
    if( fd < 0)
    {
        LOG_ERROR("invalid Parameter input\n");
        return -1;
    }
    offset=lseek(fd, 0, SEEK_END);
    LOG_DEBUG("crrunt offset is %d\n", offset);
    if( offset < 0)
    {
        LOG_ERROR("lseek failure in log file,when it log rollback:%s\n", strerror(errno));
        return -1;
    }
    if( offset >= size)
    {
        LOG_INFO("crruent size of log file is %d\n", offset);

        offset=lseek(fd, 0, SEEK_SET);//获取文件的大小
        if( offset < 0)
        {
            LOG_ERROR("lseek failure,when log rollback:%s\n", strerror(errno));
        }
        rv=ftruncate(fd, 0);//清楚文件的内容
        if(rv < 0) 
        {
            LOG_ERROR("log file rollback failure\n");   
            return -1;
        }
        LOG_DEBUG("%d offset of log file\n", offset);
        LOG_INFO("log file rollback successful\n");
        return 1;
    }
}





#if 0

int main()
{
    int   fd =0; 
    fd=log_model(FILE_ONLY, "log.txt");
    LOG_INFO("I'm Info, \n");
    LOG_WARN("I'm Warning\n");
    LOG_DEBUG("I'm Debug\n");
    LOG_ERROR("I'm Error\n");
    rollback(fd, 1000);
 //   while(1);
    return 0;
}
#endif

