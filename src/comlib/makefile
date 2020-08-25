CROSS_CMP_DIR = /opt/ 

CROSS_CMP_NAME = ${CROSS_CMP_DIR}arm-linux-

DAYLIB = libconm.so

CC = gcc

INCDIR = ../../inc/

HFLAGS = -I${INCDIR}

LFLAGS = -shared -fPIC

CFLAGS = -g ${HFLAGS} ${LFLAGS} 

USR_DIR = /usr/local/lib/



${DAYLIB}:*.c
	${CC} ${CFLAGS} -o $@ $^
	sudo cp ${DAYLIB} ${USR_DIR}

clean:${DAYLIB}
	rm $<
	sudo rm ${USR_DIR}libconm.so



