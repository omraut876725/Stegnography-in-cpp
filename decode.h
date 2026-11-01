#ifndef DECODE
#define DECODE
#include"types.h"
class decode
{    private: 
              char *stego_name;
              FILE *stego_fptr;
              char *magic_string;
              uint extension_size;
              char *extension;
              uint secret_message_size;

              char *secret_name;
              FILE *secret_fptr;

    public: 
    //for decoding of secret message
Status do_decoding(char *argv[],decode *e_decode);
// read and check the extension of stego.bmp file
Status read_and_validate(char *argv[],decode *e_decode);
//open stegop image
Status open_file(decode *e_decode);
//skip the bmp header
Status skip_bmp_header(decode *e_encode);
//DECODE magic string
Status decode_magic_string(decode *e_decode);
//decode the ldb byte
Status decode_lsb_byte(decode *e_decode,char *ch);
//decode extension size
Status decode_extension_size(decode *e_decode);
//decode the size in int
Status decode_lsb_byte_int(FILE *stego_fptr,uint *extension_size);
//decode extension 
Status decode_extension(decode *e_decode);
//decode the secret message size
Status decode_message_size(decode *e_encode);
//open  secret file
Status open_output_file(char *argv[],decode *e_decode);
//decode secret message
Status decode_secret_message(decode *e_decode);

};

#endif