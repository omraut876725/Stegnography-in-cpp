#ifndef ENCODE
#define ENCODE
#include "types.h"
class encode
{
    private : FILE *src_fptr;
              char *src_name;
              uint img_capacity;





              FILE *secret_fptr;
              char *secret_name;
              uint secret_file_size;
               char *secret_data;
              const char *secret_file_extension;



              FILE *stego_fptr;
              char *stego_name;
             
    public :
             //for Encoding the secret message 
            Status do_encoding(char *argv[],encode *e_encode);
            // check  validation of  arguments 
            Status read_and_validate(char *argv[],encode *e_encode);
            // open the all files
            Status open_files(char *argv[],encode *e_encode);
            //check image capacity
            Status check_capacity(encode *e_encode);
            //image capacity
            uint  get_img_size(encode *e_encode);
            //get secret file size
            uint get_secret_size(encode *e_encode);
         // copy the header into stego image
            Status copy_header(encode *e_encode);
            //encode magic string
            Status encode_magic_string(const char * magic_string,encode *e_encode);
            //encode lsb byte
            Status encode_lsb_byte(const char *data ,uint size,encode *e_encode);
            //encode the extension size
            Status encode_extn_size(encode *e_encode);
            //encode intger value in stego.bmp
            Status encode_lsb_int(uint num,encode *e_encode);
           //encode the extension int stego image
           Status encode_extension(encode *e_encode);
           //encode secret file size
           Status encode_secret_file_size(encode *e_encode);
           // encode secret file data into stego image
           Status encode_secret_file(encode *e_encode);
           //copy reamining bytes in stego image
           Status copy_reamining_bytes(encode *e_encode);
   
};
OperationType check_operation_type(char *argv[]);

#endif
