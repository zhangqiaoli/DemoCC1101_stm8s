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
        printlog("start...");
	delay_ms(500);
	//SPI��ʼ��
	drv_spi_init( );
	//CC1101��ʼ��
	CC1101_Init( );
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


