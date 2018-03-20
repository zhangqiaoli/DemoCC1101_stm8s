/**
  ******************************************************************************
  * @author  泽耀科技 ASHINING
  * @version V3.0
  * @date    2016-10-08
  * @brief   主函数C文件
  ******************************************************************************
  * @attention
  *
  * 官网	:	http://www.ashining.com
  * 淘宝	:	https://shop105912646.taobao.com
  * 阿里巴巴:	https://cdzeyao.1688.com
  ******************************************************************************
  */



#include "drv_CC1101.h"
#include "delay.h"
#include "Uart2Dev.h"

/** 发送模式定义 */
enum
{
	TX_MODE_1 = 0,		//发送模式1，发送固定的字符串
	TX_MODE_2			//发送模式2，发送串口接收到的数据
};


const char *g_Ashining = "ashining";
uint8_t g_TxMode = 0;
uint8_t g_UartRxBuffer[ 100 ] = { 0 };
uint8_t g_RF24L01RxBuffer[ 32 ] = { 0 }; 

void uart_puthex_nibble(const unsigned char b)
{
    unsigned char  c = b & 0x0f;
    if (c>9) c += 'A'-10;
    else c += '0';
    printlog(&c);
}

void uart_puthex_byte(const unsigned char b)
{
    uart_puthex_nibble(b>>4);
    uart_puthex_nibble(b);
} 


void show_register_settings()
{
  uint8_t config_reg_verify[47],Patable_verify[8];
  
  CC1101_Read_Multi_Reg(CC1101_IOCFG2,config_reg_verify,47);
  CC1101_Read_Multi_Reg( CC1101_PATABLE, (uint8_t*)Patable_verify, 8 );

  printlog("Cfg_reg:\r\n");

  for(uint8_t i = 0 ; i < 47; i++)  //showes rx_buffer for debug
      {
          uart_puthex_byte(config_reg_verify[i]);printlog(" ");
          if(i==9 || i==19 || i==29 || i==39) //just for beautiful output style
              {
                  printlog("\r\n");
              }
      }
      printlog("\r\n");
      printlog("PaTable:\r\n");

      for(uint8_t i = 0 ; i < 8; i++)         //showes rx_buffer for debug
          {
              uart_puthex_byte(Patable_verify[i]);printlog(" ");
          }
  printlog("\r\n");
}

//#define SENDER

/**
  * @brief :主函数 
  * @param :无
  * @note  :无
  * @retval:无
  */ 

uint8_t i = 0;
int main( void )
{	
    /* High speed internal clock prescaler: 1 */
  CLK_SYSCLKConfig(CLK_PRESCALER_HSIDIV1);  // now: HSI=16M prescale = 1; sysclk = 16M

 
	//串口初始化
	uart2_config( 9600 );
        printlog("start...\r\n");
	delay_ms(500);
	//SPI初始化
	drv_spi_init( );
	//CC1101初始化
	CC1101_Init( );
        show_register_settings();
#ifdef SENDER
 //发送
	while( 1 )	
	{
		
		//发送
		if( TX_MODE_1 == g_TxMode )
		{
			CC1101_Tx_Packet( (uint8_t *)g_Ashining, 8 , ADDRESS_CHECK );		//模式1发送固定字符,1S一包
			delay_ms( 1000 );			
		}
	}
#else
//接收

	while( 1 )
	{

                CC1101_Clear_RxBuffer( );
                CC1101_Set_Mode( RX_MODE );
		i = CC1101_Rx_Packet( g_RF24L01RxBuffer );		//接收字节
		if( 0 != i )
		{
                        //delay_ms( 200 );
			Uart2SendByteByLen( g_RF24L01RxBuffer, i );	//输出接收到的字节
		}
	}
#endif
	
}


