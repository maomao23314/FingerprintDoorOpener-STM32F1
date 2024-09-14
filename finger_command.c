#include "finger_command.h"


/*
通用指令：PS_COM
模块指令：PS_MOD
*/
//存放指令用的数组
/*包头2字节+地址0xffffffff4字节+包标识1字节+包长度2字节+指令码1字节+校验和2字节*/
/*校验和不包括包头和设备地址*/
uint8_t PS_COM_GetImage_T()
{
    uint8_t PS_getimage[]={0xef,0x01, 0xff,0xff,0xff,0xff ,0x01, 0x00,0x03 ,0x01, 0x00,0x05};
    HAL_UART_Transmit(&huart1, PS_getimage, 12, 1000);
    return 0;
}
uint8_t PS_COM_GenChar_T(uint8_t bufferID)
{
    uint8_t PS_genchar[]={0xef,0x01, 0xff,0xff,0xff,0xff ,0x01, 0x00,0x04 ,0x02,bufferID, 0x00,bufferID+7};
    HAL_UART_Transmit(&huart1, PS_genchar, 13, 1000);
    return 0;
}
uint8_t PS_COM_Match_T()
{
    //包头ef01 设备地址4B 包标识1B 包长度2B 指令码1B 校验和2B
    uint8_t PS_match[]={0xef,0x01, 0xff,0xff,0xff,0xff ,0x01, 0x00,0x03 ,0x03, 0x00,0x07};
    HAL_UART_Transmit(&huart1, PS_match, 12, 1000);
    return 0;
}
uint8_t PS_COM_Search_T()
{
    uint8_t bufferID=0x01;uint16_t StartPage=0x0000;uint16_t PageNum=0x0032;
    //注： BufferID 默认为1，以模板缓冲区中指纹模板搜索整个或部分指纹库
    uint8_t PS_search[]={0xef,0x01, 0xff,0xff,0xff,0xff,
    0x01, 0x00,0x08, 0x04,bufferID, StartPage<<8,StartPage&0xff , PageNum<<8,PageNum&0xff , 0x00,0x40/*手动计算的校验码*/};
    HAL_UART_Transmit(&huart1, PS_search, 17, 1000);
    return 0;
}

uint8_t PS_COM_RegModel_T()/*功能说明： 将特征文件融合后生成一个模板，结果存于模板缓冲区中。*/
{
    uint8_t PS_regmodel[]={0xef,0x01, 0xff,0xff,0xff,0xff, 0x01, 0x00,0x03,0x05, 0x00,0x09};
    HAL_UART_Transmit(&huart1, PS_regmodel, 10, 1000);
    return 0;
}


uint8_t PS_MOD_Cancle_T()
{
    uint8_t PS_cancle[]={0xef,0x01, 0xff,0xff,0xff,0xff, 0x01, 0x00,0x03, 0x30, 0x00,0x34};//--------------校验码不确定
    HAL_UART_Transmit(&huart1, PS_cancle, 11, 1000);
    return 0;
}

/*
*功能说明： 一站式注册指纹，包含采集指纹、生成特征、组合模板、存储模板等功能。
*输入参数： ID 号，录入次数，参数
*/
uint8_t PS_MOD_AutoEnroll_T(uint8_t ID,uint8_t Times)
{
    uint8_t param=0x00;
    /*参数：最低位为bit0。
bit0：采图背光灯控制位，0-LED 长亮，1-LED 获取图像成功后灭；
bit1：保留
bit2： 注册过程中，要求模组返回关键步骤， 0-要求返回， 1-不要求返回
bit3：是否允许覆盖ID 号， 0-不允许， 1-允许；
bit4：允许指纹重复注册控制位， 0-允许， 1-不允许；
bit5：注册时，多次指纹采集过程中，是否要求手指离开才能进入下一次指纹图像采集， 0-要求离开； 1-不要求离开；
bit6~bit15：预留。
    */
    param|=0x09;//001001//获取成功后灭灯,保留,要求返回,允许覆盖ID号,允许重复注册,要求离开
    uint16_t sum=0x3a+ID+Times+param;
    uint8_t PS_autoenroll[]={0xef,0x01, 0xff,0xff,0xff,0xff, 0x01, 
    0x00,0x08, 0x31, 0x00, ID,Times, 0x00,param, sum>>8,sum&0xff};
    HAL_UART_Transmit(&huart1, PS_autoenroll, 17, 1000);
    return 0;
}



