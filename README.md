# LONGAN_Freertos
      我是2020年应届生，喜欢LINUX。虽然想从事内存管理这一方向的工作，但由于研究方向的原因，在意向工作领域中毫无经验。这
    些都没关系，可以从头再来，linux创始人Torvalds说过， “Talk is cheap. Show me the code.”。
    这是在开源软件方向中，了解到的最新的开源指令集RISC-V。本次实验采用开发板是《Longan Nano RISC-V GD32VF103CBT6 单片
    机带液晶显示屏开发板》。板上的资源有一个三色的LED灯，和一块小液晶屏。本次实验将实现对三色的LED灯作为临界资源进行控制。
    实验内容有：
    1、移植FreeRTOS和液晶屏
    2、创建计数节拍任务（Time_task）、液晶屏显示任务（TFT_task）、红灯控制任务（ledR_task）、绿灯控制任务（ledG_task）、
    蓝灯控制任务（ledB_task）。
    3、采用队列和二值信号量。
    4、资源空闲以后，计算计数节拍任务（Time_task）发来的节拍值，哪个任务最先满足，谁就获得控制权。采用二值信号量进行访问
    控制。
    5、条件分别是：计数节拍值能够被3整除，ledR_task获得控制权；被5整除，ledG_task获得控制权；被7整除，ledB_task获得控
    制权
    6、ledR_task获取资源后占用5个计数节拍， ledG_task获取资源后占用7个计数节拍， ledB_task获取资源后占用3个计数节拍。
![好尴尬吖](https://github.com/tamir-li/LONGAN_Freertos/blob/master/%E5%AE%9E%E9%AA%8C%E6%B5%81%E7%A8%8B%E5%9B%BE.PNG 任务流程图)
# 注意，本次实验没有采用串口以及jtag进行调试，实时性不是很准。


