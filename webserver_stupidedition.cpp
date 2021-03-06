#include "TCPv2.hpp"
#include <string.h>

#define RESPONSE "HTTP/1.1 200 OK\n"\
	"Date: Wed, 19 Apr 2017 16:34:20 GMT\n"\
	"Content-Type: text/html; charset=UTF-8\n\n" \
	"<!DOCTYPE html>\n" \
	"<html class=\"client-nojs\" lang=\"it\" dir=\"ltr\">\n" \
	"<head>\n" \
	"<meta charset=\"UTF-8\"/>\n" \
	"<title>Wikipedia, l'enciclopedia libera</title>\n" \
	"</head>\n" \
	"<body>\n" \
	"\n" \
	"</body>\n" \
	"</html>"

int main(int argc, char* argv[]) {
	if(argc!=3) {
		printf("USAGE:%s PORT ROOT\n",argv[0]);
		return -1;
	}
	
	int port = atoi(argv[1]);
	char* root = argv[2];
	
	ServerTCP myself(port,false);
	Connection connection = myself.accetta();
	char* rqst = myself.ricevi();
	if(rqst==NULL) {
		errore(-2,"ServerTCP.ricevi()\n");
	}
	
	if(connection.invia(RESPONSE)) {
		errore(-3,"Connection.invia()\n");	
	}
	
	myself.disconnetti(connection);
	delete(myself);
	free(rqst);
	
	return 0;
}
