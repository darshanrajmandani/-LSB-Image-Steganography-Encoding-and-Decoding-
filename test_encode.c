#include <stdio.h>
#include "types.h"
#include "encode.h"
#include "decode.h"
#include <string.h>

// Function prototype declaration
OperationType check_operation_type(char *argv[]);

int main(int argc, char *argv[])
{
	// Check if the correct number of command-line arguments is provided
	if (argc <= 3)
	{
		printf("ERROR : Please pass CLA\n");
		return 1;
	}

	// Declare structures for encoding and decoding information
	EncodeInfo encInfo;
	DecodeInfo decInfo;

	// Determine the operation type based on the command-line argument
	int res = check_operation_type(argv);

	if (res == e_encode)
	{
		printf("INFO : Selected Encoding\n");

		// Read and validate encoding arguments
		if (read_and_validate_encode_args(argv, &encInfo) == e_success)
		{
			printf("INFO : Read and validate args are success\n");

			// Perform encoding
			if (do_encoding(&encInfo) == e_success)
			{
				printf("INFO : Encoding is successful\n");
			}
			else
			{
				printf("INFO : Encoding failure\n");
				return 0;
			}
		}
		else
		{
			printf("INFO : Read and Validate args are failure\n");
			return 0;
		}
	}
	else if (res == e_decode)
	{
		printf("INFO : Selected Decoding\n");

		// Read and validate decoding arguments
		if (read_and_validate_decode_args(argv, &decInfo) == e_success)
		{
			printf("INFO : Read and validate args of decoding are success\n");

			// Perform decoding
			if (do_decoding(&decInfo) == e_success)
			{
				printf("INFO : Decoding is successful\n");
			}
			else
			{
				printf("INFO : Decoding failure\n");
				return 0;
			}
		}
		else
		{
			printf("INFO : Read and validate args are failure\n");
			return 0;
		}
	}
	else
	{
		// Display an error message for unsupported option
		printf("ERROR : Unsupported option\n");
		printf("For Encoding : ./a.out -e image.bmp secret.txt [stego.bmp]\n");
		printf("For Decoding : ./a.out -d stego.bmp [data.txt]\n");
	}

	return 0;
}

// Function to determine the operation type based on the command-line argument
OperationType check_operation_type(char *argv[])
{
	if (strcmp(argv[1], "-e") == 0)
		return e_encode;
	else if (strcmp(argv[1], "-d") == 0)
		return e_decode;
	else
		return e_unsupported;
}

