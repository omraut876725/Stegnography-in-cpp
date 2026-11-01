#include<iostream>
#include<cstring>
#include<cstdio>
#include"encode.h"
#include"types.h"
#include"common.h"
using namespace std;
/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
/// get the size of the bmp or source file

// read and validating the output
Status encode::read_and_validate(char *argv[],encode *e_encode)
{
 
    char *src_address;
    char *secret_address;
    char *stego_address;
    if(argv[2]!=NULL)
    {
        src_address=strchr(argv[2],'.');
        if(src_address==NULL||strcmp(src_address,".bmp")!=0)
        {
           return e_failure; 
        }
      
    }
     if(argv[3]!=NULL)
    {
        secret_address=strchr(argv[3],'.');
        if(secret_address==NULL||strcmp(secret_address,".txt")!=0)
        {
             return e_failure;
        }
      
    }
    if(argv[4]!=NULL)
    {
        stego_address=strchr(argv[4],'.');
        if(stego_address==NULL||strcmp(stego_address,".bmp")!=0)
        {
           return e_failure;
        }
        else
        {
            return e_success;
        }
     
    }

    return e_failure;
}
// open the all files
Status encode ::open_files(char *argv[],encode *e_encode)
{    e_encode->src_name=new char[strlen(argv[2])+1];
     e_encode->secret_name= new char[strlen(argv[3])+1];
     e_encode->stego_name=new char[strlen(argv[4])+1];
     strcpy(e_encode->src_name,argv[2]);
    e_encode->src_fptr=fopen(e_encode->src_name,"r");
    cout<<e_encode->src_name<<endl;
    if(  e_encode->src_fptr==NULL)
    {   cout<<"Error to source file\n";
        return e_failure;
    }
      strcpy(e_encode->secret_name,argv[3]);
     e_encode->secret_fptr=fopen(e_encode->secret_name,"r");
     cout<<e_encode->secret_name<<endl;
    if(  e_encode->secret_fptr==NULL)
    { cout<<"Error to secret file\n";
        return e_failure;
    }
      strcpy(e_encode->stego_name,argv[4]);
     e_encode->stego_fptr=fopen(e_encode->stego_name,"w");
     cout<<e_encode->stego_name<<endl;
    if(  e_encode->stego_fptr==NULL)
    { cout<<"Error to stego file\n";
        return e_failure;
    }

    return e_success;
}
uint  encode::get_img_size(encode *e_encode)
{
    uint width;
    uint height;
    fseek(e_encode->src_fptr,18,SEEK_SET);
    fread(&width,1,sizeof(int),e_encode->src_fptr);
    fread(&height,1,sizeof(int),e_encode->src_fptr);
    cout<<"width= "<<width<<endl;
    cout<<"height= "<<height<<endl;

    return width*height*3;

}
  uint encode::get_secret_size(encode *e_encode)
 {
    fseek(e_encode->secret_fptr,0,SEEK_END);
    return ftell(e_encode->secret_fptr);
 }
Status encode::check_capacity(encode *e_encode)
{
    /* 1)  54 bytes for bmp header,
      2)  2bytes for magic string/secrete key,
      3)  4bytes character in secrete file  extension =.txt,
      4)  4 bytes for size in int of secrete file extension ,
      5)  4 bytes for secrete message size,
      6)  encInfo->size_secret_file for secrete message*/
    
    e_encode->img_capacity=get_img_size(e_encode);
    e_encode->secret_file_size=get_secret_size(e_encode);
    if(e_encode->img_capacity>=(54+2+4+4+4+ e_encode->secret_file_size)*8)
    {
        return e_success;
    }
    else
    {
        return e_failure;
    }
} 
//copy header to stego image
 Status encode:: copy_header(encode *e_encode)
 {
    rewind(src_fptr);
    char ch[54];
    fread(ch,54,sizeof(char),src_fptr);
    fwrite(ch,54,sizeof(char),stego_fptr);
    return e_success;
 }
 Status encode::encode_lsb_byte(const char *data ,uint size,encode *e_encode)
 {
   for(int i=0;i<size;i++)
   {
      char ch=data[i];
      int mask=1;
      for(int i=0;i<8;i++)
      {
        char tem;
        fread(&tem,1,sizeof(char),src_fptr);
        if((mask&ch)!=0)
        {
            tem=tem|1;
           fwrite(&tem,1,sizeof(char),stego_fptr);
        }
        else
        {
            tem=tem&(~1);
             fwrite(&tem,1,sizeof(char),stego_fptr);

        }
        mask=mask<<1;
      }
   }
   return e_success;
 }
 //encode magic string
Status encode ::encode_magic_string(const char * magic_string,encode *e_encode)
{ 
  if(encode_lsb_byte(magic_string,strlen(magic_string),e_encode)==e_success)
  {
    return e_success;
  }
  else
  {
    return e_failure;
  }

}
Status encode::encode_lsb_int(uint num,encode *e_encode)
{
    int mask=1;
    

  for(int i=0;i<32;i++)
  {  char tem;
    fread(&tem,1,sizeof(char),e_encode->src_fptr);
    if((mask&num)!=0)
    {
        tem=tem|1;

    }
    else
    {
        tem=tem&(~1);
        
    }
    fwrite(&tem,1,sizeof(char),e_encode->stego_fptr);
    mask=mask<<1;
  }
  return e_success;
}

 Status encode::encode_extn_size(encode *e_encode)
 {
    if(encode_lsb_int(strlen(".txt"),e_encode)==e_success)
    {
        return e_success;
    }
    return e_failure;
 }
Status encode::encode_extension(encode *e_encode)
{
   e_encode->secret_file_extension=new char[5];
   e_encode->secret_file_extension=".txt";
   if(encode_lsb_byte( e_encode->secret_file_extension,strlen(e_encode->secret_file_extension),e_encode)==e_success)
   {
   return e_success;

   }
   else
   {
    return e_failure;
   }
return e_success;
}
//encode secret file size
 Status encode:: encode_secret_file_size(encode *e_encode)
 {

   if(encode_lsb_int(e_encode->secret_file_size,e_encode)==e_success)
    {
        return e_success;
    }
    return e_failure;
 }
 // encode the secret message in stego image
 Status encode:: encode_secret_file(encode *e_encode)
 {
    rewind(e_encode->secret_fptr);
    e_encode->secret_data=new char[e_encode->secret_file_size];
    fread(e_encode->secret_data,e_encode->secret_file_size,sizeof(char),e_encode->secret_fptr);
    if(encode_lsb_byte(e_encode->secret_data,e_encode->secret_file_size,e_encode)==e_success)
    {
        return e_success;
    }
    return e_failure;
 }
 Status encode::copy_reamining_bytes(encode *e_encode)
 {
    char  ch;
    while(fread(&ch,1,sizeof(char),e_encode->src_fptr)==1)
    {
        fwrite(&ch,1,sizeof(char),e_encode->stego_fptr);
    }
    return e_success;
 }


 Status encode :: do_encoding(char *argv[],encode *e_encode)
 {
    if(e_encode->read_and_validate(argv,e_encode)==e_success)
    {
        cout<<"read and validation done successfully"<<endl;
        if(e_encode->open_files(argv,e_encode)==e_success)
        {
          cout<<"file open successfully"<<endl;
          if(e_encode->check_capacity(e_encode)==e_success)
          {
            cout<<"capacity check passed\n";
            if(e_encode->copy_header(e_encode)==e_success)
            {
            cout<<"header copy succcessfully\n";
            if(e_encode->encode_magic_string(MAGIC_STRING,e_encode)==e_success)
            {
                cout<<"magic string encode successfully\n";
               if(e_encode->encode_extn_size(e_encode)==e_success)
               {
                cout<<"extension size  encoding done successfully\n";
                 if(e_encode->encode_extension(e_encode)==e_success)
                 {
                    cout<<"encode secret file extension done successfully\n";
                    if(e_encode->encode_secret_file_size(e_encode)==e_success)
                    {
                        cout<<"encode secret file size done successfully\n";
                        if(e_encode->encode_secret_file(e_encode)==e_success)
                        {
                            cout<<"encoding of secret file done successfully\n";
                            if(e_encode->copy_reamining_bytes(e_encode)==e_success)
                            {
                                cout<<"reamining bytes is encodes successfully\n";
                            }
                            else
                            {
                                cout<<"ERROR:fail to encode reamining bytes\n";
                            }
                        }
                        else
                        {
                            cout<<"ERROR:fail to encode the secret file\n";
                        }
                    }
                    else
                    {
                        cout<<"ERROR:failt to encode secret file size\n";
                    }
                 }
                 else
                 {
                    cout<<"ERROR:fail to encode extension\n";
                    return e_failure;
                 }
               }

               else
               {
                cout<<"ERROR:fail to encode extension size\n";
                return e_failure;
               }
            }
            else
            {
                cout<<"ERROR:fail to encode magic string\n";
                return e_failure;
            }

            }
            else
            {
                cout<<"ERROR:fail to copy header\n";
            }
            
          }
          else
          {
             cout<<"ERROR:insufficient capacity to encode\n";
          }
        }
        else
        {
            cout<<"ERROR: fail to open the files\n";
        }

    }
    else
    {
         cout<<"ERROR: fail to read and validate\n";
        return e_failure;
    }
  return e_success;
 }
