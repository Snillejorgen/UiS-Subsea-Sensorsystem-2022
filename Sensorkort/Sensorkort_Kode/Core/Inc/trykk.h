/*
 * Trykksensor I2C Driver
 *
 *  Created on: Mar 23, 2022
 *      Author: Hanne Lovise Berger
 *
 *  Her er alle variabler som tilhører trykksensoren definert.
 */

#ifndef INC_TRYKK_H_
#define INC_TRYKK_H_

#include "stm32f4xx_hal.h" // Needed for I2C
//#include "stm32f4xx.h"

// Definerer datatype for suksessfull kommunikasjon eller ikke
// ----------------------------------------------------------------

typedef enum{
	TRYKK_SUKSESS 	= 0x01,
	TRYKK_FEIL 		= 0x00
} trykk_status;

// ----------------------------------------------------------------


// Definerer struct for type vann
// ----------------------------------------------------------------
typedef enum{
	SALTVANN	= 0x00,
	FERSKVANN	= 0x01
} vanntetthet;
// ----------------------------------------------------------------

// DEFINES
#define TRYKKSENSOR_DEVICE_WRITE	(0x77 << 1) 		// Pin CSB LOW
#define TRYKKSENSOR_DEVICE_READ		(0x77 << 1| 0x01)	//

// REGISTRE (p.7)
#define TRYKKSENSOR_RESET		0x1E

#define TRYKKSENSOR_D1_256		0x40 //OSR = 256
#define TRYKKSENSOR_D1_512		0x42 //OSR = 512
#define TRYKKSENSOR_D1_1024		0x44 //OSR = 1024
#define TRYKKSENSOR_D1_2048		0x46 //OSR = 2048
#define TRYKKSENSOR_D1_4096		0x48 //OSR = 4096

#define TRYKKSENSOR_D2_256		0x50 //OSR = 256
#define TRYKKSENSOR_D2_512		0x52 //OSR = 512
#define TRYKKSENSOR_D2_1024		0x54 //OSR = 1024
#define TRYKKSENSOR_D2_2048		0x56 //OSR = 2048
#define TRYKKSENSOR_D2_4096		0x58 //OSR = 4096

#define TRYKKSENSOR_ADC_READ	0x00 //24bit trykk/temp

#define TRYKKSENSOR_PROM_FAC	0xA0 // Factory calibration
#define TRYKKSENSOR_PROM_C1		0xA2 // Coefficient 1
#define TRYKKSENSOR_PROM_C2		0xA4 // Coefficient 2
#define TRYKKSENSOR_PROM_C3		0xA6 // Coefficient 3
#define TRYKKSENSOR_PROM_C4		0xA8 // Coefficient 4
#define TRYKKSENSOR_PROM_C5		0xAA // Coefficient 5
#define TRYKKSENSOR_PROM_C6		0xAC // Coefficient 6
#define TRYKKSENSOR_PROM_CRC	0xAE // CRC

// SENSOR STRUCT
typedef struct {
	I2C_HandleTypeDef *i2cHandle;

	// Trykk i bar
	float trykk_f;
	int32_t trykk_comp;				// Temperature compensated pressure

	// Digital pressure and temperature d
	uint32_t D1;
	uint32_t D2;

	// Temperatur i degC
	float temp_f;
	int32_t TEMPt;
	int32_t dT;
	int64_t OFF;
	int64_t SENS;


	// Factory calibration data
	uint16_t SENS_T1;				// C1, Pressure sensitivity
	uint16_t OFF_T1;				// C2, Pressure offset
	uint16_t TCS;					// C3, Temperature coefficient of pressure sensitivity
	uint16_t TCO;					// C4, Temperature coefficient of pressure offset
	uint16_t TREF;					// C5, Reference temperature
	uint16_t TEMPSENS;				// C6, Temperature coefficient of the temperature

} TRYKKSENSOR;


// INITIALISATION
void TRYKKSENSOR_Initialiser(TRYKKSENSOR *dev, I2C_HandleTypeDef *i2cHandle);

// READ FACTORY CALIBRATION
trykk_status TRYKKSENSOR_LesKalibrering(TRYKKSENSOR *dev);

// DATA ACQUISITION
float TRYKKSENSOR_LesTrykk(TRYKKSENSOR *dev);

// LOW LEVEL FUNCTIONS
trykk_status TRYKKSENSOR_LesRegister(TRYKKSENSOR *dev, uint8_t reg, uint8_t *data);

trykk_status TRYKKSENSOR_LesRegistrene(TRYKKSENSOR *dev, uint8_t reg, uint8_t *data, uint8_t lengde);

trykk_status TRYKKSENSOR_SkrivRegister(TRYKKSENSOR *dev, uint8_t *data);

trykk_status TRYKK_sjekk(TRYKKSENSOR *dev, uint8_t *feil);

#endif /* INC_TRYKK_H_ */
