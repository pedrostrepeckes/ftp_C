#include "helper.cpp"
//#define MAXBUF 65536

int main(int argc, char **argv)
{
  //Socket variables
  int sock, status, buflen;
  unsigned sinlen;
  bool control_comm;
  char server_path[50];
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
  cout<<buffer.filename<<endl;
  if (check_up_dw_option(buffer.opcode) == 1){
    cout<<"upload"<<endl;
    strcpy (buffer.opcode,"OK");
    strcpy (buffer.message,"You can upload");
    //Send response to client with the same data (echo)
    cout<<">> Sending response to client (echo)"<<endl;
    status = sendto(sock, &buffer, buflen, 0, (struct sockaddr*) &clientSocket, sinlen); //Only works with 'status' instead of buflen
    //cout<<">> Sendto status: "<<status<<buflen<<endl;
    int dest;
    strcpy(server_path, "Servidor/");
    strcat(server_path, buffer.filename);
    dest = creat(server_path, 0666);
    if (dest == -1)
        {
          printf(" Impossivel criar o arquivo %s\n", buffer.filename); 
          exit (1) ;
        }
    status = recvfrom(sock, &buffer, buflen, 0, (struct sockaddr *)&clientSocket, &sinlen);
    control_comm = true;
    //copy
    while(control_comm){
      //buflen = strlen(buffer.message);
      write (dest, &buffer.message, buffer.control);
      sprintf(buffer.message, " "); 
      status = recvfrom(sock, &buffer, buflen, 0, (struct sockaddr *)&clientSocket, &sinlen);
      //cout<<buffer.message<<endl;
      if (strcmp(buffer.message, "Finalizou") == 0){
        control_comm = false;
        close(dest);
      }
    }
  }
  else if (check_up_dw_option(buffer.opcode) == 2){
    cout<<"download"<<endl;
    if (check_file(buffer.filename) == true){
      strcpy (buffer.opcode,"OK");
      strcpy (buffer.message,"You can download");
      int src;
      src = open (buffer.filename,O_RDONLY);
      if (src == -1)
        {
          printf("Impossivel abrir o arquivo %s\n", buffer.filename);
          exit(1); 
        }
      sprintf(buffer.message, "Iniciando download");
      status = sendto(sock, &buffer, buflen, 0, (struct sockaddr*) &clientSocket, sinlen); 
      int cont;
      //copy
      while ((cont = read(src, &buffer.message, sizeof(buffer.message))) > 0 ){
        buffer.control = cont;
        status = sendto(sock, &buffer, buflen, 0, (struct sockaddr*) &clientSocket, sinlen); 
        sleep(0.01);
      }
      //status = sendto(sock, &buffer, buflen, 0, (struct sockaddr*) &clientSocket, sinlen); 
      cout<<"Finalizou"<<endl;
      close(src);
      sprintf(buffer.message, "Finalizou");
      status = sendto(sock, &buffer, buflen, 0, (struct sockaddr*) &clientSocket, sinlen); 
    }
  }  
  //Close socket
  shutdown(sock, 2);
  close(sock);
}
