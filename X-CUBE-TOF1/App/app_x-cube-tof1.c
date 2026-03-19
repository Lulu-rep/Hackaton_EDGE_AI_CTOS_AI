/**
  ******************************************************************************
  * @file          : app_x-cube-tof1.c
  * @author        : IMG SW Application Team
  * @brief         : This file provides code for the configuration
  *                  of the STMicroelectronics.X-CUBE-TOF1.2.0.0 instances.
  ******************************************************************************
  *
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "app_x-cube-tof1.h"
#include "main.h"
#include <stdio.h>

#include "53l5a1_ranging_sensor.h"
#include "app_tof_pin_conf.h"
#include "stm32f4xx_nucleo.h"

#include "NanoEdgeAI.h"

/*
 * The purpose of this sample application is to showcase the threshold detection
 * functionality of this device.
 *
 * When the device detects a target that match the configuration an IT is generated
 * and the host will start printing the measurement information on the serial connection (UART).
 *
 * Here is the default configuration:
 *
 * ITConfig.Criteria = RS_IT_IN_WINDOW;
 * ITConfig.LowThreshold = 200; // distance in mm
 * ITConfig.HighThreshold = 600; // distance in mm
 *
 * Other availables interrupt generation criteria for this device are:
 *
 * - RS_IT_DEFAULT          // IT if new measurement is ready (no thresholds)
 * - RS_IT_IN_WINDOW        // IT if distance > thresh_high
 * - RS_IT_OUT_OF_WINDOW    // IT if distance < LowThreshold OR distance > HighThreshold
 * - RS_IT_BELOW_LOW        // IT if distance <= LowThreshold
 * - RS_IT_ABOVE_HIGH       // IT if distance > HighThreshold
 * - RS_IT_EQUAL_LOW        // IT if distance == LowThreshold
 * - RS_IT_NOT_EQUAL_LOW    // IT if distance != LowThreshold
 */

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define SUCCESSIVE_MATRIX	1 						/* How many successive matrix we get in our neai_buffer. Possible values: power of 2 between 1 and 64 */
#define MATRIX_RESOLUTION	64 						/* Possible values: 64 for 8x8 resolution, 16 for 4x4 resolution */
#define AXES				1						/* Possible value: 1 (distance data) */
#define NEAI_BUFFER_SIZE	SUCCESSIVE_MATRIX * MATRIX_RESOLUTION * AXES	/* The size of our NEAI buffer used for classification */
#define MIN_THRESHOLD		30						/* Minimum distance in mm between sensor & object to detect. Possible values: 40 to 4000 */
#define MAX_THRESHOLD		220 					/* Maximum distance in mm between sensor & object to detect. Possible values: 40 to 4000 */
#define NEAI_MODE			1						/* Possible values: 0 for data logging mode, 1 for NanoEdgeAI classification mode */

/* Private variables ---------------------------------------------------------*/
static RANGING_SENSOR_Capabilities_t Cap;
static RANGING_SENSOR_ProfileConfig_t Profile;
static int32_t status = 0;
volatile uint8_t ToF_EventDetected = 0;
enum neai_state neai_status = 0;
float neai_buffer[NEAI_BUFFER_SIZE] = {0};
uint8_t anomaly_score = 0; // Score de similarité entre 0 et 100
uint8_t learning_mode = 1; // 1 = Phase d'apprentissage, 0 = Phase de détection


/* Private function prototypes -----------------------------------------------*/
static void MX_53L5A1_ThresholdDetection_Init(void);
static void MX_53L5A1_ThresholdDetection_Process(void);

void MX_X_CUBE_TOF1_Init(void)
{
  /* USER CODE BEGIN SV */

  /* USER CODE END SV */

  /* USER CODE BEGIN X_CUBE_TOF1_Init_PreTreatment */

  /* USER CODE END X_CUBE_TOF1_Init_PreTreatment */

  /* Initialize the peripherals and the TOF components */

  MX_53L5A1_ThresholdDetection_Init();

  /* USER CODE BEGIN X_CUBE_TOF1_Init_PostTreatment */

  /* USER CODE END X_CUBE_TOF1_Init_PostTreatment */
}

/*
 * LM background task
 */
void MX_X_CUBE_TOF1_Process(void)
{
  /* USER CODE BEGIN X_CUBE_TOF1_Process_PreTreatment */

  /* USER CODE END X_CUBE_TOF1_Process_PreTreatment */

  MX_53L5A1_ThresholdDetection_Process();

  /* USER CODE BEGIN X_CUBE_TOF1_Process_PostTreatment */

  /* USER CODE END X_CUBE_TOF1_Process_PostTreatment */
}

static void MX_53L5A1_ThresholdDetection_Init(void)
{
  /* Initialize Virtual COM Port */
  BSP_COM_Init(COM1);

  status = VL53L5A1_RANGING_SENSOR_Init(VL53L5A1_DEV_CENTER);

  if (status != BSP_ERROR_NONE)
  {
    printf("VL53L5A1_RANGING_SENSOR_Init failed\r\n");
    while(1);
  }
}

static void MX_53L5A1_ThresholdDetection_Process(void)
{
  uint32_t Id;
  uint8_t zones_per_line;

  RANGING_SENSOR_ITConfig_t ITConfig;
  RANGING_SENSOR_Result_t Result;

  VL53L5A1_RANGING_SENSOR_ReadID(VL53L5A1_DEV_CENTER, &Id);
  VL53L5A1_RANGING_SENSOR_GetCapabilities(VL53L5A1_DEV_CENTER, &Cap);

  Profile.RangingProfile = RS_PROFILE_8x8_CONTINUOUS;
  Profile.TimingBudget = 5; /* 5 ms < TimingBudget < 100 ms */
  Profile.Frequency = 15; /* Hz */
  Profile.EnableAmbient = 0; /* Enable: 1, Disable: 0 */
  Profile.EnableSignal = 1; /* Enable: 1, Disable: 0 */

  /* set the profile if different from default one */
  VL53L5A1_RANGING_SENSOR_ConfigProfile(VL53L5A1_DEV_CENTER, &Profile);

  /* threshold parameters */
  ITConfig.Criteria = RS_IT_IN_WINDOW;
  ITConfig.LowThreshold = MIN_THRESHOLD; /* mm */
  ITConfig.HighThreshold = MAX_THRESHOLD; /* mm */

  VL53L5A1_RANGING_SENSOR_ConfigIT(VL53L5A1_DEV_CENTER, &ITConfig);

  status = VL53L5A1_RANGING_SENSOR_Start(VL53L5A1_DEV_CENTER, RS_MODE_ASYNC_CONTINUOUS);

  zones_per_line = ((Profile.RangingProfile == RS_PROFILE_8x8_AUTONOMOUS) ||
      (Profile.RangingProfile == RS_PROFILE_8x8_CONTINUOUS)) ? 8 : 4;

  if (status != BSP_ERROR_NONE)
  {
    printf("VL53L5A1_RANGING_SENSOR_Start failed\r\n");
    while(1);
  }

  neai_status = neai_anomalydetection_init(false);
  if(neai_status != NEAI_OK) {
      printf("ERROR NEAI Anomaly Detection init\r\n");
      while(1);
  }

  /* Reset ToF event variable to avoid starting log at boot */
  ToF_EventDetected = 0;

  while (1)
  {
  /* interrupt mode */
	  if (ToF_EventDetected != 0) {
		for (uint8_t i = 0; i < SUCCESSIVE_MATRIX; i++) {
			do {
				status = VL53L5A1_RANGING_SENSOR_GetDistance(VL53L5A1_DEV_CENTER, &Result);
			} while(status != BSP_ERROR_NONE);
			for (uint8_t j = 0; j < Result.NumberOfZones; j += zones_per_line) {
				for (int8_t k = (zones_per_line - 1); k >= 0; k--) {
					if (Result.ZoneResult[j+k].NumberOfTargets > 0) {
						neai_buffer[(i * MATRIX_RESOLUTION) + j + k] = (((float)Result.ZoneResult[j+k].Distance[0] > ITConfig.HighThreshold) || ((float)Result.ZoneResult[j+k].Distance[0] < ITConfig.LowThreshold)) ? 0.0 : (float)Result.ZoneResult[j+k].Distance[0];
					}
					else {
						/* If no target detected we set both distance & signal data to 0.0 */
						neai_buffer[(i * MATRIX_RESOLUTION) + j + k] = 0.0;
					}
				}
			}
		}

		if (NEAI_MODE) {
			static uint32_t count = 0; // Compteur de tirs

			if (learning_mode == 1) {
			    neai_status = neai_anomalydetection_learn(neai_buffer);
			    count++;
			    printf("Apprentissage : %ld / 50\r\n", count);

			    if (count >= 50) {
			        learning_mode = 0; // On bascule en détection
			        printf("--- APPRENTISSAGE TERMINE : Passage en Mode Detection ---\r\n");
			    }
			} else {
			/* L'IA compare la mesure au modèle parfait */
			neai_status = neai_anomalydetection_detect(neai_buffer, &anomaly_score);

			if (anomaly_score > 90) {
				printf("Piece OK ! Score : %d/100\r\n", anomaly_score);
			} else {
				printf("ANOMALIE DETECTEE ! Score : %d/100\r\n", anomaly_score);
			}
		}
		    HAL_Delay(500);
		}
		else {
			/* Print the whole buffer to the serial in order to log data */
			for (uint16_t i = 0; i < NEAI_BUFFER_SIZE; i++) {
				printf("%.1f ", neai_buffer[i]);
			}
			printf("\r\n");
		}
		/* Reset ToF event variable when log has been done successfully */
		ToF_EventDetected = 0;
	}
  }
}

#ifdef __cplusplus
}
#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
