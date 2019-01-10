#include "dciCommon.h"
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <math.h>
#include <string.h>
#define DEBUG
#include "debug.h"

/* Maximum values of parameters  */
#define MAX_MCS 31
#define MAX_MCS60A 15
#define MAX_HARQ 7
#define MAX_NDI 1
#define MAX_RV 3
#define MAX_TPC 3
#define MAX_DMRS 7
#define MAX_CSI_REQ 1
#define MAX_SRS_REQ 1
#define MAX_PUSCH 3
#define MAX_PDCCH 3
#define DCI60A_MAX_NUMBER_OF_LAST_PRB 5
#define DCI60A_MAX_NUMBER_OF_ALLOCATED_RBS 6
#define PRBS_IN_ONE_NARROWBAND 6

#define MOD_8_MASK 0x7

#define FREE(ptr)\
do { \
    free(ptr);\
    ptr = NULL;\
   } while(0)

const char* const dciStrArguments[] = {"dci0", "dci1","dci60a"};

uint32_t dciBandwidth[AMOUNT_OF_BANDWIDTHS] = {1, 3, 5, 10, 15, 20};

static const uint32_t dciBandwidthToPrb[AMOUNT_OF_BANDWIDTHS] = {6, 15, 25, 50, 75, 100};

uint32_t dci1_calculateShiftOrigin(uint32_t* shiftArray)
{
    uint32_t numberOfSignificantBits = 0;
    for (dci1_OutputParameters i = dci1_raTypeValue; i < dci1_maxAmountOfArgumentsValue; i++)
    {
        numberOfSignificantBits += shiftArray[i];
    }

    uint32_t temp = numberOfSignificantBits;
    temp /= CHAR_BIT;
    if (numberOfSignificantBits & MOD_8_MASK)
    {
        temp++;
    }

    return temp * CHAR_BIT - 1;
}

void dci0_printResultsAndFreeArrays(uint32_t* readValueFromDci0, uint32_t* outputArray)
{
    dci0_OutputParameters outputElementIndex = dci0_mcsindexOutput;
    dci0_InputParameters inputElementIndex = dci0_mcsindex;

    for ( ; outputElementIndex < dci0_maxAmountOfArgumentsOutput;
            outputElementIndex++, inputElementIndex++)
    {
        outputArray[outputElementIndex] = readValueFromDci0[inputElementIndex];
    }

    for (dci0_OutputParameters i = dci0_firstPRBOutput;
            i < dci0_maxAmountOfArgumentsOutput; ++i)
    {
        fprintf(stdout, "%u ", outputArray[i]);
    }

    fprintf(stdout, "\n");

    FREE(readValueFromDci0);
    FREE(outputArray);
}

void dci1_printResults(uint32_t* readValueFromDci1, uint32_t* outputRBGIndex)
{
	for (dci1_OutputParameters i = dci1_bitmapValue; i < dci1_maxAmountOfArgumentsValue; i++)
	{
		if (i == dci1_bitmapValue)
		{
			for (uint32_t bitmapIndex = 0; bitmapIndex < outputRBGIndex[0] + 1; bitmapIndex++)
			{
			fprintf(stdout, "%u ", outputRBGIndex[bitmapIndex]);
			}
		}
		else
		fprintf(stdout, "%u ", readValueFromDci1[i]);
	}
	fprintf(stdout,"\n");
	FREE(readValueFromDci1);
	FREE(outputRBGIndex);
}

uint32_t dci0_lengthOfRIVviaBandwidth(uint32_t bandwidth)
{
	uint32_t possibleLengthBitsOfRIV[AMOUNT_OF_BANDWIDTHS] = {5, 7, 9, 11, 12 ,13};
	return possibleLengthBitsOfRIV[bandwidth];
}

uint32_t dcli1_lengthOfBitmapViaBandwidth(bandwidth_t bandwidth)
{
	uint32_t possibleLengthBitsRBG[AMOUNT_OF_BANDWIDTHS] = {6, 8, 13, 17, 19 ,25};
	return possibleLengthBitsRBG[bandwidth];
}

bandwidth_t dci_defineDci(const int argc, const char* const argv[], dciType* restrict const dci_p,
		uint32_t* restrict prb_p)
{
	uint32_t dciBandwidthPRB[AMOUNT_OF_BANDWIDTHS] = {6, 15, 25, 50, 75, 100};
	dciType dciResult = dci0;
	bandwidth_t dci_bandwidth = BW_20MHz;
	uint32_t bandwidthPRB = 100;
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
		for (bandwidth_t i = BW_1_4MHz; i < AMOUNT_OF_BANDWIDTHS; ++i)
		{
			if ((uint32_t)atoi(argv[2]) == dciBandwidth[i])
			{
				bandwidthPRB = dciBandwidthPRB[i];
				dci_bandwidth = i;
				break;
			}
		}
	}
	*prb_p = bandwidthPRB;
	*dci_p = dciResult;
	return dci_bandwidth;
}

uint32_t dci_readStdin(uint64_t *dci_readArgumentsStdin)
{
	return (uint32_t)scanf("%"SCNu64, dci_readArgumentsStdin);
}

void dci_print(char* output)
{
	fprintf(stdout, "%s", output);
}

static uint32_t getBits(uint64_t dciToRead, uint32_t startRead, uint32_t shift)
{
	uint64_t shiftedDci = (dciToRead >> (startRead + 1 - shift));
	uint64_t mask = (uint64_t)(~((uint)~0 << shift));
	uint64_t bits = shiftedDci & mask;

	return (uint32_t)bits;
}

uint32_t* dci_readValueFromDCI(uint64_t dci, uint32_t* bitLenghtOfDciParameter,
		uint32_t sizeOfArray, uint32_t startingPoint)
{
	uint32_t* outputArray = malloc(sizeof(*outputArray)*sizeOfArray);
	for (uint8_t i = 0; i < sizeOfArray; i++)
	{
		outputArray[i] = getBits(dci, startingPoint, bitLenghtOfDciParameter[i]);
		startingPoint -= bitLenghtOfDciParameter[i];
	}
	return outputArray;
}

void dci0_CorrectnessParameters(uint32_t* dciParam, const uint32_t dci0_bandwidthPRB)
{
	uint32_t errorCounter = 0;

	if (dciParam[dci0_mcsindex] > MAX_MCS)
	{
		DEBUG_PRINT("ERR_OCC_Inncorrect_value_of_MCS_parametr\n");
		errorCounter++;
	}

	if (dciParam[dci0_newDataIndicator] > MAX_NDI)
	{
		printf("ndi: %i\n", dciParam[dci0_newDataIndicator]);
		DEBUG_PRINT("ERR_OCC_Inncorrect_value of_NDI_parametr\n");
		errorCounter++;
	}

	if (dciParam[dci0_pucchTpcCommand] > MAX_TPC)
	{
		DEBUG_PRINT("ERR_OCC_Inncorrect_value of_TPC_parametr\n");
		errorCounter++;
	}

	if (dciParam[dci0_dmrsCyclic] > MAX_DMRS)
	{
		DEBUG_PRINT("ERR_OCC_Inncorrect_value of_DMRS_parametr\n");
		errorCounter++;
	}

	if (dciParam[dci0_csiRqst] > MAX_CSI_REQ)
	{
		DEBUG_PRINT("ERR_OCC_Inncorrect_value of_CSIreq_parametr\n");
		errorCounter++;
	}

	if (dciParam[dci0_srsRqst] > MAX_SRS_REQ)
	{
		DEBUG_PRINT("ERR_OCC_Inncorrect_value of_SRSreq_parametr\n");
		errorCounter++;
	}

	if (errorCounter > 0)
	{
		exit(1);
	}
}

void dci1_CorrectnessParameters(uint32_t* dciParam)
{
	uint32_t errorCounter = 0;
	if (dciParam[dci1_mcsindexValue] > MAX_MCS)
	{
		DEBUG_PRINT("ERR_OCC_Inncorrect_value of_MCS_parametr\n");
		errorCounter++;
	}

	if (dciParam[dci1_harqValue] > MAX_HARQ)
	{
		DEBUG_PRINT("ERR_OCC_Inncorrect_value_of_HARQ_parametr\n");
		errorCounter++;
	}

	if (dciParam[dci1_newDataIndicatorValue] > MAX_NDI)
	{
		DEBUG_PRINT("ERR_OCC_Inncorrect_value_of_NDI_parametr\n");
		errorCounter++;
	}

	if (dciParam[dci1_redundancyVersionValue] > MAX_RV)
	{
		DEBUG_PRINT("ERR_OCC_Inncorrect_value_of_RV_parametr\n");
		errorCounter++;
	}

	if (dciParam[dci1_pucchTpcCommandValue] > MAX_TPC)
	{
		DEBUG_PRINT("ERR_OCC_Inncorrect_value_of_TPC_parametr\n");
		errorCounter++;
	}

	if (errorCounter > 0)
	{
		exit(1);
	}
}

void dci60a_CorrectnessParameters(uint8_t* dciParam, const uint8_t dci60a_bandwidthPRB)
{
	uint32_t errorCounter = 0;

	if (dciParam[dci60a_MCSoutput] > MAX_MCS)
	{
		DEBUG_PRINT("ERR_OCC_Inncorrect_value of_MCS_parametr\n");
		errorCounter++;
	}

	if (dciParam[dci60a_PUSCHoutput] > MAX_PUSCH)
	{
		DEBUG_PRINT("ERR_OCC_Inncorrect_value of_PUSCH_parametr\n");
		errorCounter++;
	}

	if (dciParam[dci60a_HARQoutput] > MAX_HARQ)
	{
		DEBUG_PRINT("ERR_OCC_Inncorrect_value of_HARQ_parametr\n");
		errorCounter++;
	}

	if (dciParam[dci60a_NDIoutput] > MAX_NDI)
	{
		DEBUG_PRINT("ERR_OCC_Inncorrect_value of_NDI_parametr\n");
		errorCounter++;
	}

	if (dciParam[dci60a_RVoutput] > MAX_RV)
	{
		DEBUG_PRINT("ERR_OCC_Inncorrect_value of_RV_parametr\n");
		errorCounter++;
	}

	if (dciParam[dci60a_TPCoutput] > MAX_TPC)
	{

		DEBUG_PRINT("ERR_OCC_Inncorrect_value of_TPC_parametr\n");
		errorCounter++;
	}

	if (dciParam[dci60a_CSIreqOutput] > MAX_CSI_REQ)
	{

		DEBUG_PRINT("ERR_OCC_Inncorrect_value of_CSIreq_parametr\n");
		errorCounter++;
	}

	if (dciParam[dci60a_SRSreqOutput] > MAX_SRS_REQ)
	{
		DEBUG_PRINT("ERR_OCC_Inncorrect_value of_SRSreq_parametr\n");
		errorCounter++;
	}

	if (dciParam[dci60a_PDCCHOutput] > MAX_PDCCH)
	{
		DEBUG_PRINT("ERR_OCC_Inncorrect_value of_PDCCH_parametr\n");
		errorCounter++;
	}

	if (errorCounter > 0)
	{
		exit(1);
	}
}

uint16_t dci_rivDecode(dciType dci, uint32_t bandwidthPRB, uint32_t riv,
		uint32_t* restrict outFirstPRB, uint32_t* restrict outLastPRB)
{
	if (outFirstPRB == NULL || outLastPRB == NULL)
	{
		DEBUG_PRINT("ERR_OCC_invalid_pointers");
		return UINT16_MAX;
	}
	uint32_t PRBFirst = 0;
	uint32_t PRBLength = 0;
	uint32_t PRBLast = 0;

	if (dci == dci60a)
	{
		PRBFirst = (riv % bandwidthPRB);
		PRBLength = (riv / bandwidthPRB + 1);
		PRBLast = PRBFirst + PRBLength - 1;
		if (!(PRBLast < DCI60A_MAX_NUMBER_OF_LAST_PRB))
		{
			PRBFirst = (uint32_t)(bandwidthPRB - 1 - PRBFirst);
			PRBLength = (uint32_t)(bandwidthPRB + 1 - PRBLength + 1);
			PRBLast = PRBFirst + PRBLength - 1;
			if (PRBLast > DCI60A_MAX_NUMBER_OF_LAST_PRB)
			{
				DEBUG_PRINT("ERR_OCC_last_prb_out_of_range");
				return UINT16_MAX;
			}
		}
	}
	else
	{
		PRBFirst = (riv % bandwidthPRB);
		PRBLength = (riv / bandwidthPRB + 1);
		if (PRBFirst + PRBLength > bandwidthPRB)
		{
			PRBFirst = (uint32_t)(bandwidthPRB - 1 - PRBFirst);
			PRBLength = (uint32_t)(bandwidthPRB + 1 - PRBLength + 1);
		}
	}
	*outFirstPRB = PRBFirst;
	*outLastPRB = PRBFirst + PRBLength - 1;
	return 0;
}

uint32_t* dci1_DecodeValuesFromBitmap(uint32_t bitmap, uint32_t bitmapBitLenght)
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

	uint32_t reverseArrCounter = outputRBGIndex[0];
	uint32_t reverseArrCounterTwo = 1;
	while(reverseArrCounter > reverseArrCounterTwo)
	{
		uint32_t temp = outputRBGIndex[reverseArrCounter];
		outputRBGIndex[reverseArrCounter] = outputRBGIndex[reverseArrCounterTwo];
		outputRBGIndex[reverseArrCounterTwo] = temp;
		reverseArrCounter--;
		reverseArrCounterTwo++;
	}
	return outputRBGIndex;
}

void dci60a_prbsNumberDecoder(bandwidth_t dci_bandwitdh, const uint32_t *readFromDci, uint32_t *outputParameters)
{
	const uint32_t prbsToNarrowbandBW1[][PRBS_IN_ONE_NARROWBAND] = {{0, 1, 2, 3, 4, 5}};

	const uint32_t prbsToNarrowbandBW3[][PRBS_IN_ONE_NARROWBAND] = {{1, 2, 3, 4, 5, 6}, {8, 9, 10, 11, 12, 13}};

	const uint32_t prbsToNarrowbandBW5[][PRBS_IN_ONE_NARROWBAND] = {{0, 1, 2, 3, 4, 5}, {6, 7, 8, 9, 10, 11}, {13, 14, 15, 16, 17, 18}, {19, 20, 21, 22, 23, 24}};

	const uint32_t prbsToNarrowbandBW10[][PRBS_IN_ONE_NARROWBAND] = {{1, 2, 3, 4, 5, 6}, {7, 8, 9, 10, 11, 12}, {13, 14, 15, 16, 17, 18}, {19, 20, 21, 22, 23, 24},
			{25, 26, 27, 28, 29, 30}, {31, 32, 33, 34, 35, 36}, {37, 38, 39, 40, 41, 42}, {43, 44, 45, 46, 47, 48}};

	const uint32_t prbsToNarrowbandBW15[][PRBS_IN_ONE_NARROWBAND] = {{1, 2, 3, 4, 5, 6}, {7, 8, 9, 10, 11, 12}, {13, 14, 15, 16, 17, 18}, {19, 20, 21, 22, 23, 24},
			{25, 26, 27, 28, 29, 30}, {31, 32, 33, 34, 35, 36}, {38, 39, 40, 41, 42, 43},  {44, 45, 46, 47, 48, 49}, {50, 51, 52, 53, 54, 55},
			{56, 57, 58, 59, 60, 61}, {62, 63, 64, 65, 66, 67}, {68, 69, 70, 71, 72, 73}};

	const uint32_t prbsToNarrowbandBW20[][PRBS_IN_ONE_NARROWBAND] = {{2, 3, 4, 5, 6, 7}, {8, 9, 10, 11, 12, 13}, {14, 15, 16, 17, 18, 19}, {20, 21, 22, 23, 24, 25},
			{26, 27, 28, 29, 30, 31}, {32, 33, 34, 35, 36, 37}, {38, 39, 40, 41, 42, 43}, {44, 45, 46, 47, 48, 49}, {50, 51, 52, 53, 54, 55}, {56, 57, 58, 59, 60, 61},
			{62, 63, 64, 65, 66, 67}, {68, 69, 70, 71, 72, 73}, {74, 75, 76, 77, 78, 79}, {80, 81, 82, 83, 84, 85}, {86, 87, 88, 89, 90, 91}, {92, 93, 94, 95, 96, 97}};

	const uint32_t *arr[] = {&prbsToNarrowbandBW1[0][0], &prbsToNarrowbandBW3[0][0], &prbsToNarrowbandBW5[0][0], &prbsToNarrowbandBW10[0][0], &prbsToNarrowbandBW15[0][0], &prbsToNarrowbandBW20[0][0]};

	uint32_t (*value)[6] = (uint32_t(*)[6])arr[dci_bandwitdh];
	value[readFromDci[dci60a_narrowbandIndex]][outputParameters[dci60a_FirstPRBoutput]] = outputParameters[dci60a_FirstPRBoutput];
	value[readFromDci[dci60a_narrowbandIndex]][outputParameters[dci60a_LastPRBoutput]] = outputParameters[dci60a_LastPRBoutput];
}
