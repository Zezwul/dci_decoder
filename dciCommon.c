#include "dciCommon.h"
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <math.h>
#include <string.h>

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
#define DCI60A_MAX_NUMBER_OF_ALLOCATED_RBS 6
#define MAX_NUMBER_OF_AVAIBLE_FIRST_PRBS 16

const char* const dciStrArguments[] = {"dci0", "dci1","dci60a"};

uint32_t dciBandwidth[AMOUNT_OF_BANDWIDTHS] = {1, 3, 5, 10, 15, 20};

static const uint32_t dciBandwidthToPrb[AMOUNT_OF_BANDWIDTHS] = {6, 15, 25, 50, 75, 100};

static const uint32_t dci60a_avaibleFirstPrbs[AMOUNT_OF_BANDWIDTHS][MAX_NUMBER_OF_AVAIBLE_FIRST_PRBS] = {
		{0}, {1, 8}, {0, 5, 13, 19},
		{1, 7, 13, 19, 25, 31, 37, 43}, {1, 7, 13, 19, 25, 31, 38, 44, 50, 56, 62, 68},
		{2, 8, 14, 20, 26, 32, 38, 44, 50, 56, 62, 68, 74, 80, 86, 92}};

static const uint32_t dci60a_lenghtOfFirstPrbsArrays[] = {1, 2, 4, 8, 12, 16};

uint32_t dci0_lengthOfRIVviaBandwidth(uint32_t bandwidth)
{
    uint32_t possibleLengthBitsOfRIV[AMOUNT_OF_BANDWIDTHS] = {5, 7, 9, 11, 12 ,13};
    return possibleLengthBitsOfRIV[bandwidth];
}

uint32_t dci1_lengthOfBitmapViaBandwidth(bandwidth_t bandwidth)
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
	return (uint32_t)scanf("%llx", dci_readArgumentsStdin);
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

uint32_t* dci_readValueFromDCI(uint64_t dci, uint32_t* bitLenghtOfDciParameter, uint32_t sizeOfArray)
{
	uint32_t* outputArray = malloc(sizeof(*outputArray)*sizeOfArray);
	for (uint8_t i = 0; i < sizeOfArray; i++)
	{
		outputArray[i] = (uint32_t)(dci & createMask(bitLenghtOfDciParameter[sizeOfArray - i - 1]));
		dci >>= bitLenghtOfDciParameter[sizeOfArray - i - 1];
	}
	return outputArray;
}

void dci0_CorrectnessParameters(uint8_t* dciParam, const uint8_t dci0_bandwidthPRB)
{
	uint32_t errorCounter = 0;

	if (dciParam[dci0_firstPRBOutput] >= dci0_bandwidthPRB)
	{
		fprintf(stdout, "ERR_OCC_Value_of_FirstPRB_is_too_big\n");
		errorCounter++;
	}

	if (dciParam[dci0_firstPRBOutput] > dciParam[dci0_lastPRBOutput] || dciParam[dci0_lastPRBOutput] >= dci0_bandwidthPRB)
	{
		fprintf(stdout, "ERR_OCC_Inncorrect_value_of_PRB\n");
		errorCounter++;
	}

	if (dciParam[dci0_mcsindexOutput] > MAX_MCS)
	{
		fprintf(stdout, "ERR_OCC_Inncorrect_value_of_MCS_parametr\n");
		errorCounter++;
	}

	if (dciParam[dci0_newDataIndicatorOutput] > MAX_NDI)
	{
		fprintf(stdout, "ERR_OCC_Inncorrect_value of_NDI_parametr\n");
		errorCounter++;
	}

	if (dciParam[dci0_pucchTpcCommandOutput] > MAX_TPC)
	{
		fprintf(stdout, "ERR_OCC_Inncorrect_value of_TPC_parametr\n");
		errorCounter++;
	}

	if (dciParam[dci0_dmrsCyclicOutput] > MAX_DMRS)
	{
		fprintf(stdout, "ERR_OCC_Inncorrect_value of_DMRS_parametr\n");
		errorCounter++;
	}

	if (dciParam[dci0_csiRqstOutput] > MAX_CSI_REQ)
	{
		fprintf(stdout, "ERR_OCC_Inncorrect_value of_CSIreq_parametr\n");
		errorCounter++;
	}

	if (dciParam[dci0_srsRqstOutput] > MAX_SRS_REQ)
	{
		fprintf(stdout, "ERR_OCC_Inncorrect_value of_SRSreq_parametr\n");
		errorCounter++;
	}

	if (errorCounter > 0)
	{
		exit(1);
	}
}

void dci1_CorrectnessParameters(uint8_t* dciParam)
{
	uint32_t errorCounter = 0;
	if (dciParam[dci1_mcsindex] > MAX_MCS)
	{
		fprintf(stdout, "ERR_OCC_Inncorrect_value of_MCS_parametr\n");
		errorCounter++;
	}

	if (dciParam[dci1_harq] > MAX_HARQ)
	{
		fprintf(stdout, "ERR_OCC_Inncorrect_value_of_HARQ_parametr\n");
		errorCounter++;
	}

	if (dciParam[dci1_newDataIndicator] > MAX_NDI)
	{
		fprintf(stdout, "ERR_OCC_Inncorrect_value_of_NDI_parametr\n");
		errorCounter++;
	}

	if (dciParam[dci1_redundancyVersion] > MAX_RV)
	{
		fprintf(stdout, "ERR_OCC_Inncorrect_value_of_RV_parametr\n");
		errorCounter++;
	}

	if (dciParam[dci1_pucchTpcCommand] > MAX_TPC)
	{
		fprintf(stdout, "ERR_OCC_Inncorrect_value_of_TPC_parametr\n");
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
	for (bandwidth_t i = BW_1_4MHz; i < AMOUNT_OF_BANDWIDTHS; i++)
	{
		if (dci60a_bandwidthPRB == dciBandwidthToPrb[i])
		{
			for (size_t j = 0; j < dci60a_lenghtOfFirstPrbsArrays[i]; j++ )
			{
				if (!(dci60a_avaibleFirstPrbs[i][j] == dciParam[dci0_firstPRBOutput] &&
						dci60a_avaibleFirstPrbs[i][j] + DCI60A_MAX_NUMBER_OF_ALLOCATED_RBS - 1 == dciParam[dci0_lastPRBOutput]))
				{
					fprintf (stdout, "ERR_OCC_FirstPRB_or_LastPRB_is_not_avaible_for_CAT-M_cells\n");
					errorCounter++;
				}
			}
		}
	}
	if (dciParam[dci60a_MCSoutput] > MAX_MCS)
	{
		fprintf(stdout, "ERR_OCC_Inncorrect_value of_MCS_parametr\n");
		errorCounter++;
	}

	if (dciParam[dci60a_PUSCHoutput] > MAX_PUSCH)
	{
		fprintf(stdout, "ERR_OCC_Inncorrect_value of_PUSCH_parametr\n");
		errorCounter++;
	}

	if (dciParam[dci60a_HARQoutput] > MAX_HARQ)
	{
		fprintf(stdout, "ERR_OCC_Inncorrect_value of_HARQ_parametr\n");
		errorCounter++;
	}

	if (dciParam[dci60a_NDIoutput] > MAX_NDI)
	{
		fprintf(stdout, "ERR_OCC_Inncorrect_value of_NDI_parametr\n");
		errorCounter++;
	}
	
	if (dciParam[dci60a_RVoutput] > MAX_RV)
	{
		fprintf(stdout, "ERR_OCC_Inncorrect_value of_RV_parametr\n");
		errorCounter++;
	}

	if (dciParam[dci60a_TPCoutput] > MAX_TPC)
	{
		fprintf(stdout, "ERR_OCC_Inncorrect_value of_TPC_parametr\n");
		errorCounter++;
	}

	if (dciParam[dci60a_CSIreqOutput] > MAX_CSI_REQ)
	{
		fprintf(stdout, "ERR_OCC_Inncorrect_value of_CSIreq_parametr\n");
		errorCounter++;
	}

	if (dciParam[dci60a_RSreqOutput] > MAX_SRS_REQ)
	{
		fprintf(stdout, "ERR_OCC_Inncorrect_value of_SRSreq_parametr\n");
		errorCounter++;
	}

	if (dciParam[dci60a_PDCCHOutput] > MAX_PDCCH)
	{
		fprintf(stdout, "ERR_OCC_Inncorrect_value of_PDCCH_parametr\n");
		errorCounter++;
	}

	if (errorCounter > 0)
	{
		exit(1);
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
