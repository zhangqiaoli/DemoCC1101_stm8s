/**
  ******************************************************************************
  * @author  ��ҫ�Ƽ� ASHINING
  * @version V3.0
  * @date    2016-10-08
  * @brief   ������C�ļ�
  ******************************************************************************
  * @attention
  *
  * ����	:	http://www.ashining.com
  * �Ա�	:	https://shop105912646.taobao.com
  * ����Ͱ�:	https://cdzeyao.1688.com
  ******************************************************************************
  */



#include "drv_CC1101.h"
#include "delay.h"
#include "Uart2Dev.h"

/** ����ģʽ���� */
enum
{
	TX_MODE_1 = 0,		//����ģʽ1�����͹̶����ַ���
	TX_MODE_2			//����ģʽ2�����ʹ��ڽ��յ�������
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
  * @brief :������ 
  * @param :��
  * @note  :��
  * @retval:��
  */ 

uint8_t i = 0;
int main( void )
{	
    /* High speed internal clock prescaler: 1 */
  CLK_SYSCLKConfig(CLK_PRESCALER_HSIDIV1);  // now: HSI=16M prescale = 1; sysclk = 16M

 
	//���ڳ�ʼ��
	uart2_config( 9600 );
        printlog("start...\r\n");
	delay_ms(500);
	//SPI��ʼ��
	drv_spi_init( );
	//CC1101��ʼ��
	CC1101_Init( );
        show_register_settings();
#ifdef SENDER
 //����
	while( 1 )	
	{
		
		//����
		if( TX_MODE_1 == g_TxMode )
		{
			CC1101_Tx_Packet( (uint8_t *)g_Ashining, 8 , ADDRESS_CHECK );		//ģʽ1���͹̶��ַ�,1Sһ��
			delay_ms( 1000 );			
		}
	}
#else
//����

	while( 1 )
	{

                CC1101_Clear_RxBuffer( );
                CC1101_Set_Mode( RX_MODE );
		i = CC1101_Rx_Packet( g_RF24L01RxBuffer );		//�����ֽ�
		if( 0 != i )
		{
                        //delay_ms( 200 );
			Uart2SendByteByLen( g_RF24L01RxBuffer, i );	//������յ����ֽ�
		}
	}
#endif
	
}


