
#include<stdio.h>
#include<string.h> //strlen
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

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
		puts("Could not create socket");
		return 1;
	}


    z = setsockopt(socket_desc, SOL_SOCKET, SO_REUSEADDR,&so_reuseaddr, sizeof so_reuseaddr);

    if ( z )
            perror("setsockopt(2)");




	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(port);

	if (bind(socket_desc,(struct sockaddr*)&server, sizeof(server))<0){
		puts("Coult not bind to address");
		return 1;
	}
	listen(socket_desc,3);
	puts("Listening");
	c = sizeof(struct sockaddr_in);
    client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
    if (client_sock < 0){
		puts("accpet failed");
		return 1;
	}
	puts("Connection accepted");

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
	while(1){
	    opensocket(9090);
        pid = fork();
        if (pid==-1){
            puts("Error in fork");
            return 1;
        }else if (pid>0){
            int status;
            waitpid(pid,&status,0);
        }else{
    	    launchKodi();
        }
		
	}
}
