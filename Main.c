
# include <stdio.h>
# include <stdlib.h>
# include <limits.h>
# include "Calc.h"

# define DEBUG 0

int main(int argc,char* argv[])
{
    FILE *ReadFile=fopen(argv[1],"r");
    if(!ReadFile)
	{
        printf(" Open File %s Error \n",argv[1]);
    }
	
	FILE *WriteFile=fopen(argv[2],"wb");
	if(!WriteFile)
	{
        printf(" Open File %s Error \n",argv[2]);
    }
	
	int ReadFileChCount=1;
	int MaxCount=INT_MAX;
	char NumberOpCode;
	char Ch;
	
	int FirstCell=2;
	int StartBOUNDARY=-1;
	int StartSREF=-1;
	int StartTEXT=-1;
	int StartPATH=-1;
	
	int StartXY=-1;
	int StartANGLE=0;
	
	char *CharPtr=NULL;
	char CharA,CharB;
	
	char ArrayCharAngle90[]={0x00,0x0C,0x1C,0x05,0x42,0x5A,0x00,0x00,0x00,0x00,0x00,0x00};
	
    while(!feof(ReadFile))
	{
		fread(&Ch,sizeof(char),1,ReadFile);

#if DEBUG
		printf(" ++++++++++++++++++++ ReadFileChCount:  %10d , MaxCount:  %10d ++++++++++++++++++++ \n",ReadFileChCount,MaxCount);
#endif
		if(ReadFileChCount==1)
		{
#if DEBUG
			printf(" Number 1:  ");   Printf8Char(Ch);   printf("\n");
#endif
			CharA=Ch;
		}
		else
		if(ReadFileChCount==2)
		{
#if DEBUG
			printf(" Number 2:  ");   Printf8Char(Ch);   printf("\n");
#endif
			CharB=Ch;
#if DEBUG
			printf(" =====> TWO NUMBER(HexaDecimal):  ");
			Printf8Char(CharA);   printf(" , ");
			Printf8Char(CharB);   printf(" \n");
#endif
			MaxCount=CalcTwoCharNumber(CharA,CharB);
#if DEBUG
			printf(" =====> TWO NUMBER(Decimal):  %d \n",MaxCount);
#endif
			if(MaxCount==0)
				return -1;
			
			CharPtr=(char *) malloc((MaxCount+1)*sizeof(char));
			CharPtr[1]=CharA;
			CharPtr[2]=CharB;
		}
		else
		if(ReadFileChCount==3)
		{
			CharPtr[ReadFileChCount]=Ch;
#if DEBUG
			printf(" OpCode 1:  ");   Printf8Char(Ch);   printf("\n");
#endif
			CharA=Ch;
		}
		else
		if(ReadFileChCount==4)
		{
			CharPtr[ReadFileChCount]=Ch;
#if DEBUG
			printf(" OpCode 2:  ");   Printf8Char(Ch);   printf("\n");
#endif
			CharB=Ch;
#if DEBUG
			printf(" =====> OpCode(HexaDecimal):  ");
			Printf8Char(CharA);   printf(" , ");
			Printf8Char(CharB);   printf(" \n");
			printf(" =====> OpCode - JUST SHOW STRNAME, BOUNDARY, SREF, TEXT, PATH, ANGLE, XY, STRING, ENDEL, ENDSTR, ENDLIB) \n");
#endif
			// STRNAME
			if((CharA==0X06) && (CharB==0X06))
			{
				FirstCell--;
#if DEBUG
				printf(" =====> OpCode:  STRNAME \n");
#endif
			}
			else
			// BOUNDARY
			if((CharA==0X08) && (CharB==0X00))
			{
				StartBOUNDARY=1;
#if DEBUG
				printf(" =====> OpCode:  BOUNDARY \n");
#endif
			}
			else
			// SREF
			if((CharA==0X0A) && (CharB==0X00))
			{
				StartSREF=1;
#if DEBUG
				printf(" =====> OpCode:  SREF \n");
#endif
			}
			else
			// TEXT
			if((CharA==0X0C) && (CharB==0X00))
			{
				StartTEXT=1;
#if DEBUG
				printf(" =====> OpCode:  TEXT \n");
#endif
			}
			else
			// PATH
			if((CharA==0X09) && (CharB==0X00))
			{
				StartPATH=1;
#if DEBUG
				printf(" =====> OpCode:  PATH \n");
#endif
			}
			else
			// ANGLE
			if((CharA==0X1C) && (CharB==0X05))
			{
				StartANGLE=2;
#if DEBUG
				printf(" =====> OpCode:  ANGLE \n");
#endif
			}
			else
			// XY
			if((CharA==0X10) && (CharB==0X03))
			{
				StartXY=1;
#if DEBUG
				printf(" =====> OpCode:  XY \n");
#endif
			}
			else
			// STRING
			if((CharA==0X19) && (CharB==0X06))
			{
				StartXY=0;
#if DEBUG
				printf(" =====> OpCode:  STRING \n");
#endif
			}
			else
			// ENDEL
			if((CharA==0X11) && (CharB==0X00))
			{
				StartBOUNDARY=0;
				StartSREF=0;
				StartTEXT=0;
				StartPATH=0;
				StartXY=0;
				StartANGLE=0;
#if DEBUG
				printf(" =====> OpCode:  ENDEL \n");
#endif
			}
			else
			// ENDSTR
			if((CharA==0X07) && (CharB==0X00))
			{
				FirstCell--;
#if DEBUG
				printf(" =====> OpCode:  ENDSTR \n");
#endif
			}
			else
			// ENDLIB
			if((CharA==0X04) && (CharB==0X00))
			{
#if DEBUG
				printf(" =====> OpCode:  ENDLIB \n");
#endif
			}
		}
		else
		if(ReadFileChCount>=5)
		{
			CharPtr[ReadFileChCount]=Ch;
#if DEBUG
			printf(" Data:  ");   Printf8Char(Ch);   printf("\n");
#endif
		}
		ReadFileChCount++;
		
		// Read Complete ONE Section GDS Data
		if(ReadFileChCount==(MaxCount+1))
		{
			if(FirstCell>=0)
			{
				// Handle Boundary / PATH XY
				if((StartBOUNDARY==1) || (StartPATH==1))
				{
					if(StartXY==1)
					{
#if DEBUG
						printf(" -----> Go To Rotate Boundary / PATH XY =====> ");
#endif
						RotateXY(CharPtr,ReadFileChCount-1);
					}
				}
				
				// Handle SREF / TEXT Angle And XY
				if((StartSREF==1) || (StartTEXT==1))
				{
					if(StartANGLE==2)
					{
#if DEBUG
						printf(" -----> Go To Rotate SREF / TEXT Angle =====> ");
#endif
						ReadFileChCount=RotateSREFTEXTAngle(CharPtr,ReadFileChCount-1);
						StartANGLE--;
					}
					
					if(StartXY==1)
					{
						if(StartANGLE==0)
						{
#if DEBUG
							printf(" -----> WILL ADD SREF / TEXT ANGLE INFORMATION \n");
#endif
							for(int i=0;i<(sizeof(ArrayCharAngle90)/sizeof(char));i++)
								fwrite(&ArrayCharAngle90[i],sizeof(char),1,WriteFile);
						}
#if DEBUG
						printf(" -----> Go To Rotate SREF / TEXT XY =====> ");
#endif
						RotateXY(CharPtr,ReadFileChCount-1);
					}
				}
			}
			
			// Write Data To GDS File
#if DEBUG
			printf(" XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX \n");
			printf(" Write One Section Data To GDS File:  ");
#endif
			for(int i=1;i<ReadFileChCount;i++)
			{
#if DEBUG
				printf("[");   Printf8Char(CharPtr[i]);   printf("]");   printf(" , ");
#endif
				fwrite(&CharPtr[i],sizeof(char),1,WriteFile);
			}
#if DEBUG
			printf("\n");
#endif
			free(CharPtr);
			CharPtr=NULL;
			
			ReadFileChCount=1;
			MaxCount=INT_MAX;
#if DEBUG
			printf(" XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX \n");
			printf("\n");   printf("\n");   printf("\n");   printf("\n");   printf("\n");
#endif
		}
    }
	
    fclose(ReadFile);
	fclose(WriteFile);
	
	return 0;
}
