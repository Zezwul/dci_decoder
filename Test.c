#include <criterion/criterion.h>
#include <stdint.h>
#include <stdio.h>
#include "dciCommon.h"

static uint16_t resourceAllocationRIV(const uint8_t amountOfPRBs, uint8_t firstPRB, uint8_t lastPRB)
{
	if (firstPRB > lastPRB)
	{
		return 0;
	}

	uint16_t riv;
	const uint8_t PRBLength = lastPRB - firstPRB + 1;
	if ((PRBLength - 1) <= (amountOfPRBs / 2))
	{
		riv = amountOfPRBs * (PRBLength - 1) + firstPRB;
	}
	else
	{
		riv = amountOfPRBs * (amountOfPRBs - PRBLength + 1) + (amountOfPRBs - 1 - firstPRB);
	}
	return riv;
}

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
		dci_defineDci(3, args_in[i], &dciResult, &bandwidthPRB);
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
		dci_defineDci(3, args_in[i], &dciResult, &bandwidthPRB);
		cr_expect_eq(bandwidthPRB, dci_bitLengthArray_expected[i],"Error");
		cr_expect_eq(dciResult, dci_str_expected[i],"Error");
	}

	for (size_t i = 0; i < 10; i++)
	{
		dci_defineDci(9999, args_in[i], &dciResult, &bandwidthPRB);
		cr_expect_eq(bandwidthPRB, 100,"Error");
		cr_expect_eq(dciResult, dci0,"Error");
	}
}

//Test(libTest,dci_readValueFromDCITest)
//{
//	uint64_t dci = 0x7FFFFFFFFFFFFFEF;
//	uint8_t sizeOfShiftArray = 4;
//	uint8_t shiftArray[] = {1, 7, 4, 4};
//	uint32_t* output = dci_readValueFromDCI (dci, shiftArray, sizeOfShiftArray);
//
//	cr_assert(output[0] == 15, "dci_readValueFromDCI is not working propertly");
//	cr_assert(output[1] == 13, "dci_readValueFromDCI is not working propertly");
//	cr_assert(output[2] == 127, "dci_readValueFromDCI is not working propertly");
//	cr_assert(output[3] == 1, "dci_readValueFromDCI is not working propertly");
//	free(output);
//}

Test(libTest,dci1_bitmapDecoderTest)
{
	uint8_t* dci1_bitmapDecoder(uint32_t bitmap, uint8_t bitmapBitLenght);
	uint32_t bitmap[5] = {13, 25, 9000, 1564, 16777216};
	uint8_t testArray[5][6] = {{3,24,22,21}, {3, 24, 21, 20}, {5, 21, 19, 16, 15, 11}, {5, 22, 21, 20, 15, 14}, {1,0}};
	uint8_t bitmapBitLenght = 25;
	uint8_t* outputBitmap;
	for (uint8_t i = 0; i < 5; i++)
	{
	    outputBitmap = dci1_bitmapDecoder(bitmap[i], bitmapBitLenght);
		for (uint8_t j = 0; j < testArray[i][0]+1; j++)
		{
			cr_expect(testArray[i][j] == outputBitmap[j], "dci1_bitmapDecoder is not working propertly - i: %d and j: %d", i, j);
		}
	    free(outputBitmap);
	}
}

Test(RivTest, dci_rivPositiveValues)
{
	uint8_t outFirstPRB;
	uint8_t outLastPRB;
	uint8_t bandwidthPRB[] = {6, 15, 25, 50, 75, 100};
	uint16_t rivArr[9641];
	uint8_t expectedOutFirstPRB[9641];
	uint8_t expectedOutLastPRB[9641];
	for (size_t i = 0, k = 0, l = 0; i < bandwidthPRB[k]; i++)
	{
		for (size_t j = i; j < bandwidthPRB[k]; j++)
		{
			expectedOutFirstPRB[l] = i;
			expectedOutLastPRB[l] = j;
			l++;
		}
		if(i == bandwidthPRB[k] - 1)
		{
		    i = -1;
			k++;
		}
		if(k == 6)
		{
		    break;
		}
	}

	uint16_t moveBit[] = {21, 141, 466, 1741, 4591, 9641};
	for (size_t i = 0, j = 0; i < 9641 ; i++)
	{
		rivArr[i] = resourceAllocationRIV(bandwidthPRB[j],expectedOutFirstPRB[i], expectedOutLastPRB[i]);
		if(i == moveBit[j])
		{
			j++;
		}
	}
	for (size_t i = 0, j = 0; i < 9641; i++)
	{
		dci_rivDecode ( bandwidthPRB[j], rivArr[i], &outFirstPRB, &outLastPRB);
		cr_expect_eq(outFirstPRB, expectedOutFirstPRB[i],"Error");
		cr_expect_eq(outLastPRB, expectedOutLastPRB[i],"Error");
		if(i == moveBit[j])
		{
			j++;
		}
	}
}
