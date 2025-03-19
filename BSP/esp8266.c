#include "esp8266.h"

bool device_connect = false;

static  void handle_wifi_data(void);
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
			if(device_connect==false)
			{
					printf("connect\r\n");
				 	if(rx3_end_flag)
					{
						rx3_end_flag = false;

						if(memcmp(uart3_rx,connected,9)==0)
						{
								device_connect=true;					    
						}
				
						rx3_count=0;
						memset(uart3_rx,0,10000);
						HAL_UART_Receive_DMA(&huart3,uart3_rx,10000);  //需要重新启动DMA
					} 
			}
			else
			{
					if(rx3_end_flag)
					{
						rx3_end_flag = false;
						printf("%s\r\n",uart3_rx);
						handle_wifi_data();
						rx3_count=0;
						memset(uart3_rx,0,10000);
						HAL_UART_Receive_DMA(&huart3,uart3_rx,10000);  //需要重新启动DMA
					} 
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


//handle WiF
//+IPD,0,11:1234567890  //这个是处理握手信号的
//data 10:open lay
//data 11:close lay

//data 20:open fan
//data 21:close fan

//data 30:+ moto
//data 31:-moto

static  void handle_wifi_data(void)
{
	  char * conect =     "+IPD,0,11:1234567890";
		char * open_lay =   "\r\n+IPD,0,3:10";  
		char * close_lay =  "\r\n+IPD,0,3:11";
		char * open_fan =   "\r\n+IPD,0,3:20";
		char * close_fan =  "\r\n+IPD,0,3:21";
		char * zheng_moto = "\r\n+IPD,0,3:30"; 		
		char * fan_moto =   "\r\n+IPD,0,3:31"; 	
		if(memcmp(conect,uart3_rx,20)==0)
		{
					
		}
		
		if(memcmp(open_lay,uart3_rx,13)==0)
		{
			 printf("yes\r\n");
				lay_control(true);
				return;
		}
		
		if(memcmp(close_lay,uart3_rx,13)==0)
		{
				lay_control(false);
				return;
		}
		
		if(memcmp(open_fan,uart3_rx,13)==0)
		{
				HAL_GPIO_WritePin(GPIOB, FAN_Pin, GPIO_PIN_SET);  //开风扇
				return;
		}
		
				if(memcmp(open_fan,uart3_rx,13)==0)
		{
				HAL_GPIO_WritePin(GPIOB, FAN_Pin, GPIO_PIN_RESET);  //关风扇
				return;
		}
		
		
		if(memcmp(zheng_moto,uart3_rx,13)==0)
		{
										direction = 0; //Turn to loosen the soil in a positive direction
				for(int i=0;i<(motor_angle_cal(90))/8;i++)
				{
					for(uint8_t step=0;step<8;step++)
					{	
							motor_controld(step,direction);
							HAL_Delay(1);
					}
				}
				
				return;
		}
		
		
				if(memcmp(fan_moto,uart3_rx,13)==0)
		{
										direction = 1; //Turn to loosen the soil in a positive direction
				for(int i=0;i<(motor_angle_cal(90))/8;i++)
				{
					for(uint8_t step=0;step<8;step++)
					{	
							motor_controld(step,direction);
							HAL_Delay(1);
					}
				}
				
				return;
		}

		
}