/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "can.h"
#include "i2c.h"
#include "spi.h"
#include "usb_device.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "usbd_cdc_if.h"
#include "string.h"
#include <stdio.h>
#include "icm20948.h"
#include "math.h"
#include "variabler.h"
#include "extern_variabler.h"
#include "stts75.h"
#include "trykk.h"


/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */



/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

void init_usbbuf(void);
void les_og_beregne_vinkler(kommskift kommunikasjonsmetode);
void les_og_behandle_temp(kommskift kommunikasjonsmetode);
void lag_datapakke_usb(void);
void lag_datapakker_CAN(void);
void oppstartCAN(uint8_t filterGruppe, CAN_HandleTypeDef *canPort);
void sendDataCAN(uint16_t id, CAN_HandleTypeDef *canPort);
void bryteravprelling(void);
temp_status TEMP_les(I2C_HandleTypeDef *handle, uint8_t Enhetavlesning, float *temp_f);
void senddatapakkerCAN(void);
void sjekklekkasje(void);
void TRYKKSENSOR_Initialiser(TRYKKSENSOR *dev, I2C_HandleTypeDef *i2cHandle);
trykk_status TRYKKSENSOR_LesKalibrering(TRYKKSENSOR *dev);
float TRYKKSENSOR_LesTrykk(TRYKKSENSOR *dev);
trykk_status TRYKK_sjekk(TRYKKSENSOR *dev, uint8_t *feil);
uint16_t dybdeutregning(float P0, float P, vanntetthet vanntype);

//void sjekkbryter(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

//	Initialisering av buffer for dataoverf??ring over USB
//------------------------------------------------------------
  init_usbbuf();

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_CAN1_Init();
  MX_USB_DEVICE_Init();
  MX_SPI1_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */

// Initialiserer tickstart for sending og trykk
  tickstart = HAL_GetTick();
  trykktick = HAL_GetTick();

//initialiser 9DOF
//------------------------------------------------------------
  icm20948_init();

//	Oppstart av CAN
//------------------------------------------------------------
  oppstartCAN(3, &hcan1);
//------------------------------------------------------------

// Initialiserer trykksensoren.
//------------------------------------------------------------
  TRYKKSENSOR_Initialiser( &trykksensor, &hi2c1 );
  TRYKKSENSOR_LesKalibrering( &trykksensor );
//------------------------------------------------------------

//------------------------------------------------------------
//------------------------------------------------------------
//---------------------Kommunikasjontest----------------------
//------------------------------------------------------------
//  if(!icm20948_who_am_i()){
//	  kimufeil = 1;
//  }
//  icm20948_who_am_i();
//  TEMP_sjekk(&hi2c1, LES_AV_TEMP_SENS, &ktempfeil);
//  TRYKK_sjekk(&trykksensor, &ktrykkfeil);
//  sendDataCAN(85, &hcan1);
//  HAL_Delay(10);
//  if(ktempfeil == 0 && ktrykkfeil == 0 && kimufeil == 0 && canmottatt == 1){
//	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, 1);
//  }
//------------------------------------------------------------
//------------------------------------------------------------
//------------------------------------------------------------

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */


//	  sjekklekkasje();




	  //------------------------------------------------------------
	  //------------------------------------------------------------
	  //------------------------Lekkasjetest------------------------
	  //------------------------------------------------------------

//	  if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2)){		// Om pinne PA2 (LEKK3) f??r h??yt signal,
//	    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, 1);	// gi ut h??yt signal p?? pinne PA3 (aktiver lysdiode D3).
//	  }
//	  else{											// Ellers,
//	    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, 0);	// gi ut lavt signal p?? pinne PA3 (deaktiver lysdiode D3)
//	  }

	  //------------------------------------------------------------
	  //------------------------------------------------------------
	  //------------------------------------------------------------



	  //------------------------------------------------------------
	  //------------------------------------------------------------
	  //--------------Testbryterlogikk for enkelttrykk--------------
	  //------------------------------------------------------------

//	  if((HAL_GetTick()-trykktick) >= 100){
//		  bryteravprelling();
//		  trykktick = HAL_GetTick();
//	  }
//
//	  if(klarnytttrykk && !startdata){
//		  HAL_GPIO_WritePin(GPIOC, STATLED_Pin, 1);
//		  HAL_GPIO_WritePin(GPIOC, DATALED_Pin, 0);
//	  }
//
//	  else if(klarnytttrykk == 0){
//		  HAL_GPIO_WritePin(GPIOC, STATLED_Pin, 0);
//		  HAL_GPIO_WritePin(GPIOC, DATALED_Pin, 1);
//	      TRYKKSENSOR_LesTrykk(&trykksensor, P0);         // Leser initialtrykk for ?? f?? kunne regne ut dybde
//	  }


	  //------------------------------------------------------------
	  //------------------------------------------------------------
	  //------------------------------------------------------------

	  //------------------------------------------------------------
	  //------------------------------------------------------------
	  //--------------Testbryterlogikk med avprelling---------------
	  //------------------------------------------------------------

//	  if((HAL_GetTick()-trykktick) >= 100){
//		  bryteravprelling();
//		  trykktick = HAL_GetTick();
//	  }
//
//	  if(gyldigtrykk){
//		  trykk++;
//		  if(trykk == 2){
//			  trykk = 0;
//		  }
//		  gyldigtrykk = 0;
//	  }
//
//
//
//	  if(trykk == 0){
//		  HAL_GPIO_WritePin(GPIOC, STATLED_Pin, 1);
//		  HAL_GPIO_WritePin(GPIOC, DATALED_Pin, 0);
//
//		  t1 = 0;
//		  sek = 0;
//		  imuinit = 0;
//		  forstemaaling = 0;
//
//	  }
//
//	  else if(trykk == 1){
//		  HAL_GPIO_WritePin(GPIOC, STATLED_Pin, 0);
//		  HAL_GPIO_WritePin(GPIOC, DATALED_Pin, 1);

	  //------------------------------------------------------------
	  //------------------------------------------------------------
	  //------------------------------------------------------------


	  //------------------------------------------------------------
	  //------------------------------------------------------------
	  //------------------------Testswitch--------------------------
	  //------------------------------------------------------------

//	  if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_8) == 0){
//		  HAL_GPIO_WritePin(GPIOC, STATLED_Pin, 1);
//		  HAL_GPIO_WritePin(GPIOC, DATALED_Pin, 0);
//
//		  t1 = 0;
//		  sek = 0;
//		  imuinit = 0;
//		  forstemaaling = 0;
//
//	  }
//
//	  else if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_8) == 1){
//		  HAL_GPIO_WritePin(GPIOC, STATLED_Pin, 0);
//		  HAL_GPIO_WritePin(GPIOC, DATALED_Pin, 1);

//	      TRYKKSENSOR_LesTrykk(&trykksensor);         // Les trykket og temperaturen, variablene blir lagt i structen.


	  //------------------------------------------------------------
	  //------------------------------------------------------------
	  //------------------------------------------------------------


	  //------------------------------------------------------------
	  //------------------------------------------------------------
	  //-----Er det sendt melding om ?? starte dataoverf??ring?-------
	  //------------------------------------------------------------

	  if(startdata == 0){
		  HAL_GPIO_WritePin(GPIOC, STATLED_Pin, 1);
		  HAL_GPIO_WritePin(GPIOC, DATALED_Pin, 0);

//		  t1 = 0;
//		  sek = 0;
//		  imuinit = 0;
//		  forstemaaling = 0;

	  }

	  else if(startdata == 1){





		  //------------------------------------------------------------
		  //------------------------------------------------------------
		  //---------------------Temperaturtest-------------------------
		  //------------------------------------------------------------

//		  // Om det er g??tt 1 minutt,
//		  if((HAL_GetTick()-tickstart) >= 60000 || forstemaaling == 0){
//
//			  // Les av temperatursensor, sett polaritetsflagg,
//			  // gj??r om til uint8 og kopierer verdi inn i vektor for sending
//			  les_og_behandle_temp();
//
//			  // Send vektor med temperaturdata over USB
//			  CDC_Transmit_FS((uint8_t *)temp_usb, 6);
//
//			  // Resett teller
//			  tickstart = HAL_GetTick();
//
//			  // Ikke lengre f??rste m??ling
//			  forstemaaling = 1;
//		  }

		  //------------------------------------------------------------
		  //------------------------------------------------------------
		  //------------------------------------------------------------

		  // Non-blocking delayfunksjon
		  if((HAL_GetTick()-tickstart) >= 50){


		  //------------------------------------------------------------
		  //----------------------Sensordatabehandling------------------
		  //----------------------og overf??ring til ROV-----------------
	      //------------------------------------------------------------
			  if(kalibrer){
				  P0 = TRYKKSENSOR_LesTrykk(&trykksensor);
				  kalibrer = 0;
			  }
			  // Leser av data fra gyroskop og akselerometer med skalert funksjon
			  les_og_beregne_vinkler(CAN);

			  // Leser av data fra temperatursensor
			  les_og_behandle_temp(CAN);

			  // Leser av trykk og gj??r om til dybde
			  P = TRYKKSENSOR_LesTrykk(&trykksensor);
			  c_hiv = dybdeutregning(P0, P, FERSKVANN);

			  // Se etter lekkasje
			  sjekklekkasje();

			  //----------------------
			  // Sender data over CAN
			  //----------------------

			  lag_datapakker_CAN();

			  senddatapakkerCAN();

			  // resett timer
			  tickstart = HAL_GetTick();
		  }

			//------------------------------------------------------------
			//------------------------------------------------------------
			//------------------------------------------------------------

		  //------------------------------------------------------------
		  //------------------------------------------------------------
		  //-----------------------IMU-test-----------------------------
		  //------------------------------------------------------------


//			  // Leser av data fra gyroskop og akselerometer med skalert funksjon
//			  les_og_beregne_vinkler(USB);
//
//			  // Setter sammen data fra IMU
//			  lag_datapakke_usb();
//
//			  // Setter opp variabel for millisekund
//			  if(t1 == 0){
//				  tid = 0;
//				  t1 = HAL_GetTick();
//			  }
//			  else{
//				  t2 = HAL_GetTick();
//				  tid = t2-t1;
//			  }
//
//			  // Setter opp variabel for sekund
//			  if(tid >= 1000){
//				  sek += 1;
//				  tid -= 1000;
//				  t1 = HAL_GetTick();
//			  }
//
//			  // Deler opp 16-bits tidsdata til 8-bits data for sending over USB
//			  lsb_tid = ((uint16_t)tid >> 0) & 0xFF;
//			  msb_tid = ((uint16_t)tid >> 8) & 0xFF;
//
//			  lsb_sek = ((uint16_t)sek >> 0) & 0xFF;
//			  msb_sek = ((uint16_t)sek >> 8) & 0xFF;
//
//			  // Kopierer innhold fra tidsdata inn i datapakke
//			  memcpy(&imu_usb[1], &msb_sek, 1);
//			  memcpy(&imu_usb[2], &lsb_sek, 1);
//			  memcpy(&imu_usb[3], &msb_tid, 1);
//			  memcpy(&imu_usb[4], &lsb_tid, 1);
//
//
//			  // Sender IMU-datapakke med tid
//			  CDC_Transmit_FS((uint8_t *)imu_usb, 65);
//
//			  // resett timer
//			  tickstart = HAL_GetTick();
//		  }
			//------------------------------------------------------------
			//------------------------------------------------------------
			//------------------------------------------------------------


	//-------------------------------------
	// Slutt p?? if-setning rundt testbryter/varsel om dataoverf??ring
	//-------------------------------------
	  }
    //-------------------------------------
    //-------------------------------------

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 144;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 6;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
	//
	// Interupt for CAN-Buss mottak av data
	//

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan11) {
    HAL_CAN_GetRxMessage(hcan11, CAN_RX_FIFO0, &rxHeader, canRX); //Receive CAN bus message to canRX buffer
    switch (rxHeader.StdId) {


    // Testkode for ?? sjekke kommunikasjon over CAN-buss
//	case 97:
//		canmottatt = 1;
//		break;


    // Mottar beskjed for ?? starte opp systemet
    case 96:
        HAL_GPIO_WritePin(GPIOC, STATLED_Pin, 0);
        HAL_GPIO_WritePin(GPIOC, DATALED_Pin, 1);
//        P0 = TRYKKSENSOR_LesTrykk(&trykksensor);
        startdata = 1;
        break;

	// Kommando for ?? kalibrere trykksensor og IMU
	case 126:
//		P0 = TRYKKSENSOR_LesTrykk(&trykksensor);
		kalibrer = 1;
		break;

    // Heartbeat
    case 127: // IKKE FJERN DENNE
//    	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, 1);
//    	ping = HAL_GetTick();
        memcpy(csend, (uint8_t*) &"pong!\n", 6);
        sendDataCAN(190, &hcan1);
        break;

    }
}

	//
	//
	//
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

