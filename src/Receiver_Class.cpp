 /** @file Receiver_Class.cpp
 *  @brief Implementations for the receiving functionality
 *
 *  @author Thomas Krueger
 *  @bug No known bugs.
 */
#include "Receiver_Class.h"

Receiver::Receiver()
{
	Initialized = false;
}

Receiver::Receiver(char * Source_IP_Address, int Source_Port)
{
	init(Source_IP_Address, Source_Port);
}

void Receiver::init(char * Source_IP_Address, int Source_Port)
{
	int result;
	
	/////////////////
	//create the socket that listens
	
	Socket_Desc = socket(AF_INET,SOCK_DGRAM,0);
    if (Socket_Desc < 0)
    {
        printf("Error opening socketin %s %s at line: %d\n", __FILE__, __FUNCTION__, __LINE__);
		exit(EXIT_FAILURE);
    }
   
    // Clear the socket address structure before writing to it
    memset(&Source_Addr, 0, sizeof(struct sockaddr_in));
    
    // Initialize the address
    Source_Addr.sin_family = AF_INET;
    Source_Addr.sin_addr.s_addr = inet_addr(Source_IP_Address);
    Source_Addr.sin_port = htons(Source_Port);
    
    result = bind
	(	Socket_Desc,
		(const sockaddr*) &Source_Addr,
		sizeof(Source_Addr)
	);
    if (result != 0)
    {
       printf("Socket bindinf receiver class failed! \n");
	   exit(EXIT_FAILURE);
    }
	Initialized = true;
}


int Receiver::receive_Non_Blocking(char * Data, int Data_Size)
{
	int 			result;
	fd_set          rfds;
    struct timeval	tv;
	int             Bytes_Recieved;
	
	tv.tv_sec = 0;
	tv.tv_usec = 0;
	
	//check if initalized
	if (!Initialized)
	{
		printf("receiver needs to be initialzed first\n");
		exit(EXIT_FAILURE);
	}
	
	do
	{
		tv.tv_sec = 0;
		tv.tv_usec = 0;
		FD_ZERO(&rfds);
		FD_SET(Socket_Desc, &rfds);
		result = select(Socket_Desc+1, &rfds, NULL, NULL, &tv);
		if (result == -1)
		{
			printf("select failed\n");
		}
		else if (result) //data availbale
		{
			Bytes_Recieved = recv(Socket_Desc, Data, Data_Size, 0);
		}
	}while(result>0);
	return 0;
}

int Receiver::receive_Blocking(char * Data, int Data_Size)
{
	int 			result;
	fd_set          rfds;
    struct timeval	tv;
	int             Bytes_Received;
	
	//check if initalized
	if (!Initialized)
	{
		printf("receiver needs to be initialzed first\n");
		exit(EXIT_FAILURE);
	}
	
	do
	{
		tv.tv_sec = 60*60*24; //wait for one day
		tv.tv_usec = 0;
		FD_ZERO(&rfds);
		FD_SET(Socket_Desc, &rfds);
		result = select(Socket_Desc+1, &rfds, NULL, NULL, &tv);
		if (result == -1)
		{
			printf("select failed\n");
		}
		else if (result) //data availbale
		{
			Bytes_Received = recv(Socket_Desc, Data, Data_Size, 0);
			return Bytes_Received;
		}
	}while(result>0);
	return 0;
}