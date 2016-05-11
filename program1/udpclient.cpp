//Kristopher Thieneman
//kthienem
/*This program is a simple UDP client. It sends a message to a server which is 
 * determined by inputs from the user. The message is encrypted and then sent back
 * to the client. The program then decrypts the message and prints it to the
 * screen. The RTT for sending the message is also calculated and printed to the
 * screen.*/

/*http://stackoverflow.com/questions/21856025/getting-an-accurate-execution-time-in-c-micro-seconds  this site was used to help figure out how to get the RTT in microseconds*/

#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <fstream>

using namespace std;

int main(int argc, char**argv)
{
	struct hostent *hp;
	int sockfd, n;
	struct sockaddr_in servaddr;
	char *host;
	char sendline[1000];
	char recvline[1000];
	struct timeval timeSend, timeRecv;

	/*check if the correct number of arguments has been given*/
	if (argc == 4){
		host = argv[1];//is correct number of args set host to first argument
	} else {
		cout << "usage: udpcli <IP address>" << endl;
		exit(1);
	}

	/*translate host name into peer's IP address*/
	hp = gethostbyname(host);
	if (!hp) {
		cout << "simplex-talk: unknown host: " << host << endl;
		exit(1);
	}

	/*build address data structure*/
	bzero((char *)&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	bcopy(hp->h_addr, (char *)&servaddr.sin_addr, hp->h_length);
	servaddr.sin_port = htons(atoi(argv[2]));

	/*active open*/
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("simplex-talk: socket");
		exit(1);
	}

	char line[1000];//temporary char array to copy text in file into char array to send to server
	ifstream myfile(argv[3]);
	if (myfile.is_open()) {//try to open thrid argument as file
		/*if file has opened start reading in the text in the file*/
		myfile >> line;
		strcpy(sendline, line);
		/*take in one word at a time with space as a delimeter until end of file*/
		while (myfile >> line) {
			/*concatenate a space and the next line of text to get proper spacing*/
			strcat(sendline, " ");
			strcat(sendline, line);
		}
		myfile.close();
	} else {
		strcpy(sendline, argv[3]);
	}

	gettimeofday(&timeSend, NULL);//get time of day before sending string
	sendto(sockfd, sendline, strlen(sendline), 0, (struct sockaddr *)&servaddr, sizeof(servaddr));
	n = recvfrom(sockfd, recvline, 10000, 0, NULL, NULL);
	gettimeofday(&timeRecv, NULL);//get time of day after response has been received
	recvline[n] = 0;

	/*decrypt response from server*/
	for (int i = 0; i < strlen(recvline)/2; i++) {
		if (i%2 == 0){
			char temp = recvline[i];
			recvline[i] = recvline[strlen(recvline)-i-1];
			recvline[strlen(recvline)-i-1] = temp;
		}
	}
	cout << recvline << endl;//display response
	/*calculate RTT and display*/
	cout << "RTT: " << timeRecv.tv_sec - timeSend.tv_sec << " seconds ";
	cout << timeRecv.tv_usec - timeSend.tv_usec << " microseconds" << endl;
}
