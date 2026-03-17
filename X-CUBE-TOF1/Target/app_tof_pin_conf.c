/**
 ******************************************************************************
 * @file    app_tof_pin_conf.c
 * @author  IMG SW Application Team
 * @brief   This file contains functions for TOF pins
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "app_tof_pin_conf.h"

extern volatile uint8_t ToF_EventDetected;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin == TOF_INT_EXTI_PIN)
  {
    ToF_EventDetected = 1;
  }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
