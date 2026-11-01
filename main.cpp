/*
==============================================================================
 Project     : Image Steganography using LSB (Least Significant Bit)
 Name        : Om Raut 
 Date        : 15-Oct-2026
 Batch Id    : 26019f_009
 Language    : C++
 
 Files       : main.cpp, encode.cpp, decode.cpp, encode.h, decode.h, types.h,
               beautiful.bmp (source image), secret.txt (secret file)
==============================================================================

 DESCRIPTION
 -----------
 This program hides a secret file (for example a .txt file) inside a BMP
 image without visibly changing the image. This technique is called
 steganography. The hidden data can later be extracted from the image
 and restored as the original file.

 HOW IT WORKS (LSB METHOD)
 -------------------------
 Every byte of the image pixel data has 8 bits. The last bit (least
 significant bit) of each byte is replaced with one bit of the secret
 data. Changing only the last bit alters the colour so slightly that the
 human eye cannot notice the difference.

 Example: image byte  1011 0110  +  secret bit 1  ->  1011 0111

 One byte of secret data needs 8 image bytes to store it.

 ENCODING (hiding data)
 ----------------------
 1. Open the source BMP image, the secret file and the output image.
 2. Check that the image is large enough to hold the secret data:
        image capacity (bytes) >= (magic string + extension size +
        extension + file size + file data) x 8
 3. Copy the 54-byte BMP header unchanged to the output image.
 4. Encode in this order, bit by bit, into the image bytes:
        a. Magic string (a marker such as "#*" to identify encoded images)
        b. Secret file extension size
        c. Secret file extension (e.g. ".txt")
        d. Secret file size
        e. Secret file data
 5. Copy the remaining image bytes unchanged.

 DECODING (extracting data)
 --------------------------
 1. Open the encoded image and skip the 54-byte header.
 2. Read the magic string. If it does not match, the image contains
    no hidden data and decoding stops.
 3. Decode the extension size, extension, file size and file data by
    reading the last bit of each image byte and rebuilding the bytes.
 4. Write the data into a new output file with the decoded extension.

 COMMAND LINE USAGE
 ------------------
 Encode : ./a.out -e <source.bmp> <secret.txt> [output.bmp]
 Decode : ./a.out -d <encoded.bmp> [output_file]

 VALIDATIONS
 -----------
 - Only .bmp images are accepted as source images.
 - Files must exist and open successfully.
 - The image must have enough capacity for the secret data.
 - The magic string is verified before decoding.

 LIMITATIONS
 -----------
 - Works with BMP images only.
 - Secret file size is limited by the image size.
 - Saving the image in a lossy format (such as JPG) destroys the hidden data.
==============================================================================
*/

#include<iostream>
#include"types.h"
#include"encode.h"
#include"decode.h"
#include<cstring>
using namespace std;
int main(int argc,char *argv[])
{
   
 if(check_operation_type(argv)==e_encode)
 {  
    cout<<"encode operation is selected\n";
    encode e_encode;
    if(e_encode.do_encoding(argv,&e_encode)==e_success)
    {
        cout<<"encoding done Successfully\n";
    }
    else
    {
        cout<<"encoding not done\n";
    }
 }
 else if(check_operation_type(argv)==e_decode)
 {
    cout<<"decode operation is selected\n";
    decode e_decode;
    if(e_decode.do_decoding(argv,&e_decode)==e_success)
    {
     //   cout<<"INFO:decoding done successfully\n";

    }
    else
    {
        cout<<"ERROR:fail to decoding\n";
    }
 }
 else
 {
    cout<<"unsupported operation is selected\n";
    cout<<"Usage: for encode : ./a.out -e beautiful.bmp secret.txt stego.bmp \n";
    cout<<"       for decode : ./a.out -d stego.bmp output.txt\n";
 }

}
OperationType check_operation_type(char *argv[])
{  
    if(argv[1]!=NULL&&strcmp(argv[1],"-e")==0)
    {
        return e_encode;
    }
    else if(argv[1]!=NULL&&strcmp(argv[1],"-d")==0&&argv[1]!=NULL)
    {
        return e_decode;
    }
    else
    {
        return e_unsupported;
    }

}