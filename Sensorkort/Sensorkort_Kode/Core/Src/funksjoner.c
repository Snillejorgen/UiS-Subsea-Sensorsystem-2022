/*
 * funksjoner.c
 *
 *  Created on: 14. mar. 2022
 *      Author: Jurgi
 */

//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
// Her legges funksjoner til for å holde mer orden på main.c
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
#include "stm32f4xx_hal.h"
#include "icm20948.h"
#include "extern_variabler.h"
#include "string.h"
#include <stdio.h>
#include "math.h"
#include "spi.h"
#include "i2c.h"
#include "stts75.h"
#include "can.h"
#include "trykk.h"

// funksjonsprototyper
//------------------------------------------------------------------------------------------
void init_usbbuf(void);
void les_og_beregne_vinkler(kommskift kommunikasjonsmetode);
void les_og_behandle_temp(kommskift kommunikasjonsmetode);
void lag_datapakke_usb(void);
void lag_datapakker_CAN(void);
void oppstartCAN(uint8_t filterGruppe, CAN_HandleTypeDef *canPort);
void sendDataCAN(uint16_t id, CAN_HandleTypeDef *canPort);
void senddatapakkerCAN(void);
void bryteravprelling(void);
void sjekklekkasje(void);

//------------------------------------------------------------------------------------------

// Bryterfunksjoner
//------------------------------------------------------------------------------------------

void bryteravprelling(void){
	if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_8) == 0){	// Sjekker om bryter er trykt inn
		brytertrykk = 1;
	}
	else {
		brytertrykk = 0;
	}

	if(brytertrykk){								// Om bryter er trykt inn,
		slippteller = 0;							// resetter slipptelleren.
		if(klarnytttrykk){							// Om det er klart for nytt trykk,
			trykkteller++;							// inkremententer trykktelleren
			if(trykkteller > 2){					// Om trykktelleren er større enn 2,
				gyldigtrykk = 1;					// så har du gyldig trykk,
				trykkteller = 0;					// trykktelleren resettes,
				klarnytttrykk = 0;					// og det er ikke klart for nytt trykk
			}
		}
	}
	else{											// Om bryteren ikke er trykt in
		trykkteller = 0;							// resettes trykktelleren,
		slippteller++;								// og slipptelleren inkrementeres.

		if(slippteller > 2){						// Om slipptelleren er større enn 2,
			slippteller = 0;						// resettes slipptelleren,
			klarnytttrykk = 1;						// og det er klart for nytt trykk.
		}
	}

}

//------------------------------------------------------------------------------------------

// Lekkasjesjekk
// ------------------------------------------------------------------------------------

void sjekklekkasje(void){
	  if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0)){		// Om pinne PA0 (LEKK1) får høyt signal,
		  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, 1);	// gi ut høyt signal på pinne PC9 (aktiver lysdiode D5).
		  status &= ~LEKK1;
	  }
	  else{											// Ellers,
		  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, 0);	// gi ut lavt signal på pinne PC9 (deaktiver lysdiode D5)
		  status |= LEKK1;
	  }

	  if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1)){		// Om pinne PA1 (LEKK2) får høyt signal,
		  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, 1);	// gi ut høyt signal på pinne PC0 (aktiver lysdiode D4).
		  status &= ~LEKK2;
	  }
	  else{											// Ellers,
		  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, 0);	// gi ut lavt signal på pinne PC0 (deaktiver lysdiode D4)
		  status |= LEKK2;
	  }

	  if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2)){		// Om pinne PA2 (LEKK3) får høyt signal,
		  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, 1);	// gi ut høyt signal på pinne PA3 (aktiver lysdiode D3).
		  status &= ~LEKK3;
	  }
	  else{											// Ellers,
		  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, 0);	// gi ut lavt signal på pinne PA3 (deaktiver lysdiode D3)
		  status |= LEKK3;
	  }
}

// ------------------------------------------------------------------------------------




// Funksjoner for CAN-protokoll
// ------------------------------------------------------------------------------------
void oppstartCAN(uint8_t filterGruppe, CAN_HandleTypeDef *canPort) {
    canfilter.FilterBank = 0;
    canfilter.FilterMode = CAN_FILTERMODE_IDMASK;
    canfilter.FilterFIFOAssignment = CAN_RX_FIFO0;
    canfilter.FilterIdHigh = filterGruppe << 10 | 0x1F;
    canfilter.FilterIdLow = 0xFFF8;
    canfilter.FilterMaskIdHigh = 0x3F << 10 | 0x1F;
    canfilter.FilterMaskIdLow = 0xFFF8;
    canfilter.FilterScale = CAN_FILTERSCALE_32BIT;
    canfilter.FilterActivation = ENABLE;
    canfilter.SlaveStartFilterBank = 14;

    txHeader.DLC = 8; // Number of bites to be transmitted max- 8
    txHeader.IDE = CAN_ID_STD;
    txHeader.RTR = CAN_RTR_DATA;
    txHeader.StdId = 0x60;
    txHeader.ExtId = 0x00;
    txHeader.TransmitGlobalTime = DISABLE;

    HAL_CAN_ConfigFilter(canPort, &canfilter);
    HAL_CAN_Start(canPort);
    HAL_CAN_ActivateNotification(canPort, CAN_IT_RX_FIFO0_MSG_PENDING);
}

void sendDataCAN(uint16_t id, CAN_HandleTypeDef *canPort) {
    txHeader.StdId = id;
    HAL_CAN_AddTxMessage(canPort, &txHeader, csend, &canMailbox);
}
// ------------------------------------------------------------------------------------

void init_usbbuf(void){
	//Initialiser imu-databuffer
	imu_usb[0] = temp_usb[4] = 't';
	imu_usb[5] = imu_usb[10] = imu_usb[15] = imu_usb[20] = imu_usb[25] = 'g';
	imu_usb[30] = imu_usb[35] = imu_usb[40] = imu_usb[45] = imu_usb[50] = 'a';
	imu_usb[55] = imu_usb[60] = 'k';
	imu_usb[6] = imu_usb[31] = 'x';
	imu_usb[11] = imu_usb[36] = 'y';
	imu_usb[16] = imu_usb[41] = 'z';
	imu_usb[21] = imu_usb[46] = imu_usb[56] =  'r';
	imu_usb[26] = imu_usb[51] = imu_usb[61] = 's';
	temp_usb[0] = 'c';

}

void les_og_behandle_temp(kommskift kommunikasjonsmetode){

	// Funksjon for etablering av kontakt med sensor.
	// feil-variabel blir satt til 1 om kontakt ikke er etablert.
	TEMP_sjekk(&hi2c1, LES_AV_TEMP_HOVED, &feil_h);
	TEMP_sjekk(&hi2c1, LES_AV_TEMP_KRAFT, &feil_k);
	TEMP_sjekk(&hi2c1, LES_AV_TEMP_SENS, &feil_s);

	if(feil_h != 0){
		status |= MISTETHOVED;
	}
	else{
		status &= ~MISTETHOVED;
	}

	if(feil_k != 0){
		status |= MISTETKRAFT;
	}
	else{
		status &= ~MISTETKRAFT;
	}

	if(feil_s != 0){
		status |= MISTETSENS;
	}
	else{
		status &= ~MISTETSENS;
	}


	// Funksjon for å lese av temperatur i grader celsius
	TEMP_les(&hi2c1, LES_AV_TEMP_HOVED, &temp_f_hoved);
	TEMP_les(&hi2c1, LES_AV_TEMP_KRAFT, &temp_f_kraft);
	TEMP_les(&hi2c1, LES_AV_TEMP_SENS, &temp_f_sens);

	// Lagre data som intverdier for CAN-overføring

	c_thoved = (uint8_t)(ceilf(temp_f_hoved));
	c_tkraft = (uint8_t)(ceilf(temp_f_kraft));
	c_tsens = (uint8_t)(ceilf(temp_f_sens));

	if(kommunikasjonsmetode == USB){

		// Om data er negativ,
		if(temp_f_sens < 0){
			// settes polaritetsflagg høyt,
			tempsneg = 1;
			// og floatverdi rundes opp, fortegnet snus, og lagres som uint8
			tempsens = -(int)(ceilf(temp_f_sens));
		}
		// Om data er positiv,
		else{
			// settes polaritetsflagg lavt,
			tempsneg = 0;
			// og floatverdi rundes opp og lagres som uint8
			tempsens = (int)(ceilf(temp_f_sens));
		}

		// temperatur med datatype uint8 kopieres så inn i vektor for sending
		memcpy(&temp_usb[1], &tempsneg, 1);
		memcpy(&temp_usb[2], &tempsens, 1);
	}
}

int16_t dybdeutregning(float P0, float P, vanntetthet vanntype){
	int16_t dybde;
	float rho;
	// Bestemmer vanntetthet etter hvilken vanntype som er bestemt
	if(vanntype == FERSKVANN){
		rho = 997.0474;
	}
	else if(vanntype == SALTVANN){
		rho = 1023.6;
	}

	// Bruker differansen mellom faktisk dybde i millibar og initialtrykket for å finne dybden ROV-en befinner seg i i centimeter
	dybde = (int16_t)(((P-P0)*10000.0)/(rho*9.81));
	return dybde;
}

void les_og_beregne_vinkler(kommskift kommunikasjonsmetode){
	  float accelx, accely, accelz, gyrox, gyroy, gyroz;

	  // leser av data fra gyroskop og akselerometer med skalert funksjon
	  icm20948_gyro_read_dps(&my_gyro);
	  icm20948_accel_read_g(&my_accel);


	  // Akser blir snudd for å samsvare med ROV-en
	  accelx = my_accel.y;
	  accely = -my_accel.x;
	  accelz = my_accel.z;
	  gyrox = my_gyro.y;
	  gyroy = my_gyro.x;
	  gyroz = my_gyro.z;


	  // Lagrer tid som ny tid og lager tidsskritt
	  td1 = HAL_GetTick();
	  td = (td1-td2)/1000;

	  // Regner ut vinkel vhja. akselerometer

	  aks_s_f = (atan2((float)(accelx), (float)(sqrt((pow(accely,2))+(pow(accelz,2)))))* 180 / pi);
	  aks_r_f = (atan2((float)(accely), (float)(accelz))* 180 / pi);


	  // Regner ut vinkel vhja. gyroskop

	  gyro_r_f += (float)(gyrox) * td;
	  gyro_s_f += (float)(gyroy) * td;
	  komp_r_f += (float)(gyrox) * td;
	  komp_s_f += (float)(gyroy) * td;

	  // Bruker verdier fra akselerometer og gyroskop og lager komplimentærfilter
	  komp_r_f = komp_r_f*0.98 + aks_r_f*0.02;
	  komp_s_f = komp_s_f*0.98 + aks_s_f*0.02;

	  // Lagrer tid som forrige tid
	  td2 = HAL_GetTick();


	  // Lagre data som intverdier for CAN-overføring
	  c_aks_x = (int16_t)(accelx * 981);
	  c_aks_y = (int16_t)(accely * 981);
	  c_aks_z = (int16_t)(accelz * 981);

	  c_rull = (int16_t)(komp_r_f*10);
	  c_stamp = (int16_t)(komp_s_f*10);


	  if(kommunikasjonsmetode == USB){

		  // Er dataen negativ?

		  if(gyrox < 0){
			  gxneg = 1;
			  gyro_x = -(int)(gyrox * 10);
		  }
		  else{
			  gxneg = 0;
			  gyro_x = (int)(gyrox * 10);
		  }
		  if(gyroy < 0){
			  gyneg = 1;
			  gyro_y = -(int)(gyroy * 10);
		  }
		  else{
			  gyneg = 0;
			  gyro_y = (int)(gyroy * 10);
		  }

		  if(gyroz < 0){
			  gzneg = 1;
			  gyro_z = -(int)(gyroz * 10);
		  }
		  else{
			  gzneg = 0;
			  gyro_z = (int)(gyroz * 10);
		  }

		  if(accelx < 0){
			  axneg = 1;
			  aks_x = -(int)(accelx * 981);
		  }
		  else{
			  axneg = 0;
			  aks_x = (int)(accelx * 981);
		  }
		  if(accely < 0){
			  ayneg = 1;
			  aks_y = -(int)(accely * 981);
		  }
		  else{
			  ayneg = 0;
			  aks_y = (int)(accely * 981);
		  }

		  if(accelz < 0){
			  azneg = 1;
			  aks_z = -(int)(accelz * 981);
		  }
		  else{
			  azneg = 0;
			  aks_z = (int)(accelz * 981);
		  }

		  if(gyro_r_f < 0){
			  grneg = 1;
			  gyro_r = -(int)(gyro_r_f*10);
		  }
		  else{
			  grneg = 0;
			  gyro_r = (int)(gyro_r_f*10);
		  }
		  if(gyro_s_f < 0){
			  gsneg = 1;
			  gyro_s = -(int)(gyro_s_f*10);
		  }
		  else{
			  gsneg = 0;
			  gyro_s = (int)(gyro_s_f*10);
		  }

		  if((int)(aks_s_f*10) < 0){
			  asneg = 1;
			  aks_s = -(int)(aks_s_f*10);
		  }
		  else{
			  asneg = 0;
			  aks_s = (int)(aks_s_f*10);
		  }
		  if((int)(aks_r_f*10) < 0){
			  arneg = 1;
			  aks_r = -(int)(aks_r_f*10);
		  }
		  else{
			  arneg = 0;
			  aks_r = (int)(aks_r_f*10);
		  }
		  if (komp_s_f < 0){
			  ksneg = 1;
			  komp_s = -(int)(komp_s_f*10);
		  }
		  else{
			  ksneg = 0;
			  komp_s = (int)(komp_s_f*10);
		  }
		  if (komp_r_f < 0){
			  krneg = 1;
			  komp_r = -(int)(komp_r_f*10);
		  }
		  else{
			  krneg = 0;
			  komp_r = (int)(komp_r_f*10);
		  }


		  // Deler opp 16-bits data til 8-bits data for sending over USB

		  lsb_gx = ((uint16_t)gyro_x >> 0) & 0xFF;
		  msb_gx = ((uint16_t)gyro_x >> 8) & 0xFF;

		  lsb_gy = ((uint16_t)gyro_y >> 0) & 0xFF;
		  msb_gy = ((uint16_t)gyro_y >> 8) & 0xFF;

		  lsb_gz = ((uint16_t)gyro_z >> 0) & 0xFF;
		  msb_gz = ((uint16_t)gyro_z >> 8) & 0xFF;

		  lsb_gr = ((uint16_t)gyro_r >> 0) & 0xFF;
		  msb_gr = ((uint16_t)gyro_r >> 8) & 0xFF;

		  lsb_gs = ((uint16_t)gyro_s >> 0) & 0xFF;
		  msb_gs = ((uint16_t)gyro_s >> 8) & 0xFF;

		  lsb_ax = ((uint16_t)aks_x >> 0) & 0xFF;
		  msb_ax = ((uint16_t)aks_x >> 8) & 0xFF;

		  lsb_ay = ((uint16_t)aks_y >> 0) & 0xFF;
		  msb_ay = ((uint16_t)aks_y >> 8) & 0xFF;

		  lsb_az = ((uint16_t)aks_z >> 0) & 0xFF;
		  msb_az = ((uint16_t)aks_z >> 8) & 0xFF;

		  lsb_ar = ((uint16_t)aks_r >> 0) & 0xFF;
		  msb_ar = ((uint16_t)aks_r >> 8) & 0xFF;

		  lsb_as = ((uint16_t)aks_s >> 0) & 0xFF;
		  msb_as = ((uint16_t)aks_s >> 8) & 0xFF;

		  lsb_kr = ((uint16_t)komp_r >> 0) & 0xFF;
		  msb_kr = ((uint16_t)komp_r >> 8) & 0xFF;

		  lsb_ks = ((uint16_t)komp_s >> 0) & 0xFF;
		  msb_ks = ((uint16_t)komp_s >> 8) & 0xFF;
	  }


}

void lag_datapakker_CAN(void){

	// Lager datapakke til kraftkort
	memcpy(kraftpakke, &status, 1);
	memcpy(&kraftpakke[1], &c_thoved, 1);
	memcpy(&kraftpakke[2], &c_tkraft, 1);
	memcpy(&kraftpakke[3], &c_tsens, 1);

	// Lager datapakke til regulering
	memcpy(akspakke, &c_aks_x, 2);
	memcpy(&akspakke[2], &c_aks_y, 2);
	memcpy(&akspakke[4], &c_aks_z, 2);

	memcpy(regpakke, &c_hiv, 2);
	memcpy(&regpakke[2], &c_rull, 2);
	memcpy(&regpakke[4], &c_stamp, 2);

}


void lag_datapakke_usb(void){

	  // Kopierer innhold fra IMU-data inn i datapakke

	  memcpy(&imu_usb[7], &gxneg, 1);
	  memcpy(&imu_usb[8], &msb_gx, 1);
	  memcpy(&imu_usb[9], &lsb_gx, 1);

	  memcpy(&imu_usb[12], &gyneg, 1);
	  memcpy(&imu_usb[13], &msb_gy, 1);
	  memcpy(&imu_usb[14], &lsb_gy, 1);

	  memcpy(&imu_usb[17], &gzneg, 1);
	  memcpy(&imu_usb[18], &msb_gz, 1);
	  memcpy(&imu_usb[19], &lsb_gz, 1);

	  memcpy(&imu_usb[22], &grneg, 1);
	  memcpy(&imu_usb[23], &msb_gr, 1);
	  memcpy(&imu_usb[24], &lsb_gr, 1);

	  memcpy(&imu_usb[27], &gsneg, 1);
	  memcpy(&imu_usb[28], &msb_gs, 1);
	  memcpy(&imu_usb[29], &lsb_gs, 1);

	  memcpy(&imu_usb[32], &axneg, 1);
	  memcpy(&imu_usb[33], &msb_ax, 1);
	  memcpy(&imu_usb[34], &lsb_ax, 1);

	  memcpy(&imu_usb[37], &ayneg, 1);
	  memcpy(&imu_usb[38], &msb_ay, 1);
	  memcpy(&imu_usb[39], &lsb_ay, 1);

	  memcpy(&imu_usb[42], &azneg, 1);
	  memcpy(&imu_usb[43], &msb_az, 1);
	  memcpy(&imu_usb[44], &lsb_az, 1);

	  memcpy(&imu_usb[47], &arneg, 1);
	  memcpy(&imu_usb[48], &msb_ar, 1);
	  memcpy(&imu_usb[49], &lsb_ar, 1);

	  memcpy(&imu_usb[52], &asneg, 1);
	  memcpy(&imu_usb[53], &msb_as, 1);
	  memcpy(&imu_usb[54], &lsb_as, 1);

	  memcpy(&imu_usb[57], &krneg, 1);
	  memcpy(&imu_usb[58], &msb_kr, 1);
	  memcpy(&imu_usb[59], &lsb_kr, 1);

	  memcpy(&imu_usb[62], &ksneg, 1);
	  memcpy(&imu_usb[63], &msb_ks, 1);
	  memcpy(&imu_usb[64], &lsb_ks, 1);

}

void senddatapakkerCAN(void){

	  // Temperatur og lekk til:
	  memcpy(csend, &kraftpakke, 8);

	  // strømgruppe og topside
	  sendDataCAN(140, &hcan1);

	  // hiv, rull, stamp til:
	  memcpy(csend, &regpakke, 8);

	  // styring og topside
	  sendDataCAN(80, &hcan1);

	  // akselerasjon til:
	  memcpy(csend, &akspakke, 8);

	  // styring og topside
	  sendDataCAN(81, &hcan1);
}
