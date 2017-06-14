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

void child(int sd){
    int fd1,child_ret,n,a;
    FILE *fp=NULL;
    pid_t pid;
    FILE *stream;
    char *c;
    char message[100];
    //infinite loop reads command from client's socket
    //excute by fork/exec mechanism
    //then send to client side
    while (1) {
        memset(message,0,100);

        // char cc[10]={"no girl\n"};
        // write(sd,cc,7);

        if((a=read(sd,message,100))<=0){
          //read socket to message
          //then exec message
            close(sd);
            fprintf(stderr, "Bye, client dead, wait for a new client\n");
            //kill(pid,SIGTERM);
            exit(0);
        }
        // printf("HHH%sHHH\n",message );
        //     printf("GGG%cHHH\n",message[0] );
        //
        //         printf("GGG%cHHH\n",message[1] );
        //
        //             printf("GGG%cHHH\n",message[2] );
      //HHHlHHH  ls并没有显示完整
      //如果加上这一句，就只有等server结束才能显示到textview

        pid=fork();
        if(!pid){
            char *delim = " \n\0\t";
            char *a[20];
            a[0]=strtok(message,delim);
            int i =1;
            while((a[i] = strtok(NULL, delim))){
                i++;
            }
            char str[10]="/bin/";
            strcat (str,a[0]);
            if((stream = freopen("1.txt", "w", stdout)) == NULL){

                exit(-1);
            }
            int n=execv(str,a);
            fclose(stdout);
        }else{
            waitpid(pid,&child_ret,0);
            //wait the child process done the execv job
            int i=0;
            fd1 = open ("1.txt",O_RDONLY);
            int filesize = lseek(fd1,0,SEEK_END);
            //fprintf(stderr, "%d\n",filesize );
            close(fd1);
            fp=fopen("1.txt","r");
            c=(char *)malloc(filesize);
            while(fscanf(fp,"%c",c+i)!=EOF){
                i++;
            }
            //read the content
            remove("./1.txt");
            fclose(fp);
          //  fprintf(stderr, "served one client done,success return\n");

            // char cc[10]={"hello girl"};
            //   write(sd,cc,10);
            // only will be called when you enter at least one command


            write(sd,c,strlen(c)+1);
            //write the stdout to the sd(which is the socket)
            // 先传进去 write(sd,c,strlen(c)+1);
        }
    }
}

int main(int argc,char *argv[]){
    int sd,client,portNumber;
    socklen_t len;
    struct sockaddr_in servAdd;
    if(argc!=2){
        fprintf(stderr, "Server Call model:%s<Port#>\n",argv[0]);
        exit(0);
    }
    //create socket
    if((sd=socket(AF_INET,SOCK_STREAM,0))<0){
        fprintf(stderr,"Could not create socket\n");
        exit(1);
    }
    //create server adderss
    servAdd.sin_family=AF_INET;
    servAdd.sin_addr.s_addr=htonl(INADDR_ANY);
    sscanf(argv[1],"%d",&portNumber);
    servAdd.sin_port=htons((uint16_t)portNumber);
    //bind IP and Port
    bind(sd,(struct sockaddr *)&servAdd,sizeof(servAdd));
    //Server start running and wait for connections
    listen(sd,5);
    printf("Waiting for the client...\n");

    while(1){


        //printf("I am searching!!");
        //this will print only when one client connects
        //because if no client, this program
        //will pause at the listen(sd,5)
        //when new client, then program continue,
        //then go to while(1)


        //server gets a client
        client=accept(sd,(struct sockaddr *)NULL,NULL);
        printf("Got a client,wait for get a command\n");




        //give client a hint that he connected
        // char cc[10]={"hello girl"};
        // write(client,cc,10);
        //only when i enter the command from client,this can pass to the client
        //查看lab9的例子，如何直接server给client信息的，在client给server信息之前
        //搞定之后测试Android端能否收到response变量的信息在Android studio console控制台




        //forks and let the child process to take care of the client
        if(!fork())
            child(client);
            //pass the client which is one client socket instance
        close(client);
    }
}
