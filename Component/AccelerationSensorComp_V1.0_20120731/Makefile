CC	= g++
AR	= ar

CFLAGS	= -Wall -O2 -fomit-frame-pointer -fpermissive -fPIC -D_REENTRANT -D_OPROS_LINUX_
CFLAGS	+= -I../../CommonLibrary/include
CFLAGS	+= -I../../CommonLibrary/include/device
CFLAGS	+= -D_DEBUG

LDFLAGS	= -shared -W1

LIBS	= ../../CommonLibrary/lib/libOPRoSCDL.a

SRCS	= AccelerationSensorComp.cpp

OBJS	= $(SRCS:.cpp=.o)

TARGET	= AccelerationSensorComp.so

all : $(TARGET)

$(TARGET) : $(OBJS) 
	$(CC) $(LDFLAGS) -o $(TARGET) $(OBJS) $(LIBS) -ldl

%.o : %.cpp
	@echo "C++ Compiling $< ..."
	$(CC) -c $(CFLAGS) -o $@ $<

clean :
	rm -f $(TARGET) *.o

install:
	cp $(TARGET) /lib
