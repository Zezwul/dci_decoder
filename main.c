#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "dciCommon.h"

int main(const int argc, const char* argv[])
{
	uint32_t bandwidth = atoi(argv[2]);
	uint64_t inputArguments;
	dciType dci_Result;
	uint8_t* dci_bandwidthPRB = NULL;

	dci_defineDci(argc, argv, &dci_Result, dci_bandwidthPRB);
	dci_readStdin(&inputArguments);

	switch (dci_Result)
	{
	case dci0:
		//dci_readValueFromDCI(inputArguments, )
		break;
	case dci1:

		break;
	case dci60a:

		break;
	default:
		break;
	}

	//dci_print(dci_output);
}
