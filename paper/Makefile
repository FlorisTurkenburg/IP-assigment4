CC = gcc
CFLAGS = -g -Wall -Wextra -Wno-unused-parameter

build: all

all: paperclient paperserver papers.cgi paperinfo.cgi

clean:
	rm -f paperclient.o paperserver_clnt.o paperserver_xdr.o paperserver_svc.o \
	paperserver.o paperserver_clnt.c paperserver.h paperserver_xdr.c \
	paperserver_svc.c paperclient paperserver ../web/cgi-bin/papers.cgi papers.o ../web/cgi-bin/paperinfo.cgi \
	paperinfo.o

papers.cgi: papers.o paperserver_clnt.o paperserver_xdr.o 
	$(CC) -o ../web/cgi-bin/papers.cgi papers.o paperserver_clnt.o paperserver_xdr.o -lnsl  

paperinfo.cgi: paperinfo.o paperserver_clnt.o paperserver_xdr.o 
	$(CC) -o ../web/cgi-bin/paperinfo.cgi paperinfo.o paperserver_clnt.o paperserver_xdr.o -lnsl

paperclient: paperclient.o paperserver_clnt.o paperserver_xdr.o 
	$(CC) -o paperclient paperclient.o paperserver_clnt.o paperserver_xdr.o -lnsl  

paperserver: paperserver.o paperserver_svc.o paperserver_xdr.o
	$(CC) -o paperserver paperserver.o paperserver_svc.o paperserver_xdr.o -lnsl 

paperserver_clnt.c paperserver.h paperserver_xdr.c paperserver_svc.c: paperserver.x
	rpcgen paperserver.x

paperclient.o: paperclient.c paperserver.h
	$(CC) $(CFLAGS) -c -o paperclient.o paperclient.c

papers.o: papers.c paperserver.h config.h
	$(CC) $(CFLAGS) -c -o papers.o papers.c

paperinfo.o: paperinfo.c paperserver.h config.h
	$(CC) $(CFLAGS) -c -o paperinfo.o paperinfo.c

paperserver_clnt.o: paperserver_clnt.c paperserver.h
	$(CC) $(CFLAGS) -c -o paperserver_clnt.o paperserver_clnt.c

paperserver_xdr.o: paperserver_xdr.c paperserver.h
	$(CC) $(CFLAGS) -c -o paperserver_xdr.o paperserver_xdr.c

paperserver_svc.o: paperserver_svc.c paperserver.h
	$(CC) $(CFLAGS) -c -o paperserver_svc.o paperserver_svc.c

paperserver.o: paperserver.c paperserver.h
	$(CC) $(CFLAGS) -c -o paperserver.o paperserver.c