#ifndef DCI_COMMON_H
#define DCI_COMMON_H_

#include <inttypes.h>

typedef enum dci0_InputParameters
{
	dci0_firstPRB,
	dci0_lastPRB,
	dci0_mcsindex,
	dci0_newDataIndicator,
	dci0_pucchTpcCommand,
	dci0_dmrsCyclic,
	dci0_csiRqst,
	dci0_srsRqst,
	dci0_maxAmountOfArguments
} dci0_InputParameters;

typedef enum dci0_OutputParameters
{
	dci0_formtFlagOutput,
	dci0_hoppingFlagOutput,
	dci0_rivOutput,
	dci0_mcsindexOutput,
	dci0_newDataIndicatorOutput,
	dci0_pucchTpcCommandOutput,
	dci0_dmrsCyclicOutput,
	dci0_csiRqstOutput,
	dci0_srsRqstOutput,
	dci0_maxAmmountOfArgumentsOutput
} dci0_OutputParameters;

typedef enum dci1_InputParameters
{
	dci1_rbgNumber,
	dci1_rbgIndex,
	dci1_mcsindex,
	dci1_harq,
	dci1_newDataIndicator,
	dci1_redundancyVersion,
	dci1_pucchTpcCommand,
	dci1_maxAmountOfArguments
} dci1_InputParameters;

typedef enum dci1_OutputParameters
{
	dci1_resourdeAllocationType,
	dci1_bitmap,
	dci1_mcsindexOutput,
	dci1_harqOutput,
	dci1_newDataIndicatorOutput,
	dci1_redundancyVersionOutput,
	dci1_pucchTpcCommandOutput,
	dci1_maxAmmountOfArgumentsOutput
} dci1_OutputParameters;

/* 2 values, which will be changed */
#define RIV_LEN 13 //[riv len] bandwidth 20
#define BITMAP_LEN 25 //[bitmap len] bandwidth 20

/* Shared Length of parameters */
#define FORMAT_FLAG 1
#define HOPPING_FLAG 1
#define RA 1
#define RIV 13
#define CSIR 1
#define SRSR 1
#define NDI 1
#define TPC 2
#define HARQ 3
#define MCS 5
#define RV 2

/* dci0 Length of parameters */
#define DMRS 3
#define DCI0_NUMBER_PARAM 9

/* dci1 Length of parameters */
#define DCI1_NUMBER_PARAM 7

/* dci60a Length of parameters */
#define MCS60A 4
#define PUSCH 2
#define PDCCH 2
#define DCI60A_NUMBER_PARAM 10

/* Maximum values of parameters  */
#define MAX_MCS 31
#define MAX_MCS60A 15
#define MAX_HARQ 7
#define MAX_NDI 1
#define MAX_RV 3
#define MAX_TPC 3
#define MAX_DMRS 7
#define MAX_CSI_REQ 1
#define MAX_SRS_REQ 1
#define MAX_PUSCH 3
#define MAX_PDCCH 3

typedef enum bandwidth_t
{
    BW_1_4MHz,
    BW_3MHz,
    BW_5MHz,
    BW_10MHz,
    BW_15MHz,
    BW_20MHz,
    AMOUNT_OF_BANDWIDTHS
} bandwidth_t;

typedef enum dciType { dci0, dci1, dci60a, maxDci } dciType;
enum dci1_Parameters { paramMCS1, paramHARQ1, paramNDI1, paramRV1, paramTPC1 };
enum dci0_Parameters { paramFirstPRB0, paramLastPRB0, paramMCS0, paramNDI0, paramTPC0,
					   paramDMRS0, paramCSIreq0, paramSRSreq0 };

enum dci60a_Parameters { paramFirstPRB60a, paramLastPRB60a, paramMCS60a, paramPUSCH60a,
						 paramHARQ60a, paramNDI60a, paramRV60a, paramTPC60a, paramCSIreq60a,
						 paramSRSreq60a, paramPDCCH60a };

uint32_t dci_lengthOfRIVviaBandwidth(uint32_t bandwidth);
void dci0_CorrectnessParameters(uint8_t* dciParam, const uint8_t dci0_bandwidthPRB);
void dci1_CorrectnessParameters(uint8_t* dciParam);
uint32_t dci1_lengthOfBitmapViaBandwidth(uint32_t bandwidth);
void dci60a_CorrectnessParameters(uint8_t* dciParam, const uint8_t dci60a_bandwidthPRB);


/* > Function: dci_readValueFromDCI
**********************************************************************************************************
 * @brief   Creates array of uint32_t which are parameters decode from DCI with use
 *          bitLenghtOfDciParameter array.
 *
 * @param[in]   dci:                Number (DCI message) read from input of programm
 * @param[in]   bitLenghtOfDciParameter[]:  Array containing bitlenght of every DCI parameter in readed dci
 * @param[in]   sizeOfArray:            number of elements of bitLenghtOfDciParameter array
 * @param[in]   dci_readValueFromDCI_p:     pointer to array containing a DCI parameter decofe from readed dci
 *
**********************************************************************************************************/

uint32_t* dci_readValueFromDCI(uint64_t dci, uint32_t bandwidth, dciType selectedDci);

/* > Function: dci1_bitmapDecoder
**********************************************************************************************************
 * @brief   Creates array of uint32_t which are parameters decode from DCI with use
 *          bitLenghtOfDciParameter array.
 *
 * @param[in]   bitmap:             is number specific to DCI1 format. Its first readed number in array
 *                      from dci_readValueFromDCI function
 * @param[in]   bitmapBitLenght:        Its number dependend on bandwidth
 * @param[in]   dci1_bitmapDecoder_p:       pointer to array containing a DCI parameter decofe from readed dci
 *
**********************************************************************************************************/

uint32_t* dci1_bitmapDecoder(uint32_t bitmap, uint32_t bitmapBitLenght);

/* > Function: dci_DefineDci
 *******************************************************************************************************/
/*
 * @brief   Validate arguments from argv[] and assign them to @dci_p and @prb_p. When argv[1]
 *          is different than values in dciStrArguments function return standard dci0 type.
 *          This same happend to argv[2] where standard bandwith size is 20 MHz and return number
 *          of Power Resoure Block
 *
 * @param[in] argc              Amount of elements in argv[] array
 * @param[in] argv[]                Array with type of dci and bandwith
 * @param[out] dci_p                Pointer return type of dci
 * @param[out] prb_p                Pointer return nr of PRB's
 *
 *****************************************************************************************************/

void dci_defineDci(const int argc, const char* const argv[], dciType* restrict const dci_p,
        uint32_t* restrict prb_p);

uint8_t dci_readStdin(uint64_t* dci_readArgumentsStdin);

uint16_t dci_rivDecode(uint32_t bandwidthPRB, uint32_t riv,
        uint32_t* restrict outFirstPRB, uint32_t* restrict outLastPRB);

void dci_print(char* output);

#endif /*DCI_COMMON_H_*/
