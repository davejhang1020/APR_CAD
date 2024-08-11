
#define SWAP(A,B,C) C=A,A=B,B=C

void Printf4Char(char Ch)
{
	if(Ch<=9)
		printf("<%c>",'0'+Ch);
	else
		printf("<%c>",'A'+Ch-10);
}

void Printf8Char(char Ch)
{
	char CharA,CharB;
	CharA=(Ch & 0xF0)>>4;
	CharB=(Ch & 0x0F);
	
	Printf4Char(CharA);
	Printf4Char(CharB);
}

int CalcTwoCharNumber(char A,char B)
{
	int Number=0;
	Number=(Number | (unsigned char)A)<<8;
	Number=(Number | (unsigned char)B);
	
	return Number;
}

char CalcTwoCharOpCode(char A,char B)
{
	char OpCode=0;
	
	OpCode=OpCode | (A<<8);
	OpCode=OpCode | (B<<0);
	
	return OpCode;
}

void RotateXY(char ArrayChar[],int ArrayCharSize)
{
#if DEBUG
	printf(" In RotateXY \n");
	
	printf(" -----> OpCode:  ");
	
	for(int i=1;i<=4;i++)
	{
		Printf8Char(ArrayChar[i]);
		printf(" , ");
	}
	printf("\n");
#endif
	unsigned int X,Y;
	unsigned int Temp;
	unsigned char WriteBackData;
#if DEBUG
	printf(" -----> XY:  \n");
#endif
	for(int i=1;i<=(ArrayCharSize-4)>>3;i++)
	{
		X=0,Y=0;
		
		// Set X Coordinate
		for(int j=1;j<=4;j++)
		{
#if DEBUG
			printf(" ----->");
			printf(" Data:  ");
			Printf8Char(ArrayChar[4+(8*(i-1)+j)]);
			printf(" , ");
#endif
			Temp=0;
			Temp=(unsigned char)(ArrayChar[4+(8*(i-1)+j)]);
			Temp<<=(8*(4-j));
			X|= Temp;
#if DEBUG
			printf(" Shift %02d Bits Data:  [%08X] ",8*(4-j),Temp);
			printf(" , ");
			printf(" Current X:  [%08X] ",X);
			printf("\n");
#endif
		}
		// Set Y Coordinate
		for(int j=5;j<=8;j++)
		{
#if DEBUG
			printf(" ----->");
			printf(" Data:  ");
			Printf8Char(ArrayChar[4+(8*(i-1)+j)]);
			printf(" , ");
#endif
			Temp=0;
			Temp=(unsigned char)(ArrayChar[4+(8*(i-1)+j)]);
			Temp<<=(8*(8-j));
			Y|= Temp;
#if DEBUG
			printf(" Shift %02d Bits Data:  [%08X] ",8*(8-j),Temp);
			printf(" , ");
			printf(" Current Y:  [%08X] ",Y);
			printf("\n");
#endif
		}
#if DEBUG
		printf(" -----> Original Coordinate(Decimal)     - X:  %12d , Y:  %12d \n",X,Y);
		printf(" -----> Original Coordinate(HexaDecimal) - X:      %08X , Y:      %08X \n",X,Y);
#endif

// START Method

// Original Method
#if 1
		X=((0)*X)+((-1)*Y);
		Y=((-1)*(-1)*X)+((0)*Y);
#endif

// Improve Method I
#if 0
		// SWAP X And Y Coordinate
		SWAP(X,Y,Temp);
		// Let X * (-1)
		X=(-1)*X;
#endif

// Improve Method II
#if 0
		// SWAP X And Y Coordinate
		SWAP(X,Y,Temp);
		// Let X * (-1)
		X=(~X)+1;
#endif

// END Method

#if DEBUG
		printf(" -----> SWAP X And Y Coordinate + Let X * (-1) \n");
		
		printf(" -----> Transfer Coordinate(Decimal)     - X:  %12d , Y:  %12d \n",X,Y);
		printf(" -----> Transfer Coordinate(HexaDecimal) - X:      %08X , Y:      %08X \n",X,Y);
#endif
		// Write Back Data To X
#if DEBUG
		printf(" ----->");
		printf(" X:  ");
#endif
		Temp=X;
		for(int j=4;j>=1;j--)
		{
			WriteBackData=(Temp>>(8*(4-j)));
			ArrayChar[4+(8*(i-1)+j)]=WriteBackData;
#if DEBUG
			Printf8Char(ArrayChar[4+(8*(i-1)+j)]);
			printf(" , ");
#endif
		}
#if DEBUG
		printf("\n");
#endif
		// Write Back Data To Y
#if DEBUG
		printf(" ----->");
		printf(" Y:  ");
#endif
		Temp=Y;
		for(int j=8;j>=5;j--)
		{
			WriteBackData=(Temp>>(8*(8-j)));
			ArrayChar[4+(8*(i-1)+j)]=WriteBackData;
#if DEBUG
			Printf8Char(ArrayChar[4+(8*(i-1)+j)]);
			printf(" , ");
#endif
		}
#if DEBUG
		printf("\n");
#endif
	}
}

int RotateSREFTEXTAngle(char ArrayChar[],int ArrayCharSize)
{
#if DEBUG
	printf(" In RotateSREFTEXTAngle \n");
	
	printf(" -----> OpCode:  ");
	for(int i=1;i<=4;i++)
	{
		Printf8Char(ArrayChar[i]);
		printf(" , ");
	}
	printf("\n");
	
	printf(" -----> Remain Data:  ");
	for(int i=5;i<=ArrayCharSize;i++)
	{
		Printf8Char(ArrayChar[i]);
		printf(" , ");
	}
	printf("\n");
#endif
	if(ArrayChar[5]==(char)0x42)
	{
		// Angle 90 --> Angle 180
		if((ArrayChar[6]==(char)0x5A))
		{
#if DEBUG
			printf(" -----> Angle 90 To Angle 180 \n");
#endif
			ArrayChar[5]=0x42;
			ArrayChar[6]=0xB4;
			ArrayChar[7]=0x00;
			return ArrayCharSize+1;
		}
		
		// Angle 180 --> Angle 270
		if(ArrayChar[6]==(char)0xB4)
		{
#if DEBUG
			printf(" -----> Angle 180 To Angle 270 \n");
#endif
			ArrayChar[5]=0x43;
			ArrayChar[6]=0x10;
			ArrayChar[7]=0xE0;
			return ArrayCharSize+1;
		}
	}
	
	// Angle 270 --> Angle 0
	if((ArrayChar[5]==(char)0x43) && (ArrayChar[6]==(char)0x10) && (ArrayChar[7]==(char)0xE0))
	{
#if DEBUG
		printf(" -----> Angle 270 To Angle 0 \n");
#endif
		return 0;
	}
	
	// Angle 0 --> Angle 90
	// ==> NOT HERE HANDLE
	
	// MeaningLess Return Value
	return -1;
}
