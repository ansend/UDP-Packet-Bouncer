 /** @file Meteron_Link_Test_Common.h
 *  @brief Common header for all senders and bouncers
 *
 *  @author Thomas Krueger
 *  @bug No known bugs.
 */
#ifndef METERON_LINK_TEST_COMMON_HDR_
#define METERON_LINK_TEST_COMMON_HDR_

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/mman.h>
#include <memory.h>
#include <time.h>

const int Max_Payload_Data_Size = 1024; //in bytes
const int Min_Payload_Data_Size = 32; //in bytes

#endif