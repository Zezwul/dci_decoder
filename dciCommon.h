#ifndef DCI_COMMON_H
#define DCI_COMMON_H_

#include <stdint.h>

#define AMOUNT_OF_BANDWIDTHS 6

typedef enum dciType {dci0, dci1, dci60a, maxDci} dciType;

extern uint64_t dci_readArgumentsStdin;
extern uint8_t dci_bandwidthPRB;
extern dciType dci_Result;
extern uint32_t* dci_tempParameters;

extern char* dci_output;
extern uint32_t* dci_argsToValid;

void dci_defineDci(const int argc, const char* const argv[], dciType* restrict const  dci_p,
		uint8_t* restrict const prb_p);
uint32_t* dci_readValueFromDci(uint64_t dci, uint8_t shiftArray[], uint8_t sizeOfShiftArray);
void dci_readStdin(void);
void dci_print(char* output);

#endif /*DCI_COMMON_H_*/
