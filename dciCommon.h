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

void correctnessParametersPRB(uint8_t* dciParam);
void dci1_CorrectnessParameters(uint8_t* dciParam);
void dci0_CorrectnessParameters(uint8_t* dciParam, const uint8_t dciBandwidthPRB);
void dci60a_CorrectnessParameters(uint8_t* dciParam, const uint8_t dciBandwidthPRB);
uint32_t* dci_readValueFromDCI(uint64_t dci, const uint8_t bitLenghtOfDciParameter[],
							   const uint8_t sizeOfArray);
uint8_t dci_lengthOfRIVviaBandwidth(uint8_t Bandwidth, uint8_t LengthBitsRIV);
uint8_t dci1_lengthOfBitmapViaBandwidth(uint8_t Bandwidth, uint8_t bitmapBitLenght);
void dci_defineDci(const int argc, const char* const argv[], dciType* restrict const  dci_p,
				   uint8_t* restrict const prb_p);

uint8_t dci_readStdin(uint64_t dci_readArgumentsStdin);
void dci_print(char* output);

#endif /*DCI_COMMON_H_*/
