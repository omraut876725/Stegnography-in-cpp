#include<iostream>
#include"types.h"
#include"decode.h"
#include"common.h"
#include<cstring>
using namespace  std;
// read and check the extension of stego.bmp file
Status decode::read_and_validate(char *argv[],decode *e_decode)
{
    char * stego_address=NULL;
    if(argv[2]!=NULL)
    {
        stego_address=strstr(argv[2],".");
        if(stego_address!=NULL)
        {
            if(strcmp(stego_address,".bmp")==e_success)
            {
              e_decode->stego_name = new char[strlen(argv[2]) + 1];
               strcpy(e_decode->stego_name, argv[2]);
              //cout<< e_decode->stego_name<<endl;
                return e_success;
            }
        }
    }
    return e_failure;
}
Status decode:: open_file(decode *e_decode)
{
    e_decode->stego_fptr=NULL;
    e_decode->stego_fptr=fopen(e_decode->stego_name,"r");
    if(e_decode->stego_fptr!=NULL)
    {
       return e_success;
    }
    return e_failure;
}
Status decode::skip_bmp_header(decode *e_decode)
{
    fseek(e_decode->stego_fptr,54,SEEK_SET);
    return e_success;
}
Status decode:: decode_lsb_byte(decode *e_decode,char *data)
{
  
    char mask=1;
    for(int j=0;j<8;j++)
    {
       char ch;
       fread(&ch,1,sizeof(char),e_decode->stego_fptr);
       if((ch&1)!=0)
       {
          *data=*data|mask;
       }
       else
       {
        *data=*data&(~mask);
       }
       mask=mask<<1;
    }
    
  
  return e_success;
}
Status decode:: decode_magic_string(decode *e_decode)
{
    cout<<"Enter magic string\n";
   e_decode->magic_string=new char[20];
    cin>> e_decode->magic_string;
  for(int i=0;i<strlen(MAGIC_STRING);i++)
  { char ch;
    decode_lsb_byte(e_decode,&ch);
    if(ch!=e_decode->magic_string[i])
    {
        return e_failure;
    }
  }
   
    return e_success;
}
Status decode::decode_lsb_byte_int(FILE *stego_fptr,uint *extension_size)
{
     uint mask=1;
    for(int i=0;i<32;i++)
    {
        char tem;
       
        fread(&tem,1,sizeof(char),stego_fptr);
        if((tem&1)!=0)
        {
          *extension_size=*extension_size|mask;
        }
        else
        {
            *extension_size=*extension_size&(~mask);
        }
        mask=mask<<1;
    }
    return e_success;
}
Status decode::decode_extension_size(decode *e_decode)
{
   if(decode_lsb_byte_int(e_decode->stego_fptr,&e_decode->extension_size)==e_success)
   {
    cout<<"decoding of extension size done successfully"<<endl;
   // cout<<e_decode->extension_size<<endl;
         
   }
   else
   {
     return e_failure;
   }
   return e_success;
}
Status decode::decode_extension(decode *e_decode)
{
    e_decode->extension=new char[7];
    int i=0;
    for(i=0;i<e_decode->extension_size;i++)
    {  char ch;
       if(decode_lsb_byte(e_decode,&ch)!=e_success)
       {
        return e_failure;
       }
      e_decode->extension[i]=ch;
    }
    e_decode->extension[i]='\0';
   // cout<<e_decode->extension<<endl;
    return e_success;

}
Status decode::decode_message_size(decode *e_decode)
{
   if(decode_lsb_byte_int(e_decode->stego_fptr,&e_decode->secret_message_size)==e_success)
   {
    cout<<"decoding of extension size done successfully"<<endl;
   cout<<e_decode->secret_message_size<<endl;
         
   }
   else
   {
     return e_failure;
   }
   return e_success;
}
Status decode::open_output_file(char *argv[],decode *e_decode)
{ char *secret_address=NULL;
   if(argv[3]==NULL)
   {
    e_decode->secret_name=new char[11];
      strcpy(e_decode->secret_name,"output.txt");
      //cout<<e_decode->secret_name<<endl;
      e_decode->secret_fptr=fopen(e_decode->secret_name,"w+");

   }  
   else if(argv[3]!=NULL) 
   {  
    secret_address=strstr(argv[3],".");
    if(secret_address!=NULL)
    {
        if(strcmp(secret_address,".txt")==0)
        {
            e_decode->secret_name=new char[strlen(argv[3])+1];
           strcpy(e_decode->secret_name,argv[3]);
            e_decode->secret_fptr=fopen(e_decode->secret_name,"w+");
            cout<<e_decode->secret_name<<endl;
        }
        else
        {
             e_decode->secret_name=new char[strlen(argv[3])+5];
             int i=0;
             for(i=0;argv[3][i]!='.';i++)
             {
                e_decode->secret_name[i]=argv[3][i];
             }
              e_decode->secret_name[i]='\0';
              strcat(secret_name,".txt");
              e_decode->secret_fptr=fopen(e_decode->secret_name,"w+");
            cout<<e_decode->secret_name<<endl;

        }
    }
    else
    { e_decode->secret_name=new char[strlen(argv[3])+5];
      strcpy(e_decode->secret_name,argv[3]);
      strcat(e_decode->secret_name,".txt");
      e_decode->secret_fptr=fopen(e_decode->secret_name,"w+");
            cout<<e_decode->secret_name<<endl;

    }
   }
   return e_success;
}
Status decode::decode_secret_message(decode *e_decode)
{
    for(int i=0;i<e_decode->secret_message_size;i++)
    {
        char ch;
       if(decode_lsb_byte(e_decode,&ch)!=e_success)
       {
        return e_failure;
       }
      fwrite(&ch,1,sizeof(char),e_decode->secret_fptr);

    }
    return e_success;
}
Status decode:: do_decoding(char *argv[],decode *e_decode)
{  // read and check the extension of stego.bmp file
    if(e_decode->read_and_validate(argv,e_decode)==e_success)
    {    
        cout<<"I/;0NFO:check and validation done successfully\n";
        if(e_decode->open_file(e_decode)==e_success)
        {
            cout<<"INFO:opening file done successfully\n";
            if(e_decode->skip_bmp_header(e_decode)==e_success)
             {
                  cout<<"INFO:skip the bmp header successfully\n";
                  if(e_decode->decode_magic_string(e_decode)==e_success)
                  {
                    cout<<"INFO:decode magic string successfully\n";
                    if(e_decode->decode_extension_size(e_decode)==e_success)
                    {
                        cout<<"INFO:extension size decoding successfully"<<endl;
                         if(e_decode->decode_extension(e_decode)==e_success)
                         {
                            cout<<"INFO:extension decoding done successfully"<<endl;
                            if(e_decode->decode_message_size(e_decode)==e_success)
                            {
                                cout<<"INFO:secret message decoding successfully"<<endl;
                                if(e_decode->open_output_file(argv,e_decode)==e_success)
                                {
                                    cout<<"INFO:secret file opening successfully"<<endl;
                                    if(e_decode->decode_secret_message(e_decode)==e_success)
                                    {
                                        cout<<"INFO:decode secret message done successfully"<<endl;
                                        fseek(e_decode->secret_fptr,0,SEEK_SET);
                                        char ch;
                                        cout<<"The secret message is : ";
                                        while(fread(&ch,1,sizeof(char),e_decode->secret_fptr))
                                        {
                                           cout<<ch;
                                        }
                                        cout<<endl;
                                        
                                          
                                    }
                                    else
                                    {
                                     cout<<"ERROR:fail to decode secret messsage "<<endl;
                                     return e_failure;
                                    }
                            

                                }
                                else
                                {
                                    cout<<"ERROR:fail to open seret file"<<endl;
                                    return e_failure;
                                }

                            }
                            else
                            {
                                cout<<"ERROR:fail to decode the secret message size "<<endl;
                                return e_failure;
                            }

                         }
                         else
                         {
                            cout<<"ERROR:fail to decode extension"<<endl;
                            return e_failure;
                         }

                    }
                    else
                    {
                        cout<<"ERROR:fail to decoding the size of extension"<<endl;
                        return e_failure;
                    }

                  }
                  else
                  {
                    cout<<"ERROR:fail to decode magic string\n";
                    return e_failure;
                  }
             }
             else
            {
                 cout<<"ERROR: fail to skip bmp header"<<endl;
                  return e_failure;
             }
            
        }
        else
        {
            cout<<"ERROR: fail to open file \n ";
            return e_failure;
        }
       
        
    }
    else
    {
        cout<<"ERROR: fail to check and validatation \n";
        return e_failure;
    }
    return e_success;
}