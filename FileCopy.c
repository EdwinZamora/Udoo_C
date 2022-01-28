///////////////////////////////////////////////////////////////////////////////////////////////////
//                                      Includes Section
///////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

///////////////////////////////////////////////////////////////////////////////////////////////////
//                                   Defines & Macros Section
///////////////////////////////////////////////////////////////////////////////////////////////////

#define RW50K_BLOCK 50000

///////////////////////////////////////////////////////////////////////////////////////////////////
//                                       Typedef Section
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//                                  Function Prototypes Section
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
//                                   Global Constants Section
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//                                   Static Constants Section
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//                                   Global Variables Section
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//                                   Static Variables Section
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//                                      Functions Section
///////////////////////////////////////////////////////////////////////////////////////////////////

int main (int32_t argc, char *argv[])
{
    int32_t dwFileDescriptor;
    int32_t dwFileDescriptorCP;
    char * bpFileBuffer; /*Pointer to read/write buffer*/
    int8_t bSize;
    char * bpCopyFileName; /*Pointer to malloc buffer for the copy file name*/
    char * bpFileName;
    int8_t bCounter = 0; /*Used to return the pointer of the copy malloc buffer to its first position*/
    int8_t bReadReturn = 0;
    int8_t bWriteReturn = 0;
    int8_t bStatReturn = 0;
    uint16_t wTotalBlocks = 0; 
    int8_t flgExtension = 0; /*Used to differentiate between a file name with extension to one without*/
    struct stat sFileControlBlock; /*Struct will store file characteristics. File size is the one we need*/
    
    if(argc == 2)
    {
        bSize = (strlen(argv[1]) + 6);
        bpFileName = argv[1];
        bpCopyFileName = (char*)malloc(bSize); /*Allocate a bSize size buffer*/
        if(bpCopyFileName != NULL)
        {
        	while(bSize--)
        	{
        		if(*bpFileName == '.')
        		{
        		   strcat((bpCopyFileName-bCounter),"_copy.");
        		   bpCopyFileName += 6;
        		   bpFileName++;
			       flgExtension = 1;
			       bCounter += 6;
        		}
        		else
        		{
        			*bpCopyFileName++ = *bpFileName++;
        			bCounter++;
        		}
        	}
		    if(!flgExtension)
		    {
			    strcat((bpCopyFileName-bCounter),"_copy");
		    }
	        bpCopyFileName -= bCounter;
            dwFileDescriptor = open(argv[1], O_RDONLY); /*Read only*/
            dwFileDescriptorCP = open(bpCopyFileName,O_RDWR | O_CREAT | O_APPEND | O_EXCL,S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH); /*Write only, appending to end of file, create file if it does not exist. Open permissions: write and read permissions for user, group and others*/
            
            if(dwFileDescriptor > 0 && dwFileDescriptorCP > 0)
            {
                bStatReturn = fstat(dwFileDescriptor, &sFileControlBlock);
                if(bStatReturn < 0) /*Fill struct with file characteristics*/
                {
                    printf("\n\rStat Error\n\r");
                    close(dwFileDescriptor);
                    close(dwFileDescriptorCP);
                }
                else
                {
                    bpFileBuffer = (char*)malloc(RW50K_BLOCK); /*Allocate a 50K size buffer*/
                    if(bpFileBuffer != NULL)
                    {   
                        wTotalBlocks = sFileControlBlock.st_size / RW50K_BLOCK;                        
			            while(wTotalBlocks--)
                        {
                            bReadReturn = read(dwFileDescriptor,bpFileBuffer,RW50K_BLOCK);
                            if(bReadReturn > 0)
                            {
                                bWriteReturn = write(dwFileDescriptorCP,bpFileBuffer,RW50K_BLOCK);
                                if(bWriteReturn < 0)
                                {
                                     printf("\n\rError while writing buffer\n\r");
                                }
                            }
                            else
                            {
                                printf("\n\rError while reading buffer\n\r");
                            }
                        }

                        /*Copy the remainder, in case the file has a data size which isn't a multiple of 50k*/
			             wTotalBlocks = sFileControlBlock.st_size % RW50K_BLOCK;
			             if(wTotalBlocks)
			             {
                            free(bpFileBuffer); /*Reusing memory & variables*/
				            bpFileBuffer = (char*)malloc(wTotalBlocks); /*Allocate a wTotalBlocks size buffer*/
                       		if(bpFileBuffer != NULL)
                       	 	{                                                                                  
                            	bReadReturn = read(dwFileDescriptor,bpFileBuffer,wTotalBlocks);
	                            if(bReadReturn != -1)
        	                    {
	                                bWriteReturn = write(dwFileDescriptorCP,bpFileBuffer,wTotalBlocks);
        	                        if(bWriteReturn < 0)
                	                {
                        	             printf("\n\rError while writing remainder buffer\n\r");
	                                }
	                            }
        	                    else
                	            {
                        	        printf("\n\rError while reading remainder buffer\n\r");
	                            }			                           
        	                }
                	        else
                        	{
	                            printf("\n\rError allocating remainder buffer\n\r");
        	                }
			             }
		             }                
                     else
                     {
                        printf("\n\rError while allocating block buffer\n\r");
               	     }
                     free(bpFileBuffer);
                     free(bpCopyFileName);
		             close(dwFileDescriptor);
                     close(dwFileDescriptorCP);
                }
            }
            else
            {
                printf("\n\rError while opening the file\n\r");
            }
        }
        else
        {
                printf("\n\rError while allocating copy buffer\n\r");
        }
    }
    else
    {
         printf("\n\rParameter error\n\r");
    }
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// EOF
//////////////////////////////////////////////////////////////////////////////////////////////////

