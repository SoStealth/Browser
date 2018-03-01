#define REQUEST_HEADER "GET "
#define REQUEST_FOOTER " HTTP/1.1\n" \
			"User-Agent: Mozilla/5.0\n\n"
#define HTTP_PORT 80
#define MAX_STR 4096
#define CORRECT_RETURN_CODE "200 OK"

int main(int argc, char* argv[]) {
	if(argc!=3) {
		printf("USAGE:%s SERVER_IP FILENAME\n",argv[0]);
		return -1;
	}
	//Assegnazione dei dati passati da riga di comando
	char* server_ip = argv[1];
	char* filename = argv[2];
	char rqst[MAX_STR];
	//Client per l'invio della richiesta e per la ricezione della risposta
	ClientTCP* myself = new ClientTCP();
	//Connessione al server
	Address server(server_ip,HTTP_PORT);
	if(myself->connetti(server)) {
		errore(-2,"connetti()\n");	
	}
	//Costruzione della richiesta
	sprintf(rqst,"%s%s%s",	REQUEST_HEADER,
				filename,
				REQUEST_FOOTER);
	//Invio della richiesta
	myself->invia_raw(rqst,strlen(rqst));
	//Ricezione della risposta
	int length;
	char* rspn = 
		myself->(char*)ricevi_raw(&length);
	rspn[length] = '\0';
	//Controllo validità della risposta
	if(strstr(rspn,CORRECT_RETURN_CODE)==NULL) {
		printf("Errore ricezione: %s\n",rspn);
		return -3;
	}
	//Scorrimento risposta fino all'inizio del codice HTML
	//Scorre fino a che *s vale '\0' o ci sono due '\n' di fila
	char* s = rspn;
	while(*s && ((*s!='\n') || (*(s+1)!='\n'))) {
		s++;
	}
	if(*s=='\0) {
		printf("Errore ricezione: %s\n",rspn);
	   	return -4;
	}
	s+=2;
	//Creazione file
	FILE* out = fopen(filename,"w");
	if(out==NULL) {
		errore(-5,"fopen()\n");	
	}
	//Scrittura html su file
	fprintf(out,"%s\n",s);
	//Chiusura programma
	free(answer);
	delete(myself);
	return 0;
}
