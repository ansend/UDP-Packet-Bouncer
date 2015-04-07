 /** @file Measurement_Sender_Main.cpp
 *  @brief Main functions for the bouncer
 *
 *  @author Thomas Krueger
 *  @bug No known bugs.
 */
#include "Measurement_Sender_Main.h"
#include "posix_periodic.h"

periodic_info period_info;

void * sender(void * Data)
{
  int			result;
  unsigned long		overruns;
  char			Dummy_Data[Max_Payload_Data_Size];
  long			Counter = 0;
	
  //set the current task to periodic mode
  make_periodic(Sample_Rate, &period_info);

  fprintf(stdout, "Task start ...\n");
  while(true)
  {
    wait_period (&period_info);
    clock_gettime(CLOCK_REALTIME, &Start_Operation);
    memcpy(&Dummy_Data, &Start_Operation, sizeof(Start_Operation));
    memcpy(&Dummy_Data[sizeof(Start_Operation)],&Counter, sizeof(Counter));

    result = MySender.transmit(Dummy_Data, Payload_Size);
    if (result < 0)
    {
      printf("Error sending data in %s %s\n",  __FILE__, __FUNCTION__);
      exit(EXIT_FAILURE);
    }
    //printf("Send: %ld \n", Counter);
    Counter++;
  }//end while loop task loop

  return 0;
}

void * receiver(void * Data)
{
	struct timespec		Extracted_Time;
	struct timespec		Max_Latency = {.tv_sec = 0, .tv_nsec = 0};
	struct timespec		Message_Delay;
	struct timespec		Data_Received;
	struct timespec		Time_Difference;
	long			Bounced_Counter = -1;
	long			Bounced_Counter_Prev = 0;
	char			Data_Buffer[Max_Payload_Data_Size];
	unsigned		Lost_Messages = 0;
	std::ofstream 		Log_File;
	char 			Output_String[512];
	
	
	if (Log_File_Path == NULL)
	{
	    printf("-- WARNING: no log specified - switching to verbose mode\n");
	    verbose = true;
	}
	else
	{
	  printf("Logging to: %s\n", Log_File_Path);
	  Log_File.open(Log_File_Path);
	  if (Log_File==NULL)
	  {
	    printf("-- ERROR: could not create log file - check path/permissions\n");
	    exit(EXIT_FAILURE);
	  }
	  Log_File << "Latency ms | Counter | Lost messages | Timer-misses | Max_Latency ms\n" << std::endl;
	  Log_File << "====================================================================\n" << std::endl;
	}
	
	
	//set the current task to periodic mode
	Bounced_Counter_Prev = -1;
	int i=0;
	while(true)
	{	
	  
	  //get the data
		MyReceiver.receive_Blocking(Data_Buffer, Payload_Size);
		
		
		Bounced_Counter_Prev = Bounced_Counter;
		
		//extract the received time from the data
		Extracted_Time = *((timespec*) Data_Buffer);
		memcpy(&Bounced_Counter, (char*) &Data_Buffer[sizeof(Extracted_Time)], sizeof(Bounced_Counter));
		//measure the time
		
		clock_gettime(CLOCK_REALTIME, &Data_Received);
				
		Message_Delay = Calc_Time_Diff(Extracted_Time, Data_Received);
		
		Time_Difference = Calc_Time_Diff(Max_Latency, Message_Delay);
		
		if (Time_Difference.tv_sec < 0)
		{
		  //printf("NO CHANGED\n");
		}
		else
		{
		  Max_Latency = Message_Delay;
		  //printf("Max_Latency changed %d s %d ns \n", Max_Latency.tv_sec, Max_Latency.tv_nsec);  
		}
		
		if(Bounced_Counter - Bounced_Counter_Prev > 1)
		{
			Lost_Messages = Lost_Messages + (Bounced_Counter - Bounced_Counter_Prev);
		}
		
		//definition of the output
		int char_return = sprintf
			(	Output_String,
				"%f \t %d \t %u \t %u \t %f \n\0",
				timespec2double(Message_Delay)*1000.0,
				Bounced_Counter,
				Lost_Messages,
				period_info.wakeups_missed,
				timespec2double(Max_Latency)*1000.0
			);
		printf("DEBUG: return chars: %d\n", char_return);
		
		if (verbose)
		{
		  /*printf(	"%f \t %d \t %u \t %u \t %f \n\0",
				timespec2double(Message_Delay)*1000.0,
				Bounced_Counter,
				Lost_Messages,
				period_info.wakeups_missed,
				timespec2double(Max_Latency)*1000.0
			);*/
		  std::cout << timespec2double(Message_Delay)*1000.0 << "  \t" << Bounced_Counter << "\t" << Lost_Messages << "\t" << period_info.wakeups_missed << "\t" << timespec2double(Max_Latency)*1000.0 << std::endl;
		  //printf("%s", Output_String);			
		}//end if verbose
		
		if (Log_File_Path != NULL)
		{
		
		 Log_File << timespec2double(Message_Delay)*1000.0 << "  \t" << Bounced_Counter << "\t" << Lost_Messages << "\t" << period_info.wakeups_missed << "\t" << timespec2double(Max_Latency)*1000.0 << std::endl;
		 Log_File.flush();
		}
	}//end while loop task loop
	
	return 0;
}

int main(int argc,  char **argv)
{
	int 				result;
	char 				Dest_IP_Address[32];
	int					Dest_Port;
	char				Source_IP_Address[32];
	int					Source_Port;
	
#ifdef XENOMAI_USAGE
    rt_print_auto_init(1);
#endif
	//for the command line parsing
	GError	*			error = NULL;
    GOptionContext *	context;
	
	context = g_option_context_new (Argument_Order);
    g_option_context_add_main_entries(context, options, NULL);
	
	g_option_context_set_summary(context, Summary);
	g_option_context_set_description (context, Description);
	
    if (!g_option_context_parse(context, &argc, &argv, &error))
    {
		g_print ("option parsing failed: %s\n\n", error->message);
		printf(g_option_context_get_help (context, TRUE, NULL));
		exit(EXIT_FAILURE);
    }
	
	if (argc < 4)
	{
		printf("Not enough arguments...\n\n");
		printf(g_option_context_get_help (context, TRUE, NULL));
		exit(EXIT_FAILURE);
	}
	
	sprintf(Source_IP_Address, "0.0.0.0");
	sprintf(Dest_IP_Address, "%s", argv[1]);
	Dest_Port = atoi(argv[2]);
	Source_Port = atoi(argv[3]);
	
	if (Payload_Size > Max_Payload_Data_Size)
	{
		printf("The maximum allowed payload size is %d\n", Max_Payload_Data_Size);
		exit(EXIT_FAILURE);
	}
	
	if (Payload_Size < Min_Payload_Data_Size)
	{
		printf("The minimum allowed payload size is %d\n", Min_Payload_Data_Size);
		exit(EXIT_FAILURE);
	}
	
	if (Sample_Rate < 1)
	{
		printf("Please enter a reasonable sample rate > 1 us ;-)\n");
		exit(EXIT_FAILURE);
	}
	
	printf("The programm will be running with the following settngs\n");
	printf("====================\n");
	printf("Sample rate            : %d us \n", Sample_Rate);
	printf("Payload size           : %d byte \n", Payload_Size);
	printf("Destination ip address : %s\n", Dest_IP_Address);
	printf("Destination port       : %d\n", Dest_Port);
	printf("Source ip address      : %s\n", Source_IP_Address);
	printf("Source port            : %d\n", Source_Port);
	
	//init the sockets
	MySender.init(Dest_IP_Address, Dest_Port);
	MyReceiver.init(Source_IP_Address, Source_Port);
	
	//lock all memory before starting rt-task
	mlockall(MCL_CURRENT | MCL_FUTURE);
	
	

    fprintf(stdout, "Starting standard posix bouncing thread\n");
  //invoke the bouncer as posix task
  result = pthread_create(&Send_Task, NULL, &sender, NULL);
  result = pthread_create(&Receive_Task, NULL, &receiver, NULL);
  if(result != 0)
  {
	  fprintf(stderr, "Posic task creation failed in %s at line %d\n", __FILE__, __LINE__);
	  return -1;
  }
  //wait before task returns
  result = pthread_join(Receive_Task, NULL);
  if (result != 0)
  {
	  fprintf(stderr, "Xenomai task does not rejoin properly\n");
	  return -1;
  }

	fprintf(stderr, "Program terminated nominally\n");
	return 0;	
}