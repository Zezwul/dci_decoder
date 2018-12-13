#ifndef DCI_COMMON_H
#define DCI_COMMON_H_

#include <stdint.h>

/* Shared Length of parameters */
#define FIRSTPRB 7
#define LASTPRB 7
#define CSIR 1
#define SRSR 1
#define NDI 1
#define TPC 2
#define HARQ 3
#define MCS 5

/* dci0 Length of parameters */
#define DMRS 3
#define DCI0_NUMBER_PARAM 8
#define DCI0_OFFSET_ARRAY { FIRSTPRB, LASTPRB, MCS, NDI, TPC, DMRS, CSIR, SRSR };

/* dci1 Length of parameters */
#define RV1 2
#define DCI1_NUMBER_PARAM 5
#define DCI1_OFFSET_ARRAY { MCS, HARQ, NDI, RV1, TPC };

/* dci60a Length of parameters */
#define MCS60A 4
#define PUSCH 2
#define RV60A 2
#define PDCCH 2

#define DCI60A_NUMBER_PARAM 11
#define DCI60A_OFFSET_ARRAY { FIRSTPRB, LASTPRB, MCS60A, PUSCH, HARQ, NDI,\
	RV60A, TPC, CSIR, SRSR, PDCCH };

/* Maximum values of parameters  */
#define MAX_MCS 31
#define MAX_MCS60A 15
#define MAX_HARQ 7
#define MAX_NDI 1
#define MAX_RV1 1
#define MAX_RV60A 3
#define MAX_TPC 3
#define MAX_DMRS 7
#define MAX_CSI_REQ 1
#define MAX_SRS_REQ 1
#define MAX_PUSCH 3
#define MAX_PDCCH 3

#define AMOUNT_OF_BANDWIDTHS 6

typedef enum dciType { dci0, dci1, dci60a, maxDci } dciType;
enum dci1_Parameters { paramMCS1, paramHARQ1, paramNDI1, paramRV1, paramTPC1 };
enum dci0_Parameters { paramFirstPRB0, paramLastPRB0, paramMCS0, paramNDI0, paramTPC0,
					   paramDMRS0, paramCSIreq0, paramSRSreq0 };
enum dci60a_Parameters { paramFirstPRB60a, paramLastPRB60a, paramMCS60a, paramPUSCH60a,\
						 paramHARQ60a, paramNDI60a, paramRV60a, paramTPC60a, paramCSIreq60a,\
						 paramSRSreq60a, paramPDCCH60a };

void dci1_CorrectnessParameters(uint8_t* dciParam);
void dci0_CorrectnessParameters(uint8_t* dciParam, const uint8_t dciBandwidthPRB);
void dci60a_CorrectnessParameters(uint8_t* dciParam, const uint8_t dciBandwidthPRB);

uint8_t dci_lengthOfRIVviaBandwidth(uint8_t Bandwidth, uint8_t LengthBitsRIV);
uint8_t dci1_lengthOfBitmapViaBandwidth(uint8_t Bandwidth, uint8_t bitmapBitLenght);

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
