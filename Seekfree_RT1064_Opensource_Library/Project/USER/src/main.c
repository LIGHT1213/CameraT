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
uint8 status;
//uint8 cmd233[4]= {0x00,0xff,0x01,0x01};
int main(void)
{
    //uint8 cmd[4]= {0x00,0xff,0x01,0x01};
    DisableGlobalIRQ();
    board_init();//��ر��������������ڳ�ʼ��MPU ʱ�� ���Դ���

    //�˴���д�û�����(���磺�����ʼ�������)
    scc8660_csi_init();
    usb_cdc_init();
		gpio_init(B13,GPI,0,GPIO_PIN_CONFIG);
    gpio_init(B9,GPO,0,GPIO_PIN_CONFIG);
    spi_init(SPI_3,SPI3_SCK_B0,SPI3_MOSI_B1,SPI3_MISO_B2,SPI3_CS0_B3,0,10*1000*1000);
    uart_init(USART_8,6000000,UART8_TX_D16,UART8_RX_D17);
    //���ж������
    EnableGlobalIRQ(0);
    while (1)
    {
				status=gpio_get(B13);
        if(scc8660_csi_finish_flag)
        {
            //seekfree_sendimg_scc8660_usb_cdc((uint8 *)scc8660_csi_image,160,120);
            //uart_putbuff(USART_8,cmd,4);
            //csi_seekfree_sendimg_scc8660(USART_8,(uint8 *)scc8660_csi_image,160,120);
            SPI_SendPicture_16bit(160,120);
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

		uint8 cmd[4],imagePixle[2];
		cmd[0] = 0x09; cmd[1] = 0xff; cmd[2] = 0x01; cmd[3] = 0x01; 
		if(status==0)
		{
//		for(int k=0;k<4;k++)
//    spi_mosi(SPI_3,SPI3_CS0_B3,&cmd[i],NULL,1,0);
			spi_mosi(SPI_3,SPI3_CS0_B3,cmd,NULL,4,1);
		}
//    uint8 imagePixle[2];
		else
		{
			spi_mosi(SPI_3,SPI3_CS0_B3,(uint8 *)(scc8660_csi_image),NULL,SCC8660_CSI_PIC_H*SCC8660_CSI_PIC_W*2,0);
//    for(i=0; i<=120; i++)
//    {
//        for(j=0; j<160; j++)
//        {
//					imagePixle[0]=scc8660_csi_image[i][j]>>8;
//					imagePixle[1]=(uint8)scc8660_csi_image[i][j];
////       		spi_mosi(SPI_3,SPI3_CS0_B3,(uint8 *)(scc8660_csi_image+(160*i+j)),NULL,2,0);
//					spi_mosi(SPI_3,SPI3_CS0_B3,&imagePixle[1],NULL,1,0);
//					spi_mosi(SPI_3,SPI3_CS0_B3,&imagePixle[1],NULL,1,0);
//        }
//    }
		}
}




