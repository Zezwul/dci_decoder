#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "dciCommon.h"

int main(const int argc, const char* argv[])
{
	dci_defineDci(argc, argv, &dci_Result, &dci_bandwidthPRB);
	dci_readStdin();

	switch (dci_Result)
	{
	case dci0:
//		dci_readValueFromDCI();
//		dci0_validateStdinParameters(&dci_tempParameters);
		break;
	case dci1:
//		dci_readValueFromDCI();
//		dci0_validateStdinParameters(&dci_tempParameters);
		break;
	case dci60a:
//		dci_readValueFromDCI();
//		dci0_validateStdinParameters(&dci_tempParameters);
		break;
	default:
		break;
	}

	dci_print(dci_output);
}
