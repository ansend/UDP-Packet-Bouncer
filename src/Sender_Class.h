/** @file Sender_Class.h
 *  @brief Declarations for the sending functionality
 *
 *  @author Thomas Krueger
 *  @bug No known bugs.
 */

#ifndef id06D17375_0DD4_4A72_AF7BEB7EE5F310B1
#define id06D17375_0DD4_4A72_AF7BEB7EE5F310B1

#include "Meteron_Link_Test_Common.h"
#include "memory.h"

class Sender
{
private:
	int						Socket_Desc;
	struct   sockaddr_in	Dest_Addr; //address of the bouncer
	bool					Initialized ;


public:
	Sender();
	Sender(char * Destination_IP_Address, int Destination_Port);
	void init(char * Destination_IP_Address, int Destination_Port);
	int transmit(char * Data, int Data_Length);

	int transmit_by_addr(char * Data, int Data_Length);
};

#endif // header


