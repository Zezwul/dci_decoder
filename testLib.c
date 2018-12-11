#include <criterion/criterion.h>
#include <stdio.h>
#include "dciCommon.h"

Test(libTest,dci_readValueFromDCITest)
{
	uint64_t dci = 0x7FFFFFFFFFFFFFEF;
	uint8_t sizeOfShiftArray = 4;
	uint8_t shiftArray[] = {1, 7, 4, 4};
	uint32_t* output = dci_readValueFromDCI (dci, shiftArray, sizeOfShiftArray);

	cr_assert(output[0] == 15, "dci_readValueFromDCI working fking bad");
	cr_assert(output[1] == 13, "dci_readValueFromDCI working fking bad");
	cr_assert(output[2] == 127, "dci_readValueFromDCI working fking bad");
	cr_assert(output[3] == 1, "dci_readValueFromDCI working fking bad");
}
