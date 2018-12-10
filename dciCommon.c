#include "dciCommon.h"
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

char* dciStrArguments[] = {"dci0", "dci1","dci60a"};

uint8_t dciBandwidth[] = {1, 3, 5, 10, 15, 20};

void dci_ValidArguments(const int argc, const char* const argv[], dciType* restrict const  dci_p,
		uint8_t* restrict const prb_p )
{
	uint8_t dciBandwidthPRB[] = {6, 15, 25, 50, 75, 100};
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

void dci_readStdin(void)
{
	scanf("%li", &dci_readArgumentsStdin);
}


void dci_print(char* output /*?*/)
{
	fprintf(stdout, "%s", output);
}

static uint64_t ipow(uint64_t base, uint8_t n)
{
int result = 1;
    while(1)
    {
        if (n & 1)
        {
            result *= base;
        }
        n >>= 1;

        if (!n)
        {
            break;
        }
        base *= base;
    }
    return result;
}

static uint64_t createMask (const uint8_t n)
{
	uint64_t mask = 0;
	if (n == 0)
	{
		return mask;
	}
	mask = ipow(2,n) - 1;
	return mask;
}

uint32_t* dci_readValueFromDCI (uint64_t dci, uint8_t shiftArray[], const uint8_t sizeOfShiftArray)
{
	uint8_t bitLenghtOfDCI = 0;
	for (uint8_t i = 0; i < sizeOfShiftArray; i++)
	{
		bitLenghtOfDCI += shiftArray[i];
	}

	uint32_t* outputArray = malloc(sizeof(*outputArray)*sizeOfShiftArray);
	uint8_t alignedToRight = CHAR_BIT-(bitLenghtOfDCI%CHAR_BIT);
	if (alignedToRight != CHAR_BIT)
	{
		dci >>= alignedToRight;
	}
	for (uint8_t i = 0; i < sizeOfShiftArray; i++)
	{
		outputArray[i] = dci & createMask(shiftArray[sizeOfShiftArray - i - 1]);
		dci >>= shiftArray[sizeOfShiftArray - i - 1];
	}
	return outputArray;
}

