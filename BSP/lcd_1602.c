#include "lcd_1602.h"
int time_set = 5;


/**
  * @brief  Reads the specified GPIO output data port.
  * @param  GPIOx: where x can be (A..G) to select the GPIO peripheral.
  * @retval GPIO output data port value.
  */
uint16_t GPIO_ReadOutputData(GPIO_TypeDef* GPIOx)
{
  /* Check the parameters */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
    
  return ((uint16_t)GPIOx->ODR);
}

/**
  * @brief  Writes data to the specified GPIO data port.
  * @param  GPIOx: where x can be (A..G) to select the GPIO peripheral.
  * @param  PortVal: specifies the value to be written to the port output data register.
  * @retval None
  */
void GPIO_Write(GPIO_TypeDef* GPIOx, uint16_t PortVal)
{
  /* Check the parameters */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  
  GPIOx->ODR = PortVal;
}


void LCD_WRITE_CMD(uint8_t cmd)
{
	
	 HAL_GPIO_WritePin(GPIOC, LCD_RS_Pin, GPIO_PIN_RESET);
	 HAL_GPIO_WritePin(GPIOC, LCD_RW_Pin, GPIO_PIN_RESET);
	 HAL_GPIO_WritePin(GPIOC, LCD_EC_Pin, GPIO_PIN_RESET);
	  HAL_Delay(5);
	
	
	GPIO_Write(GPIOA, (GPIO_ReadOutputData(GPIOA) & 0xFF00) | cmd); // 发送数据
	HAL_Delay(5);
	HAL_GPIO_WritePin(GPIOC, LCD_EC_Pin, GPIO_PIN_SET);
	HAL_Delay(5);
	HAL_GPIO_WritePin(GPIOC, LCD_EC_Pin, GPIO_PIN_RESET);

}

void LCD_WRITE_DATA(uint8_t cmd)
{
	
	 HAL_GPIO_WritePin(GPIOC, LCD_RS_Pin, GPIO_PIN_SET);
	 HAL_GPIO_WritePin(GPIOC, LCD_RW_Pin, GPIO_PIN_RESET);
	 HAL_GPIO_WritePin(GPIOC, LCD_EC_Pin, GPIO_PIN_RESET);
	  HAL_Delay(5);
	
	
	GPIO_Write(GPIOA, (GPIO_ReadOutputData(GPIOA) & 0xFF00) | cmd); // 发送数据
	HAL_Delay(5);
	HAL_GPIO_WritePin(GPIOC, LCD_EC_Pin, GPIO_PIN_SET);
	HAL_Delay(5);
	HAL_GPIO_WritePin(GPIOC, LCD_EC_Pin, GPIO_PIN_RESET);

}


void LCD_INIT(void)
{

	
	LCD_WRITE_CMD(0x38);
	LCD_WRITE_CMD(0x0c);
	LCD_WRITE_CMD(0x06);
	LCD_WRITE_CMD(0x01);

}

void LCD_CLR(void)
{
	LCD_WRITE_CMD(0x01);
}


void lcd1602_show_string(uint8_t x,uint8_t y,char *str)
{
	uint8_t i=0;

	if(y>1||x>15)return;//行列参数不对则强制退出

	if(y<1)	//第1行显示
	{	
		while(*str!='\0')//字符串是以'\0'结尾，只要前面有内容就显示
		{
			if(i<16-x)//如果字符长度超过第一行显示范围，则在第二行继续显示
			{
				LCD_WRITE_CMD(0x80+i+x);//第一行显示地址设置	
			}
			else
			{
				LCD_WRITE_CMD(0x40+0x80+i+x-16);//第二行显示地址设置	
			}
			LCD_WRITE_DATA(*str);//显示内容
			str++;//指针递增
			i++;	
		}	
	}
	else	//第2行显示
	{
		while(*str!='\0')
		{
			if(i<16-x) //如果字符长度超过第二行显示范围，则在第一行继续显示
			{
				LCD_WRITE_CMD(0x80+0x40+i+x);	
			}
			else
			{
				LCD_WRITE_CMD(0x80+i+x-16);	
			}
			LCD_WRITE_DATA(*str);
			str++;
			i++;	
		}	
	}				
}

//设定自动松土的时间
void ui_setting(void)
{
	 int position = 0;
	  char show_data_temperature[10]={0};
		char show_data_light[10]={0};
		char show_data_sl[10]={0};
		
	  while(1)
		{
			
		sprintf(show_data_temperature,"T:%02d",ambient_temperature);
		lcd1602_show_string(0,0,show_data_temperature);
		
		sprintf(show_data_light,"L:%.02f",light_data);
		lcd1602_show_string(5,0,show_data_light);
		
				sprintf(show_data_sl,"TS:%02d",timer_set);
		lcd1602_show_string(11,0,show_data_sl);
		
		sprintf(show_data_sl,"SL:%.02f",soil_moisture_miner);
		lcd1602_show_string(0,1,show_data_sl);
				
		sprintf(show_data_sl,"SM:%.02f",soil_moisture_lager);
		lcd1602_show_string(8,1,show_data_sl);
			
			 if(botton == LEFT)
			{
				  botton = UNPRESS;
					switch(position)
					{
						case 0:
							ambient_temperature--;
						  if(ambient_temperature<10)
							{
									ambient_temperature=40;
							}
							break;
						case 1:
							light_data=light_data-0.1;
						  if(light_data<0.2)
							{
									light_data=0.8;
							}
							break;
						case 2:
							timer_set--;
						  if(timer_set<2)
							{
									timer_set=10;
							}
						break;
						case 3:
							soil_moisture_miner=soil_moisture_miner-0.1;
						  if(soil_moisture_miner<1.0)
							{
									soil_moisture_miner=2.0;
							}
						break;
						case 4:
							soil_moisture_lager=soil_moisture_lager-0.1;
						  if(soil_moisture_lager<2.7)
							{
									soil_moisture_lager=3.3;
							}
							break;
					}
			}
			
			
			if(botton == RIGHT)
			{
				  botton = UNPRESS;
					switch(position)
					{
						case 0:
							ambient_temperature++;
						  if(ambient_temperature>40)
							{
									ambient_temperature=10;
							}
							break;
						case 1:
							light_data=light_data+0.1;
						  if(light_data>0.8)
							{
									light_data=0.2;
							}
							break;
						case 2:
							timer_set++;
						  if(timer_set>10)
							{
									timer_set=2;
							}
						break;
						case 3:
							soil_moisture_miner=soil_moisture_miner+0.1;
						  if(soil_moisture_miner>2.0)
							{
									soil_moisture_miner=1.0;
							}
						break;
						case 4:
							soil_moisture_lager=soil_moisture_lager+0.1;
						  if(soil_moisture_lager>3.3)
							{
									soil_moisture_lager=2.7;
							}
							break;
					}
			}
			
			if(botton == MIDLE)
			{
				  botton = UNPRESS;
				  position++;
				  if(position>4)
					{
						 	LCD_CLR();
						break;
					}
						

			}
			
			
		}
}
