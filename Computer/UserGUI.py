import sys
from threading import Thread,Lock
import multiprocessing 
import time
# 系统包
from PyQt5 import uic
from PyQt5.QtCore import *
from PyQt5.QtGui import QImage,QPixmap,QPainter
# pyqt5 部分
import serial
import serial.tools.list_ports
#opencv部分
import cv2
import array as arr
# 全局变量部分
global MODE,CMD

MODE=["UDP网络传输","串口"]
CMD = bytes([9,1,255,255])
class MainGUI:
    PortList=""
    OldPortList=""
    OpenSerSingal=0
    SerLock=Lock()
    UartStr=""
    RecFlag=0
    Image=""
    #BGRImage=arr.array("I",[0 for i in range(19200)],dtype='uint8')
    RGBr=0
    RGBg=0
    RGBb=0
    #RGB888=arr.array("I",[0 for i in range(19200)],dtype='uint8')
    ImageRecFlag=0
    def __init__(self):
        super(MainGUI, self).__init__()
        MainGUI.ui = uic.loadUi("UserUI/UserUI.ui")
        self.ser = serial.Serial()
        #相关信号槽
        self.ui.OpenSerial.clicked.connect(self.OpenSerial)
        #以下两行代码可以放在qtdesginer中，但是还没找到对应的选项位置
        self.ui.ModeChoose.addItem(MODE[0])
        self.ui.ModeChoose.addItem(MODE[1])
        #定时器部分
        self.timer = QTimer()
        self.timer.timeout.connect(self.RecUI)
        self.timer.start(100)



        #MainGUI.init(self)
    def OpenSerial(self):
        #global PortList
        if  self.OpenSerSingal==0 :
            for i in list(serial.tools.list_ports.comports()):
                if MainGUI.ui.UartList.currentText() == i[1] :
                    self.ser.baudrate=int(self.ui.BaudRateText.toPlainText().encode('utf-8'))
                    self.ser.port=i[0]
                    try:
                        self.ser.open()
                    except Exception:
                        print("发生了什么错误qaq")
                    if self.ser.isOpen() :
                        self.ui.RxStates.clear()
                        self.ui.RxStates.setText("串口已经打开")
                        self.ui.OpenSerial.setText("关闭串口")
                        #self.ui.OpenSerial.setEnabled(False)
                        self.ui.ModeChoose.setEnabled(False)
                        self.OpenSerSingal=1
                    else :
                        self.ui.RxStates.clear()
                        self.ui.RxStates.setText("串口未打开")
                        self.OpenSerSingal=0
        elif self.OpenSerSingal==1:
            
            try:
                self.ser.close()
            except Exception:
                print("发生了什么错误qaq")
            if not self.ser.isOpen() :
                self.ui.RxStates.clear()
                self.ui.RxStates.setText("串口已经关闭")
                self.ui.OpenSerial.setText("打开串口")
                self.ui.ModeChoose.setEnabled(True)
                self.OpenSerSingal=0
            else:
                self.ui.RxStates.clear()
                self.ui.RxStates.setText("串口未关闭")
                self.OpenSerSingal=0
    def SerBegin(self):
        SeryThread=Thread(target=self.SerRun)
        SeryThread.start()
    def DrawThread(self):
        DrawThread=Thread(target=self.Image)#实例化一个画图像线程
        DrawThread.start()#开始线程
    def Image(self):
        while(True):
            
            time.sleep(0.1)
            if self.ImageRecFlag==1:
                self.SerLock.acquire()
                for i in range(120*160):
                    self.BGRImage[i]=(self.Image[i*2]*256+self.Image[i*2+1])
                    # self.RGBb=(self.BGRImage[i]&0x001f)<< 3
                    # self.RGBg=(self.BGRImage[i]&0x07e0)>> 3
                    # self.RGBr=(self.BGRImage[i]&0xf800)>> 8
                    # self.RGB888[i] = (self.RGBr << 16) + (self.RGBg << 8) + (self.RGBb << 0)
                # heigt, width =self.BGRImage.shape[:2]
                width=160
                heigt=120
                ShowImage = QImage((uint8)(self.BGRImage), width, heigt, QImage.Format_RGB16)
                ShowImage = QPixmap.fromImage(ShowImage)
                self.ui.ImageShow.setPixmap(ShowImage)
                self.ImageRecFlag=0
                self.SerLock.release()
    def SerRun(self):
        TempData=""
        while(True):
            time.sleep(0.1)
            #self.SerLock.locked()
            self.SerLock.acquire()
            if self.ser.isOpen():
                if not self.ui.ImageEnable.isChecked():
                    count = self.ser.inWaiting()
                    if  count!=0:
                        TempData=self.ser.read(count)
                        if not self.ui.HexEnable.isChecked() :
                            self.UartStr=TempData.decode('iso-8859-1')
                        else :
                            self.UartStr = ''
                            for i in range(0, len(TempData)):
                                self.UartStr = self.UartStr + '{:02X}'.format(TempData[i]) + ' '
                        self.RecFlag=1
                        
                        #MainGUI.UartPrint.UartPrintStr.emit(UartStr)
                        #MainGUI.ui.UartRec.append(UartStr) 
                        self.ser.flushInput()
                    else:
                        pass
                else:
                    if self.ImageRecFlag==0:
                        b=bytes.fromhex('ff')
                        self.ser.write(b)
                        count= self.ser.inWaiting()
                        while count!=38400:
                            count= self.ser.inWaiting()
                            self.ui.TEST.setText("CMD NOT FOUND")
                        self.Image=self.ser.read(count)
                        self.ImageRecFlag=1
                        self.ser.flushInput()
                        count=0
                        self.ui.TEST.setText("CMD FOUND")
                    # count= self.ser.inWaiting()
                    # if count>=76808 :
                    #     TempData=self.ser.read(count)
                    #     if CMD in TempData:
                    #         temp=TempData.index(CMD)
                    #         self.ui.TEST.setText("CMD FOUND")
                    #         self.UartStr=TempData[temp+4:temp+3+(120*160*2)]
                    #     else:
                    #         self.ui.TEST.setText("CMD NOT FOUND")
                    #     self.ser.flushInput()
                    #     count=0
                    #     TempData=""
                    # else:
                    #     pass

            else :
                pass
            self.SerLock.release()
    #定时器刷新
    def RecUI(self):
        if not self.ui.ImageEnable.isChecked():
            if self.RecFlag==1:
                self.SerLock.acquire()
                self.ui.SerialText.append(str(self.UartStr))
                self.SerLock.release()
                self.RecFlag=0
        else:
            pass
        #global PortList
        if MainGUI.ui.ModeChoose.currentText()==MODE[0] :
            self.ui.OpenSerial.setEnabled(False)
            self.ui.OpenUDP.setEnabled(True)
        else:
            self.ui.OpenSerial.setEnabled(True)
            self.ui.OpenUDP.setEnabled(False)
        
        self.PortList = list(serial.tools.list_ports.comports())
        if self.PortList != self.OldPortList:
            self.ui.UartList.clear()
            for i in list(self.PortList) :
                MainGUI.ui.UartList.addItem(i[1])
            self.OldPortList=self.PortList
            self.PortList=""
    
