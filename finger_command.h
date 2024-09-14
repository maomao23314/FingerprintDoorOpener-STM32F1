#ifndef FINGER_COMMAND_H
#define FINGER_COMMAND_H

#include "main.h"

uint8_t PS_COM_GetImage_T();
uint8_t PS_COM_GenChar_T(uint8_t bufferID);
uint8_t PS_COM_Match_T();
uint8_t PS_COM_Search_T();
uint8_t PS_COM_RegModel_T();
uint8_t PS_MOD_Cancle_T();
uint8_t PS_MOD_AutoEnroll_T(uint8_t ID,uint8_t Times);


#endif /* FINGER_COMMAND_H */