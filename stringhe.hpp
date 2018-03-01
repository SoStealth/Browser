/*
stringhe.h
*/
char* concat(char* first_string,char* second_string) {
	char* ret=(char*)malloc(strlen(first_string)+strlen(second_string)+1);
	char* tmp = ret;

	for(;*tmp = *first_string; tmp++, first_string++) { }
	for(;*tmp = *second_string; tmp++, second_string++) { }

	return ret;
}
