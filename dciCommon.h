#ifndef DCI_COMMON_H
#define DCI_COMMON_H_

#define AMOUNT_OF_BANDWIDTHS 6

#include <stdint.h>

typedef enum dciType {dci0, dci1, dci60a, maxDci} dciType;

extern uint64_t dci_readArgumentsStdin;
extern uint8_t dci_bandwidthPRB;
extern dciType dci_Result;
extern uint32_t* dci_tempParameters;

extern char* dci_output;
extern uint32_t* dci_argsToValid;

/*
 *	 =====	dci_readValueFromDCI =====
 * Function return a pointer (uint32_t*) to array with values decode from DCI.
 * dci is our DCI read from input of programm;
 * bitLenghtOfDciParameteris array with bit lenght of every DCI parameter;
 * sizeOfArray is number of arrays element of shiftArray;
 */

uint32_t* dci_readValueFromDCI (uint64_t dci, const uint8_t bitLenghtOfDciParameter[], const uint8_t sizeOfArray);

void dci_defineDci(const int argc, const char* const argv[], dciType* restrict const  dci_p,
		uint8_t* restrict const prb_p);
uint8_t dci_readStdin(uint64_t dci_readArgumentsStdin);
void dci_print(char* output);

#endif /*DCI_COMMON_H_*/
