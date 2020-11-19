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
void Send_MasterComputer(uint8* image,uint16 width,uint16 height);
void ImageUartInit();
void ImageUART_CallBack(LPUART_Type *base, lpuart_handle_t *handle, status_t status, void *userData);
uint8 datx[2];
AT_DTCM_SECTION_ALIGN(uint16 i=0,2);
AT_DTCM_SECTION_ALIGN(uint16 j=0,2);
uint8 status;

uint8 RecSendFlag=0;
uint8 ImageUart_Data;
uint8 ImageRx_Buffer;
lpuart_transfer_t   ImageReceivexfer;
lpuart_handle_t     ImageLpuartHandle;


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
    //uart_init(USART_8,115200,UART8_TX_D16,UART8_RX_D17);
		ImageUartInit();
    //���ж������
    EnableGlobalIRQ(0);
    while (1)
    {
				status=gpio_get(B13);
        if(scc8660_csi_finish_flag && RecSendFlag)
        {
            Send_MasterComputer((uint8 *)scc8660_csi_image,160,120);
            //uart_putbuff(USART_8,cmd,4);
            //csi_seekfree_sendimg_scc8660(USART_8,(uint8 *)scc8660_csi_image,160,120);
            //SPI_SendPicture_16bit(160,120);
            //gpio_toggle(B9);
            //SPI_SendPicture_16bit(160,120);
					RecSendFlag=0;
					scc8660_csi_finish_flag=0;
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
			spi_mosi(SPI_3,SPI3_CS0_B3,cmd,NULL,4,1);
		}
		else
		{
			spi_mosi(SPI_3,SPI3_CS0_B3,(uint8 *)(scc8660_csi_image),NULL,SCC8660_CSI_PIC_H*SCC8660_CSI_PIC_W*2,0);
		}
}
void Send_MasterComputer(uint8* image,uint16 width,uint16 height)
{
//	uint8 cmd[6];
//	cmd[0] = 0x09; cmd[1] = 0xff; cmd[2] = 0x01; cmd[3] = 0x01; cmd[4]=0xff;cmd[5]=0x01;
//	uart_putbuff(USART_8,cmd,4);
//	systick_delay_ms(1);
  uart_putbuff(USART_8,image, width*height*2); 
	//usb_cdc_send_buff(image, width*height*2);	
	//uart_putbuff(USART_8,&cmd[4],1);
	//uart_putbuff(USART_8,&cmd[5],1);
}	
void ImageUartInit()
{
	uart_init(USART_8,921600,UART8_TX_D16,UART8_RX_D17);
	NVIC_SetPriority(LPUART8_IRQn,0);
	uart_rx_irq(USART_8,1);
	ImageReceivexfer.dataSize =1;
	ImageReceivexfer.data =&ImageRx_Buffer;
	uart_set_handle(USART_8, &ImageLpuartHandle, ImageUART_CallBack, NULL, 0, ImageReceivexfer.data, 1);
}
void ImageUART_CallBack(LPUART_Type *base, lpuart_handle_t *handle, status_t status, void *userData)
{
		if(kStatus_LPUART_RxIdle == status)
    {
        ImageUart_Data = ImageRx_Buffer;
				RecSendFlag=1;
    }
    
    handle->rxDataSize = ImageReceivexfer.dataSize;
    handle->rxData = ImageReceivexfer.data;
}


