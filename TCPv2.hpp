#ifndef __SOCKETTCP_HPP
#define __SOCKETTCP_HPP

#include <sys/socket.h>
#include <arpa/inet.h>
#include <list>
#include <algorithm>
#include "Address.hpp"
#include "errore.h"

#define IP_LOOPBACK "127.0.0.1"
#define IP_MYSELF "0.0.0.0"
#define MAX_CONN 50
#define MAX_MSG 4096

class SocketTCP{
protected:int sock_id;
public:   SocketTCP(); /* API: socket() */
          ~SocketTCP(); /* API: close() */
          bool broadcast(bool broadcast); /* API: setsockopt() */
};
SocketTCP::SocketTCP(){
          sock_id = socket(AF_INET, SOCK_STREAM, 0);
          if(sock_id==-1)
                    errore(-1,"socket()\n");
}
SocketTCP::~SocketTCP(){
          int ret = close(sock_id);
          if(ret==-1)
                    errore(-2,"close()\n");
}
bool SocketTCP::broadcast(bool broadcast){
          int option_value = broadcast? 1:0;
          int ret = setsockopt(sock_id,
                               SOL_SOCKET,
                               SO_BROADCAST,
                               &option_value,
                               sizeof(int));
          return(ret==-1);
}
//-------------------------------------------------------------------------------------
class Connection{
private:  int conn_id;
          bool fuffa;
public:   Connection(int conn_id, bool fuffa);
          ~Connection(); /* API: shutdown() */
	  int get_conn_id();
          bool invia(char* msg);
          bool invia_raw(void* buffer, int length); /* API: send() */
          char* ricevi();
          char* ricevi_raw(int* length); /* API: recv() */
};
Connection::Connection(int conn_id, bool fuffa){
          this->fuffa = fuffa;
          this->conn_id = conn_id;
}
int Connection::get_conn_id() {
	return this->conn_id;	
}
bool Connection::invia(char* msg){
	return invia_raw(msg,strlen(msg)+1);
}
char* Connection::ricevi(){
	int length;
	char* buffer = ricevi_raw(&length);
	if(buffer!=NULL)
		return NULL;
	*(buffer+length) = '\0';
	return buffer;
}
bool Connection::invia_raw(void* buffer, int length){
	int ret;	
	ret = send( conn_id,
		    buffer,
		    length,
		    0);
	return(ret!=length);
}
void* duplica(char* buffer, int* length){
	char* ret = (char*)malloc(sizeof(char*)*(*length+1));
	for(int i=0;i<=*length;i++)
		*(ret+i)=*(buffer+i);
	return ret;
}
char* Connection::ricevi_raw(int* length){
	char buffer[MAX_MSG+1];
	*length = recv(	conn_id,
			buffer,
			MAX_MSG,
			0);
	return(*length==-1?NULL:duplica(buffer,length));
}
//-------------------------------------------------------------------------------------
class ServerTCP: public SocketTCP{
private:  std::list<Connection> connessioni;
public:   ServerTCP(int port, bool loopback); /* API: bind(),listen() */
          ~ServerTCP(); /* API: close() */
          Connection* accetta(); /* API: accept() */
          void invia_a_tutti(char* msg);
          void disconnetti(Connection connessione);
};
ServerTCP::ServerTCP(int port, bool loopback) {
		Address myself(IP_MYSELF,port);
		sock_id = socket(AF_INET, SOCK_STREAM, 0);
		bind(	sock_id,
			(struct sockaddr)myself.get_address(),
			(socklen_t) sizeof(struct sockaddr));
		listen(sock_id,1);
}
ServerTCP::~ServerTCP(){
          connessioni.clear();
}
Connection* ServerTCP::accetta() {
          Connection* ret;
	  struct sockaddr_in client;
	  int length;
          int conn_id;
          conn_id = accept(sock_id,(struct sockaddr*)&client,(socklen_t*)&length);
          ret = new Connection(conn_id,false);
          connessioni.push_front(*ret);
          return ret;
}
void ServerTCP::invia_a_tutti(char* msg) {
          for(Connection c : connessioni)
                    c.invia(msg);
}
void ServerTCP::disconnetti(Connection connessione){
	for(Connection c : connessioni) {
		if(c.get_conn_id() == connessione.get_conn_id()) {
			connessioni.erase(c); 
		}
	}
}
//-------------------------------------------------------------------------------------
class ClientTCP: public SocketTCP{
private:  Connection* connessione;
public:   ClientTCP(); /* API: socket() */
          ~ClientTCP(); /* API: close() */
          bool connetti(Address server); /* API: connect() */
          bool invia(char* msg);
          bool invia_raw(void* buffer, int length);
          char* ricevi();
          char* ricevi_raw(int* length);
};
bool ClientTCP::connetti(Address server){
          struct sockaddr_in server_addr;
          server_addr = server.get_address();
          
          int ret = connect(sock_id,
                                (struct sockaddr*) &server_addr,
                                (socklen_t)sizeof(struct sockaddr));          
          if(ret) return true;
    
          /*si può passare sock_id perché sul client ha la stessa funzione di conn_id */
          connessione = new Connection(sock_id);
          
          return false;
}
ClientTCP::ClientTCP(): SocketTCP() {
}
bool ClientTCP::invia(char* msg){
          return connessione->invia(msg);
}
char* ClientTCP::ricevi(){
          return connessione->ricevi();
}
bool ClientTCP::invia_raw(void* buffer, int length) {
	  return connessione->invia_raw(buffer, length);
}
char* ClientTCP::ricevi_raw(int* length) {
	  return connessione->ricevi_raw(length);
}






#endif  //__SERVERTCP_HPP
