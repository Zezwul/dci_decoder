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
uint8_t possibleLengthBitsOfRIV[AMOUNT_OF_BANDWIDTHS] = {5, 7, 7, 11, 12 ,13};
uint8_t possibleLengthBitsRBG[AMOUNT_OF_BANDWIDTHS] = {6, 8, 13, 17, 19 ,25};



//void dci_validationForBandwidth(uint8_t Bandwidth, const uint8_t bandwidthPRB)
//{
//    uint32_t lenghtRivBits[BANDWITDH_VALUES] = {5, 7, 7, 11, 12, 13};
//    uint32_t outputElements[BANDWITDH_VALUES] = {3, 4, 4, 4, 4, 4};
//    uint32_t amountOfPRBs[BANDWITDH_VALUES] = {6, 15, 25, 50, 75, 100};
//
//    uint8_t control = 0;
//
//    for (uint32_t i = 0; i < BANDWITDH_VALUES; i++)l
//    {
//        if (amountOfPRBs[i] == bandwidthPRB)
//        {
//            dci->outputArrayElements = outputElements[i];
//            dci->lenghtRivBits = lenghtRivBits[i];
//            dci60a_validancePRBs(dci, bandwidthPRB);
//            control++;
//            break;
//        }
//    }
//    if (control == 0)
//    {
//        abortWithMsg("ERR_OCC_invalid_bandwidth");
//    }
//}

uint8_t dci_lengthOfRIVviaBandwidth(uint8_t Bandwidth, uint8_t LengthBitsRIV)
{
	for (uint8_t i = 0; i < AMOUNT_OF_BANDWIDTHS; ++i)
	{
		if (Bandwidth == dciBandwidth[i])
		{
			LengthBitsRIV = possibleLengthBitsOfRIV[i];
			return LengthBitsRIV;
		}
		else
		{
			fprintf(stdout, "ERR_OCC_Typed_bandwidth_is_wrong");
			return 0;
		}
	}
}

uint8_t dci1_lengthOfBitmapViaBandwidth(uint8_t Bandwidth, uint8_t bitmapBitLenght)
{
	for (uint8_t i = 0; i < AMOUNT_OF_BANDWIDTHS; ++i)
	{
		if (Bandwidth == dciBandwidth[i])
		{
			bitmapBitLenght = possibleLengthBitsRBG[i];
			return bitmapBitLenght;
		}
		else
		{
			fprintf(stdout, "ERR_OCC_Typed_bandwidth_is_wrong");
			return 0;
		}
	}
}

void dci_defineDci(const int argc, const char* const argv[], dciType* restrict const  dci_p,
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

uint8_t dci_readStdin(uint64_t dci_readArgumentsStdin)
{
	uint8_t val = scanf("%li", &dci_readArgumentsStdin);
	return val;
}


void dci_print(char* output /*?*/)
{
	fprintf(stdout, "%s", output);
}

static uint64_t ipow(uint64_t base, uint8_t n)
{
    uint64_t result = 1;
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

uint32_t* dci_readValueFromDCI (uint64_t dci, const uint8_t bitLenghtOfDciParameter[], const uint8_t sizeOfArray)
{
	uint8_t bitLenghtOfDCI = 0;
	for (uint8_t i = 0; i < sizeOfArray; i++)
	{
		bitLenghtOfDCI += bitLenghtOfDciParameter[i];
	}

	uint32_t* outputArray = malloc(sizeof(*outputArray)*sizeOfArray);
//	uint8_t alignedToRight = CHAR_BIT-(bitLenghtOfDCI%CHAR_BIT);
//	if (alignedToRight != CHAR_BIT)
//	{
//		dci >>= alignedToRight;
//	}
	for (uint8_t i = 0; i < sizeOfArray; i++)
	{
		outputArray[i] = dci & createMask(bitLenghtOfDciParameter[sizeOfArray - i - 1]);
		dci >>= bitLenghtOfDciParameter[sizeOfArray - i - 1] - 1;
	}
	return outputArray;
}



void dci0_CorrectnessParameters(uint8_t* dciParam, const uint8_t dciBandwidthPRB)
{
	if (dciParam[paramFirstPRB0] >= dciBandwidthPRB)
	{
		fprintf(stdout, "ERR_OCC_Value_of_FirstPRB_is_too_big\n");
	}

	if (dciParam[paramFirstPRB0] > dciParam[paramLastPRB0] || dciParam[paramFirstPRB0] >= dciBandwidthPRB)
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

	if (dciParam[paramRV1] > MAX_RV1)
	{
		fprintf(stdout, "ERR_OCC_Inncorrect_value_of_RV_parametr\n");
	}

	if (dciParam[paramTPC1] > MAX_TPC)
	{
		fprintf(stdout, "ERR_OCC_Inncorrect_value_of_TPC_parametr\n");
	}
}

void dci60a_CorrectnessParameters(uint8_t* dciParam, const uint8_t dciBandwidthPRB)
{
	if (dciParam[paramFirstPRB60a] >= dciBandwidthPRB)
	{
		fprintf(stdout, "ERR_OCC_Value_of_FirstPRB_is_too_big\n");
	}

	if (dciParam[paramFirstPRB60a] > dciParam[paramLastPRB60a] || dciParam[paramFirstPRB60a] >= dciBandwidthPRB)
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

	if (dciParam[paramRV60a] > MAX_RV60A)
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
