#include "esp8266.h"

bool device_connect = false;
bool start_esp8266(void)
{ 
	
			HAL_UART_Transmit(&huart3,(uint8_t*)(AT_MODE2),13,0xffff);
			HAL_Delay(100);
	    while(1)
			{
				 if(rx3_end_flag)
					{
						//printf("%s\r\n",uart3_rx);
						rx3_end_flag = false;
						rx3_count=0;
						HAL_UART_Receive_DMA(&huart3,uart3_rx,10000);  //需要重新启动DMA
						break;
					}
			}
			
			HAL_UART_Transmit(&huart3,(uint8_t*)(AT_RST),8,0xffff);
			HAL_Delay(1000);
	    while(1)
			{
				 if(rx3_end_flag)
					{
						//printf("%s\r\n",uart3_rx);
						rx3_end_flag = false;
						rx3_count=0;
						HAL_UART_Receive_DMA(&huart3,uart3_rx,10000);  //需要重新启动DMA
						break;
					}
			}
			
			HAL_UART_Transmit(&huart3,(uint8_t*)(AT_CIPMUX),13,0xffff);
			HAL_Delay(100);
	    while(1)
			{
				 if(rx3_end_flag)
					{
						//	printf("%s\r\n",uart3_rx);
						rx3_end_flag = false;
						rx3_count=0;
						HAL_UART_Receive_DMA(&huart3,uart3_rx,10000);  //需要重新启动DMA
						break;
					}
			}
			
				 HAL_UART_Transmit(&huart3,(uint8_t*)(AT_CIPSERVER),16,0xffff);
			   HAL_Delay(100);
				 rx3_end_flag = false;
				 rx3_count=0;
				HAL_UART_Receive_DMA(&huart3,uart3_rx,10000);  //需要重新启动DMA
			
			 return true;
			 
}

void handle_esp8266(void)
{
	    uint8_t data[3]={'1','2','3'};
	    char* connected ="0,CONNECT";
			if(rx3_end_flag)
			{
					rx3_end_flag = false;

					if(memcmp(uart3_rx,connected,9)==0)
					{
								device_connect= true;
						    //printf("%d\r\n",device_connect);
					}
				
				
					rx3_count=0;
				  memset(uart3_rx,0,10000);
					HAL_UART_Receive_DMA(&huart3,uart3_rx,10000);  //需要重新启动DMA
			}
			
			if(device_connect==true)
			{
				 printf("hello\r\n");
				
				 HAL_Delay(10000);
				  device_connect = false;
				  send_wifi(data,3);
			}
}

void send_wifi(uint8_t *data,int size)
{
//			char * data1 = "AT+CIPSEND=0,4\r\n";
//			char * data2 = "1234\r\n";
//	    printf("sned\r\n");
//	    HAL_UART_Transmit(&huart3,(uint8_t*)(data1),16,0xffff);
//	    HAL_Delay(100);
//		  HAL_UART_Transmit(&huart3,(uint8_t*)(data2),6,0xffff);
	  char send_data[50]={0};
		char send_data1[50]={0};
	  sprintf(send_data,"AT+CIPSEND=0,%d\r\n",size);
		HAL_UART_Transmit(&huart3,(uint8_t*)(send_data),16,0xffff);

		HAL_Delay(100);
		
		sprintf(send_data1,"%s\r\n",data);
		HAL_UART_Transmit(&huart3,(uint8_t*)send_data1,size+2,0xffff);
}