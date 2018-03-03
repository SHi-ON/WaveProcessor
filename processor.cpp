/* Wave Signal Processor
	by Shawn Azdam
	https://shawnazdam.com
	spring 2012
*/

/*	This program is a wave signal processor. 
	The program opens the file and modifies its contents .
	The operations that could be run: up-sampling, down-sampling, selecting or deleting part of file .
	
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
using namespace std;


/* The wav files header information is found on the first 44 bytes.
This structure is used to recover the header information from the wav file */
struct wav_hdr				
{
    char                RIFF[4];        // RIFF String, "RIFF" 
    unsigned long       ChunkSize;      // RIFF Chunk Size  
    char                WAVE[4];        // Wave String, "WAVE"   
    char                fmt[4];         // fmt String, "fmt" 
    unsigned long       Subchunk1Size;  // Size of the FMT chunk
    unsigned short      AudioFormat;    // Audio format 1=PCM , 6=mulaw , 7=alaw , 257=IBM Mu-Law , 258=IBM A-Law , 259=ADPCM
    unsigned short      NumChan;		// Number of channels 1=Mono , 2=Stereo
    unsigned long       SampRate;		// Sampling Frequency in (sample per second) or (Hz)
    unsigned long       BytesPerSec;    // bytes per second
    unsigned short      BlockAlign;     // 2=16-bit mono , 4=16-bit stereo
    unsigned short      bps;			// Number of bits per sample
    char                Subchunk2ID[4]; // data String, "data"
    unsigned long       Subchunk2Size;  // Sampled data length
};					
wav_hdr wavHeader;				// struct label


char path[1000] ;				// Holds the path of file to open or save that user input. 
FILE *wavFile ;					// Wave file pointer
int wavSize ;					// Keeps size of wave file
int n ;							// Contains time of Down or Up Sampling 
int NumSample ;					// Number of samples in wave file
int ti , tf;					// Initial and final time 
int sampi , sampf , len;		// Initial and final sample, length of new l and r.

unsigned __int16 *left_ch = NULL;
unsigned __int16 *right_ch = NULL;

int wavOpen(char path[]);		// open function prototype
int getFileSize(FILE *inFile);	// file size calculator function prototype
void print();
int _tmain(int argc, _TCHAR* argv[])
{
	printf(" Wave Processor [Version 1.0] \n Copyright (c) 2012 Shawn Azdam.  All rights reserved\n\n");
	print();
	printf("-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_\n");
	printf("Available Commands: \n");
	printf("open  \t\t open the wav file\n");
	printf("save \t\t Save the modified wave signals into a new .wav file\n");
	printf("ds   \t\t Down-sample current signal\n");
	printf("us   \t\t Up-sample current signal \n");
	printf("del  \t\t Delete specific part of wave file \n");
	printf("sel  \t\t Select specific part of wave file \n");
	printf("exit \t\t Quit the program \n");
	printf("-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_\n");
	printf("Enter a command...\n");

													// NULL initializing  

													// NULL initializing 
	unsigned __int16 *l = NULL;
	unsigned __int16 *r = NULL;

	while(1)
	{
		cin.ignore();
		char StrIn[100] ;						// Room for user input string.
		gets(StrIn);								// Gets user input string
		if(strcmp(StrIn,"open") == 0)			    // You may see This step somwhere else in the code too.
		{										    // This part compares input string with pre-defined commands name to identify entered command.
		printf("Please enter file path:\n");
		scanf("%s",&path);
		
		int res = 0;
		res = wavOpen(path);						// Examine the value that returns from "wavOpen" Function.
		if(res = 1)
			{
				
			wavSize = getFileSize(wavFile);						
																					// Printing wav header information.
			printf("\t File size:       \t %d Bytes\n",wavSize);
			printf("\t Duration:        \t %d second\n",wavSize/wavHeader.BytesPerSec);
			printf("\t Audio format:        \t ",wavHeader.AudioFormat);
			switch (wavHeader.AudioFormat)			// A switch control to printing relevant Audio Format.
				{
				case 1:
					printf("PCM");					//Pulse Code Modulation, an audio encoding algorithm which encodes the amplitude of a repetitive series of audio samples
					break;
				case 6:
					printf("mu-law");				// The compressing and expanding algorithm used primarily in North America and Japan when converting from analog to digital speech data.
					break;
				case 7:
					printf("A-law");				// The compressing and expanding algorithm used in Europe, Latin America, and other countries when converting from analog to digital speech data.
					break;
				case 257:
					printf("IBM mu-law");			// IBM developed mu-law standard
					break;
				case 258:
					printf("IBM IBM A-law");		// IBM developed A-law standard
					break;
				case 259:
					printf("ADPCM");				// Adaptive Differential Pulse Code Modulation, method for converting analog sound into low bit-rate digital signals through the use of a linear sampling algorithm.
					break;
				}		
			printf("\n\t Sampling Rate :    \t %d Hz\n",wavHeader.SampRate);		
			printf("\t Number of samples :     %d\n",NumSample);
			printf("\t Bits Per Sample (bps):  %d bits\n",wavHeader.bps);
			if(wavHeader.NumChan == 1)													// Printing number of channels ( Stereophonic or Monophonic)
				printf("\t Number of channels: \t 1 (Mono) \n");		
			else
				printf("\t Number of channels: \t 2 (Stereo) \n");
			}
			continue;
		
		}
		else if(strcmp(StrIn,"ds") == 0)						 					// Down-sample command codes
		{
			printf("Please enter times that you want to down-sample the wave file: \n");
			scanf("%d",n);
			
			unsigned __int16 *l = new unsigned __int16 [NumSample];					// Memory allocating
			unsigned __int16 *r = new unsigned __int16 [NumSample];

			for(int c=0 ; c<11 ; c++)												// for Loop to copying header to *l and *r
			{	
				l[c]=left_ch[c];
				r[c]=right_ch[c];
			}
			l[6]=0;
			r[6]=0;
			int p=11 , q=12;														// Down-sampling and putting left and right_ch channels data in *l and *r at the same time
			for(p , q ; q<NumSample ; p++, q+=2)
			{
			l[p]=left_ch[q];
			r[p]=right_ch[q];
			}
			continue;
		}
		else if(strcmp(StrIn,"us") == 0)											// Up-sample command codes
		{
			printf("Please enter times that you want to up-sample the wave file: \n");
			scanf("%d",n);
			unsigned __int16 *l = new unsigned __int16 [(NumSample*n)];				// Memory allocating
			unsigned __int16 *r = new unsigned __int16 [(NumSample*n)];
			for(int c=0 ; c<11 ; c++)												// for Loop to copying header to *l and *r
			{	
				l[c]=left_ch[c];
				r[c]=right_ch[c];
			}
			l[6]=0;
			r[6]=0;
			int p=11 , q=11;
			if(n=2)																	// Down-sampling and putting left and right_ch channels data in *l and *r at the same time
			{
				for(p , q ; q<(NumSample*n) ; p+=n, q++)
				{		
					l[p]=left_ch[q];
					l[p+1]=left_ch[q];
					r[p]=right_ch[q];
					r[p+1]=right_ch[q];
				}
			}
			if(n=3)
			{
				for(p , q ; q<(NumSample*n) ; p+=n, q++)
				{		
					l[p]=left_ch[q];
					l[p+1]=left_ch[q];
					l[p+2]=left_ch[q];
					r[p]=right_ch[q];
					r[p+1]=right_ch[q];
					r[p+2]=right_ch[q];
				}
			}
			continue;
		}
			
		else if(strcmp(StrIn,"save") == 0)										// Save command codes
		{
			printf("Please enter file save path:\n");
			scanf("%s",&path);
			FILE *NewFile;
			NewFile = fopen("path","wb+");
			for( int k = 0 ; k<(NumSample/n) ; k++)
			{
				fwrite(&l[k],2,1,NewFile);
				fwrite(&r[k],2,1,NewFile);
			}
			continue;
		}
		else if(strcmp(StrIn,"sel") == 0)										// Select command codes
		{
			int ti , tf ;
			int sampi , sampf , len;
			printf("Please enter initial time:\n");
			scanf("%d",&ti);
			printf("Please enter final time:\n");
			scanf("%d",&tf);
			sampi = ti * wavHeader.SampRate;									// Calculate the number of initial sample
			sampf = tf * wavHeader.SampRate;									// Calculate the number of final sample
			len = sampf - sampi;												// length of *l and *r to handling new command
			unsigned __int16 *l = new unsigned __int16 [len];					// Memory allocating
			unsigned __int16 *r = new unsigned __int16 [len];
			for(int c=0 ; c<11 ; c++)											// for Loop to copying header to *l and *r
			{	
				l[c]=left_ch[c];
				r[c]=right_ch[c];
			}
		
			for(int i = 11 , j=sampi ; j <= sampf ; i++ , j++)
			{
				l[i]=left_ch[j];
				r[i]=right_ch[j];
			}
			
			continue;
		}
		else if(strcmp(StrIn,"del") == 0)										// Delete command codes
		{
			
			
			printf("Please enter initial time:\n");
			scanf("%s",&ti);
			printf("Please enter final time:\n");
			scanf("%s",&tf);
			sampi = ti * wavHeader.SampRate;									// Calculate the number of initial sample
			sampf = tf * wavHeader.SampRate;									// Calculate the number of final sample
			len = sampf - sampi;												// length of *l and *r to handling new command
			unsigned __int16 *l = new unsigned __int16 [len];					// Memory allocating
			unsigned __int16 *r = new unsigned __int16 [len];
			for(int c=0 ; c<11 ; c++)											// for Loop to copying header to *l and *r
			{		
				l[c]=left_ch[c];
				r[c]=right_ch[c];
			}
			for(int i = 11 ; i < sampi ; i++)									// Except (0 - sampi) and (sampf - NumSample)
			{																	// other part of Left and right_ch will be deleted.
				l[i]=left_ch[i];
				r[i]=right_ch[i];

			}
			for(int j = sampf ; j < NumSample ; j++)
			{	
				l[j]=left_ch[j];
				r[j]=right_ch[j];
			}
			continue;
		}
		else if(strcmp(StrIn,"exit") == 0)										// Exit command codes
		{
		exit(EXIT_SUCCESS);														// This part exits program and send an event 
																				// to windows event manager and notify 
		}																		// program exited in successfull manner.
		else
		{	
		printf("\t The Command that you entered is wrong \n");												 																		
		}																		
																
	}

	_fcloseall();																// Closes all streams
																				// Deleting the memory that allocated before
	delete left_ch;
	delete right_ch;
	
	delete l;
	delete r;
	
	return 0;
	
}
int wavOpen(char path[])															// Open function codes
{
	wavFile = fopen(path,"rb");														// Open the file in read binary mode
	if(wavFile != NULL)																// Check file to sure about having data in file													
    {
		printf(" \t->File opened Successfully\n");
		int headerSize = sizeof(wavHeader);											// Calculates the size of wav header
		fread(&wavHeader,headerSize,1,wavFile);										// Puts header in wavHeader structure
		NumSample = (8*wavHeader.Subchunk2Size)/(wavHeader.bps * wavHeader.NumChan);// Calculating total number of samples
		left_ch = new unsigned __int16 [NumSample];
		right_ch = new unsigned __int16 [NumSample];					// Memory allocating
		
		for( int i = 0 ; i<NumSample ; i++)											// Reading wavFile and saving data
		{																			// in *left and *right_ch
		fread(&left_ch[i],2,1,wavFile);	
		fread(&right_ch[i],2,1,wavFile);
		}
		
		return 1;
	}
	else																			// Prints proper error if file not exist or open with error
	{
		printf(" \t->File not found, check path correction and file existence\n");
        return -1;

	}
}
int getFileSize(FILE *inFile)														// Seek end of file and with calling 
{																					// file cursor position calculate 
 int fileSize = 0;																	// file size in byte. 
    fseek(inFile,0,SEEK_END);	
    fileSize=ftell(inFile);
    rewind(inFile);
 return fileSize;
}
void print()
{
printf("__@@@@@_______________________@@\n");
printf("__@@__@@_____________________@@@@\n");
printf("___@@__@@____________________@@@@@\n");
printf("____@@_@@@___________________@@__@@\n");
printf("____@@_@@@___________________@@__@@\n");
printf("_____@@@@@___________________@@_@@@\n");
printf("_____@@@@______________@@@@ @@__@@\n");
printf("____@@@@_____________@@@@@@@@@_@@\n");
printf("___@@@_@@__@@@_______@@@@@@@@\n");
printf("__@@@___@@@@@@@@@_____@@@@@@\n");
printf("_@@@@__@@@@___@@@@_______________________@@@\n");
printf("_@@@__@@@@_@@@__@@@__________________@@@@@@\n");
printf("@@@@__@@@@@@@@@__@@@______________@@@@@@@@@\n");
printf("_@@@__@@@_@@__@__@@@___________@@@@@@@___@@\n");
printf("_@@@@__@@@@@@@@__@@________@@@@@@@@______@@\n");
printf("__@@@@____@@@__@@@______@@@@@@@@@@_______@@\n");
printf("___@@@@@@___@@@@@_____@@@@@@@@___@@_______@@\n");
printf("_____@@@@@@@@@@________@@@@@_____@@___@@@@@@\n");
printf("________@@@_@@@________@@________@@__@@@@@@@\n");
printf("_______@@@@@_@@_______@@@_____@@@@___@@@@@\n");
printf("_______@@@___@@_________@@___@@@@@@\n");
printf("_________@@@@@__________@@___@@@@@@\n");
printf("_________________________@@__@@@@\n");
printf("_____________________@@@@@@\n");
printf("____________________@@@@@@@\n");
printf("____________________@@@@@@\n");
}