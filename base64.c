#include "base64.h"


char base64_map[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                     'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
                     'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
                     'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'};


bool base64_encode_buffer(unsigned char* input, unsigned char *encoded_output, int size, int output_buff_size) {
    bool status = true;
    int counts = 0;
    char buffer[3];
    int i = 0, c = 0;

    //calculate number of characters required in relation to output buff size
    int required_buff_size =  4 * (int)((size + 2)/3);

    if(output_buff_size < required_buff_size)
        return false;
    
    for(i = 0; i < size; i++) {
        buffer[counts++] = input[i];
        if(counts == 3) {
            encoded_output[c++] = base64_map[buffer[0] >> 2];
            encoded_output[c++] = base64_map[((buffer[0] & 0x03) << 4) + (buffer[1] >> 4)];
            encoded_output[c++] = base64_map[((buffer[1] & 0x0f) << 2) + (buffer[2] >> 6)];
            encoded_output[c++] = base64_map[buffer[2] & 0x3f];
            counts = 0;
        }
    }

    if(counts > 0) {
        encoded_output[c++] = base64_map[buffer[0] >> 2];
        if(counts == 1) {
            encoded_output[c++] = base64_map[(buffer[0] & 0x03) << 4];
            encoded_output[c++] = '=';
        } else {                      // if counts == 2
            encoded_output[c++] = base64_map[((buffer[0] & 0x03) << 4) + (buffer[1] >> 4)];
            encoded_output[c++] = base64_map[(buffer[1] & 0x0f) << 2];
        }
        encoded_output[c++] = '=';
    }

    return status;
}

bool base64_decode_buffer(unsigned char* cipher, unsigned char *output_plain, int size, int output_buff_size) {

    if((size % 4))
    {
        //size is not divisible by 4, not possible in a base64 encoded string
        return false;
    }

    int no_of_padding_chars = 0;

    for(int i = 0; i < size; i++)
    {
        //count the padding chars
        if(cipher[i] == '=')
            ++no_of_padding_chars;
    }

    int buff_size_required = ((int)((size * 3)/4) - (no_of_padding_chars))/2;

    if(buff_size_required > output_buff_size)
    {
        return false;
    }

    int counts = 0;
    char buffer[4];
    int i = 0, p = 0;
    bool status = true;
    bool char_found = false;

    for(i = 0; i < size; i++) {
        int k;
        for(k = 0 ; (k < 64); k++)
        {
            if(base64_map[k] == cipher[i])
            {
                char_found = true;
                break;
            }    
        }
        if(!char_found)
        {
            //char not found in base64 map
            return false;
        }

        buffer[counts++] = k;
        if(counts == 4) {
            output_plain[p++] = (buffer[0] << 2) + (buffer[1] >> 4);
            if(buffer[2] != 64)
                output_plain[p++] = (buffer[1] << 4) + (buffer[2] >> 2);
            if(buffer[3] != 64)
                output_plain[p++] = (buffer[2] << 6) + buffer[3];
            counts = 0;
        }
    }

    return status;
}