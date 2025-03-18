#include "esp8266.h"


void start_esp8266(void)
{ 
	
}

void handle_esp8266(void)
{
		for(int i=0;i<1000;i++)
		{
				printf("%02x",uart3_rx[i]);
		}
		printf("\r\n");
}