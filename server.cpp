#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <strings.h>
#include <cstring>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>

using namespace std;

int PORT;

int sockfd, newsockfd, portno, clilen, n;
struct sockaddr_in serv_addr, cli_addr;

const int SOCKETS = 10;
int sockets[SOCKETS];
int sockets_used = 0;

void start_server() {
	char buffer[256];
	struct sockaddr_in serv_addr, cli_addr;
	int n;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(PORT);
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		perror("ERROR on binding");
		exit(1);
	}
	listen(sockfd,5);
	fcntl(sockfd, F_SETFL, O_NONBLOCK);
}

void accept_connections() {
	while(true) {
		newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, (socklen_t *) &clilen);
		cout<<"Newsockfd: "<<newsockfd<<endl;
		if (newsockfd < 0) break;

		const int BSIZE = 255;
		char buffer[BSIZE];

		// check if interested
		n = read(newsockfd,buffer,BSIZE); 

		if(n < 0) {
			cout<<"CONTINUING"<<endl;
			continue;		
		}

		cout<<"Adding socket"<<endl;

		sockets[sockets_used] = newsockfd;
		++sockets_used;

		fcntl(newsockfd, F_SETFL, O_NONBLOCK);


		const char* message = "Connection established";
		n = write(newsockfd,message,strlen(message));
	}
}

/*
void read_messages() {

	while(n = accept(sockfd, (struct sockaddr *) &cli_addr, (socklen_t *) &clilen)) {
		cout<<"N: "<<n<<endl;
		if(n < 0) break;	
		n = read(n,buffer,BSIZE);
		if(n >= 0) {
			printf("Got: %s\n",buffer);
		}
	}

}
*/

void read_messages() {
	const int BSIZE = 255;
	char buffer[BSIZE];
	for(int i = 0; i < SOCKETS; ++i) {
		if(sockets[i]) {
			const int BSIZE = 255;
			char buffer[BSIZE];
			cout<<"Socket "<<i<<endl;
			/*
			while(read(sockets[i],buffer,BSIZE) >= 0) {
				printf("Length %u\n",(unsigned)strlen(buffer));
				printf("Read %s from %d\n",buffer,sockets[i]);
			}
			*/
			n = read(sockets[i],buffer,BSIZE);
			if(n < 0) continue;
			printf("Read %s from %d\n",buffer,sockets[i]);
		}
	}
}

int main(int argc, char** argv) {
	if(argc < 2) {
		cout<<"Give port."<<endl;
		return 0;
	}
	PORT = atoi(argv[1]);
	start_server();
	while(true) {
		cout<<"Accepting connections again"<<endl;
		accept_connections();
		cout<<"Reading messages"<<endl;
		read_messages();
		sleep(5);
	}
	return 0;
}
