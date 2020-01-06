#include <iostream>
#include <wiringPi.h>
#include "SonarLibrary/libSonar.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
using namespace std;

int trigger = 1;
int echo = 5;
int pir = 0;

void error_handling(char *message);
void quit_routine_listener(int   sock,
                  char *buf);
void write_routine(int   sock,
                   char *buf,Sonar sonar);

int main(int argc, char *argv[])
{
    
  int sock;
  struct sockaddr_in serv_addr;

if (wiringPiSetup() == -1)
        return -1;
 pinMode(pir,INPUT);
 Sonar sonar;
 sonar.init(trigger, echo);

    
  if (argc != 3) {
    printf("Usage : %s <IP> <port>\n", argv[0]);
    exit(1);
  }

  sock = socket(PF_INET, SOCK_STREAM, 0);

  if (sock == -1) {
    error_handling("socket() error");
  }

  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family      = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
  serv_addr.sin_port        = htons(atoi(argv[2]));

  if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
    error_handling("connect() error!");
  } else {
    puts("Connected.....");
  }
cout<<digitalRead(pir);
  char msg[BUF_SIZE] = "";

    cout<<sonar.distance(30000);

  pid_t pid = fork();
  if (pid == 0) {
    write_routine(sock, msg,sonar);
  } else {
    quit_routine_listener(sock, msg);
  }

  close(sock);
  
    return 0;
}





void quit_routine_listener(int sock, char *buf)
{
  int read_len = 0;

  while (1)
  {
    read_len = read(sock, buf, BUF_SIZE - 1);

    if (read_len == 0) {
      exit(0);
    }

  }
}

void write_routine(int sock, char *buf,Sonar sonar)
{
  while (1)
  {
    //fgets(buf, BUF_SIZE, stdin);
    //buf=(char*)&sonar.distance(30000);
    sprintf(buf,"%f,%d",sonar.distance(30000),digitalRead(pir));
    if (!strcmp(buf, "q\n") || !strcmp(buf, "Q\n")) {
      shutdown(sock, SHUT_WR);
      return;
    }
    if(strlen(buf)<=10 && strlen(buf)>=9)
        cout<<buf<<endl;
        write(sock, buf, strlen(buf));
    delay(220);
  }
}

void error_handling(char *message)
{
  fputs(message, stderr);
  fputc('\n', stderr);
  exit(1);
}
