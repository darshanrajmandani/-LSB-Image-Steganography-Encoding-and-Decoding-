#ifndef DECODE_H
#define DECODE_H

#include "types.h"

// Maximum size of the secret buffer
#define MAX_SECRET_BUF_SIZE 1

// Maximum size of the image buffer (derived from the secret buffer size)
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)

// Maximum size of the file suffix
#define MAX_FILE_SUFFIX 4

// Structure to hold decoding information
typedef struct _DecodeInfo
{
    char *image_to_decode_name;       // Name of the stego image file
    FILE *fptr_to_decode_stego_img;   // File pointer for the stego image file
    char output_data[MAX_IMAGE_BUF_SIZE + 1];  // Buffer to store decoded data
    char *output_file_name;           // Name of the output file
    FILE *fptr_of_output_file;        // File pointer for the output file
} DecodeInfo;

// Function to read and validate decoding arguments
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

// Function to open files for decoding
Status open_files_to_decode(DecodeInfo *decInfo);

// Function to perform LSB decoding of a single byte
Status decode_from_lsb(char data, char *image_buffer);

// Function to decode data from the stego image
Status decode_data_from_image(const char *data, int size, DecodeInfo *decInfo);

// Function to decode the magic string from the stego image
Status decode_magic_string(DecodeInfo *decInfo);

// Function to decode the size of the secret file extension
Status decode_secret_file_extn_size(DecodeInfo *decInfo);

// Function to decode the secret file extension
Status decode_secret_file_extn(DecodeInfo *decInfo);

// Function to decode the size of the secret file
Status decode_secret_file_size(DecodeInfo *decInfo);

// Function to decode the data of the secret file and write to the output file
Status decode_secret_file_data(DecodeInfo *decInfo);

// Function to perform the decoding process
Status do_decoding(DecodeInfo *decInfo);

#endif  // DECODE_H

