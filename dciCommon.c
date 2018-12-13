#include "dciCommon.h"
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <math.h>
#include <string.h>

char* dciStrArguments[] = {"dci0", "dci1","dci60a"};

uint8_t dciBandwidth[AMOUNT_OF_BANDWIDTHS] = {1, 3, 5, 10, 15, 20};
uint8_t dciBandwidthPRB[AMOUNT_OF_BANDWIDTHS] = {6, 15, 25, 50, 75, 100};

uint8_t dci_lengthOfRIVviaBandwidth(uint8_t Bandwidth, uint8_t LengthBitsRIV)
{
	uint8_t possibleLengthBitsOfRIV[AMOUNT_OF_BANDWIDTHS] = {5, 7, 7, 11, 12 ,13};

	for (uint8_t i = 0; i < AMOUNT_OF_BANDWIDTHS; ++i)
	{
		if (Bandwidth == dciBandwidth[i])
		{
			LengthBitsRIV = possibleLengthBitsOfRIV[i];
			return 0;
		}
		else
		{
			fprintf(stdout, "ERR_OCC_Typed_bandwidth_is_wrong");
		}
	}
}

uint8_t dci1_lengthOfBitmapViaBandwidth(uint8_t Bandwidth, uint8_t LengthBitsRBG, uint8_t rbgNumber)
{
	uint8_t possibleLengthBitsRBG[AMOUNT_OF_BANDWIDTHS] = {6, 8, 13, 17, 19 ,25};

	for (uint8_t i = 0; i < AMOUNT_OF_BANDWIDTHS; ++i)
	{
		if (Bandwidth == dciBandwidth[i])
		{
			LengthBitsRBG = possibleLengthBitsRBG[i];

			if (rbgNumber > LengthBitsRBG)
			{
				fprintf(stdout, "ERR_OCC_Wrong_length_bits\n");
			}
			return 0;
		}
	}
	fprintf(stdout, "ERR_OCC_Typed_bandwidth_is_wrong");
}

void dci_defineDci(const int argc, const char* const argv[], dciType* restrict const  dci_p,
		uint8_t* restrict prb_p )
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

uint8_t dci_readStdin(uint64_t *dci_readArgumentsStdin)
{
	uint8_t val = scanf(SCNu64, dci_readArgumentsStdin);
	return val;
}


void dci_print(char* output /*?*/)
{
	fprintf(stdout, "%s", output);
}

static uint64_t createMask (const uint8_t n)
{
	uint64_t mask = 1;
	if (n == 0)
	{
		return 0;
	}
	mask <<= n;
	mask -= 1;
	return mask;
}

static void setRivLength(uint8_t* bitLenghtOfDciParameter, uint32_t bandwidthPRB)
{
	const uint32_t rivPosition = 3;
	bitLenghtOfDciParameter[rivPosition] = ceil((log2(bandwidthPRB * ((bandwidthPRB + 1) / 2))));
}

uint32_t* dci_readValueFromDCI (uint64_t dci, uint8_t bitLenghtOfDciParameter[],
		const uint8_t sizeOfArray)
{
	uint8_t bitLenghtOfDCI = 0;
	for (uint8_t i = 0; i < sizeOfArray; i++)
	{
		bitLenghtOfDCI += bitLenghtOfDciParameter[i];
	}

	uint32_t* outputArray = malloc(sizeof(*outputArray)*sizeOfArray);
	for (uint8_t i = 0; i < sizeOfArray; i++)
	{
		outputArray[i] = dci & createMask(bitLenghtOfDciParameter[sizeOfArray - i - 1]);
		dci >>= bitLenghtOfDciParameter[sizeOfArray - i - 1] - 1;
	}
	return outputArray;
}

uint8_t* dci1_bitmapDecoder(uint32_t bitmap, uint8_t bitmapBitLenght)
{
	uint8_t counter = bitmapBitLenght-1;
	uint8_t* outputRBGIndex = malloc(sizeof(*outputRBGIndex));
	uint8_t i, j = 0;
	while (i < bitmapBitLenght)
	{
		if (bitmap & 1)
		{
			++j;
			outputRBGIndex = realloc(outputRBGIndex,(j+1)*sizeof(*outputRBGIndex));
			outputRBGIndex[j] = counter;
		}
		bitmap >>= 1;
		counter--;
		i++;
	}
	outputRBGIndex[0] = j;
	return outputRBGIndex;
}

