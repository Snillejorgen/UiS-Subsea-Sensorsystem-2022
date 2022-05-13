/*
 * trykk.c
 *
 *  Created on: Mar 23, 2022
 *      Author: Hanne Lovise Berger
 *
 * Her er alle funksjoner som utføres i registeret til trykksensoren, og som beregner trykket
 * og temperaturen til trykksensoren.
 */

// INCLUDES
#include "math.h"
#include "trykk.h"
#include "extern_variabler.h"

// INITIALISATION
void TRYKKSENSOR_Initialiser(TRYKKSENSOR *dev, I2C_HandleTypeDef *i2cHandle){

//	// Resett sensor ved oppstart


	// Sett struct parametre
	dev->i2cHandle	= i2cHandle;

	dev->temp_f 		= 0.0f;
	dev->TEMPt			= 0;
	dev->dT				= 0;
	dev->OFF			= 0;
	dev->SENS			= 0;
	dev->trykk_comp 	= 0;
	dev->D1				= 0;
	dev->D2				= 0;

	dev->SENS_T1		= 0;
	dev->OFF_T1			= 0;
	dev->TCS			= 0;
	dev->TCO			= 0;
	dev->TREF			= 0;
	dev->TEMPSENS		= 0;

}

// READ FACTORY CALIBRATION
trykk_status TRYKKSENSOR_LesKalibrering(TRYKKSENSOR *dev){

	trykk_status stat;
	uint8_t SENS_T1_buf[2];			// pressure sensitivity buffer
	uint8_t OFF_T1_buf[2];			// Pressure offset buffer
	uint8_t TCS_buf[2];				// Temperature coefficient of pressure sensitivity buffer
	uint8_t TCO_buf[2];				// Temperature coefficient of pressure offset buffer
	uint8_t TREF_buf[2];			// Temperatur referanse buffer
	uint8_t TEMPSENS_buf[2];		// Temperatur sensitivitet buffer
	uint8_t reset_buf[1];

	reset_buf[0] = TRYKKSENSOR_RESET;

	// Reset trykksensor

	stat = TRYKKSENSOR_SkrivRegister(dev, (uint8_t *)reset_buf);

	if(stat != TRYKK_SUKSESS){
		trykkfeil += 1;
	}
	HAL_Delay(10);

	// Les pressure sensitivity
	stat = TRYKKSENSOR_LesRegistrene( dev, TRYKKSENSOR_PROM_C1, (uint8_t *)SENS_T1_buf, 2 );

	if(stat != TRYKK_SUKSESS){
		trykkfeil += 1;
	}

	// Les pressure offset buffer
	stat = TRYKKSENSOR_LesRegistrene( dev, TRYKKSENSOR_PROM_C2, (uint8_t *)OFF_T1_buf, 2 );

	if(stat != TRYKK_SUKSESS){
		trykkfeil += 1;
	}

	// Les temperature coefficient of pressure sensitivity
	stat = TRYKKSENSOR_LesRegistrene( dev, TRYKKSENSOR_PROM_C3, (uint8_t *)TCS_buf, 2 );

	if(stat != TRYKK_SUKSESS){
		trykkfeil += 1;
	}

	// Les temperature coefficient of pressure offset
	stat = TRYKKSENSOR_LesRegistrene( dev, TRYKKSENSOR_PROM_C4, (uint8_t *)TCO_buf, 2 );

	if(stat != TRYKK_SUKSESS){
		trykkfeil += 1;
	}

	// Les temperatur referanse
	stat = TRYKKSENSOR_LesRegistrene( dev, TRYKKSENSOR_PROM_C5, (uint8_t *)TREF_buf, 2 );

	if(stat != TRYKK_SUKSESS){
		trykkfeil += 1;
	}

	// Les temperatur sensitivitet og legger denne ie TEMPSENSbuf
	stat = TRYKKSENSOR_LesRegistrene( dev, TRYKKSENSOR_PROM_C6, (uint8_t *)TEMPSENS_buf, 2 );

	if(stat != TRYKK_SUKSESS){
		trykkfeil += 1;
	}

	// Samle leste verdier til 16-bits variabler.
	dev->SENS_T1 = (SENS_T1_buf[0]<<8|SENS_T1_buf[1]);
	dev->OFF_T1 = (OFF_T1_buf[0]<<8|OFF_T1_buf[1]);
	dev->TCS = (TCS_buf[0]<<8|TCS_buf[1]);
	dev->TCO = (TCO_buf[0]<<8|TCO_buf[1]);
	dev->TREF = (TREF_buf[0]<<8|TREF_buf[1]);
	dev->TEMPSENS = (TEMPSENS_buf[0]<<8|TEMPSENS_buf[1]);

	if(trykkfeil > 0) return TRYKK_FEIL;

	return TRYKK_SUKSESS;
}

// DATA ACQUISITION
float TRYKKSENSOR_LesTrykk(TRYKKSENSOR *dev){

	trykk_status stat;

	uint8_t D2_buf[3];			// Digital temperatur value buffer
	uint8_t D1_buf[3];			// Digital pressure value buffer
	uint8_t trykkbuf[1];
	uint8_t tempbuf[1];
	float trykk_f;

	trykkbuf[0] = TRYKKSENSOR_D1_256;
	tempbuf[0] = TRYKKSENSOR_D2_256;


	float T2;
	float OFF2;
	float SENS2;


	dev->D2				= 0; // Digital Temperature value
	dev->D1 			= 0; // Digital Pressure value

	// Les Digital temperature value og legg det i D2buf

	stat = TRYKKSENSOR_SkrivRegister(dev, (uint8_t *)tempbuf);

	if(stat != TRYKK_SUKSESS){
		trykkfeil += 1;
	}
	HAL_Delay(10);


	stat = TRYKKSENSOR_LesRegistrene( dev, TRYKKSENSOR_ADC_READ, (uint8_t *)D2_buf, 3 );

	if(stat != TRYKK_SUKSESS){
		trykkfeil += 1;
	}

	// Les Digital pressure value og legg det i D1buf
	stat = TRYKKSENSOR_SkrivRegister(dev, (uint8_t *)trykkbuf);

	if(stat != TRYKK_SUKSESS){
		trykkfeil += 1;
	}
	HAL_Delay(10);

	stat = TRYKKSENSOR_LesRegistrene( dev, TRYKKSENSOR_ADC_READ, (uint8_t *)D1_buf, 3 );

	if(stat != TRYKK_SUKSESS){
		trykkfeil += 1;
	}

	// Samle leste verdier til 32-bits variabler.
	dev->D2 = ((D2_buf[0]<<16)|(D2_buf[1]<<8)|D2_buf[2]);
	dev->D1 = (D1_buf[0]<<16|D1_buf[1]<<8|D1_buf[2]);

	// Regn ut temperaturen
	dev->dT = dev->D2 - (dev->TREF *pow(2,8));				// Formel fra datablad.
	dev->TEMPt = 2000 + (dev->dT*dev->TEMPSENS/pow(2,23));	// Formel fra datablad.
	dev->temp_f = dev->TEMPt/100.0;							//

	// Regn ut temperaturkompensert trykk
	dev->OFF = (dev->OFF_T1*pow(2,18)) + ((dev->TCO*dev->dT)/pow(2,5));		// Formel fra datablad
	dev->SENS = (dev->SENS_T1*pow(2,17)) + (dev->TCS*dev->dT)/pow(2,7); 	// Formel fra datablad.



	// SECOND ORDER TEMPERATURE COMPENSATION FLOWCHART
	if(dev->TEMPt<2000){
		T2 = (float)(3*pow(dev->dT,2) / pow(2,33));
		OFF2 = (float)(3* pow((dev->TEMPt-2000),2) / pow(2,3));
		SENS2 = (float)(7*pow((dev->TEMPt-2000),2) / pow(2,3));

		if(dev->TEMPt<(-1500)){
			SENS2 = (float)(SENS2 + 3*pow((dev->TEMPt+1500),2));
		}

	}
	else {
		T2 = 0;
		OFF2 = 0;
		SENS2 = 0;
	}

	dev->TEMPt = dev->TEMPt - T2;
	dev->OFF = dev->OFF-OFF2;
	dev->SENS = dev->SENS-SENS2;

	dev->trykk_comp = (int32_t)(((dev->D1*dev->SENS/pow(2,21)) - dev->OFF)/pow(2,15));	// Formel fra datablad.
	trykk_f = (float)(dev->trykk_comp/100.0);

	if(trykkfeil > 0) return TRYKK_FEIL;

	return trykk_f;
}

// Sjekk kommunikasjon
trykk_status TRYKK_sjekk(TRYKKSENSOR *dev, uint8_t *feil){

	uint8_t stat;
	stat = HAL_I2C_IsDeviceReady(dev->i2cHandle, TRYKKSENSOR_DEVICE_READ, 10, 1000);
	if(HAL_OK != stat){
		*feil = 1;
		return TRYKK_FEIL;}
	*feil = 0;
	return TRYKK_SUKSESS;
}


// LOW LEVEL FUNCTIONS
trykk_status TRYKKSENSOR_LesRegister(TRYKKSENSOR *dev, uint8_t reg, uint8_t *data){
	HAL_StatusTypeDef status;

	status = HAL_I2C_Mem_Read( dev->i2cHandle, TRYKKSENSOR_DEVICE_READ, reg, I2C_MEMADD_SIZE_8BIT, data, 1, HAL_MAX_DELAY);

	if(status != HAL_OK) return TRYKK_FEIL;

	return TRYKK_SUKSESS;
}

trykk_status TRYKKSENSOR_LesRegistrene(TRYKKSENSOR *dev, uint8_t reg, uint8_t *data, uint8_t lengde){
	HAL_StatusTypeDef status;

	status = HAL_I2C_Mem_Read( dev->i2cHandle, TRYKKSENSOR_DEVICE_READ, reg, I2C_MEMADD_SIZE_8BIT, data, lengde, HAL_MAX_DELAY);

	if(status != HAL_OK) return TRYKK_FEIL;

	return TRYKK_SUKSESS;
}

trykk_status TRYKKSENSOR_SkrivRegister(TRYKKSENSOR *dev, uint8_t *data){
	HAL_StatusTypeDef status;

	status = HAL_I2C_Master_Transmit(dev->i2cHandle, TRYKKSENSOR_DEVICE_WRITE, data, 1, HAL_MAX_DELAY);
//	status =  HAL_I2C_Mem_Write(dev->i2cHandle, TRYKKSENSOR_DEVICE_WRITE, reg, I2C_MEMADD_SIZE_8BIT, data, 1, HAL_MAX_DELAY);

	if(status != HAL_OK) return TRYKK_FEIL;

	return TRYKK_SUKSESS;
}
