/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2019,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：一群：179029047(已满)  二群：244861897
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		main
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ3184284598)
 * @version    		查看doc内version文件 版本说明
 * @Software 		IAR 8.3 or MDK 5.28
 * @Target core		NXP RT1064DVL6A
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2019-04-30
 ********************************************************************************************************************/


//整套推荐IO查看Projecct文件夹下的TXT文本


//打开新的工程或者工程移动了位置务必执行以下操作
//第一步 关闭上面所有打开的文件
//第二步 project  clean  等待下方进度条走完

//下载代码前请根据自己使用的下载器在工程里设置下载器为自己所使用的

#include "headfile.h"
void SPI_SendPicture_16bit(uint16 width,uint16 height);
uint8 datx[2];
AT_DTCM_SECTION_ALIGN(uint16 i=0,2);
AT_DTCM_SECTION_ALIGN(uint16 j=0,2);
int main(void)
{
	uint8 cmd[4]={0x00,0xff,0x01,0x01};
	DisableGlobalIRQ();
    board_init();//务必保留，本函数用于初始化MPU 时钟 调试串口
	
	//此处编写用户代码(例如：外设初始化代码等)
	scc8660_csi_init();
	usb_cdc_init();
	gpio_init(B9,GPO,0,GPIO_PIN_CONFIG);
	//spi_init(SPI_3,SPI3_SCK_B0,SPI3_MOSI_B1,SPI3_MISO_B2,SPI3_CS0_B3,0,48*1000*1000);
	uart_init(USART_8,6000000,UART8_TX_D16,UART8_RX_D17);
	//总中断最后开启
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
        //此处编写需要循环执行的代码
		
		
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
  



