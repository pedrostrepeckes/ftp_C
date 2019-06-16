#include "helper.cpp"
//#define MAXBUF 65536

int main(int argc, char **argv)
{
  //Socket variables
  int sock, status, buflen;
  unsigned sinlen;
  //socklen_t sinlen;
  struct sockaddr_in sock_in, clientSocket;
  //char buffer[MAXBUF]; //Where the message will be stored
  struct message buffer;

  //Get my IP variables
  FILE *f;
  char ip[30];

  //Socket set up
  sinlen = sizeof(struct sockaddr_in);
  memset(&sock_in, 0, sinlen);

  sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

  sock_in.sin_addr.s_addr = htonl(INADDR_ANY);
  sock_in.sin_port = htons(46160);
  sock_in.sin_family = AF_INET;

  //Socket bind
  status = bind(sock, (struct sockaddr *)&sock_in, sinlen);
  cout<<">> Bind status: "<<status<<endl;

  status = getsockname(sock, (struct sockaddr *)&sock_in, &sinlen);
  cout<<">> Socket port: "<<htons(sock_in.sin_port)<<endl;

  buflen = sizeof(buffer);
  //memset(&buffer, 0, buflen);

  //Receive message
  status = recvfrom(sock, &buffer, buflen, 0, (struct sockaddr *)&clientSocket, &sinlen);
  cout<<">> Recvfrom status: "<<status<<endl;
  cout<<">> Message received: "<<buffer.message<<endl;
  cout<<">> Message received from: "<<inet_ntoa(clientSocket.sin_addr)<<ntohs(clientSocket.sin_port)<<endl;

  //Get own IP, compare with IP received in the message, check if it's mine
  cout<<">> Getting own IP"<<endl;
  system("ifconfig | grep inet | tail -2|head -1|awk '{print$2}' > ip.txt");

	f = fopen ("ip.txt", "r");
    while(!feof(f)){
		fscanf(f, "%s", &ip);
	}

  cout<<">> Own IP is: "<<ip<<endl;
  cout<<">> Checking if IP received is equals to own IP"<<endl;

  if (strstr(ip,buffer.message))
  {
    cout<<">> IPs match"<<endl;
    //cout<<buffer.message<<endl;
  }else{
    cout<<">> IPs do not match"<<endl;
    cout<<">> bye bye"<<endl;
    //exit(1);
  }

  //if it is a download command, is there the file?
  if(trans_type(buffer.opcode)==2){
    //check_file
  }

  strcpy (buffer.opcode,"OK");
  strcpy (buffer.message,"Teste22");
  //Send response to client with the same data (echo)
  cout<<">> Sending response to client (echo)"<<endl;
  status = sendto(sock, &buffer, buflen, 0, (struct sockaddr*) &clientSocket, sinlen); //Only works with 'status' instead of buflen
  //cout<<">> Sendto status: "<<status<<buflen<<endl;
  int dest;
  dest = creat("vivendo.txt", 0666);
  if (dest == -1)
       {
         printf(" Impossivel criar o arquivo %s\n", argv[2]); 
         exit (1) ;
       }
  status = recvfrom(sock, &buffer, buflen, 0, (struct sockaddr *)&clientSocket, &sinlen);
  buflen = sizeof(buffer.message);
  write (dest, &buffer.message, buflen) ;
  //Close socket
  shutdown(sock, 2);
  close(sock);
}
