#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/signal.h>

int main(int argc, char *argv[]) {
    char message[255];
    char message2[1024];
    int server,portNumber;
    socklen_t len;
    struct sockaddr_in servAdd;
    if(argc!=3){
      printf("Client Call model:%s<IP><Port#>\n",argv[0]);
      exit(0);
    }
    //Step1:
    //create socket and store in (int) server
    if((server= socket(AF_INET,SOCK_STREAM,0))<0){
      fprintf(stderr, "cannot create socket\n");
      exit(1);
    }
    //Step2:
    //set server address
    servAdd.sin_family=AF_INET;
    sscanf(argv[2],"%d",&portNumber);
    servAdd.sin_port=htons((uint16_t)portNumber);
    if(inet_pton(AF_INET,argv[1],&servAdd.sin_addr)<0){
      //inet_pton:http://blog.chinaunix.net/uid-27016779-id-3230445.html
      //int inet_pton(int af, const char *src, void *dst);
      //当af = AF_INET
      //src为指向字符型的地址，即ASCII的地址的首地址（ddd.ddd.ddd.ddd格式的)
      //函数将该地址转换为in_addr的结构体，并复制在*dst中
      fprintf(stderr, "inet_pton() has failed\n");
      exit(2);
    }
    //Step3:
    //connect to the server
    if(connect(server,(struct sockaddr *)&servAdd,sizeof(servAdd))<0){
      fprintf(stderr, "connect() failed, exiting\n");
      exit(3);
    }

    //Step4:
    //sent command
    while(1){
        fprintf(stderr,"enter a command for the server or the CTR-D to quit\n");
        if(fgets(message,254,stdin)==NULL){
          //store the command string from the user input to (char string array) message
            close(server);
            exit(0);
        }
        write(server,message,strlen(message)+1);
        //send to the server
        if(read(server,message2,1024)<0){
            fprintf(stderr, "read() error\n");
            exit(3);
        }
        fprintf(stderr, "Server's message:\n%s\n",message2);
    }
}
