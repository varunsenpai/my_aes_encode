#ifndef BASE46_H
#define BASE46_H

#include <stdlib.h>
#include <memory.h>
#include <stdbool.h>

/***********************************************
Encodes ASCII string into base64 format string
@param input ASCII string to be encoded
@param encoded_output buffer to store encoded output
@param size size/length of the input plain string
@return true if decode successful
***********************************************/
bool base64_encode_buffer(unsigned char* input, unsigned char *encoded_output, int size, int output_buff_size);

/***********************************************
decodes base64 format string into ASCII string
@param cipher base64 encoded input string
@param output_plain buffer to store decoded output
@param input_size size/length of the input string
@return true if decode successful
***********************************************/
bool base64_decode_buffer(unsigned char* cipher, unsigned char *output_plain, int input_size, int output_buff_size);


#endif //BASE46_H