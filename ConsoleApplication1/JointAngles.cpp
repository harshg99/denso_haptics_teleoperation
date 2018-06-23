#include"stdafx.h"
#include <stdio.h>

#include "dhdc.h"
#include "drdc.h"

//header files for communication
#include<WinSock2.h>
#include <iostream>
#include <WS2tcpip.h>

//header files for string computations
#include<conio.h>
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<iostream>
#include<sstream>

//to include the winsock library
#pragma comment(lib, "ws2_32.lib")

#define k 10000

int computeForce(double px, double py, double pz, double *fx, double *fy, double *fz) {
	
	*fx  = k * px;
	*fy = k * py;
	*fz = k * pz;

	if (pz > 0.05) return 1;
	else
	return 0;
	
}

//class for communicating the joint angles locally
class comm_TCP {
	private:
		//private datatype to send data
		char sendData[64];
		SOCKET sserver;
		sockaddr_in server;
		double dP(double val) {
			return ((double)(((int)((val*180/3.14159) * 1000)))) / (double)1000;
		}

	public:
		//converts the double value to three decimal places for transfer 
		
		//initialises data to string
		void initData(double angles[DHD_MAX_DOF]) {
			std::string data="";
			
			for (int i = 0; i < DHD_MAX_DOF - 1; i++) {
				std::stringstream ss;
				ss << dP(angles[i]);
				data += ss.str() + ",";
				ss.clear();
			}
			printf("Output String: %s\n", data.c_str());
		}

		comm_TCP() {
			//some varaibles to intialise winsock stream (Windows Socket Implementation)
			WSADATA data;
			//further intialising
			WORD version = MAKEWORD(2, 2);

			// Start WinSock
			int wsOk = WSAStartup(version, &data);
			if (wsOk != 0)
			{
				// Not ok! Get out quickly
				std::cerr<< "Can't start Winsock! " << wsOk;
			}
		}

		boolean SocketBind() {
			//TCP is a protocol that facilitates data transfer
			//need to initialise socket to the required address with port number to enable connection


			// Create a hint structure for the server
		

			//filling the socket adrres structure with zeros to prevent garbage values
			memset(&server, 0, sizeof(server));
			server.sin_family = AF_INET; // AF_INET = IPv4 addresses
			server.sin_port = htons(34398); // Little to big endian conversion which is the TCP/IP network byte order
			server.sin_addr.S_un.S_addr = INADDR_ANY;
											//change the address accordingly for communication
			//inet_pton(AF_INET, INADDR_ANY, &server.sin_addr); // Convert from string to byte array : aloocating an adress
															  // Socket creation, note that the socket type is datagram
			SOCKET sserver = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			if (sserver == INVALID_SOCKET) {
				std::cerr<<"Could not send: Sockent not intialised\n";
				closesocket(sserver);

				// Close down Winsock
				WSACleanup();
				return false;
			}

			//bind links the socket we just created with the sockaddr_in 
			//structure. Basically it connects the socket with 
			//the local address and a specified port.
			//If it returns non-zero quit, as this indicates error
			if (bind(sserver, (sockaddr*)&server, sizeof(server)) != 0)
			{
				closesocket(sserver);
				std::cerr << "Bound error\n";
				// Close down Winsock
				WSACleanup();
				return 0;
			}
			printf("Bound   ");
			//listen instructs the socket to listen for incoming 
			//connections from clients. The second arg is the backlog
			if (listen(sserver, 10) != 0)
			{
				closesocket(sserver);
				std::cerr << "Error in listening\n";
				// Close down Winsock
				WSACleanup();
				return 0;
			}
			printf("Listening    ");
			fflush(stdin);
		}


		boolean Data(){

		//	listening to a connection
		
		sockaddr clientadd;
		int c = sizeof(clientadd);
		SOCKET client = accept(sserver, (sockaddr *)&clientadd,&c);
		if (client = INVALID_SOCKET) {	
			printf("Error in client recieve\n");	
			return 0;
		}
		if (send(client, sendData, sizeof(sendData), 0) == SOCKET_ERROR) {
			printf("Data Not Sent");
			return false;
		}
		fflush(stdin);
		printf("Sent  \n");
		closesocket(client);
		return true;
		// Close the socket
	
	}
		~comm_TCP() {
			closesocket(sserver);
			// Close down Winsock
			WSACleanup();
			
		}
};
double toDeg(double rad) {
	return rad * (180 / 3.14159);
}

int main(int argc, char **argv) {

	int    done = 0;
	double px, py, pz;
	double fx, fy, fz;
	double oa, ob, og;
	double f[DHD_MAX_DOF];
	double p[DHD_MAX_DOF];
	double angles[DHD_MAX_DOF];
	double forces[3];
	double AngX, AngZ, AngDev, AngGrip;

	if (dhdGetDeviceCount() <= 0) {
		printf("error: no device found (%s)\n", dhdErrorGetLastStr());
		return 0;
	}
	if (dhdOpen() < 0) {
		printf("error: cannot open device\n");
		return 0;
	}

	if (dhdEmulateButton(1) < 0) {
		printf("error: could not emulate button");
		return 0;
	}
	
	dhdEnableExpertMode();

	comm_TCP TCP_Object;
	TCP_Object.SocketBind();

	int ctr = 0;
	while (ctr<100 || done!=1) {
			
		dhdGetPositionAndOrientationDeg(&px, &py, &pz, &oa, &ob, &og);
		done = computeForce(px, py, pz, &fx, &fy, &fz);
		printf("Position: %lf %lf %lf %lf %lf %lf \n", px, py, pz, oa, ob, og);
		//dhdEnableExpertMode();
		dhdGetJointAngles(angles);
		for (int i = 0; i < DHD_MAX_DOF - 1; i++)
			printf(" Joint Angles:%d %lf\n ",i+1, toDeg(angles[i]));

		//communications
	
		TCP_Object.initData(angles);
		TCP_Object.Data();
			
		//dhdDisableExpertMode();
		/*dhdGetBaseAngleXDeg(&AngX);
		dhdGetBaseAngleZDeg(&AngZ);
		dhdGetDeviceAngleDeg(&AngDev);
		
		printf("Position: %lf %lf %lf %lf \n", AngX, AngZ, AngDev, AngGrip);*/
		//apply forces
		dhdSetForce(fx, fy, fz);
		//dhdGetGripperAngleDeg(&AngGrip);
		//if (AngGrip > 25) done = 1;
		done += dhdGetButton(0);
		dhdSleep(1);
		ctr++;
	}
	printf("exiting application\n");
	dhdDisableExpertMode();
	dhdStop();
	dhdClose();


	
	return 0;
}