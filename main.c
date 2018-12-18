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

		break;
	}
	case dci1:
	{

		break;
	}
	case dci60a:
	{

		break;
	}
	default:
		break;
	}

}
