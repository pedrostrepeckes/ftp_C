#include "helper.cpp"

//#define MAXBUF 65536

int main(int argc, char *argv[])
{
  int sock, status, buflen;
  unsigned sinlen;
  struct message buffer;
  struct sockaddr_in sock_in;
  int yes=1;
  char answer_server_opcode[2];
  char answer_server_message[MAXBUF];

  if(enough_number_of_param(argc) == false 
    || check_up_dw_option(argv[2]) == false 
    || check_file(argv[3]) == false){
    exit(1);
  }

  //Socket set up
  sinlen = sizeof(struct sockaddr_in);
  memset(&sock_in, 0, sinlen);
  buflen = MAXBUF;

  sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

  //sock_in.sin_addr.s_addr = htonl(-1);
  sock_in.sin_port = htons(46160);
  sock_in.sin_family = AF_INET;

  //Socket bind
  status = bind(sock, (struct sockaddr *)&sock_in, sinlen);
  cout<<">> Bind status: "<<status<<endl;

  status = setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &yes, sizeof(int));
  cout<<">> Setsockopt status: "<<status<<endl;


  //Send argument (server IP) to the buffer
  sprintf(buffer.opcode, argv[2]);
  sprintf(buffer.message, argv[1]);
  
  //buflen = strlen(buffer);
  buflen = sizeof(buffer);
  //Send buffer with IP to look for the FTP server
  cout<<">> Message sent: "<<buffer.message<<endl;
  status = sendto(sock, &buffer, buflen, 0, (struct sockaddr *)&sock_in, sinlen);
  cout<<">> Sendto status: "<<status<<endl;

  //Receive response from server
  //memset(buffer,'\0',buflen);
  
  status = recvfrom(sock, &buffer, buflen, 0, (struct sockaddr *)&sock_in, &sinlen);
  //strncpy(answer_server_opcode,buffer.opcode,2);
  answer_server_opcode[0] = buffer.opcode[0];
  answer_server_opcode[1] = buffer.opcode[1];
  answer_server_opcode[2] = '\0';
  //strcpy(answer_server_message,buffer.message);
  cout<<">> Message received: "<<answer_server_opcode<<endl;
  if(strcmp(answer_server_opcode,"OK\0") == 0){
    cout<<">> Recvfrom status: "<<status<<endl;
    cout<<">> Message received: "<<answer_server_opcode<<endl;
    int src;
    src = open ("teste.txt",O_RDONLY);
    if (src == -1)
       {
         printf("Impossivel abrir o arquivo %s\n", argv[1]);
         exit (1); 
       }
    int cont;
    cont = read(src, &buffer.message, sizeof(buffer.message));
    status = sendto(sock, &buffer, buflen, 0, (struct sockaddr *)&sock_in, sinlen);
    //while ((cont = read(src, &buffer.message, sizeof(buffer.message))) > 0 )
    //        write (dest, &buffer.message, cont) ;
    //tranfer_data_to_server(argv[3]);
  }else{
    cout<<"There is a problem with the server"<<endl;
  }

  //Close socket
  shutdown(sock, 2);
  close(sock);
}
