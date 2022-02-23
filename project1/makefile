CC = g++
CFLAGS = -Wall
LDFLAGS = -lm
INCLUDES = -I/usr/local/include

VPATH = source
EXENAME = test
OBJECTS = main.o admin.o buyer.o data.o seller.o user.o deal_sys.o caculator.o

all : ${OBJECTS}
	${CC} -o ${EXENAME} *.o ${LDFLAGS} ${INCLUDES}
clean:
	rm -rf ${EXENAME} ${OBJECTS}
%.o: %.cpp
	${CC} ${CFLAGS} ${INCLUDES} -c $< -o $@