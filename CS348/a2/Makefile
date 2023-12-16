# A3 Makefile

###############################################
# don't touch anything below this line
###############################################
DB2PATH = /home/db2inst2/sqllib
# The following compile and link options are for the gcc
CC=gcc

CFLAGS=-I$(DB2PATH)/include 
#LIBS=-L$(DB2PATH)/lib -R$(DB2PATH)/lib -ldb2
LIBS=-L$(DB2PATH)/lib -ldb2


all: enroll schedule

enroll: enroll.sqc util.o
	db2 connect to cs348
	db2 prep enroll.sqc bindfile
	db2 bind enroll.bnd
	db2 connect reset
	$(CC) $(CFLAGS) -c enroll.c 
	$(CC) $(CFLAGS) -o enroll enroll.o util.o $(LIBS)

schedule: schedule.sqc util.o
	db2 connect to cs348
	db2 prep schedule.sqc bindfile
	db2 bind schedule.bnd
	db2 connect reset
	$(CC) $(CFLAGS) -c schedule.c 
	$(CC) $(CFLAGS) -o schedule schedule.o util.o $(LIBS)

clean:
	rm -f $(NAME) $(NAME).c $(NAME).o $(NAME).bnd

util.o : util.c
	$(CC) -w -c util.c $(CFLAGS)

