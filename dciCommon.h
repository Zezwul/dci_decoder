#ifndef DCI_COMMON_H
#define DCI_COMMON_H_

#define AMOUNT_OF_BANDWIDTHS 6

#include <stdint.h>

typedef enum dciType {dci0, dci1, dci60a, maxDci} dciType;


/* > Function: dci_readValueFromDCI
**********************************************************************************************************
 * @brief	Creates array of uint32_t which are parameters decode from DCI with use
 * 			bitLenghtOfDciParameter array.
 *
 * @param[in]	dci: 						 Number (DCI message) read from input of programm
 * @param[in]	bitLenghtOfDciParameter[]: 	 Array containing bitlenght of every DCI parameter in readed dci
 * @param[in]	sizeOfArray: 				 number of elements of bitLenghtOfDciParameter array
 * @param[in]	dci_readValueFromDCI_p: 	 pointer to array containing a DCI parameter decofe from readed dci
 *
**********************************************************************************************************/
uint32_t* dci_readValueFromDCI (uint64_t dci, const uint8_t bitLenghtOfDciParameter[], const uint8_t sizeOfArray);
uint8_t dci0_lengthOfRIVviaBandwidth(uint8_t Bandwidth, uint8_t LengthBitsRIV, uint8_t RIV);
uint8_t dci60a_lengthOfRIVviaBandwidth(uint8_t Bandwidth, uint8_t LengthBitsRIV, uint8_t RIV);
uint8_t dci1_lengthOfBitmapViaBandwidth(uint8_t Bandwidth, uint8_t LengthBitsRBG, uint8_t rbgNumber);

/* > Function: dci1_bitmapDecoder
**********************************************************************************************************
 * @brief	Creates array of uint32_t which are parameters decode from DCI with use
 * 			bitLenghtOfDciParameter array.
 *
 * @param[in]	bitmap: 					is number specific to DCI1 format. Its first readed number in array
 * 											from dci_readValueFromDCI function
 * @param[in]	bitmapBitLenght: 			Its number dependend on bandwidth
 * @param[in]	dci1_bitmapDecoder_p: 		pointer to array containing a DCI parameter decofe from readed dci
 *
**********************************************************************************************************/
uint8_t* dci1_bitmapDecoder(uint32_t bitmap, uint8_t bitmapBitLenght);

uint8_t dci0_lengthOfRIVviaBandwidth(uint8_t Bandwidth, uint8_t LengthBitsRIV, uint8_t RIV);
uint8_t dci60a_lengthOfRIVviaBandwidth(uint8_t Bandwidth, uint8_t LengthBitsRIV, uint8_t RIV);
uint8_t dci1_lengthOfBitmapViaBandwidth(uint8_t Bandwidth, uint8_t LengthBitsRBG, uint8_t rbgNumber);

/* > Function: dci_DefineDci
 *******************************************************************************************************/
/*
 * @brief 	Validate arguments from argv[] and assign them to @dci_p and @prb_p. When argv[1]
 * 			is different than values in dciStrArguments function return standard dci0 type.
 * 			This same happend to argv[2] where standard bandwith size is 20 MHz and return number
 * 			of Power Resoure Block
 *
 * @param[in] argc							Amount of elements in argv[] array
 * @param[in] argv[]						Array with type of dci and bandwith
 * @param[out] dci_p						Pointer return type of dci
 * @param[out] prb_p						Pointer return nr of PRB's
 *
 *****************************************************************************************************/
void dci_defineDci(const int argc, const char* const argv[], dciType* restrict const  dci_p,
		uint8_t* restrict const prb_p);

uint8_t dci_readStdin(uint64_t dci_readArgumentsStdin);
void dci_print(char* output);

#endif /*DCI_COMMON_H_*/
