/** @file Sender_Class.cpp
 *  @brief Definitions for the sending functionality
 *
 *  @author Thomas Krueger
 *  @bug No known bugs.
 */
#include "Sender_Class.h"

Sender::Sender()
{
	Initialized = false;
}

Sender::Sender(char * Destination_IP_Address, int Destination_Port)
{
	init(Destination_IP_Address, Destination_Port);
}

void Sender::init(char * Destination_IP_Address, int Destination_Port)
{
	/////////////
	// Create the socket that sends
	
    Socket_Desc = socket(AF_INET,SOCK_DGRAM,0);
    if (Socket_Desc < 0)
    {
        printf("Error opening socketin %s %s\n", __FILE__, __FUNCTION__);
		exit(EXIT_FAILURE);
    }
   
    // Clear the socket address structure before writing to it
    memset(&Dest_Addr,0,sizeof(struct sockaddr_in));
    
    // Initialize the destination address
    Dest_Addr.sin_family = AF_INET;
    Dest_Addr.sin_addr.s_addr = inet_addr(Destination_IP_Address);
    Dest_Addr.sin_port = htons(Destination_Port);
	Initialized = true;
}

int Sender::transmit(char * Data, int Data_Length)
{
	int		result;
	
	//check if initalized
	if (!Initialized)
	{
		printf("receiver needs to be initialzed first\n");
		exit(EXIT_FAILURE);
	}
	
	result = 
	sendto
	(	Socket_Desc,
		Data,
		Data_Length,
		0,
		(const sockaddr*) &Dest_Addr,
		sizeof(struct sockaddr_in));
	if (result < 0)
	{
		printf("Error sending data in %s %s\n",  __FILE__, __FUNCTION__);
		return result;
	}
	return result;
}

int Sender::transmit_by_addr(char * Data, int Data_Length)
{
  int     result;

   if (!Initialized)
   {
      printf("receiver needs to be initialzed first\n");

      result =
      sendto
      (   Socket_Desc,
          Data,
          Data_Length,
          0,
          (const sockaddr*) &Dest_Addr,
          sizeof(struct sockaddr_in));
      if (result < 0)
      {
          printf("Error sending data in %s %s\n",  __FILE__, __FUNCTION__);
          return result;
      }
      return result;

}

}


