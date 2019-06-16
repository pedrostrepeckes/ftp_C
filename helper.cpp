#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <sys/un.h>
#include <errno.h>
#include <iostream>
#include <fcntl.h>
using namespace std;
#define MAXBUF 512

struct message{
	char opcode[3];
    char message[MAXBUF];
};


bool enough_number_of_param(int argc){
	if (argc < 4)
	{
		printf("You have to set the Server IP, UP/DW and filename\n");
		cout<<">> Usage: ./client <Server IP address> UP file.txt"<<endl;
		return false;
	}else if(argc > 4){
		printf("Here is more than 4 parameters\n");
		cout<<">> Usage: ./client <Server IP address> UP file.txt"<<endl;
		return false;
	}else{
		return true;
	}
}

bool check_up_dw_option(char *option){
	if(option[0] == 'U' || option[1] == 'P'){
		return true;
	}
	if(option[0] == 'D' || option[1] == 'W'){
		return true;
	}
	cout<<">> Wrong option Upload or Download"<<endl;
	return false;
}

bool check_file(char *filename){
	FILE *file;
	if(file = fopen(filename,"r")){
		fclose(file);
		return true;
	}
	cout<<"File not found"<<endl;
	return false;
}

int trans_type(char opcode[]){
	if(opcode == "UP"){
		return 1;	
	}
	if(opcode == "DW"){
		return 2;
	}
}

void tranfer_data_to_server(char *filename){
	cout<<"oi"<<endl;
	/*int cont, i, src, dest, j, k ;
    int buffer [512];
	printf (" Comecei a copia\n");    
    //sleep (1) ;
    if (argc != 3) 
       {
         printf (" Numero de argumentos insuficientes\n") ;
         exit (1) ;
       } 
    src = open (argv[1],O_RDONLY);
    if (src == -1)
       {
         printf("Impossivel abrir o arquivo %s\n", argv[1]);
         exit (1); 
       }
    dest = creat(argv[2], 0666);
    if (dest == -1)
       {
         printf(" Impossivel criar o arquivo %s\n", argv[2]); 
         exit (1) ;
       }
    
    while ((cont = read(src, &buffer, sizeof(buffer))) > 0 )
            write (dest, &buffer, cont) ;
    printf(" Terminei a copia\n");*/

}