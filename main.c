#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "dciCommon.h"

int main(const int argc, const char* argv[])
{
	uint64_t inputArguments;
	dciType dci_Result;
	uint8_t dci_bandwidthPRB;
	bandwidth_t dci_bandwidth;

	dci_bandwidth = dci_defineDci(argc, argv, &dci_Result, &dci_bandwidthPRB);
	dci_readStdin(&inputArguments);

	switch (dci_Result)
	{
	case dci0:
	{
        uint32_t dci0_offsetArray[DCI0_NUMBER_PARAM] =
        { FORMAT_FLAG, HOPPING_FLAG, RIV, MCS, NDI, TPC, DMRS, CSIR, SRSR };

		break;
	}
	case dci1:
	{
	    uint32_t dci1_offsetArray[DCI1_NUMBER_PARAM] = {RA, BITMAP_LEN, MCS, HARQ, NDI, RV, TPC};

		break;
	}
	case dci60a:
	{
	    uint32_t dci60a_offsetArray[DCI60A_NUMBER_PARAM] = { RIV_LEN, MCS60A, PUSCH, HARQ, NDI, RV,
	            TPC, CSIR, SRSR, PDCCH };

		break;
	}
	default:
		break;
	}

}
