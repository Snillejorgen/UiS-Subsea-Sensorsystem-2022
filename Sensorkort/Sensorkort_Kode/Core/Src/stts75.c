/*
 * stts75.c
 *
 *  Created on: Mar 21, 2022
 *      Author: Jurgi
 */

#include "stts75.h"


/**********************************************************
 * Funksjonsnavn:	TEMP_skrivtilreg
 * Beskrivelse: 	Funksjon for å skrive til register
 * Input:			Registeradresse, enhet som skal skrives til, peker til buffer som skal skrives til, lengden av bufferen
 * Output:			ingenting
 * Return: 			temp_status [TEMP_FEIL, TEMP_SUKSESS]
 **********************************************************/

temp_status TEMP_skrivtilreg(I2C_HandleTypeDef *handle, uint8_t Enhetskriving, uint8_t komreg, uint8_t *buffer, uint16_t len){
	HAL_StatusTypeDef status;

	status = HAL_I2C_Mem_Write(handle, Enhetskriving & 0xFE, komreg, 1, buffer, len, 1000);
	if(HAL_OK != status){return TEMP_FEIL;}
	return TEMP_SUKSESS;
}

/**********************************************************
 * Funksjonsnavn:	TEMP_lesreg
 * Beskrivelse: 	Funksjon for å lese av register
 * Input:			Registeradresse, enhet som skal skrives til, peker til buffer som skal skrives til, lengden av bufferen
 * Output:			ingenting
 * Return: 			temp_status [TEMP_FEIL, TEMP_SUKSESS]
 **********************************************************/

temp_status TEMP_lesreg(I2C_HandleTypeDef *handle, uint8_t Enhetlesing, uint8_t komreg, uint8_t *buffer, uint16_t len){
	HAL_StatusTypeDef status;

	status = HAL_I2C_Mem_Read(handle, Enhetlesing, komreg, 1, buffer, len, 1000);
	if(HAL_OK != status){return TEMP_FEIL;}
	return TEMP_SUKSESS;
}

/**********************************************************
 * Funksjonsnavn:	TEMP_les_temp_pol_bit
 * Beskrivelse: 	Funksjon for å lese polaritetsbit av temperaturregister
 * Input:			Peker til uint8_t verdi, enhetsadresse å lese av
 * Output:			verdi på polaritetsbit
 * Return: 			temp_status [TEMP_FEIL, TEMP_SUKSESS]
 **********************************************************/

temp_status TEMP_les_temp_pol_bit(void *handle, uint8_t Enhetlesing, uint16_t *verdi){
	if( !TEMP_lesreg(handle, Enhetlesing, KOMMANDO_TEMP, (uint8_t *)verdi, 2))return TEMP_FEIL;

	*verdi &= TEMP_BIT_SB; 					// Isoler polaritetsbit
	*verdi = *verdi >> TEMP_BIT_POSISJON;	// Maske for å forskyve polaritetsbitet 15 plasser bak

	return TEMP_SUKSESS;
}

/**********************************************************
 * Funksjonsnavn:	TEMP_skriv_opplosning
 * Beskrivelse: 	Funksjon for å bestemme oppløsning
 * Input:			TEMP_KONF_REG_t, enhetsadresse å lese av, enhetsadresse å skrive til
 * Output:			Ingenting
 * Return: 			temp_status [TEMP_FEIL, TEMP_SUKSESS]
 **********************************************************/
temp_status TEMP_skriv_opplosning(I2C_HandleTypeDef *handle, uint8_t Enhetlesing, TEMP_KONF_REG_t nyverdi){

	uint8_t verdi;
	if( !TEMP_lesreg(handle, Enhetlesing, KOMMANDO_KONF, &verdi, 1) ) return TEMP_FEIL;

	verdi &= ~TEMP_KONF_MASKE;
	verdi |= nyverdi;

	if( !TEMP_skrivtilreg(handle, Enhetlesing, KOMMANDO_KONF, &verdi, 1) ) return TEMP_FEIL;

	return TEMP_SUKSESS;
}

/**********************************************************
 * Funksjonsnavn:	TEMP_les_opplosning
 * Beskrivelse: 	Funksjon for å lese hvilken oppløsning sensoren er konfigurert til
 * Input:			peker til TEMP_KONF_REG_t, enhetsadresse å lese av, enhetsadresse å skrive til
 * Output:			Ingenting
 * Return: 			temp_status [TEMP_FEIL, TEMP_SUKSESS]
 **********************************************************/

temp_status TEMP_les_opplosning(I2C_HandleTypeDef *handle, uint8_t Enhetlesing, TEMP_KONF_REG_t *verdi){

	if( !TEMP_lesreg(handle, Enhetlesing, KOMMANDO_KONF, (uint8_t *)verdi, 1) ) return TEMP_FEIL;

	*verdi &= TEMP_KONF_MASKE;

	return TEMP_SUKSESS;

}


/**********************************************************
 * Funksjonsnavn:	TEMP_sjekk
 * Beskrivelse: 	Sjekker om enheten er online
 * Input:			Enhetsadresse, verdi til å indikere feil
 * Output:			Status på enhet
 * Return: 			temp_status [TEMP_FEIL, TEMP_SUKSESS]
 **********************************************************/

temp_status TEMP_sjekk(I2C_HandleTypeDef *handle, uint8_t Enhetslesing, uint8_t *feil){

	uint8_t stat;
	stat = HAL_I2C_IsDeviceReady(handle, Enhetslesing, 10, 1000);
	if(HAL_OK != stat){
		*feil = 1;
		return TEMP_FEIL;}
	*feil = 0;
	return TEMP_SUKSESS;
}

/**********************************************************
 * Funksjonsnavn:	TEMP_les
 * Beskrivelse: 	Leser av temperatur
 * Input:			Peker til uint16_t, hvilken enhet som leses av
 * Output:			Status på enhet
 * Return: 			temp_status [TEMP_FEIL, TEMP_SUKSESS]
 **********************************************************/

temp_status TEMP_les(I2C_HandleTypeDef *handle, uint8_t Enhetavlesning, float *temp_f){

	uint8_t stat;
	uint8_t temp_v[2];
	stat = HAL_I2C_Mem_Read(handle, Enhetavlesning, KOMMANDO_TEMP, 1, temp_v, 2, 1000);

	if(HAL_OK != stat){return TEMP_FEIL;}

	if(((temp_v[0]<<8|temp_v[1]) & 0x8000) >> 15)
		*temp_f = -(0x8000 - (0x7fff & (temp_v[0]<<8|temp_v[1])));
	else
		*temp_f = (temp_v[0]<<8|temp_v[1]) / 256.0;

	return TEMP_SUKSESS;
}
