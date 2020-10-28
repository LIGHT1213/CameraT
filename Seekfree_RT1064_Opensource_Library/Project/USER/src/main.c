/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2019,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		main
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ3184284598)
 * @version    		�鿴doc��version�ļ� �汾˵��
 * @Software 		IAR 8.3 or MDK 5.28
 * @Target core		NXP RT1064DVL6A
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2019-04-30
 ********************************************************************************************************************/


//�����Ƽ�IO�鿴Projecct�ļ����µ�TXT�ı�


//���µĹ��̻��߹����ƶ���λ�����ִ�����²���
//��һ�� �ر��������д򿪵��ļ�
//�ڶ��� project  clean  �ȴ��·�����������

//���ش���ǰ������Լ�ʹ�õ��������ڹ���������������Ϊ�Լ���ʹ�õ�

#include "headfile.h"
void SPI_SendPicture_16bit(uint16 width,uint16 height);
uint8 datx[2];
AT_DTCM_SECTION_ALIGN(uint16 i=0,2);
AT_DTCM_SECTION_ALIGN(uint16 j=0,2);
int main(void)
{
	uint8 cmd[4]={0x00,0xff,0x01,0x01};
	DisableGlobalIRQ();
    board_init();//��ر��������������ڳ�ʼ��MPU ʱ�� ���Դ���
	
	//�˴���д�û�����(���磺�����ʼ�������)
	scc8660_csi_init();
	usb_cdc_init();
	gpio_init(B9,GPO,0,GPIO_PIN_CONFIG);
	//spi_init(SPI_3,SPI3_SCK_B0,SPI3_MOSI_B1,SPI3_MISO_B2,SPI3_CS0_B3,0,48*1000*1000);
	uart_init(USART_8,6000000,UART8_TX_D16,UART8_RX_D17);
	//���ж������
    EnableGlobalIRQ(0);
    while (1)
    {
			if(scc8660_csi_finish_flag)
			{
				//seekfree_sendimg_scc8660_usb_cdc((uint8 *)scc8660_csi_image,160,120);
				//uart_putbuff(USART_8,cmd,4);
				csi_seekfree_sendimg_scc8660(USART_8,(uint8 *)scc8660_csi_image,160,120);
				//gpio_toggle(B9);
				//SPI_SendPicture_16bit(160,120);
			}
			//uart_putchar(USART_8,0x11);
			//gpio_toggle(B9);
			//systick_delay_ms(500);
        //�˴���д��Ҫѭ��ִ�еĴ���
		
		
    }
}
void SPI_SendPicture_16bit(uint16 width,uint16 height)
{    
	
	static uint32 count;
	uart_putchar(USART_8,0x11);
//			datx[0]=scc8660_csi1_image[i][j]>>8;
//			datx[1]=(uint8)scc8660_csi1_image[i][j];
			datx[0]=0x11;
			datx[1]=0xff;
			//spi_mosi(SPI_3,SPI3_CS0_B3,datx,datx,2,1);
	//spi_mosi(SPI_3,SPI3_CS0_B3,image,dat1,width*height*2,1);
	count++;
	if(count==160*120)
	{
		gpio_toggle(B9);
		count=0;
	}
	//spi_mosi(SPI_3,SPI3_CS0_B3,dat1,dat1,2,1);
}
  



