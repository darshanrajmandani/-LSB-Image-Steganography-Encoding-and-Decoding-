#include <stdio.h>
#include "decode.h"
#include "types.h"
#include "common.h"
#include <string.h>
#include <stdlib.h>

// Function to read and validate decoding arguments
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
	// Validate BMP file extension
	if (strcmp(strstr(argv[2], ".bmp"), ".bmp") == 0)
	{
		printf("INFO: BMP validated successfully\n");
		decInfo->image_to_decode_name = argv[2];
	}
	else
	{
		printf("INFO: BMP validation is unsuccessful\n");
		return e_failure;
	}

	// Validate secret file extension
	if (strcmp(strstr(argv[3], ".txt"), ".txt") == 0)
	{
		printf("INFO: Secret file validated successfully\n");
		decInfo->output_file_name = argv[3];
	}
	else
	{
		printf("INFO: Secret file validation unsuccessful\n");
		return e_failure;
	}

	return e_success;
}

// Function to open files for decoding
Status open_files_to_decode(DecodeInfo *decInfo)
{
	// Open stego image file for reading
	decInfo->fptr_to_decode_stego_img = fopen(decInfo->image_to_decode_name, "r");
	if (decInfo->fptr_to_decode_stego_img == NULL)
	{
		perror("fopen");
		fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->image_to_decode_name);
		return e_failure;
	}

	// Open output file for writing
	decInfo->fptr_of_output_file = fopen(decInfo->output_file_name, "w");
	if (decInfo->fptr_of_output_file == NULL)
	{
		perror("fopen");
		fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->image_to_decode_name);
		return e_failure;
	}

	return e_success;
}

// Function to perform LSB decoding of a single byte
Status decode_from_lsb(char data, char *image_buffer)
{
	for (int i = 0; i < 8; i++)
	{
		// Check LSB of image_buffer and data
		if ((image_buffer[i] & 1) == ((data >> i) & 1))
		{
			// Modify the image_buffer based on the LSB of data
			image_buffer[i] = (data >> (7 - i)) & 1;
		}
		else
		{
			printf("INFO: Decoding data from LSB failure\n");
			return e_failure;
		}
	}
	return e_success;
}

// Function to decode data from the stego image
Status decode_data_from_image(const char *data, int size, DecodeInfo *decInfo)
{
	// Move the file pointer to the start of image data (after the BMP header)
	fseek(decInfo->fptr_to_decode_stego_img, 54, SEEK_SET);

	printf("INFO: Decoding Magic String Signature\n");
	for (int i = 0; i < size; i++)
	{
		// Read 8 bits from stego image
		fread(decInfo->output_data, 8, 1, decInfo->fptr_to_decode_stego_img);

		// Decode data using LSB
		if (decode_from_lsb(data[i], decInfo->output_data) == e_success)
		{
			continue;
		}
		else
		{
			return e_failure;
		}
	}
	return e_success;
}

// Function to decode the magic string from the stego image
Status decode_magic_string(DecodeInfo *decInfo)
{
	if (decode_data_from_image(MAGIC_STRING, strlen(MAGIC_STRING), decInfo) == e_success)
	{
		return e_success;
	}
	else
	{
		return e_failure;
	}
}

// Function to decode the size of the secret file extension
Status decode_secret_file_extn_size(DecodeInfo *decInfo)
{
	char str[32];
	int file_size = 0;
	fread(str, 32, 1, decInfo->fptr_to_decode_stego_img);
	return e_success;
}

// Function to decode the secret file extension
Status decode_secret_file_extn(DecodeInfo *decInfo)
{
	char str[32];
	for (int i = 1; i <= 4; i++)
	{
		fread(str, 32, 1, decInfo->fptr_to_decode_stego_img);
	}
	return e_success;
}

// Function to decode the size of the secret file
Status decode_secret_file_size(DecodeInfo *decInfo)
{
	int file_size[32];
	fread(file_size, 32, 1, decInfo->fptr_to_decode_stego_img);
	return e_success;
}

// Function to decode the data of the secret file and write to the output file
Status decode_secret_file_data(DecodeInfo *decInfo)
{
	char single_letter[8];
	for (int i = 0; i < 25; i++)
	{
		char output_byte = 0x00;
		fread(single_letter, 8, 1, decInfo->fptr_to_decode_stego_img);
		for (int j = 0; j < 8; j++)
		{
			// Reconstruct the output byte by combining LSBs
			output_byte = ((single_letter[j] & 1) << (7 - i)) | output_byte;
		}
		// Write the reconstructed byte to the output file
		fwrite(&output_byte, 1, 1, decInfo->fptr_of_output_file);
	}

	return e_success;
}

// Function to perform the decoding process
Status do_decoding(DecodeInfo *decInfo)
{
	// Open stego image and output file
	if (open_files_to_decode(decInfo) == e_success)
	{
		printf("INFO: Open files successful\n");

		// Decode the magic string from the stego image
		if (decode_magic_string(decInfo) == e_success)
		{
			printf("INFO: Done\n");

			// Decode the size of the secret file extension
			if (decode_secret_file_extn_size(decInfo) == e_success)
			{
				printf("INFO: Decoded secret file extension size\n");

				// Decode the secret file extension
				if (decode_secret_file_extn(decInfo) == e_success)
				{
					printf("INFO: Decoded secret file extension successfully\n");

					// Decode the size of the secret file
					if (decode_secret_file_size(decInfo) == e_success)
					{
						printf("INFO: Decoded Secret file size successfully\n");

						// Decode the data of the secret file and write to the output file
						if (decode_secret_file_data(decInfo) == e_success)
						{
							printf("INFO: Decoded the file data successfully and stored in %s\n", decInfo->output_file_name);
						}
						else
						{
							printf("INFO: Decoding the file data is unsuccessful\n");
							return e_failure;
						}
					}
					else
					{
						printf("INFO: Decoding secret file size failure\n");
						return e_failure;
					}
				}
				else
				{
					printf("INFO: Decoding secret file extension unsuccessful\n");
					return e_failure;
				}
			}
			else
			{
				printf("INFO: Failed to decode secret file extension\n");
				return e_failure;
			}
		}
		else
		{
			printf("INFO: Decoding magic string unsuccessful\n");
			return e_failure;
		}
	}
	else
	{
		printf("INFO: Open files not successful\n");
		return e_failure;
	}

	return e_success;
}

