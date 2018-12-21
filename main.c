#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "dciCommon.h"

int main(const int argc, const char* argv[])
{
	uint64_t inputArguments;
	dciType dci_Result;
	uint32_t dci_bandwidthPRB;
	bandwidth_t dci_bandwidth;

	dci_bandwidth = dci_defineDci(argc, argv, &dci_Result, &dci_bandwidthPRB);
	dci_readStdin(&inputArguments);

	switch (dci_Result)
	{
	case dci0:
	{
		uint32_t dci0_offsetArray[DCI0_NUMBER_PARAM] =
		{ FORMAT_FLAG, HOPPING_FLAG, RIV_LEN, MCS, NDI, TPC, DMRS, CSIR, SRSR };
		dci0_offsetArray [dci0_rivOutput] = dci0_lengthOfRIVviaBandwidth(dci_bandwidth);

		const uint32_t dci0_shiftOrigin = 31;

		uint32_t* readValueFromDci0 = malloc(sizeof(uint32_t) * dci0_maxAmmountOfArguments);
		readValueFromDci0 = dci_readValueFromDCI(inputArguments, dci0_offsetArray,
				DCI0_NUMBER_PARAM, dci0_shiftOrigin);
		uint32_t* outputArray = malloc(sizeof(uint32_t) * dci0_maxAmountOfArgumentsOutput);
		dci_rivDecode(dci_bandwidthPRB, readValueFromDci0[dci0_rivOutput],
				&outputArray[dci0_firstPRBOutput], &outputArray[dci0_lastPRBOutput]);

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
		break;
	}
	case dci1:
	{
		uint32_t dci1_offsetArray[DCI1_NUMBER_PARAM] = {RA, BITMAP_LEN, MCS, HARQ, NDI, RV, TPC};
		dci1_offsetArray[dci1_bitmap] = dci1_lengthOfBitmapViaBandwidth(dci_bandwidth);
		uint32_t dci1_shiftOrigin = dci1_calculateShiftOrigin(dci1_offsetArray);

		uint32_t* readValueFromDci1 = dci_readValueFromDCI(inputArguments, dci1_offsetArray,
				DCI1_NUMBER_PARAM, dci1_shiftOrigin);

		uint32_t* outputRBGIndex =  dci1_bitmapDecoder(readValueFromDci1[dci1_bitmap], dci1_offsetArray[dci1_bitmap]);

		dci1_printResults(readValueFromDci1, outputRBGIndex);
		break;
	}
	case dci60a:
	{
		uint32_t dci60a_offsetArray[DCI60A_NUMBER_PARAM] = { RIV_LEN, MCS60A, PUSCH, HARQ, NDI, RV,
				TPC, CSIR, SRSR, PDCCH };
		const uint32_t dci60a_shiftOrigin = 31;
		break;
	}
	default:
		break;
	}
	return 0;
}
