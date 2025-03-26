#include "esp8266.h"

bool device_connect = false;


bool fan_control = false;
bool lay_to_control = false;
bool pum_control = false;


static  void handle_wifi_data(void);
bool start_esp8266(void)
{ 
	
			HAL_UART_Transmit(&huart3,(uint8_t*)(AT_MODE2),13,0xffff);
			HAL_Delay(1000);
      HAL_UART_Transmit(&huart3,(uint8_t*)(AT_SET),38,0xffff);
				HAL_Delay(1000);
			
			HAL_UART_Transmit(&huart3,(uint8_t*)(AT_RST),8,0xffff);
			HAL_Delay(4000);
			
			HAL_UART_Transmit(&huart3,(uint8_t*)(AT_CIPMUX),13,0xffff);
			HAL_Delay(1000);
			
		  HAL_UART_Transmit(&huart3,(uint8_t*)(AT_CIPSERVER),21,0xffff);
			HAL_Delay(1000);
			
				__HAL_UART_ENABLE_IT(&huart3,UART_IT_IDLE);  
			HAL_UART_Receive_DMA(&huart3,uart3_rx,1000);   
			
			 return true;
			 
}

void handle_esp8266(void)
{
//	char *send = "hello\r\n";
	char *wifi_connect = "0,CONNECT";
	char *wifi_rec = "\r\n+IPD,";
	char *layon = "layon";
	char *fanon = "fanon";
	char *fanoff = "fanoff";
	char *pumpon = "pumpon";
	char *pumpoff = "pumpoff";
	char *motor = "motor";
	char *timeset7 = "\r\n+IPD,0,5:set07";
	char *timeset8 = "\r\n+IPD,0,5:set08";
	char *timeset9 = "\r\n+IPD,0,5:set09";
	char *timeset10 = "\r\n+IPD,0,5:set10";
	if(rx3_end_flag)
	{
		 //printf("HANDLE %s\r\n",uart2_rx);
			rx3_end_flag = false;
		
		  if(memcmp(uart3_rx,wifi_connect,9)==0)  //wifi已连接
			{
						//printf("yes\r\n");
						device_connect=true;
			}
			
			if(strstr((char *)uart3_rx,layon)!=NULL)  //wifi已连接
			{
							//	HAL_GPIO_WritePin(GPIOB, FAN_Pin, GPIO_PIN_RESET);
				   lay_control(true);  
			}
			
			if(strstr((char *)uart3_rx,fanon)!=NULL)  //wifi已连接
			{
				  fan_control= true;
					HAL_GPIO_WritePin(GPIOB, FAN_Pin, GPIO_PIN_RESET); 
			}

			
			if(strstr((char *)uart3_rx,fanoff)!=NULL)  //wifi已连接
			{
				  fan_control= false;
					HAL_GPIO_WritePin(GPIOB, FAN_Pin, GPIO_PIN_SET); 
			}			
			
			if(strstr((char *)uart3_rx,pumpon)!=NULL)  //wifi已连接
			{
					HAL_GPIO_WritePin(WATER_CONTROL_GPIO_Port, WATER_CONTROL_Pin, GPIO_PIN_RESET); 
			}
			
						if(strstr((char *)uart3_rx,motor)!=NULL)  //wifi已连接
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
			}
			
			
//			if(strstr((char *)uart3_rx,pumpoff)!=NULL)  //wifi已连接
//			{
//					HAL_GPIO_WritePin(WATER_CONTROL_GPIO_Port, WATER_CONTROL_Pin, GPIO_PIN_SET); 
//			}
//			

			rx3_count=0;
			memset(uart3_rx,0,1000);
			HAL_UART_Receive_DMA(&huart3,uart3_rx,1000);  //需要重新启动DMA
	} 
				
}

void send_wifi(uint8_t *data,int size)
{
	  int send_size=0;
	  if(size<10)
		{
			 send_size=1; 
		}
		
		if(size>10&&size<100)
		{
				send_size=2;
		}
		
			
	  if(device_connect)
		{
			char send_data[50]={0};
			char send_data1[50]={0};
			sprintf(send_data,"AT+CIPSEND=0,%d\r\n",size);
			HAL_UART_Transmit(&huart3,(uint8_t*)(send_data),15+send_size,0xffff);

			HAL_Delay(100);
			HAL_UART_Transmit(&huart3,(uint8_t*)data,size,0xffff);
		}
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