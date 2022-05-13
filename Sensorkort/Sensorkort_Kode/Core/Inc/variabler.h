/*
 * variabler.h
 *
 *  Created on: 14. mar. 2022
 *      Author: Jurgi
 */

//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
// Her legges variabler til for å holde mer orden på main.c
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------

#include "stts75.h"
#include "trykk.h"



// Variabler for CAN-buss

CAN_RxHeaderTypeDef rxHeader; //CAN Bus Transmit Header
CAN_TxHeaderTypeDef txHeader; //CAN Bus Receive Header
uint8_t canRX[8] = {0,0,0,0,0,0,0,0};  //CAN Bus Receive Buffer
uint8_t csend[8] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08}; // Tx Buffer
CAN_FilterTypeDef canfilter;
uint32_t canMailbox; //CAN Bus Mail box variable

// Variabler for brytertrykk
uint8_t brytertrykk = 0;
uint8_t slippteller = 0;
uint8_t trykkteller = 0;
uint8_t klarnytttrykk = 0;
uint8_t gyldigtrykk = 0;
uint8_t trykk = 0;
uint8_t trykktick = 0;


// Variabel for mottatt "ping!"
uint8_t ping = 0;

// Variabel for non-blocking delayfunksjon
uint32_t tickstart;

// Lagring av sensordata
axises my_gyro;
axises my_accel;
axises my_mag;

uint16_t gyro_x = 0;
uint16_t gyro_y = 0;
uint16_t gyro_z = 0;
uint16_t gyro_r = 0;	// Stamp gitt av gyroskop
uint16_t gyro_s = 0;	// Stamp gitt av gyroskop
uint16_t aks_x = 0;
uint16_t aks_y = 0;
uint16_t aks_z = 0;
uint16_t aks_r = 0;		// Rull gitt av akselerometer
uint16_t aks_s = 0;		// Stamp gitt av akselerometer
uint16_t komp_r = 0;	// Rull gitt av komplimentærfilter
uint16_t komp_s = 0;	// Stamp gitt av komplimentærfilter
uint16_t hiv = 545;		// Hiv gitt av trykksensor (STEDFORTREDER)

uint8_t temphoved = 25;	// Variabel for temperatursensor på hovedkort (STEDFORTREDER)
uint8_t tempkraft = 25;	// Variabel for temperatursensor på kraftkort (STEDFORTREDER)
uint8_t tempsens = 0;	// Variabel for temperatursensor på sensorkort

float temp_f_hoved = 0;	// Rådata fra temperatursensor på hovedkort i floatverdi
float temp_f_kraft = 0;	// Rådata fra temperatursensor på kraftkort i floatverdi
float temp_f_sens = 0;	// Rådata fra temperatursensor på sensorkort i floatverdi
uint8_t feil_h = 0;		// Flagg for mislykket kontakt med temperatursensor på hovedkort
uint8_t feil_k = 0;		// Flagg for mislykket kontakt med temperatursensor på kraftkort
uint8_t feil_s = 0;		// Flagg for mislykket kontakt med temperatursensor på sensorkort
uint8_t tempsneg = 0;	// Flagg for negativ verdi på temperatursensor (USB)

// int variabler for sending over CAN-buss

int16_t c_aks_x = 0;
int16_t c_aks_y = 0;
int16_t c_aks_z = 0;
int16_t c_rull = 0;
int16_t c_stamp = 0;
int16_t c_hiv = 0;

uint8_t c_thoved = 0;
uint8_t c_tkraft = 0;
uint8_t c_tsens = 0;

uint8_t startdata = 0;
uint8_t kalibrer = 0;


// Variabler for bruk i mellomregning
float gyro_r_f = 0;
float gyro_s_f = 0;
float aks_r_f = 0;
float aks_s_f = 0;
float komp_r_f = 0;
float komp_s_f = 0;

// Tidskonstant (10ms)
float td = 0;
float td1 = 0;
float td2 = 0;

// Variabler for å dele data opp i 8bit for usb sending

uint8_t msb_gx = 0; uint8_t lsb_gx = 0;

uint8_t msb_gy = 0; uint8_t lsb_gy = 0;

uint8_t msb_gz = 0; uint8_t lsb_gz = 0;

uint8_t msb_gr = 0; uint8_t lsb_gr = 0;

uint8_t msb_gs = 0; uint8_t lsb_gs = 0;

uint8_t msb_ax = 0; uint8_t lsb_ax = 0;

uint8_t msb_ay = 0; uint8_t lsb_ay = 0;

uint8_t msb_az = 0; uint8_t lsb_az = 0;

uint8_t msb_ar = 0; uint8_t lsb_ar = 0;

uint8_t msb_as = 0; uint8_t lsb_as = 0;

uint8_t msb_kr = 0; uint8_t lsb_kr = 0;

uint8_t msb_ks = 0; uint8_t lsb_ks = 0;

uint8_t msb_hiv = 0; uint8_t lsb_hiv = 0;

uint8_t msb_tid = 0; uint8_t lsb_tid = 0;

uint8_t msb_sek = 0; uint8_t lsb_sek = 0;

// Flagg for negativ verdi	(0 = positivt tall, 1 = negativt tall)

uint8_t gxneg = 0;
uint8_t gyneg = 0;
uint8_t gzneg = 0;
uint8_t grneg = 0;
uint8_t gsneg = 0;
uint8_t axneg = 0;
uint8_t ayneg = 0;
uint8_t azneg = 0;
uint8_t arneg = 0;
uint8_t asneg = 0;
uint8_t krneg = 0;
uint8_t ksneg = 0;

uint8_t neg6dof = 0b00000000;
uint8_t status = 0b00000000;

// Initialiserer tilfeldig nummer
uint8_t num = 0;

// Hjelpevariabel for å indeksere imu_str
uint8_t i = 0;

// Definerer π
float pi = 3.14159265359;

// Strengbuffer for data over USB
uint8_t imu_usb[65];
uint8_t temp_usb[6];


// Teller
uint8_t count = 0;

// Tidsvariabel
uint16_t tid = 0;
uint16_t sek = 0;
uint32_t t1 = 0;
uint32_t t2 = 0;
char tid_str[10];
uint8_t forstemaaling = 0;


// Hjelpevariabel for initialisering av imu

uint8_t imuinit = 0;

// Vektor for datapakker
uint8_t kraftpakke[8] = {0, 0, 0, 0, 0, 0, 0, 0};
uint8_t regpakke[8] = {0, 0, 0, 0, 0, 0, 0, 0};
uint8_t akspakke[8] = {0, 0, 0, 0, 0, 0, 0, 0};

// Trykk
uint8_t trykkfeil = 0;
TRYKKSENSOR trykksensor;
float P = 0.0;
float P0 = 0.0;

// Kommunikasjonstest
uint8_t ktempfeil;
uint8_t ktrykkfeil = 0;
uint8_t kimufeil = 0;
uint8_t canmottatt = 0;
uint8_t acknowledge = 0x6;
