#include <errno.h>

void errore(int n, char* s){
		printf("ERROR: %d %s \n", n, s);
		printf("%d: %s \n", errno, strerror(errno)); //descrizione errore
		exit(n);
}
