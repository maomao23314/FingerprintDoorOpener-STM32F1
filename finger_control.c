#include "finger_control.h"
#include "stdio.h"
#include "string.h"


enum SYSTEM_STATE system_state = SYSTEM_STATE_IDLE;//全局状态机

uint8_t receive_flag = 0xff;//ff为初始复位哦与模块定义区分开
uint8_t receive_data[16] = {0};//接收到的数据


/*
搜索流程(基于逻辑分析仪)
     ↓----------------------------↑
PS_GetImage->(收到0x02说明没有手指—↑)(收到00说明成功)->PS_GenChar(bufferID=1)->

返回0x00成功->PS_COM_Search()->接受数据
*/
uint8_t finger_search()
{
    system_state = SYSTEM_STATE_SEARCH;
    HAL_UART_Transmit(&huart2, (uint8_t*)"Start_search\n", 13, 1000);
    receive_flag=0x02;//这里给赋值，就会等待rx接收到新数据后再更新，是一种取巧的做法
    while(receive_flag == 0x02)//等待手指按压
    {
        PS_COM_GetImage_T();
        HAL_Delay(250);
        HAL_UART_Transmit(&huart2, (uint8_t*)"wait\n", 5, 1000);
    }
    if(receive_flag == 0x00)
    {
        PS_COM_GenChar_T(1);
        receive_flag=0x02;
        while(receive_flag == 0x02)//等待接收到数据
        {
            HAL_Delay(1);
        }
        if(receive_flag == 0x00)
        {
            PS_COM_Search_T();
            receive_flag=0x02;
            while(receive_flag == 0x02)//等待接收到数据
            {
                HAL_Delay(1);
            }
            char temp[16];
            if(receive_flag == 0x00)//搜寻正常并返回正常
            {
                HAL_UART_Transmit(&huart2, (uint8_t*)"Searched", 8, 1000);
                sprintf(temp, "%x,%x,%x,%x\n", receive_data[0], receive_data[1], receive_data[2], receive_data[3]);
                HAL_UART_Transmit(&huart2, (uint8_t*)temp, strlen(temp), 1000);
                int ID=0;
                ID = ((receive_data[0]<<8) | receive_data[1]);
                sprintf(temp, "ID:%d   ", ID);
                OLED_PrintString(0 ,0 ,temp, 16, OLED_COLOR_NORMAL);
                OLED_Refresh();

                return 0x00;
            }
            else{
                sprintf(temp, "error");
                OLED_PrintString(0 ,0 ,temp, 16, OLED_COLOR_NORMAL);
                OLED_Refresh();
                return 0x02;
            }
        }
    }
    else{
        HAL_UART_Transmit(&huart2, (uint8_t*)"error", 5, 1000);
        return 0x01;
    }
    return 0x00;
}

uint8_t decode_packet(uint8_t *data, uint16_t size){
    if(data[0] != 0xEF || data[1] != 0x01 || 
        data[2] != 0xff || data[3] != 0xff|| data[4] != 0xff || data[5] != 0xff
        || data[6] != 0x07)
    {
        return 0x01;//包头或者设备地址或  8者包标识不对
    }
    //HAL_UART_Transmit(&huart2, data, size, 1000);//测试查看模块回传数据用
    uint16_t sum = 0;//校验和
    uint16_t len = 0;
    len = (data[7]<<8) | data[8];//数据长度
    receive_flag=data[9];//接收确认码
    for(uint8_t i=0; i<len-3; i++)//-3是包括确认码和两字节校验和的
    {
        //如len=3，data[9]已取走则从data[10]开始
        receive_data[i] = data[10+i];//接收到的数据
    }

    for(uint8_t i=6; i<6+len+1; i++)//计算校验和
    {
        sum += data[i];
    }
    if(sum!= ((data[size-2]<<8) | data[size-1]))
    {
        HAL_UART_Transmit(&huart2, (uint8_t*)"wrong_sum\n", 10, 1000);
        return 0x02;//校验和不对
    }
    return 0x00;
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    if (huart->Instance == USART1)
    {
        // 使用DMA将接收到的数据发送给2号UART
        //HAL_UART_Transmit_DMA(&huart2, u1_rx_buffer, Size);
        //HAL_UART_Transmit(&huart2, (uint8_t*)"receive\n", 8, 1000);

        /*解析数据包*/
        decode_packet(u1_rx_buffer, Size);


        // 重新启动接收，使用Ex函数，接收不定长数据
        HAL_UARTEx_ReceiveToIdle_DMA(&huart1, u1_rx_buffer, sizeof(u1_rx_buffer));
        // 关闭DMA传输过半中断（HAL库默认开启，但我们只需要接收完成中断）
        __HAL_DMA_DISABLE_IT(huart2.hdmarx, DMA_IT_HT);
    }
}
