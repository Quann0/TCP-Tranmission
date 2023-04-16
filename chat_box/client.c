#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <arpa/inet.h>
#include <errno.h>
#include <pthread.h>

 int sockfd, portno = 1300, n;
    struct sockaddr_in serv_addr; //Cau truc chua dia chi server ma client can biet de ket noi toi

    char sendbuff[256];
    char recvbuff[256];

void *my_thread_read(void *arg)
{
while(1){
        memset(recvbuff, 0, 256);
      //read data from socket
      n = read(sockfd,recvbuff,255);    //block den khi co du lieu tu client gui toi
      if (n < 0) perror("ERROR reading from socket");

         printf("Recv: %s\n",recvbuff);
}
      //write data via socket
    //  strcpy(sendbuff, "Server has got message\n");
    //  n = write(newsockfd,sendbuff,sizeof(sendbuff));
    //  if (n < 0) perror("ERROR writing to socket");
}
void *my_thread_send(void *arg)
{
while(1){
        memset(recvbuff, 0, 256);
        fgets(recvbuff, 255, stdin);
      //read data from socket
      n = write(sockfd,recvbuff,255);    //block den khi co du lieu tu client gui toi
      if (n < 0) perror("ERROR reading from socket");
}
      //   printf("Message from client: %s\n",recvbuff);
}



int main(int argc, char *argv[])
{
pthread_t thread1;
	pthread_t thread2;
char *a = (char *)malloc(100 * sizeof(char));
	memset(a, 0, 100);	
//    int sockfd, portno = 1300, n;
//    struct sockaddr_in serv_addr; //Cau truc chua dia chi server ma client can biet de ket noi toi

 //   char sendbuff[256];
 //   char recvbuff[256];

    //Client nhan tham so hostname va port tu dong lenh
   // if (argc < 3) {
   //    fprintf(stderr,"usage %s hostname port\n", argv[0]);
   //    exit(0);
   // }
   // portno = atoi(argv[2]); //Chuyen cong dich vu thanh so nguyen
   
   //Tao socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        perror("ERROR opening socket");
      
    
    memset(&serv_addr, '0', sizeof(serv_addr));

    //Thiet lap dia chi cua server de ket noi den
    serv_addr.sin_family = AF_INET;        //Mac dinh
    serv_addr.sin_port = htons(portno);    //Cong dich vu
   
    //Dia chi ip/domain may chu
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
    {
        printf("\n inet_pton error occured\n");
        return 1;
    }

    //Goi ham connect de thuc hien mot ket noi den server
    if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
       printf("\n Error : Connect Failed \n");
       return 1;
    }
        //create thread_1	
	pthread_create(&thread1, NULL, my_thread_read, "thread 1!");
	//create thread_2
	pthread_create(&thread2, NULL, my_thread_send, "thread 2!");
   // printf("Please enter the message: ");
    pthread_join(thread1, &a);//a is return value (void **)
	pthread_join(thread2, &a);//a is return value(void **)
   // memset(sendbuff, 0, 256); //Khoi tao buffer
   // fgets(sendbuff,255,stdin); //Chua thong diep doc tu ban phim 
    
    //Gui du lieu den server bang cach ghi ra socket vua thiet lap
/*    
n = write(sockfd,sendbuff,strlen(sendbuff));
    if (n < 0) 
         perror("ERROR writing to socket");
    
    //Nhan du lieu tu server
    memset(recvbuff, 0, 256);
    n = read(sockfd,recvbuff,255);
    if (n < 0) 
         perror("ERROR reading from socket");
    printf("%s\n",recvbuff);
*/   
    close(sockfd); //Dong socket 
    return 0;
}
