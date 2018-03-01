int main(int argc, char* argv[]) {
	if(argc!=3) {
		printf("USAGE:%s SERVER_IP FILENAME\n",argv[0]);
		return -1;
	}
	
	char* server_ip = argv[1];
	char* file = argv[2];
	
	ClientTCP myself = new ClientTCP();
	
	
}
