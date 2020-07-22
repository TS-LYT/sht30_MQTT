#当前目录下的所有.c文件
SRC = $(wildcard *.c)

#当前目录下的所有.h文件
SRH = $(wildcard *.h)

#gcc -c 编译的中间文件默认为只更改后缀名.c变成.o\
	gcc -c hh.c  生成的中间名为 hh.o

OBJ = $(patsubst %.c, %.o, $(SRC))

#设定生成可执行文件的名称
app=mosquitto_pub

#设定使用的编译器
cc=gcc


#目标 依赖 执行动作的编写

#将中间文件相互连接，然后链接好动态库
all:$(OBJ)
	$(cc) $(OBJ) -o ${app} -lm -lmosquitto 

#生成中间文件, @表示不再bash也就是命令行中打印执行的命令
$(OBJ): $(SRC) $(SRH)
	$(cc) -c ${SRC} 
	
clear:$(OBJ)
	rm *.o

clean:clear
	rm $(app)


