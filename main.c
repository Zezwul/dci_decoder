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

		uint32_t* readValueFromDci0 = dci_readValueFromDCI(inputArguments, dci0_offsetArray,
				DCI0_NUMBER_PARAM, dci0_shiftOrigin);

		dci0_CorrectnessParameters(readValueFromDci0, dci_bandwidthPRB);

		uint32_t* outputArray = malloc(sizeof(uint32_t) * dci0_maxAmountOfArgumentsOutput);
		dci_rivDecode(dci_bandwidthPRB, readValueFromDci0[dci0_rivOutput],
				&outputArray[dci0_firstPRBOutput], &outputArray[dci0_lastPRBOutput]);
		dci0_printResultsAndFreeArrays(readValueFromDci0, outputArray);
		break;
	}
	case dci1:
	{
		uint32_t dci1_offsetArray[DCI1_NUMBER_PARAM] = {RA, BITMAP_LEN, MCS, HARQ, NDI, RV, TPC};
		dci1_offsetArray[dci1_bitmapValue] = dci1_lengthOfBitmapViaBandwidth(dci_bandwidth);
		uint32_t dci1_shiftOrigin = dci1_calculateShiftOrigin(dci1_offsetArray);
		uint32_t* readValueFromDci1 = dci_readValueFromDCI(inputArguments, dci1_offsetArray,
				DCI1_NUMBER_PARAM, dci1_shiftOrigin);

		dci1_CorrectnessParameters(readValueFromDci1);

		uint32_t* outputRBGIndex =  dci1_DecodeValuesFromBitmap(readValueFromDci1[dci1_bitmapValue], dci1_offsetArray[dci1_bitmapValue]);
		dci1_printResults(readValueFromDci1, outputRBGIndex);
		break;
	}
	case dci60a:
	{
		uint32_t dci60a_offsetArray[DCI60A_NUMBER_PARAM] = { FORMAT_FLAG, NARROWBAND_INDEX, DCI60A_RIV_LEN,
				MCS60A, PUSCH, HARQ, NDI, RV, TPC, CSIR, SRSR, PDCCH };
		const uint32_t dci60a_shiftOrigin = 31;

		uint32_t* readValueDCI;
		readValueDCI = dci_readValueFromDCI(inputArguments, dci60a_offsetArray, DCI60A_NUMBER_PARAM, dci60a_shiftOrigin);
		uint32_t* outputArray = malloc(sizeof(uint32_t) * dci60a_maxAmountOfArgumentsOutput);
		dci_rivDecode(dci_Result, 6, readValueDCI[dci60a_rivLength],
				&outputArray[dci60a_FirstPRBoutput], &outputArray[dci60a_LastPRBoutput]);
		printf("F: %d, L: %d", outputArray[dci60a_FirstPRBoutput, outputArray[dci60a_LastPRBoutput]]);
		dci60a_prbsNumberDecoder(dci_bandwidthPRB, readValueDCI, outputArray);
		dci60a_OutputParameters outputElement = dci60a_MCSoutput;
		dci60a_InputParameters inputElement = dci60a_MCS;

		for ( ; outputElement < dci60a_maxAmountOfArgumentsOutput;
				outputElement++, inputElement++)
		{
			outputArray[outputElement] = readValueDCI[inputElement];
		}

		for (dci60a_OutputParameters i = dci60a_FirstPRBoutput;
				i < dci60a_maxAmountOfArgumentsOutput; ++i)
		{
			fprintf(stdout, "%u ", outputArray[i]);
		}
		fprintf(stdout, "\n");

		free(readValueDCI);
		free(outputArray);

		break;
	}
	default:
		break;
	}
	return 0;
}
