#ifndef DCI_SHARED_FORMAT_H_
#define DCI_SHARED_FORMAT_H_
#define AMOUNT_OF_BANDWIDTHS 6
#include <stdint.h>

typedef enum dciType {dci0, dci1, dci60a, maxDci} dciType;

extern uint8_t dciBandwidth[AMOUNT_OF_BANDWIDTHS];
extern uint8_t dciBandwidthPRB[AMOUNT_OF_BANDWIDTHS];

void dci_DefineDci(const int argc, const char* const argv[], dciType* restrict const  dci_p,
		uint8_t* restrict const prb_p );

#endif /*DCI_SHARED_FORMAT_H_*/
