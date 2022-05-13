/*
 * extern_variabler.h
 *
 *  Created on: 14. mar. 2022
 *      Author: Jurgi
 */

#ifndef INC_VARIABLER_H_
#define INC_VARIABLER_H_

#include <stdbool.h>
#include "icm20948.h"
#include "trykk.h"

// Registre status

#define LEKK1				0b00000001
#define LEKK2				0b00000010
#define LEKK3  				0b00000100
#define MISTETSENS  		0b00001000
#define MISTETKRAFT			0b00010000
#define MISTETHOVED			0b00100000

// Navngivning for å velge mellom USB og CAN-dataoverføring

typedef enum{
	CAN 	= 0x00,
	USB 	= 0x01
}kommskift;

#endif /* INC_VARIABLER_H_ */

// Variabler for brytertrykk
extern uint8_t brytertrykk;
extern uint8_t slippteller;
extern uint8_t trykkteller;
extern uint8_t klarnytttrykk;
extern uint8_t gyldigtrykk;
extern uint8_t trykktick;

// Variabler for CAN-buss

extern CAN_RxHeaderTypeDef rxHeader; //CAN Bus Transmit Header
extern CAN_TxHeaderTypeDef txHeader; //CAN Bus Receive Header
extern uint8_t canRX[8];  //CAN Bus Receive Buffer
extern uint8_t csend[8]; // Tx Buffer
extern CAN_FilterTypeDef canfilter;
extern uint32_t canMailbox; //CAN Bus Mail box variable

// Variabler for lagring og behandling av IMU-data
//-----------------------------------------------------------

// Lagring av sensordata
extern axises my_gyro;
extern axises my_accel;

extern uint16_t gyro_x;
extern uint16_t gyro_y;
extern uint16_t gyro_z;
extern uint16_t gyro_r;		// Stamp gitt av gyroskop
extern uint16_t gyro_s;		// Stamp gitt av gyroskop
extern uint16_t aks_x;
extern uint16_t aks_y;
extern uint16_t aks_z;
extern uint16_t aks_r;		// Rull gitt av akselerometer
extern uint16_t aks_s;		// Stamp gitt av akselerometer
extern uint16_t komp_r;		// Rull gitt av komplimentærfilter
extern uint16_t komp_s;		// Stamp gitt av komplimentærfilter
extern uint16_t hiv;		// Hiv gitt av trykksensor

extern uint8_t temphoved;	// Variabel for temperatursensor på hovedkort
extern uint8_t tempkraft;	// Variabel for temperatursensor på kraftkort
extern uint8_t tempsens;	// Variabel for temperatursensor på sensorkort

extern float temp_f_hoved;	// Rådata fra temperatursensor på hovedkort i floatverdi
extern float temp_f_kraft;	// Rådata fra temperatursensor på kraftkort i floatverdi
extern float temp_f_sens;	// Rådata fra temperatursensor på sensorkort i floatverdi
extern uint8_t feil_h;		// Flagg for mislykket kontakt med temperatursensor på hovedkort
extern uint8_t feil_k;		// Flagg for mislykket kontakt med temperatursensor på kraftkort
extern uint8_t feil_s;		// Flagg for mislykket kontakt med temperatursensor på sensorkort
extern uint8_t tempsneg;	// Flagg for negativ verdi på temperatursensor (USB)

// int variabler for sending over CAN-buss

extern int16_t c_aks_x;
extern int16_t c_aks_y;
extern int16_t c_aks_z;
extern int16_t c_rull;
extern int16_t c_stamp;
extern int16_t c_hiv;

extern uint8_t c_thoved;
extern uint8_t c_tkraft;
extern uint8_t c_tsens;

extern uint8_t kalibrer;
extern uint8_t startdata;

// Tidskonstant (10ms)
extern float td;
extern float td1;
extern float td2;

// Variabler for bruk i mellomregning
extern float gyro_r_f;
extern float gyro_s_f;
extern float aks_r_f;
extern float aks_s_f;
extern float komp_r_f;
extern float komp_s_f;

// Variabler for å dele data opp i 8bit for usb sending

extern uint8_t msb_gx;
extern uint8_t lsb_gx;

extern uint8_t msb_gy;
extern uint8_t lsb_gy;

extern uint8_t msb_gz;
extern uint8_t lsb_gz;

extern uint8_t msb_gr;
extern uint8_t lsb_gr;

extern uint8_t msb_gs;
extern uint8_t lsb_gs;

extern uint8_t msb_ax;
extern uint8_t lsb_ax;

extern uint8_t msb_ay;
extern uint8_t lsb_ay;

extern uint8_t msb_az;
extern uint8_t lsb_az;

extern uint8_t msb_ar;
extern uint8_t lsb_ar;

extern uint8_t msb_as;
extern uint8_t lsb_as;

extern uint8_t msb_kr;
extern uint8_t lsb_kr;

extern uint8_t msb_ks;
extern uint8_t lsb_ks;

extern uint8_t msb_hiv;
extern uint8_t lsb_hiv;

extern uint8_t msb_tid;
extern uint8_t lsb_tid;

extern uint8_t msb_sek;
extern uint8_t lsb_sek;

// Flagg for negativ verdi	(0 = positivt tall, 1 = negativt tall)

extern uint8_t gxneg;
extern uint8_t gyneg;
extern uint8_t gzneg;
extern uint8_t grneg;
extern uint8_t gsneg;
extern uint8_t axneg;
extern uint8_t ayneg;
extern uint8_t azneg;
extern uint8_t arneg;
extern uint8_t asneg;
extern uint8_t krneg;
extern uint8_t ksneg;

extern uint8_t neg6dof;
extern uint8_t status;

// Definerer π
extern float pi;

// Strengbuffer for data over USB
extern uint8_t imu_usb[65];
extern uint8_t temp_usb[6];

// Annet
//-----------------------------------------------------------

// Initialiserer tilfeldig nummer
extern uint8_t num;

// Hjelpevariabel for å indeksere imu_str
extern uint8_t i;

// Teller
extern uint8_t count;

// Tidsvariabel
extern uint16_t tid;
extern uint16_t sek;
extern uint32_t t1;
extern uint32_t t2;
extern char tid_str[10];
extern uint8_t forstemaaling;

// Vektor for datapakker
uint8_t kraftpakke[8];
uint8_t regpakke[8];
uint8_t akspakke[8];

// Sett nytt trykk
extern float P;
extern float P0;

// Trykk
extern uint8_t trykkfeil;

