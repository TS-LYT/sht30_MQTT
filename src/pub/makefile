CROSS_CMP_DIR = /opt/

CROSS_CMP_NAME = ${CROSS_CMP_DIR}arm-linux-

BINNAME = pub

DAYLIB = conm

DAYLIB_DIR = ../comlib/

CC = gcc

INCDIR = ../../inc/

HFLAGS = -I${INCDIR}

CFLAGS = -g ${HFLAGS}  

LDFLAGS = -L${DAYLIB_DIR}

LDFLAGS += -l${DAYLIB} -lm -lmosquitto -lpthread

SRC = ${wildcard *.c}

OBJ = ${patsubst %.c, %.o, ${wildcard *.c}}

CONF_DIR = ../../etc/

CONF_FILE = ${CONF_DIR}/emqx_pub.conf

${BINNAME}:${OBJ}
	${CC}  ${CFLAGS} -o $@  ${OBJ} ${LDFLAGS}
	rm $^
	mv ${BINNAME} ../../bin/

%.o:%.c ${CONF_FILE}
	${CC} ${CFLAGS} -c -o $@ $<

clean: 
	rm ../../bin/pub


