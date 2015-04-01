#pragma once
#ifndef idE4EF1137_8D2A_47C6_AD1C81A112E6BE91
#define idE4EF1137_8D2A_47C6_AD1C81A112E6BE91

#include "Meteron_Link_Test_Common.h"

class Receiver
{
private:
	int						Socket_Desc;
	struct   sockaddr_in 	Source_Addr; 
	bool					Initialized = false;
	
public:
	Receiver();
	Receiver(char * Source_IP_Address, int Source_Port);
	
	void init(char * Source_IP_Address, int Source_Port);
	int receive_Non_Blocking(char * Data, int Data_Size);
	int receive_Blocking(char * Data, int Data_Size);
};

#endif // header
