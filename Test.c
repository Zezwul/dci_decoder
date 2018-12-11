#include <criterion/criterion.h>
#include <stdint.h>
#include <stdio.h>
#include "dciCommon.h"

Test(TestArguments, dci_ValidPositiveArguments)
{
	const char* args_in[][3] = {{"0", "dci0", "1"}, {"0", "dci0", "3"}, {"0", "dci0", "5"}, {"0", "dci0", "10"},
			{"0", "dci0", "15"}, {"0", "dci0", "20"}, {"0", "dci1", "1"}, {"0", "dci1", "3"}, {"0", "dci1", "5"},
			{"0", "dci1", "10"}, {"0", "dci1", "15"}, {"0", "dci1", "20"},{"0", "dci60a", "1"}, {"0", "dci60a", "3"},
			{"0", "dci60a", "5"}, {"0", "dci60a", "10"}, {"0", "dci60a", "15"}, {"0", "dci60a", "20"},};
	uint8_t dci_bitLengthArray_expected[] = {6, 15, 25, 50, 75, 100, 6, 15, 25, 50, 75, 100, 6, 15, 25, 50, 75, 100};
	dciType dci_str_expected[] = {dci0, dci0, dci0, dci0, dci0, dci0, dci1, dci1, dci1,
			dci1, dci1, dci1, dci60a, dci60a, dci60a, dci60a, dci60a, dci60a};

	uint8_t bandwidthPRB;
	dciType dciResult;
	for (size_t i = 0; i < sizeof(dci_str_expected) / sizeof(int); i++)
	{
		dci_DefineDci(3, args_in[i], &dciResult, &bandwidthPRB);
		cr_expect_eq(bandwidthPRB, dci_bitLengthArray_expected[i],"Error");
		cr_expect_eq(dciResult, dci_str_expected[i],"Error");
	}
}

Test(TestArguments, dci_ValidNegativeArguments)
{
	const char* args_in[][3] = {{"0", "ala", "-1"}, {"0", "dci31", "0"}, {"0", "dci21", "2.1"}, {"0", "dci09", "9999999999"},
			{"0", "dci60A", "51"}, {"0", "dci60abc", "33"}, {"0", "!@#$%^&*(", "103"}, {"0", "dci-1", "151"},
			{"0", "dci6size0", "-100000"}, {"0", "0dci", "202"}};
	uint8_t dci_bitLengthArray_expected[] = {100, 100, 100, 100, 100, 100, 100, 100, 100, 100};
	dciType dci_str_expected[] = {dci0, dci0, dci0, dci0, dci0, dci0, dci0, dci0, dci0, dci0 };

	uint8_t bandwidthPRB;
	dciType dciResult;
	for (size_t i = 0; i < 10; i++)
	{
		dci_DefineDci(3, args_in[i], &dciResult, &bandwidthPRB);
		cr_expect_eq(bandwidthPRB, dci_bitLengthArray_expected[i],"Error");
		cr_expect_eq(dciResult, dci_str_expected[i],"Error");
	}

	for (size_t i = 0; i < 10; i++)
	{
		dci_DefineDci(9999, args_in[i], &dciResult, &bandwidthPRB);
		cr_expect_eq(bandwidthPRB, 100,"Error");
		cr_expect_eq(dciResult, dci0,"Error");
	}
}
