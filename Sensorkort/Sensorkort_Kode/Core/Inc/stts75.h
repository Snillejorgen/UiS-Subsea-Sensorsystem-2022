/*
 * stts75.h
 *
 *  Created on: Mar 21, 2022
 *      Author: Jurgi
 */

#ifndef INC_STTS75_H_
#define INC_STTS75_H_

#include "stm32f4xx.h"
#include "i2c.h"
#include "stdio.h"



// Definerer datatype for suksessfull kommunikasjon eller ikke
// ----------------------------------------------------------------

typedef enum{
	TEMP_SUKSESS 	= 0x01,
	TEMP_FEIL 		= 0x00
} temp_status;

// ----------------------------------------------------------------

// Definerer makro for lesing og skriving av temperatursensorer
// ----------------------------------------------------------------
	// 		| Sensorkort | Kraftkort | Hovedkort |
	// A0		GND			  VCC		 GND
	// A1		GND			  GND	     VCC
	// A2		GND			  GND        GND
// ----------------------------------------------------------------

#define LES_AV_TEMP_SENS		0b10010001
#define SKRIV_TIL_TEMP_SENS		0b10010000

#define LES_AV_TEMP_KRAFT		0b10010011
#define SKRIV_TIL_TEMP_KRAFT	0b10010010

#define LES_AV_TEMP_HOVED		0b10010111
#define SKRIV_TIL_HOVED			0b10010110

// ----------------------------------------------------------------

// Definerer kommandoregistre
// ----------------------------------------------------------------

#define KOMMANDO_TEMP			0x0
#define KOMMANDO_KONF			0x1
#define KOMMANDO_HYST			0x2
#define KOMMANDO_OVERTEMP		0x3

// ----------------------------------------------------------------

// Setter opp funksjon for å lese av temperatur og sjekke status
// ----------------------------------------------------------------

temp_status TEMP_sjekk(I2C_HandleTypeDef *handle, uint8_t Enhetsadresse, uint8_t *feil);
temp_status TEMP_les(I2C_HandleTypeDef *handle, uint8_t Enhetavlesning, float *temp_f);


/**********************************************************
 * Register:		TEMP
 * Adresse:		 	0x00
 * Gruppenavn:		Temperature register
 * Tillatelse:		Avlesning (R)
 * Standardoppsett:	Ikke definert (-55ᵒC - 125ᵒC)
 **********************************************************/

#define TEMP_BIT_SB				0x8000; // Polaritetsbit
#define TEMP_BIT_POSISJON		15;		// Posisjon for mest signifikante bit

/**********************************************************
 * Register:		CONF
 * Adresse:		 	0x01
 * Gruppenavn:		Configuration register
 * Tillatelse:		Avlesning (R) og Tilskriving (W)
 * Standardoppsett:	0x00
 **********************************************************/

typedef enum{
	TEMP_KONF_REG_1				= 0x00,		// 0,5ᵒC
	TEMP_KONF_REG_2				= (0x01<<5),// 0,25ᵒC
	TEMP_KONF_REG_3				= (0x02<<5),// 0,125ᵒC
	TEMP_KONF_REG_4				= (0x03<<5)	// 0,0625ᵒC
}TEMP_KONF_REG_t;

#define	TEMP_KONF_MASKE		(0x03<<5)
temp_status TEMP_skriv_opplosning(I2C_HandleTypeDef *handle, uint8_t Enhetlesing, TEMP_KONF_REG_t nyverdi);
temp_status TEMP_les_opplosning(I2C_HandleTypeDef *handle, uint8_t Enhetlesing, TEMP_KONF_REG_t *verdi);


#endif /* INC_STTS75_H_ */
