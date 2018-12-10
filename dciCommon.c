#include "dciCommon.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

char* dciStrArguments[] = {"dci0", "dci1","dci60a"};
uint8_t dciBandwidth[AMOUNT_OF_BANDWIDTHS] = {1, 3, 5, 10, 15, 20};
uint8_t dciBandwidthPRB[AMOUNT_OF_BANDWIDTHS] = {6, 15, 25, 50, 75, 100};

/* > Function: dci_DefineDci
 *******************************************************************************************/
/*
 * @brief 	Validate arguments from argv[] and assign them to @dci_p and @prb_p. When argv[1]
 * 			is different than values in dciStrArguments function return standard dci0 type.
 * 			This same happend to argv[2] where standard bandwith size is 20 MHz and return number
 * 			of Power Resoure Block
 *
 * @param[in] argc 							Amount of elements in argv[] array
 * @param[in] argv[] 						Array with type of dci and bandwith
 * @param[out] dci_p						Pointer return type of dci
 * @param[out] prb_p						Pointer return nr of PRB's
 *
 *******************************************************************************************/
void dci_DefineDci(const int argc, const char* const argv[], dciType* restrict const  dci_p,
		uint8_t* restrict const prb_p )
{
	dciType dciResult = dci0;
	uint8_t bandwidthPRB = 100;
	if (argc > 2 && argc < 4)
	{
		for (dciType dci = dci0; dci < maxDci; ++dci)
		{
			if (strcmp(dciStrArguments[dci], argv[1]) == 0)
			{
				dciResult = dci;
				break;
			}
		}
		for (size_t i = 0; i < sizeof(dciBandwidth) / sizeof(uint8_t); ++i)
		{
			if (atoi(argv[2]) == dciBandwidth[i])
			{
				bandwidthPRB = dciBandwidthPRB[i];
				break;
			}
		}
	}
	*prb_p = bandwidthPRB;
	*dci_p = dciResult;
}
