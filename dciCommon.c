#include "dciCommon.h"
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <math.h>
#include <string.h>

const char* const dciStrArguments[] = {"dci0", "dci1","dci60a"};

uint8_t dciBandwidth[AMOUNT_OF_BANDWIDTHS] = {1, 3, 5, 10, 15, 20};
uint8_t dciBandwidthPRB[AMOUNT_OF_BANDWIDTHS] = {6, 15, 25, 50, 75, 100};
uint8_t possibleLengthBitsOfRIV[AMOUNT_OF_BANDWIDTHS] = {5, 7, 9, 11, 12 ,13};

uint8_t dci_lengthOfRIVviaBandwidth(uint8_t bandwidth)
{
	uint8_t LengthBitsRIV;
	for (uint8_t i = 0; i < AMOUNT_OF_BANDWIDTHS; ++i)
	{
		if (bandwidth == dciBandwidth[i])
		{
			LengthBitsRIV = possibleLengthBitsOfRIV[i];
			return LengthBitsRIV;
		}
		else
		{
			fprintf(stdout, "ERR_OCC_Typed_bandwidth_is_wrong");
		}
	}
	return 0;
}

uint8_t dci1_lengthOfBitmapViaBandwidth(uint8_t bandwidth)
{
	uint8_t bitmapBitLenght;
	uint8_t possibleLengthBitsRBG[AMOUNT_OF_BANDWIDTHS] = {6, 8, 13, 17, 19 ,25};
	for (uint8_t i = 0; i < AMOUNT_OF_BANDWIDTHS; ++i)
	{
		if (bandwidth == dciBandwidth[i])
		{
			bitmapBitLenght = possibleLengthBitsRBG[i];
			return bitmapBitLenght;
		}
		else
		{
			fprintf(stdout, "ERR_OCC_Typed_bandwidth_is_wrong");
		}
	}
	return 0;
}

void dci_defineDci(const int argc, const char* const argv[], dciType* restrict const  dci_p,
		uint8_t* restrict prb_p)
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
	uint8_t val = (uint8_t)scanf(SCNu64, &dci_readArgumentsStdin);
	return val;
}


void dci_print(char* output /*?*/)
{
	fprintf(stdout, "%s", output);
}

static uint64_t createMask(const uint32_t n)
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

uint64_t getBits(uint64_t dci, uint32_t startRead, uint32_t shift)
{
    return (dci >> (startRead + 1 - shift)) & ~(~0 << shift);
}

uint32_t* dci_readValueFromDCI(uint64_t dci, uint32_t bandwidth, dciType selectedDci)
{
    uint32_t sizeOfArray = 0;
    uint32_t* outputArray = malloc(sizeof(*outputArray)*sizeOfArray);
    uint32_t* bitLenghtOfDciParameter;
    if (selectedDci == dci0)
    {
        uint32_t valueToChange = 31;
        for (uint32_t i = 0; i < DCI1_NUMBER_PARAM; i++)
        {
            outputArray[i] = getbits(dci, valueToChange, dci0_offsetArray[i]);
            valueToChange -= dci0_offsetArray[i];
        }
    }
    else if ( selectedDci == dci1)
    {
        uint32_t valueToChange = dci1_numberOfAllocatedBytes(bandwidth);
        valueToChange = valueToChange * CHAR_BIT - 1;
        for (uint32_t i = 0; i < DCI1_NUMBER_PARAM; i++)
        {
            outputArray[i] = getbits(dci, valueToChange, dci1_offsetArray[i]);
            valueToChange -= dci1_offsetArray[i];
        }
    }
    else if ( selectedDci == dci60a)
    {
        uint32_t valueToChange = 31;
        for (uint32_t i = 0; i < DCI1_NUMBER_PARAM; i++)
        {
            outputArray[i] = getbits(dci, valueToChange, dci60a_offsetArray[i]);
            valueToChange -= dci60a_offsetArray[i];
        }
    }
    return outputArray;
}

void dci0_CorrectnessParameters(uint8_t* dciParam, const uint8_t dci0_bandwidthPRB)
{
	if (dciParam[paramFirstPRB0] >= dci0_bandwidthPRB)
	{
		fprintf(stdout, "ERR_OCC_Value_of_FirstPRB_is_too_big\n");
	}

	if (dciParam[paramFirstPRB0] > dciParam[paramLastPRB0] || dciParam[paramFirstPRB0] >= dci0_bandwidthPRB)
	{
		fprintf(stdout, "ERR_OCC_Inncorrect_value_of_PRB\n");
	}

	if (dciParam[paramMCS0] > MAX_MCS)
	{
		fprintf(stdout, "ERR_OCC_Inncorrect_value_of_MCS_parametr\n");
	}

	if (dciParam[paramNDI0] > MAX_NDI)
	{
		fprintf(stdout, "ERR_OCC_Inncorrect_value of_NDI_parametr\n");
	}

	if (dciParam[paramTPC0] > MAX_TPC)
	{
		fprintf(stdout, "ERR_OCC_Inncorrect_value of_TPC_parametr\n");
	}

	if (dciParam[paramDMRS0] > MAX_DMRS)
	{
		fprintf(stdout, "ERR_OCC_Inncorrect_value of_DMRS_parametr\n");
	}

	if (dciParam[paramCSIreq0] > MAX_CSI_REQ)
	{
		fprintf(stdout, "ERR_OCC_Inncorrect_value of_CSIreq_parametr\n");
	}

	if (dciParam[paramSRSreq0] > MAX_SRS_REQ)
	{
		fprintf(stdout, "ERR_OCC_Inncorrect_value of_SRSreq_parametr\n");
	}
}

void dci1_CorrectnessParameters(uint8_t* dciParam)
{
	if (dciParam[paramMCS1] > MAX_MCS)
	{
		fprintf(stdout, "ERR_OCC_Inncorrect_value of_MCS_parametr\n");
	}

	if (dciParam[paramHARQ1] > MAX_HARQ)
	{
		fprintf(stdout, "ERR_OCC_Inncorrect_value_of_HARQ_parametr\n");
	}

	if (dciParam[paramNDI1] > MAX_NDI)
	{
		fprintf(stdout, "ERR_OCC_Inncorrect_value_of_NDI_parametr\n");
	}

	if (dciParam[paramRV1] > MAX_RV)
	{
		fprintf(stdout, "ERR_OCC_Inncorrect_value_of_RV_parametr\n");
	}

	if (dciParam[paramTPC1] > MAX_TPC)
	{
		fprintf(stdout, "ERR_OCC_Inncorrect_value_of_TPC_parametr\n");
	}
}

void dci60a_CorrectnessParameters(uint8_t* dciParam, const uint8_t dci60a_bandwidthPRB)
{
	if (dciParam[paramFirstPRB60a] >= dci60a_bandwidthPRB)
	{
		fprintf(stdout, "ERR_OCC_Value_of_FirstPRB_is_too_big\n");
	}

	if (dciParam[paramFirstPRB60a] > dciParam[paramLastPRB60a] || dciParam[paramFirstPRB60a] >= dci60a_bandwidthPRB)
	{
		fprintf(stdout, "ERR_OCC_Inncorrect_value_of_PRB\n");
	}

	if (dciParam[paramMCS60a] > MAX_MCS)
	{
		fprintf(stdout, "ERR_OCC_Inncorrect_value of_MCS_parametr\n");
	}

	if (dciParam[paramPUSCH60a] > MAX_PUSCH)
	{
		fprintf(stdout, "ERR_OCC_Inncorrect_value of_PUSCH_parametr\n");
	}

	if (dciParam[paramHARQ60a] > MAX_HARQ)
	{
		fprintf(stdout, "ERR_OCC_Inncorrect_value of_HARQ_parametr\n");
	}

	if (dciParam[paramNDI60a] > MAX_NDI)
	{
		fprintf(stdout, "ERR_OCC_Inncorrect_value of_NDI_parametr\n");
	}

	if (dciParam[paramRV60a] > MAX_RV)
	{
		fprintf(stdout, "ERR_OCC_Inncorrect_value of_RV_parametr\n");
	}

	if (dciParam[paramTPC60a] > MAX_TPC)
	{
		fprintf(stdout, "ERR_OCC_Inncorrect_value of_TPC_parametr\n");
	}

	if (dciParam[paramCSIreq60a] > MAX_CSI_REQ)
	{
		fprintf(stdout, "ERR_OCC_Inncorrect_value of_CSIreq_parametr\n");
	}

	if (dciParam[paramSRSreq60a] > MAX_SRS_REQ)
	{
		fprintf(stdout, "ERR_OCC_Inncorrect_value of_SRSreq_parametr\n");
	}

	if (dciParam[paramPDCCH60a] > MAX_PDCCH)
	{
		fprintf(stdout, "ERR_OCC_Inncorrect_value of_PDCCH_parametr\n");
	}
}

uint16_t dci_rivDecode(uint32_t bandwidthPRB, uint32_t riv,
		uint32_t* restrict outFirstPRB, uint32_t* restrict outLastPRB)
{
	if (outFirstPRB == NULL || outLastPRB == NULL)
	{
		printf("ERR_OCC_invalid_pointers");
		return UINT16_MAX;
	}
	uint32_t PRBFirst = 0;
	uint32_t PRBLength = 0;
	PRBFirst = (riv % bandwidthPRB);
	PRBLength = (riv / bandwidthPRB + 1);
	if (PRBFirst + PRBLength > bandwidthPRB)
	{
		PRBFirst = (uint32_t)(bandwidthPRB - 1 - PRBFirst);
		PRBLength = (uint32_t)(bandwidthPRB + 1 - PRBLength + 1);
	}
	*outFirstPRB = PRBFirst;
	*outLastPRB = PRBFirst + PRBLength - 1;
	return 0;
}

uint32_t* dci1_bitmapDecoder(uint32_t bitmap, uint32_t bitmapBitLenght)
{
	uint32_t counter = bitmapBitLenght-1;
	uint32_t* outputRBGIndex = malloc(sizeof(*outputRBGIndex));
	uint32_t i = 0, j = 0;
	while (i < bitmapBitLenght)
	{
		if (bitmap & 1)
		{
			++j;
			outputRBGIndex = realloc(outputRBGIndex, (j + 1) * sizeof(*outputRBGIndex));
			outputRBGIndex[j] = counter;
		}
		bitmap >>= 1;
		counter--;
		i++;
	}
	outputRBGIndex[0] = j;
	return outputRBGIndex;
}
