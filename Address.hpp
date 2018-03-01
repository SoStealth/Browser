#include <string.h>     //per atoi
#include <stdio.h>      //per la sprintf che scrive in una stringa
#include <stdlib.h>     //per la funzione malloc,free
#include <sys/socket.h> //per sin.family
#include <arpa/inet.h>  //per sockaddr,inet_aton,
#include <unistd.h>

class Address{
    private:
     char* ip;
     int port;
    public:
     //Costruttori
      Address();
      Address(char* ip,int port);
      Address(struct sockaddr_in address);
      Address(const Address &dolly);

     //Distruttore
      ~Address();

     //Metodi accessors
      void set_ip(char* ip);
      void set_port(int port);
      char* get_ip();
      int get_port();
      struct sockaddr_in get_address();
      void set_address(struct sockaddr_in address);

      //Serializzatore
      char* toString();
      void show();
};

//Costruttori

Address::Address(){
  this->ip=strdup("0.0.0.0");
  this->port=0;
}

Address::Address (char* ip, int port) {
  this->ip=strdup(ip);
  this->port=port;
}

Address::Address(struct sockaddr_in address){
  this->ip = inet_ntoa(address.sin_addr);
  this->port = ntohs(address.sin_port);
}

Address::Address(const Address &dolly){
  this->ip = strdup(dolly.ip);
  this->port = dolly.port;
}

//Distruttore

Address::~Address(){
  free(this->ip);
}

//Accessors

char* Address::get_ip(){
  return strdup(this->ip); 
}

int Address::get_port(){
  return this->port; 
}

struct sockaddr_in Address::get_address(){ 
  struct sockaddr_in address;
  address.sin_family = AF_INET;
  inet_aton(this->ip,&address.sin_addr);
  address.sin_port=htons(this->port);
  for(int i = 0; i < 8 ; i++) address.sin_zero[i] = 0 ;
  return address; 
}

void Address::set_ip(char* ip){
 free(this->ip);
 this->ip= strdup(ip);
}

void Address::set_port(int port){
  this->port=port;
}

void Address::set_address(struct sockaddr_in address){
  free(this->ip);
  this->ip = strdup(inet_ntoa(address.sin_addr));
  this->port = ntohs(address.sin_port); 
}

//Serializzatori

void Address::show(){
  printf("%s:%d",this->ip,this->port);
}
char* Address::toString(){
  char str[22];
  sprintf(str, "%s:%d",this->ip,this->port);
  return strdup(str);
}


     
