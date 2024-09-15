#ifndef FINGER_CONTROL_H_
#define FINGER_CONTROL_H_

#include "main.h"
#include "finger_command.h"

enum COMFIRM_CODE {//确认码定义：
    FINGER_OK = 0x00,//00H：表示指令执行完毕或OK；
    FINGER_DATA_RECEIVE_ERROR = 0x01,//01H：表示数据包接收错误；
    FINGER_NO_FINGER = 0x02,//02H：表示传感器上没有手指；
    FINGER_IMAGE_SAVE_ERROR = 0x03,//03H：表示录入指纹图像失败；
    FINGER_IMAGE_TOO_BRIGHT = 0x04,//04H：表示指纹图像太干、太淡而生不成特征；
    FINGER_IMAGE_TOO_DUSTY = 0x05,//05H：表示指纹图像太湿、太糊而生不成特征；
    FINGER_IMAGE_TOO_SMOOTH = 0x06,//06H：表示指纹图像太乱而生不成特征；
    FINGER_IMAGE_TOO_SMALL = 0x07,//07H：表示指纹图像正常，但特征点太少（或面积太小）而生不成特征；
    FINGER_IMAGE_TOO_NOISY = 0x08,//08H：表示指纹不匹配；
    FINGER_FINGER_NOT_FOUND = 0x09,//09H：表示没搜索到指纹；
    FINGER_MERGE_FEATURE_FAILED = 0x0a,//0aH：表示特征合并失败；
    FINGER_ADDRESS_OUT_OF_RANGE = 0x0b,//0bH：表示访问指纹库时地址序号超出指纹库范围；
    FINGER_READ_TEMPLATE_FAILED = 0x0c,//0cH：表示从指纹库读模板出错或无效；
    FINGER_UPLOAD_FEATURE_FAILED = 0x0d,//0dH：表示上传特征失败；
    FINGER_MODULE_CANNOT_RECEIVE_DATA = 0x0e,//0eH：表示模组不能接收后续数据包；
    FINGER_UPLOAD_IMAGE_FAILED = 0x0f,//0fH：表示上传图像失败；
    FINGER_DELETE_TEMPLATE_FAILED = 0x10,//10H：表示删除模板失败；
    FINGER_CLEAR_DATABASE_FAILED = 0x11,//11H：表示清空指纹库失败；
    FINGER_LOW_POWER_MODE_ERROR = 0x12,//12H：表示不能进入低功耗状态；
    FINGER_PASSWORD_ERROR = 0x13,//13H：表示口令不正确；
    FINGER_RESET_ERROR = 0x14,//14H：表示系统复位失败；
    FINGER_NO_ORIGINAL_IMAGE = 0x15,//15H：表示缓冲区内没有有效原始图而生不成图像；
    FINGER_FINGER_NOT_MOVED = 0x17,//17H：表示残留指纹或两次采集之间手指没有移动过；
    FINGER_FLASH_ERROR = 0x18,//18H：表示读写FLASH 出错；
    FINGER_INVALID_REGISTER_NUMBER = 0x1a,//1aH：无效寄存器号；
    FINGER_REGISTER_SETTING_ERROR = 0x1b,//1bH：寄存器设定内容错误号；
    FINGER_NOTEPAD_PAGE_ERROR = 0x1c,//1cH：记事本页码指定错误；
    FINGER_PORT_OPERATION_ERROR = 0x1d,//1dH：端口操作失败；
    FINGER_AUTO_ENROLL_FAILED = 0x1e,//1eH：自动注册（enroll）失败；
    FINGER_FINGER_DATABASE_FULL = 0x1f,//1fH：指纹库满；
    FINGER_DEVICE_ADDRESS_ERROR = 0x20,//20H：设备地址错误；
    FINGER_PASSWORD_INCORRECT = 0x21,//21H：密码有误；
    FINGER_TEMPLATE_NOT_EMPTY = 0x22,//22 H：指纹模板非空；
    FINGER_TEMPLATE_EMPTY = 0x23,//23 H：指纹模板为空；
    FINGER_FINGER_DATABASE_EMPTY = 0x24,//24 H：指纹库为空；
    FINGER_ENROLL_COUNT_ERROR = 0x25,//25 H：录入次数设置错误；
    FINGER_TIMEOUT = 0x26,//26 H：超时；
    FINGER_FINGER_EXIST = 0x27,//27 H：指纹已存在；
    FINGER_TEMPLATE_RELATED = 0x28,//28 H：指纹模板有关联；
    FINGER_SENSOR_INIT_ERROR = 0x29,//29 H：传感器初始化失败；
    FINGER_MODULE_INFO_NOT_EMPTY = 0x2a,//2AH：模组信息非空；
    FINGER_MODULE_INFO_EMPTY = 0x2b,//2BH：模组信息为空；
    FINGER_IMAGE_AREA_TOO_SMALL = 0x33,//33H：图像面积小
    FINGER_IMAGE_UNAVAILABLE = 0x34,//34H：图像不可用
    FINGER_ILLEGAL_DATA = 0x35,//35H：非法数据
    FINGER_ENROLL_COUNT_LESS_THAN_LIMIT = 0x40,//40H：注册次数少于规定次数
};


enum SYSTEM_STATE {
    SYSTEM_STATE_IDLE = 0x00, //空闲状态
    SYSTEM_STATE_ENROLL = 0x01, //注册状态
    SYSTEM_STATE_WATING = 0x02, //等待状态
    SYSTEM_STATE_SEARCH = 0x03, //搜索状态
    
    SYSTEM_STATE_ERROR = 0xFF, //错误状态
};
extern enum SYSTEM_STATE system_state;//全局状态机


extern uint8_t receive_flag;
extern uint8_t receive_data[16];


uint8_t finger_search();


#endif /* FINGER_CONTROL_H_ */