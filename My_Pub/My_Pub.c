#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MQTTClient.h"
#include <unistd.h> 
#include <sys/select.h>
#include <termios.h>


#define MaxLen 255
#define CLIENTID    "My_Pub"
#define QOS         1
#define TIMEOUT     10000L

static struct termios stored_settings;

void printHelp (void)
{
	printf("To change the program configuration you must use keys:\n");
	printf("-h for help.\n");
	printf("-a host IP:port.\n");
	printf("-l login(username).\n");
	printf("-p password.\n");
	printf("-n name file.\n");
	printf("-t name topic.\n");
	printf("-s parcel interval in seconds \n");
	printf("For example: -n 1.txt -a 192.168.1.130:1883 -l IoT -p student1 -t /node-red/temp -s 5\n\n");
	printf("press any key to finish");
	
}

int kbhit()
{
   struct timeval tv;
   fd_set fds;
   tv.tv_sec = 0;
   tv.tv_usec = 0;
   FD_ZERO(&fds);
   FD_SET(STDIN_FILENO, &fds);
   select(STDIN_FILENO+1, &fds, NULL, NULL, &tv);
   return FD_ISSET(STDIN_FILENO, &fds);
}

void disable_console_canonical (void)
{
   struct termios new_settings;

   tcgetattr(0, &stored_settings);

   new_settings = stored_settings;

   new_settings.c_lflag &= (~ICANON);
   new_settings.c_cc[VTIME] = 0;
   new_settings.c_cc[VMIN] = 1;

   tcsetattr(0, TCSANOW, &new_settings);
   return;
}

void waiting(int t)
{
	while(t>0)
	{
		t--;
		sleep(1);
		if (kbhit())
		{
			return;
		}
	}
	
}
int main(int argc, char* argv[])
{
	int CoutDataWrite = 0;
	char NameFile[MaxLen] = "Data.txt";
	char Address [MaxLen] = "192.168.1.130:1883";
	char Topic   [MaxLen] = "/node-red/inData";
	char Payload [MaxLen] = " ";
	char Login   [MaxLen] = "IoT";
	char Pswd    [MaxLen] = "student1";
	int  time             = 5;
	
	int rez;
	while ((rez = getopt(argc, argv, "ha:l:p:n:t:s:")) != -1)
	{
		switch(rez)
		{
		case 'h' : printHelp();return 0;
		case '?' : printf("unknown argument, please read help option -h");return 0;
		case 'a' : 
					if(strlen(optarg)>MaxLen)
					{
						printf("invalid IP");
						return 0;
					}
					strcpy(Address,optarg);break;
		case 'l' :  if(strlen(optarg)>MaxLen)
					{
						printf("invalid Login");
						return 0;
					}
					strcpy(Login,optarg);break;
		case 'p' :  if(strlen(optarg)>MaxLen)
					{
						printf("invalid pswd");
						return 0;
					}
					strcpy(Pswd,optarg);break;
		case 'n' :  if(strlen(optarg)>MaxLen)
					{
						printf("invalid name file");
						return 0;
					}
					strcpy(NameFile,optarg);break;
					
		case 't' :  if(strlen(optarg)>MaxLen)
					{
						printf("invalid name topic");
						return 0;
					}
					strcpy(Topic,optarg);break;
		case 's' :  if(strlen(optarg)>10)
					{
						printf("invalid time");
						return 0;
					}
					time = atoi(optarg);break;
					
		}
	}


	printf("connection parameters:\n");
	printf("topic = %s; login = %s;\npassword = %s;nameFale = %s; time = %d sec\n ",Topic,Login,Pswd, NameFile, time);
	printf("Use keys to change parameters. Help -h\n\n");
	
	FILE *file;
    if (!(file = fopen(NameFile,"r")))
    {
		printf("Error opening file");
		return 0;
	}
	

	printf("press any key to finish\n");
	
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;
    int rc;

    MQTTClient_create(&client, Address, CLIENTID,
        MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;
    conn_opts.username = Login;
    conn_opts.password = Pswd;

    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to connect, return code %d\n", rc);
        exit(-1);
    }
    else
	{
		printf("CONNECTED %s\n", Address);
	}
    
    signed char c;
    c=fgetc(file);
    fseek(file,-1, SEEK_CUR);
    
    disable_console_canonical();
    while (!kbhit())
    {
		
		
		fscanf(file,"%[^\n]",Payload);

		fseek(file,1, SEEK_CUR);

		if((c=fgetc(file)) == EOF)
		{
			//fseek(file,0, SEEK_SET);                                  //reading a file from the beginning
			break;														//comment out for reading a file from the beginning
		}
		fseek(file,-1, SEEK_CUR);
		
		pubmsg.payload = Payload;
		pubmsg.payloadlen = strlen(Payload);
		pubmsg.qos = QOS;
		pubmsg.retained = 0;
		MQTTClient_publishMessage(client, Topic, &pubmsg, &token);
		rc = MQTTClient_waitForCompletion(client, token, TIMEOUT);
		printf("Message with delivery token %d delivered\n", token);
		
		CoutDataWrite++;
		waiting(time);	
	}
	if (kbhit())
	{
		getchar();
	}
	printf("---Total data transferred = %d\n---", CoutDataWrite);
	printf("=>press any key to exit the program<=");
	fclose(file);
	getchar();
    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
    return rc;
}
