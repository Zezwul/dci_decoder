#include <criterion/criterion.h>
#include <stdint.h>
#include <stdio.h>
#include "dciCommon.h"

static uint32_t resourceAllocationRIV(const uint32_t amountOfPRBs,
		uint32_t firstPRB, uint32_t lastPRB)
{
	if (firstPRB > lastPRB)
	{
		return 0;
	}

	uint32_t riv;
	const uint32_t PRBLength = lastPRB - firstPRB + 1;
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

	uint32_t bandwidthPRB;
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

	uint32_t bandwidthPRB;
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

Test(libTest,dci_readValueFromDCITest)
{
	uint64_t dci = 0x2420001B74;
	uint32_t dci1_offsetArray[DCI1_NUMBER_PARAM] = {RA, BITMAP_LEN, MCS, HARQ, NDI, RV, TPC};
	uint32_t* output = dci_readValueFromDCI (dci, dci1_offsetArray,
	        DCI1_NUMBER_PARAM, dci1_calculateShiftOrigin(dci1_offsetArray));

	cr_assert(output[0] == 0, "Expected %d, got %d", 0, output[0]);
	cr_assert(output[1] == 9469952, "Expected %d, got %d", 9469952, output[1]);
	cr_assert(output[2] == 13, "Expected %d, got %d", 13, output[2]);
	cr_assert(output[3] == 5, "Expected %d, got %d", 5, output[3]);
	free(output);
}

Test(libTest,dci1_bitmapDecoderTest)
{
	uint32_t bitmap[5] = {13, 25, 9000, 1564, 16777216};
	uint32_t testArray[5][6] = {{3,24,22,21}, {3, 24, 21, 20}, {5, 21, 19, 16, 15, 11}, {5, 22, 21, 20, 15, 14}, {1,0}};
	uint32_t bitmapBitLenght = 25;
	uint32_t* outputBitmap;
	for (uint32_t i = 0; i < 5; i++)
	{
	    outputBitmap = dci1_bitmapDecoder(bitmap[i], bitmapBitLenght);
		for (uint32_t j = 0; j < testArray[i][0]+1; j++)
		{
			cr_expect(testArray[i][j] == outputBitmap[j], "dci1_bitmapDecoder is not working propertly - i: %d and j: %d", i, j);
		}
	    free(outputBitmap);
	}
}

Test(RivTest, dci_rivPositiveValues)
{
	uint32_t outFirstPRB;
	uint32_t outLastPRB;
	uint32_t bandwidthPRB[] = {6, 15, 25, 50, 75, 100};
	for (uint32_t bwIndex = 0; bwIndex < 6; bwIndex++)
	{
		for (uint32_t startIdx = 0; startIdx < bandwidthPRB[bwIndex]; startIdx++)
		{
			for (uint32_t endIdx = startIdx; endIdx < bandwidthPRB[bwIndex]; endIdx++)
			{
				uint32_t rivValue = resourceAllocationRIV(bandwidthPRB[bwIndex], startIdx, endIdx);
				dci_rivDecode ( bandwidthPRB[bwIndex], rivValue, &outFirstPRB, &outLastPRB);
				cr_expect_eq(outFirstPRB, startIdx,"Error");
				cr_expect_eq(outLastPRB, endIdx,"Error");
			}
		}
	}
}

Test(dci1Test, dci1_calculateShiftOriginTest)
{
	uint32_t dci1_offsetArray[DCI1_NUMBER_PARAM] = {RA, BITMAP_LEN, MCS, HARQ, NDI, RV, TPC};
	uint32_t possibleLengthBitsRBG[AMOUNT_OF_BANDWIDTHS] = {6, 8, 13, 17, 19 ,25};
	uint32_t correctValues[AMOUNT_OF_BANDWIDTHS] = {24, 24, 32, 32, 40, 40};
	uint32_t testVal;
	for (bandwidth_t i = BW_1_4MHz; i < AMOUNT_OF_BANDWIDTHS; i++)
	{
		dci1_offsetArray[dci1_bitmap] = possibleLengthBitsRBG[i];
		testVal = dci1_calculateShiftOrigin(dci1_offsetArray);
		cr_expect(testVal = correctValues[i], "Value returned from dci1_calculateShiftOrigin [%d] test loop are incorrect", i);
	}
}
