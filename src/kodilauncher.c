
#include<stdio.h>
#include<string.h> //strlen
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<syslog.h>

int opensocket(int port){
	int socket_desc;
	int client_sock;
	int c;
	struct sockaddr_in server, client;
    int so_reuseaddr=1;
    int z;
    char msg[2000];


	socket_desc = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_desc == -1){
		syslog(LOG_INFO,"Could not create socket");
		return 1;
	}


    z = setsockopt(socket_desc, SOL_SOCKET, SO_REUSEADDR,&so_reuseaddr, sizeof so_reuseaddr);

    if ( z ) syslog(LOG_ERR,"setsockopt error");




	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(port);

	if (bind(socket_desc,(struct sockaddr*)&server, sizeof(server))<0){
		syslog(LOG_ERR,"Coult not bind to address");
		return 1;
	}
	listen(socket_desc,3);
	syslog(LOG_INFO,"Listening, waiting for Kore mobile app to start");
	c = sizeof(struct sockaddr_in);
    client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
    if (client_sock < 0){
		syslog(LOG_INFO,"accept failed");
		return 1;
	}
	syslog(LOG_INFO,"Connection accepted");

    recv(client_sock, msg, 2000, 0);

	close(client_sock);
	close(socket_desc);
	return 0;
}

int isKodiRunning(){
	return 0;
}

int launchKodi(){
    execv("/usr/bin/kodi",NULL);
	return 1;
}

int main(int argc, char *argv[]){
    pid_t parent = getpid();
    pid_t pid;
    openlog("Kodilauncher", LOG_PID|LOG_CONS, LOG_USER);
	while(1){
	    opensocket(9090);
        pid = fork();
        if (pid==-1){
            syslog(LOG_ERR,"Error in fork");
            return 1;
        }else if (pid>0){
            int status;
            waitpid(pid,&status,0);
        }else{
            syslog(LOG_INFO,"Launching Kodi");
    	    launchKodi();
        }
		
	}
    closelog();
}
