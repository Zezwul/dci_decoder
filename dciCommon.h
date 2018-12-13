#ifndef DCI_COMMON_H
#define DCI_COMMON_H_

#define AMOUNT_OF_BANDWIDTHS 6

/* dci0 Length of parameters */
#define FORMAT_FLAG 1
#define HOPPING_FLAG 1
#define RIV 13
#define FIRSTPRB0 7
#define LASTPRB0 7
#define MCS0 5
#define NDI0 1
#define TPC0 2
#define DMRS 3
#define CSIR0 1
#define SRSR0 1

#define DCI0_NUMBER_PARAM 8
#define DCI0_OFFSET_INPUT_ARRAY { FORMAT_FLAG, HOPPING_FLAG, RIV, MCS0, \
								NDI0, TPC0, DMRS, CSIR0, SRSR0 };
#define DCI0_OFFSET_ARRAY { FIRSTPRB0, LASTPRB0, MCS0, NDI0, TPC0, DMRS, CSIR0, SRSR0 };

/* dci1 Length of parameters */
#define MCS1 5
#define HARQ1 3
#define NDI1 1
#define RV1 2
#define TPC1 2

#define DCI1_NUMBER_PARAM 5
#define DCI1_OFFSET_ARRAY {MCS1, HARQ, NDI1, RV1, TPC1};

/* dci60a Length of parameters */
#define FIRSTPRB60A 7
#define LASTPRB60A 7
#define MCS60A 4
#define PUSCH 2
#define HARQ60A 3
#define NDI60A 1
#define RV60A 2
#define TPC60A 2
#define CSIR60A 1
#define SRSR60A 1
#define PDCCH 2

#define DCI60A_NUMBER_PARAM 11
#define DCI60A_OFFSET_ARRAY {FIRSTPRB60A, LASTPRB60A, MCS60A, PUSCH, HARQ60A, NDI60A,\
	RV60A, TPC60A, CSIR60A, SRSR60A, PDCCH};

#include <inttypes.h>

typedef enum dciType {dci0, dci1, dci60a, maxDci} dciType;


uint32_t* dci_readValueFromDCI (uint64_t dci, uint8_t bitLenghtOfDciParameter[],
		const uint8_t sizeOfArray, uint32_t bandwidth);
uint8_t dci_lengthOfRIVviaBandwidth(uint8_t Bandwidth, uint8_t LengthBitsRIV);
uint8_t dci1_lengthOfBitmapViaBandwidth(uint8_t Bandwidth, uint8_t LengthBitsRBG, uint8_t rbgNumber);

void dci_defineDci(const int argc, const char* const argv[], dciType* restrict const  dci_p,
		uint8_t* restrict const prb_p);

uint8_t dci_readStdin(uint64_t* dci_readArgumentsStdin);
void dci_print(char* output);

#endif /*DCI_COMMON_H_*/
