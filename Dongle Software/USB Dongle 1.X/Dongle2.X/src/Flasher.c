//---------------------------------------------
#include <xc.h>
#include <string.h>
#include <stdint.h>
#include <dsp.h>

typedef short          Word16;
typedef unsigned short UWord16;
typedef long           Word32;
typedef unsigned long  UWord32;
//---
typedef union tuReg32
{
	UWord32 Val32;

	struct
	{
		UWord16 LW;
		UWord16 HW;
	} Word;

	char Val[4];
} uReg32;

extern UWord32 ReadLatch(UWord16, UWord16);
extern UWord16 Erase(UWord16, UWord16, UWord16);
extern UWord16 WriteLatch(UWord16 , UWord16 , UWord16 , UWord16 );
extern UWord16 WriteMem(UWord16);
//-----------------------------------------------------------------------
// Local Memory
//-----------------------------------------------------------------------
char sysex_data_array[1536 + 1];
unsigned char flash_block_address;
//-----------------------------------------------------------------------
#define PM_ROW_ERASE 		0x4042
#define PM_ROW_WRITE 		0x4001
#define PM_ROW_SIZE 			512

#define BAD		0
#define GOOD	1
//-----------------------------------------------------------------------
void WritePM(char * ptrData, uReg32 SourceAddr)
{
	int    Size, Size1;
	uReg32 Temp;
	uReg32 TempAddr;
	uReg32 TempData;

	for(Size = 0, Size1 = 0 ; Size < PM_ROW_SIZE ; Size++)
		{
			Temp.Val[0] = ptrData[Size1 + 0];
			Temp.Val[1] = ptrData[Size1 + 1];
			Temp.Val[2] = ptrData[Size1 + 2];
			Temp.Val[3] = 0;
			Size1 += 3;
	   	WriteLatch(SourceAddr.Word.HW, SourceAddr.Word.LW, Temp.Word.HW, Temp.Word.LW);		
			// Device ID errata workaround: Save data at any address that has LSB 0x18
			if((SourceAddr.Val32 & 0x0000001F) == 0x18)
				{
					TempAddr.Val32 = SourceAddr.Val32;
					TempData.Val32 = Temp.Val32;
				}
			if((Size != 0) && (((Size + 1) % 64) == 0))
				{
				  	// Device ID errata workaround: Reload data at address with LSB of 0x18
	      		WriteLatch(TempAddr.Word.HW, TempAddr.Word.LW,TempData.Word.HW,TempData.Word.LW);
					WriteMem(PM_ROW_WRITE);
				}
			SourceAddr.Val32 = SourceAddr.Val32 + 2;
		}   
}
//-----------------------------------------------------------------------
unsigned char VerifyPM(char * ptrData, uReg32 SourceAddr)
{
	unsigned int n, point, status;
	uReg32 Temp;
	
	point = 0;
	status = GOOD;
	for (n = 0 ; n < 512 ; n++)
		{
			Temp.Val32 = ReadLatch(SourceAddr.Word.HW, SourceAddr.Word.LW);			    
			if ((Temp.Val[0] != ptrData[point + 0]) ||
			    (Temp.Val[1] != ptrData[point + 1]) ||
			    (Temp.Val[2] != ptrData[point + 2]))  
				status = BAD;
			point = point + 3;
			SourceAddr.Val32 = SourceAddr.Val32 + 2;
		}
	return status;
}
//-----------------------------------------------------------------------
unsigned char write_512(void)
{
	uReg32 SourceAddr;

	SourceAddr.Val32 = (unsigned long)flash_block_address * 1024;
	Erase(SourceAddr.Word.HW, SourceAddr.Word.LW, PM_ROW_ERASE);
	WritePM(sysex_data_array, SourceAddr);
	return VerifyPM(sysex_data_array, SourceAddr);
}		
//---------------------------------------------
unsigned char read_revision(uReg32 SourceAddr)
{
	uReg32 Temp;
	
	Temp.Val32 = ReadLatch(SourceAddr.Word.HW, SourceAddr.Word.LW);
	return(Temp.Val[0]);	
}	
//---------------------------------------------