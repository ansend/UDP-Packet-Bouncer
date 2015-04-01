 /** @file ASAP_Bouncer_Main.h
 *  @brief Commandline info and globals for the bouncer
 *
 *  @author Thomas Krueger
 *  @bug No known bugs.
 */

#ifndef ASAP_BOUNCE_MAIN_H_45609BF
#define ASAP_BOUNCE_MAIN_H_45609BF

//inlcudes
#include "Meteron_Link_Test_Common.h"
#include "Sender_Class.h"
#include "Receiver_Class.h"
#include <glib.h>

//globals
Sender		MySender;
Receiver	MyReceiver;

pthread_t	Bouncing_Task;

//the help and description of the command line

char * Argument_Order = "Destination_IP Destination_Port Source_Port";
char * Description =
"The parameters have to be entered manner showed above\n\
Example:\n\
             Dest_IP       De_pt Sc_pt\n\
ASAP_Bouncer 192.168.1.232 25001 25002 --verbose\n\
\n\ 
The source IP address will be 0.0.0.0 by default\n";


char * Summary = 
"This programm listens to a specified socket for upd packets and forwards/returns the message to the specified address and port. \n\n\
The execution is a standard posix thread and the reaction is as fast as possible";

static gboolean verbose = FALSE;

//the valid options
GOptionEntry options[] =
{
	{"verbose", 'v', 0, G_OPTION_ARG_NONE, &verbose, "Be verbose", NULL },
	{ NULL }
};

#endif
