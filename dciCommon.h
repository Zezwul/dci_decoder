#ifndef DCI_COMMON_H
#define DCI_COMMON_H_

#include <inttypes.h>

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

typedef enum dci0_OutputParameters
{
	dci0_firstPRBOutput,
	dci0_lastPRBOutput,
	dci0_mcsindexOutput,
	dci0_newDataIndicatorOutput,
	dci0_pucchTpcCommandOutput,
	dci0_dmrsCyclicOutput,
	dci0_csiRqstOutput,
	dci0_srsRqstOutput,
	dci0_maxAmountOfArgumentsOutput
} dci0_OutputParameters;

typedef enum dci0_InputParameters
{
	dci0_formtFlag,
	dci0_hoppingFlag,
	dci0_rivOutput,
	dci0_mcsindex,
	dci0_newDataIndicator,
	dci0_pucchTpcCommand,
	dci0_dmrsCyclic,
	dci0_csiRqst,
	dci0_srsRqst,
	dci0_maxAmmountOfArguments
} dci0_InputParameters;

typedef enum dci1_OutputParameters
{
	dci1_raType,
	dci1_bitmap,
	dci1_mcsindex,
	dci1_harq,
	dci1_newDataIndicator,
	dci1_redundancyVersion,
	dci1_pucchTpcCommand,
	dci1_maxAmountOfArguments
} dci1_OutputParameters;

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

typedef enum dciType
{
	dci0,
	dci1,
	dci60a,
	maxDci
} dciType;

enum dci60a_InputParameters {
	dci60a_FirstPRB60a,
	dci60a_LastPRB60a,
	dci60a_MCS60a,
	dci60a_PUSCH60a,
	dci60a_HARQ60a,
	dci60a_NDI60a,
	dci60a_RV60a,
	dci60a_TPC60a,
	dci60a_CSIreq60a,
	dci60a_RSreq60a,
	dci60a_PDCCH60a,
	dci60a_narrowbandIndex,
	dci60a_rivLength,
} dci60a_InputParameters;

//enum dci60a_OutputParameters {
//	dci60a_FirstPRB60a,
//	dci60a_LastPRB60a,
//	dci60a_MCS60a,
//	dci60a_PUSCH60a,
//	dci60a_HARQ60a,
//	dci60a_NDI60a,
//	dci60a_RV60a,
//	dci60a_TPC60a,
//	dci60a_CSIreq60a,
//	dci60a_RSreq60a,
//	dci60a_PDCCH60a
//} dci60a_OutputParameters;


uint32_t dci_lengthOfRIVviaBandwidth(bandwidth_t bandwidth);
void dci0_CorrectnessParameters(uint8_t* dciParam, const uint8_t dci0_bandwidthPRB);
void dci1_CorrectnessParameters(uint8_t* dciParam);
uint32_t dci1_lengthOfBitmapViaBandwidth(bandwidth_t bandwidth);
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

uint32_t* dci_readValueFromDCI(uint64_t dci, uint32_t* bitLenghtOfDciParameter, uint32_t sizeOfArray);
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

bandwidth_t dci_defineDci(const int argc, const char* const argv[], dciType* restrict const dci_p,
		uint32_t* restrict prb_p);

uint32_t dci_readStdin(uint64_t *dci_readArgumentsStdin);

uint16_t dci_rivDecode(uint32_t bandwidthPRB, uint32_t riv,
        uint32_t* restrict outFirstPRB, uint32_t* restrict outLastPRB);

void dci_print(char* output);

#endif /*DCI_COMMON_H_*/
