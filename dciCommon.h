#ifndef DCI_COMMON_H
#define DCI_COMMON_H_

#define AMOUNT_OF_BANDWIDTHS 6

#include <inttypes.h>

typedef enum dciType {dci0, dci1, dci60a, maxDci} dciType;


uint32_t* dci_readValueFromDCI (uint64_t dci, const uint8_t bitLenghtOfDciParameter[], const uint8_t sizeOfArray);
uint8_t dci0_lengthOfRIVviaBandwidth(uint8_t Bandwidth, uint8_t LengthBitsRIV, uint8_t RIV);
uint8_t dci60a_lengthOfRIVviaBandwidth(uint8_t Bandwidth, uint8_t LengthBitsRIV, uint8_t RIV);
uint8_t dci1_lengthOfBitmapViaBandwidth(uint8_t Bandwidth, uint8_t LengthBitsRBG, uint8_t rbgNumber);

void dci_defineDci(const int argc, const char* const argv[], dciType* restrict const  dci_p,
		uint8_t* restrict const prb_p);
uint16_t dci_rivDecode (uint8_t bandwidthPRB, uint16_t RIV, uint8_t* restrict outFirstPRB, uint8_t* restrict outLastPRB);
uint8_t dci_readStdin(uint64_t dci_readArgumentsStdin);
void dci_print(char* output);

#endif /*DCI_COMMON_H_*/
