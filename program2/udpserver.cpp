/*Kristopher Thieneman
 * kthienem
 * Simple UDP Server that connects with a client. The server looks for a string 
 * sent by the client then adds a time stamp and encrypts the message then sends 
 * it back to the client.
 */
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>

using namespace std;

int main(int argc, char *argv[])
{
	struct sockaddr_in sin, client_addr;
	int len;
	socklen_t addr_len;
	int s, port, hour, min, sec, usec;
	char sendline[10000];
	char recvline[10000];
	char temp[20];
	struct timeval timeRecv;
	time_t rawtime;
	struct tm *timeinfo;
	char end;

	if (argc != 2) {//no port number given
		perror("improper number of arguments");
		exit(1);
	} else {
		port = atoi(argv[1]);
	}

	/*build address data structure*/
	bzero((char *)&sin, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_port = htons(port);

	/*set up passive open*/
	if ((s = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("simplex-talk: socket");
		exit(1);
	}

	if ((bind(s, (struct sockaddr *)&sin, sizeof(sin))) < 0) {
		perror("simplex-talk: bind");
		exit(1);
	}

	addr_len = sizeof(client_addr);

	cout << "Welcome to Simple UDP Server." << endl;
	while(1) {
		if (recvfrom(s, recvline, sizeof(recvline), 0, (struct sockaddr *)&client_addr, &addr_len) == -1) {
			perror("Receive error");
			exit(1);
		}
		time(&rawtime);
		timeinfo = localtime(&rawtime);//get time struct that has hours, mins, and secs
		gettimeofday(&timeRecv, NULL);//gets time of day for microseconds
		hour = timeinfo->tm_hour;
		min = timeinfo->tm_min;
		sec = timeinfo->tm_sec;

		//creates time stamp and appends to string to send
		strcpy(sendline, recvline);
		strcat(sendline, " Timestamp: ");
		sprintf(temp, "%d:%d:%d.%d", hour, min, sec, timeRecv.tv_usec);
		strcat(sendline, temp);
		
		sendline[strlen(sendline)] = 0;
		for (int i = 0; i < strlen(sendline)/2; i++) {//encrypts string
			if (i%2 == 0) {
				char temp = sendline[i];
				sendline[i] = sendline[strlen(sendline)-i-1];
				sendline[strlen(sendline)-i-1] = temp;
			}
		}
		
		if (sendto(s, sendline, strlen(sendline), 0, (struct sockaddr *)&client_addr, sizeof(client_addr)) < 0) {
			perror("sendto() error");
			exit(1);
		}
		bzero((char *)&recvline, sizeof(recvline));//clears recvline
		bzero((char *)&sendline, sizeof(sendline));//clears sendline
	}
	close(s);
}
