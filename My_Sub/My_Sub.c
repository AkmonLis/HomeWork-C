
#include "MQTTClient.h"
#include "MQTTClientPersistence.h"
#include "pubsub_opts.h"
#include <unistd.h>


#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <sys/select.h>
#include <termios.h>

#define MaxLen 255


#if defined(_WIN32)
#define sleep Sleep
#else
#include <sys/time.h>
#include <time.h>
#endif

volatile int toStop = 0;

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

static struct termios stored_settings;

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


struct pubsub_opts opts =
{
	0, 0, 0, 0, "\n", 100,  	/* debug/app options */
	NULL, NULL, 1, 0, 0, /* message options */
	//~ MQTTVERSION_DEFAULT, NULL, "paho-cs-sub", 0, 0, NULL, NULL, "localhost", "1883", NULL, 10, /* MQTT options */
	MQTTVERSION_DEFAULT, "/node-red/OutData", "My-SUB", 0, 0, "IoT", "student1", "192.168.1.130", "1883", NULL, 10, /* MQTT options */
	NULL, NULL, 0, 0, /* will options */
	0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, /* TLS options */
	0, {NULL, NULL}, /* MQTT V5 options */
	NULL, NULL, /* HTTP and HTTPS proxies */
};


int myconnect(MQTTClient client)
{
	MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
	MQTTClient_SSLOptions ssl_opts = MQTTClient_SSLOptions_initializer;
	MQTTClient_willOptions will_opts = MQTTClient_willOptions_initializer;
	int rc = 0;

	if (opts.verbose)
		printf("Connecting\n");

	if (opts.MQTTVersion == MQTTVERSION_5)
	{
		MQTTClient_connectOptions conn_opts5 = MQTTClient_connectOptions_initializer5;
		conn_opts = conn_opts5;
	}

	conn_opts.keepAliveInterval = opts.keepalive;
	conn_opts.username = opts.username;
	conn_opts.password = opts.password;
	conn_opts.MQTTVersion = opts.MQTTVersion;
	conn_opts.httpProxy = opts.http_proxy;
	conn_opts.httpsProxy = opts.https_proxy;

	if (opts.will_topic) 	/* will options */
	{
		will_opts.message = opts.will_payload;
		will_opts.topicName = opts.will_topic;
		will_opts.qos = opts.will_qos;
		will_opts.retained = opts.will_retain;
		conn_opts.will = &will_opts;
	}

	if (opts.connection && (strncmp(opts.connection, "ssl://", 6) == 0 ||
			strncmp(opts.connection, "wss://", 6) == 0))
	{
		if (opts.insecure)
			ssl_opts.verify = 0;
		else
			ssl_opts.verify = 1;
		ssl_opts.CApath = opts.capath;
		ssl_opts.keyStore = opts.cert;
		ssl_opts.trustStore = opts.cafile;
		ssl_opts.privateKey = opts.key;
		ssl_opts.privateKeyPassword = opts.keypass;
		ssl_opts.enabledCipherSuites = opts.ciphers;
		conn_opts.ssl = &ssl_opts;
	}

	if (opts.MQTTVersion == MQTTVERSION_5)
	{
		MQTTProperties props = MQTTProperties_initializer;
		MQTTProperties willProps = MQTTProperties_initializer;
		MQTTResponse response = MQTTResponse_initializer;

		conn_opts.cleanstart = 1;
		response = MQTTClient_connect5(client, &conn_opts, &props, &willProps);
		rc = response.reasonCode;
		MQTTResponse_free(response);
	}
	else
	{
		conn_opts.cleansession = 1;
		rc = MQTTClient_connect(client, &conn_opts);
	}

	if (opts.verbose && rc == MQTTCLIENT_SUCCESS)
		fprintf(stderr, "Connected\n");
	else if (rc != MQTTCLIENT_SUCCESS && !opts.quiet)
		fprintf(stderr, "Connect failed return code: %s\n", MQTTClient_strerror(rc));

	return rc;
}


void cfinish(int sig)
{
	signal(SIGINT, NULL);
	toStop = 1;
}


void trace_callback(enum MQTTCLIENT_TRACE_LEVELS level, char* message)
{
	fprintf(stderr, "Trace : %d, %s\n", level, message);
}


void printHelp (void)
{
	printf("To change the program configuration you must use keys:\n");
	printf("-h for help.\n");
	printf("-a host IP.\n");
	printf("-l login(username).\n");
	printf("-p password.\n");
	printf("-n name file.\n");
	printf("-t name topic.\n");
	printf("For example: -n 1.txt -a 192.168.1.130 -l IoT -p student1 -t /node-red/temp\n\n");
	printf("press any key to finish");
	
}


int main(int argc, char** argv)
{
	int CoutDataWrite = 0;
	char NameFile[MaxLen] = "datauot.csv";
	
	int rez;
	while ((rez = getopt(argc, argv, "ha:l:p:n:t:")) != -1)
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
					opts.host = optarg;break;
		case 'l' :  if(strlen(optarg)>MaxLen)
					{
						printf("invalid Login");
						return 0;
					}
					opts.username = optarg;break;
		case 'p' :  if(strlen(optarg)>MaxLen)
					{
						printf("invalid pswd");
						return 0;
					}
					opts.password = optarg;break;
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
					opts.topic = optarg;break;
					
		}
	}


	printf("connection parameters\n");
	printf("topic = %s; login = %s;\npassword = %s; NameFale = %s\n",opts.topic,opts.username,opts.password, NameFile);
	printf("Use keys to change parameters. Help -h\n\n");
	printf("press any key to finish\n");
	
	
	
	FILE *file;
    file = fopen(NameFile,"w");
	
	disable_console_canonical();
	
	
	MQTTClient client;
	MQTTClient_createOptions createOpts = MQTTClient_createOptions_initializer;
	int rc = 0;
	char* url;

#if !defined(_WIN32)
    struct sigaction sa;
#endif

	if (strchr(opts.topic, '#') || strchr(opts.topic, '+'))
		opts.verbose = 1;

	if (opts.connection)
		url = opts.connection;
	else
	{
		url = malloc(100);
		sprintf(url, "%s:%s", opts.host, opts.port);
	}
	if (opts.verbose)
		printf("URL is %s\n", url);

	if (opts.tracelevel > 0)
	{
		MQTTClient_setTraceCallback(trace_callback);
		MQTTClient_setTraceLevel(opts.tracelevel);
	}

	if (opts.MQTTVersion >= MQTTVERSION_5)
		createOpts.MQTTVersion = MQTTVERSION_5;
	rc = MQTTClient_createWithOptions(&client, url, opts.clientid, MQTTCLIENT_PERSISTENCE_NONE,
			NULL, &createOpts);
	if (rc != MQTTCLIENT_SUCCESS)
	{
		if (!opts.quiet)
			fprintf(stderr, "Failed to create client, return code: %s\n", MQTTClient_strerror(rc));
		exit(EXIT_FAILURE);
	}


#if defined(_WIN32)
	signal(SIGINT, cfinish);
	signal(SIGTERM, cfinish);
#else
    memset(&sa, 0, sizeof(struct sigaction));
    sa.sa_handler = cfinish;
    sa.sa_flags = 0;

    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
#endif

	if (myconnect(client) != MQTTCLIENT_SUCCESS)
		goto exit;

	if (opts.MQTTVersion >= MQTTVERSION_5)
	{
		MQTTResponse response = MQTTClient_subscribe5(client, opts.topic, opts.qos, NULL, NULL);
		rc = response.reasonCode;
		MQTTResponse_free(response);
	}
	else
		rc = MQTTClient_subscribe(client, opts.topic, opts.qos);
	if (rc != MQTTCLIENT_SUCCESS && rc != opts.qos)
	{
		if (!opts.quiet)
			fprintf(stderr, "Error %d subscribing to topic %s\n", rc, opts.topic);
		goto exit;
	}
	else
	{
		printf("CONNECTED %s\n", opts.host);
	}
	
	while (!toStop)
	{		
		time_t mytime = time (NULL);
		struct tm *now = localtime(&mytime);
		
		char* topicName = NULL;
		int topicLen;
		MQTTClient_message* message = NULL;

		rc = MQTTClient_receive(client, &topicName, &topicLen, &message, 1000);
		
		if(kbhit())
		{
			getchar();
			goto exit;                 
		}
			
			
		if (rc == MQTTCLIENT_DISCONNECTED)
			myconnect(client);
		else if (message)
		{
			char str[255]={0};
			char time[20];
			char date[20];
			
			strftime(date,sizeof(str),"%D",now);
			strftime(time,sizeof(str),"%T",now);

			strcat(str,(char*)message->payload);
						
			fprintf(file,"m\c: %s Time: %s Date: %s\n",str,time,date);

			fflush(stdout);
			MQTTClient_freeMessage(&message);
			MQTTClient_free(topicName);
			printf("Data processed\n");
			CoutDataWrite++;
		}
	}

exit:

    fclose(file);
	MQTTClient_disconnect(client, 0);

	MQTTClient_destroy(&client);
	printf("DISCONNECTED\n");
	
	printf("---Total data received = %d\n---", CoutDataWrite);
	printf("=>press any key to exit the program<=");
	getchar();
	return EXIT_SUCCESS;
}
