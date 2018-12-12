#ifndef DCI_COMMON_H
#define DCI_COMMON_H_

#define AMOUNT_OF_BANDWIDTHS 6

#include <stdint.h>

typedef enum dciType {dci0, dci1, dci60a, maxDci} dciType;

extern uint8_t dciBandwidth[AMOUNT_OF_BANDWIDTHS];
extern uint8_t dciBandwidthPRB[AMOUNT_OF_BANDWIDTHS];
extern uint64_t dci_readArgumentsStdin;
extern uint8_t dci_bandwidthPRB;
extern dciType dci_Result;
extern uint32_t* dci_tempParameters;

extern char* dci_output;
extern uint32_t* dci_argsToValid;


/* > Function: dci_readValueFromDCI
 *******************************************************************************************/
/*
 * Function return a pointer (uint32_t*) to array with values decode from DCI.
 * dci is our DCI read from input of programm;
 * bitLenghtOfDciParameteris array with bit lenght of every DCI parameter;
 * sizeOfArray is number of arrays element of shiftArray;
 *
 *******************************************************************************************/

uint32_t* dci_readValueFromDCI (uint64_t dci, const uint8_t bitLenghtOfDciParameter[], const uint8_t sizeOfArray);

/* > Function: dci0_lengthOfRIVviaBandwidth and dci60a_lengthOfRIVviaBandwidth
 *******************************************************************************************/
/*
 * @brief
 * Checks correctness of length RIV for dci0 and dci60a.
 * Created Bandwidth and RIV parameter.
 * If RIV parameter is greater than possible length RIV view information
 *
 *******************************************************************************************/
uint8_t dci0_lengthOfRIVviaBandwidth(uint8_t Bandwidth, uint8_t LengthBitsRIV, uint8_t RIV);
uint8_t dci60a_lengthOfRIVviaBandwidth(uint8_t Bandwidth, uint8_t LengthBitsRIV, uint8_t RIV);

/* > Function: dci1_lengthOfBitmapVviaBandwidth
 *******************************************************************************************/
/*
 * @brief
 * Checks correctness of length BitsRBG for dci1.
 * Created Bandwidth and rbgNumber parameter.
 * If rbgNumber parameter is greater than possible length bits RBG view information.
 *
 *******************************************************************************************/
uint8_t dci1_lengthOfBitmapViaBandwidth(uint8_t Bandwidth, uint8_t LengthBitsRBG, uint8_t rbgNumber);

/* > Function: dci_DefineDci
 *******************************************************************************************/
/*
 * @brief 	Validate arguments from argv[] and assign them to @dci_p and @prb_p. When argv[1]
 * 			is different than values in dciStrArguments function return standard dci0 type.
 * 			This same happend to argv[2] where standard bandwith size is 20 MHz and return number
 * 			of Power Resoure Block
 *
 * @param[in] argc							Amount of elements in argv[] array
 * @param[in] argv[]						Array with type of dci and bandwith
 * @param[out] dci_p						Pointer return type of dci
 * @param[out] prb_p						Pointer return nr of PRB's
 *
 *******************************************************************************************/
void dci_defineDci(const int argc, const char* const argv[], dciType* restrict const  dci_p,
		uint8_t* restrict const prb_p);

uint8_t dci_readStdin(uint64_t dci_readArgumentsStdin);
void dci_print(char* output);

#endif /*DCI_COMMON_H_*/
