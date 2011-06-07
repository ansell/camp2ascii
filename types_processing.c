/**************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*             Copyright SLF/WSL, 03/2007 - www.slf.ch                     *
**************************************************************************/
/* Here, we put all functions related to the details of reading a given data type */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>		/* for time calculations */
#include <math.h>
#include <inttypes.h>		/* C99 portable data types */
#include "limits.h"
#include "types_processing.h"

/* Functions for swapping byte order. */
/* Inspired from  http://www.gamedev.net/reference/articles/article2091.asp (GPL)*/
/* Itself inspired by Quake2 source code */

int16_t ReadInt2(frame_structure *frame) {
/* This function reads an int coded on 2 bytes */
/* It also updates the char index to point to the next byte */
	int16_t result;

	#ifdef BIGENDIAN
		result=(int16_t)((frame->raw[frame->cindex+0] << 8) + frame->raw[frame->cindex+1]);
	#else
		result=(int16_t)((frame->raw[frame->cindex+1] << 8) + frame->raw[frame->cindex+0]);
	#endif
	frame->cindex += 2;
	return result;
}

uint16_t ReadUInt2(frame_structure *frame) {
/* This function reads an int coded on 2 bytes */
/* It also updates the char index to point to the next byte */
	uint16_t result;

	#ifdef BIGENDIAN
		result=(uint16_t)((frame->raw[frame->cindex+0] << 8) + frame->raw[frame->cindex+1]);
	#else
		result=(uint16_t)((frame->raw[frame->cindex+1] << 8) + frame->raw[frame->cindex+0]);
	#endif
	frame->cindex += 2;
	return result;
}

int16_t ReadInt2Swap(frame_structure *frame) {
/* This function reads an int coded on 2 bytes */
/* It also updates the char index to point to the next byte */
	int16_t result;

	#ifndef BIGENDIAN
		result=(int16_t)((frame->raw[frame->cindex+0] << 8) + frame->raw[frame->cindex+1]);
	#else
		result=(int16_t)((frame->raw[frame->cindex+1] << 8) + frame->raw[frame->cindex+0]);
	#endif
	frame->cindex += 2;
	return result;
}

uint16_t ReadUInt2Swap(frame_structure *frame) {
/* This function reads an int coded on 2 bytes */
/* It also updates the char index to point to the next byte */
	uint16_t result;

	#ifndef BIGENDIAN
		result=(uint16_t)((frame->raw[frame->cindex+0] << 8) + frame->raw[frame->cindex+1]);
	#else
		result=(uint16_t)((frame->raw[frame->cindex+1] << 8) + frame->raw[frame->cindex+0]);
	#endif
	frame->cindex += 2;
	return result;
}

int32_t ReadInt4(frame_structure *frame) {
/* This function reads an int coded on 4 bytes */
/* It also updates the char index to point to the next byte */
	int32_t result;

	#ifdef BIGENDIAN
		result=(int32_t)( frame->raw[frame->cindex+3] + (frame->raw[frame->cindex+2] << 8) + (frame->raw[frame->cindex+1] << 16) + (frame->raw[frame->cindex+0] << 24) );
	#else
		result=(int32_t)( frame->raw[frame->cindex+0] + (frame->raw[frame->cindex+1] << 8) + (frame->raw[frame->cindex+2] << 16) + (frame->raw[frame->cindex+3] << 24) );
	#endif
	frame->cindex += 4;

	return result;
}

uint32_t ReadUInt4(frame_structure *frame) {
/* This function reads an int coded on 4 bytes */
/* It also updates the char index to point to the next byte */
	uint32_t result;

	#ifdef BIGENDIAN
		result=(uint32_t)( frame->raw[frame->cindex+3] + (frame->raw[frame->cindex+2] << 8) + (frame->raw[frame->cindex+1] << 16) + (frame->raw[frame->cindex+0] << 24) );
	#else
		result=(uint32_t)( frame->raw[frame->cindex+0] + (frame->raw[frame->cindex+1] << 8) + (frame->raw[frame->cindex+2] << 16) + (frame->raw[frame->cindex+3] << 24) );
	#endif
	frame->cindex += 4;

	return result;
}

int32_t ReadInt4Swap(frame_structure *frame) {
/* This function reads an int coded on 4 bytes */
/* It also updates the char index to point to the next byte */
	int32_t result;

	#ifdef BIGENDIAN
		result=(int32_t)( frame->raw[frame->cindex+0] + (frame->raw[frame->cindex+1] << 8) + (frame->raw[frame->cindex+2] << 16) + (frame->raw[frame->cindex+3] << 24) );
	#else
		result=(int32_t)( frame->raw[frame->cindex+3] + (frame->raw[frame->cindex+2] << 8) + (frame->raw[frame->cindex+1] << 16) + (frame->raw[frame->cindex+0] << 24) );
	#endif
	frame->cindex += 4;

	return result;
}

uint32_t ReadUInt4Swap(frame_structure *frame) {
/* This function reads an int coded on 4 bytes */
/* It also updates the char index to point to the next byte */
	uint32_t result;

	#ifdef BIGENDIAN
		result=(uint32_t)( frame->raw[frame->cindex+0] + (frame->raw[frame->cindex+1] << 8) + (frame->raw[frame->cindex+2] << 16) + (frame->raw[frame->cindex+3] << 24) );
	#else
		result=(uint32_t)( frame->raw[frame->cindex+3] + (frame->raw[frame->cindex+2] << 8) + (frame->raw[frame->cindex+1] << 16) + (frame->raw[frame->cindex+0] << 24) );
	#endif
	frame->cindex += 4;

	return result;
}


float32_t ReadFloat4(frame_structure *frame) {
/* This function reads a float coded on 4 bytes */
/* It also updates the char index to point to the next byte */
	union
	{
		float32_t f;
		uint8_t b[4];
	} dat2;

	#ifndef BIGENDIAN
		dat2.b[0] = frame->raw[frame->cindex];
		dat2.b[1] = frame->raw[frame->cindex+1];
		dat2.b[2] = frame->raw[frame->cindex+2];
		dat2.b[3] = frame->raw[frame->cindex+3];
	#else
		dat2.b[0] = frame->raw[frame->cindex+3];
		dat2.b[1] = frame->raw[frame->cindex+2];
		dat2.b[2] = frame->raw[frame->cindex+1];
		dat2.b[3] = frame->raw[frame->cindex];
	#endif

	frame->cindex += 4;
	return dat2.f;
}

float32_t ReadFloat4Swap(frame_structure *frame) {
/* This function reads a float coded on 4 bytes */
/* It also updates the char index to point to the next byte */
	union
	{
		float32_t f;
		uint8_t b[4];
	} dat2;

	#ifdef BIGENDIAN
		dat2.b[0] = frame->raw[frame->cindex];
		dat2.b[1] = frame->raw[frame->cindex+1];
		dat2.b[2] = frame->raw[frame->cindex+2];
		dat2.b[3] = frame->raw[frame->cindex+3];
	#else
		dat2.b[0] = frame->raw[frame->cindex+3];
		dat2.b[1] = frame->raw[frame->cindex+2];
		dat2.b[2] = frame->raw[frame->cindex+1];
		dat2.b[3] = frame->raw[frame->cindex];
	#endif

	frame->cindex += 4;
	return dat2.f;
}

/////////////////////////////////////////////////////////////////////////////////////////
//////////////////// higher level functions, adapted to Cambell types ///////////////////
/////////////////////////////////////////////////////////////////////////////////////////

// SPECIAL FLOATING POINTS
int Read_FP2(frame_structure *frame,const config_structure *config) {
	//2 bytes float
	SIresult=(int16_t)ReadInt2Swap(frame);
	sign     = ((0x8000 & SIresult) >> 15 );
	exponent = ((0x6000 & SIresult) >> 13 );
	mantissa = ((0x1FFF & SIresult)       );
	switch(exponent) {
		case 0: Fresult=(float)mantissa;
		break;
		case 1: Fresult=(float)mantissa*1e-1;
		break;
		case 2: Fresult=(float)mantissa*1e-2;
		break;
		default: Fresult=(float)mantissa*1e-3;
	}
	if(Fresult>=frame->fp2_nan || isnan(Fresult))
		fprintf(config->output,config->NANs);
	else {
		if(sign ==0)
			fprintf(config->output,config->fp2_format,Fresult);
		else
			fprintf(config->output,config->fp2_format,-Fresult);
	}

	return EXIT_SUCCESS;
}

int Read_FP4(frame_structure *frame,const config_structure *config) {
	//in progress... but it should work! see Appendix C of CR10X manual
	Iresult=ReadInt4(frame);
	sign     = ((0x80000000 & Iresult) >> 31 );
	exponent = ((0x7F000000 & Iresult) >> 24 );//isn't it 23 instead?
	mantissa = ((0x00FFFFFF & Iresult)       );
	Fresult=(float)mantissa/16777216.0*(float)pow(2.0,(double)(exponent-64));//TODO: optimize...(exponent is INT)
#ifdef DEBUG
	fprintf(stderr,"0x%0x->",Iresult);
	fprintf(stderr,"(%d)_%d*10^(%d)->%g\n",sign,mantissa,exponent,Fresult);
#endif
	fprintf(config->output,"\"FP4\"");
	/*if(Fresult>=frame->fp4_nan || isnan(Fresult))
		fprintf(config->output,config->NANs,config->separator);
	else {
		if(sign ==0)
			fprintf(config->output,config->fp4_format,config->separator,Fresult);
		else
			fprintf(config->output,config->fp4_format,config->separator,-Fresult);
}*/

	return EXIT_SUCCESS;
}

//IEEE4 FLOATING POINTS
int Read_IEEE4(frame_structure *frame,const config_structure *config) {
	//IEEE4 & L -> Little endian		LSB
	Fresult=ReadFloat4(frame);
	if(isnan(Fresult))
		fprintf(config->output,config->NANs);
	else
		fprintf(config->output,config->floats_format,Fresult);

	return EXIT_SUCCESS;
}

int Read_IEEE4B(frame_structure *frame,const config_structure *config) {
	//IEEE4, Big endian version		MSB
	Fresult=ReadFloat4Swap(frame);
	if(isnan(Fresult))
		fprintf(config->output,config->NANs);
	else
		fprintf(config->output,config->floats_format,Fresult);

	return EXIT_SUCCESS;
}

//INTEGERS
int Read_USHORT(frame_structure *frame,const config_structure *config) {
	//2 bytes unsigned int			LSB
	Iresult=(int32_t)ReadUInt2(frame);
	if(Iresult>=frame->uint2_nan || isnan(Iresult))
		fprintf(config->output,config->NANs);
	else {
		fprintf(config->output,config->ints_format,Iresult);
	}

	return EXIT_SUCCESS;
}

int Read_SHORT(frame_structure *frame,const config_structure *config) {
	//2 bytes signed int			LSB
	Iresult=(int32_t)ReadInt2(frame);
	if(Iresult>=frame->uint2_nan || isnan(Iresult))
		fprintf(config->output,config->NANs);
	else {
		fprintf(config->output,config->ints_format,Iresult);
	}

	return EXIT_SUCCESS;
}

int Read_UINT2(frame_structure *frame,const config_structure *config) {
	//2 bytes unsigned int			MSB
	Iresult=(int32_t)ReadUInt2Swap(frame);
	if(Iresult>=frame->uint2_nan || isnan(Iresult))
		fprintf(config->output,config->NANs);
	else {
		fprintf(config->output,config->ints_format,Iresult);
	}

	return EXIT_SUCCESS;
}

int Read_INT2(frame_structure *frame,const config_structure *config) {
	//2 bytes signed int			MSB
	Iresult=(int32_t)ReadInt2Swap(frame);
	if(SIresult>=frame->uint2_nan || isnan(Iresult))
		fprintf(config->output,config->NANs);
	else {
		fprintf(config->output,config->ints_format,Iresult);
	}

	return EXIT_SUCCESS;
}

int Read_UINT4(frame_structure *frame,const config_structure *config) {
	//4 bytes unsigned int			MSB
	fprintf(config->output,config->ints_format,ReadUInt4Swap(frame));

	return EXIT_SUCCESS;
}

int Read_INT4(frame_structure *frame,const config_structure *config) {
	//4 bytes signed int				MSB
	fprintf(config->output,config->ints_format,ReadInt4Swap(frame));

	return EXIT_SUCCESS;
}

int Read_ULONG(frame_structure *frame,const config_structure *config) {
	//4 bytes unsigned int			LSB
	fprintf(config->output,config->ints_format,ReadUInt4(frame));

	return EXIT_SUCCESS;
}

int Read_LONG(frame_structure *frame,const config_structure *config) {
	//4 bytes signed int			LSB
	fprintf(config->output,config->ints_format,ReadInt4(frame));

	return EXIT_SUCCESS;
}

//BOOLEANS
int Read_BOOL(frame_structure *frame,const config_structure *config) {
	//1 byte boolean: either FF or 00
	if((int16_t)(frame->raw[frame->cindex]) == 0)
		fprintf(config->output,config->bool_false);
	else
		fprintf(config->output,config->bool_true);
	frame->cindex += 1;

	return EXIT_SUCCESS;
}

int Read_BOOL2(frame_structure *frame,const config_structure *config) {
	//2 bytes boolean: either FFFF or 0000 - TODO: check
	if(ReadInt2Swap(frame)==0)
		fprintf(config->output,config->bool_false);
	else
		fprintf(config->output,config->bool_true);

	return EXIT_SUCCESS;
}

int Read_BOOL4(frame_structure *frame,const config_structure *config) {
	//4 bytes boolean: either FFFFFFFF or 00000000
	if(ReadInt4Swap(frame)==0)
		fprintf(config->output,config->bool_false);
	else
		fprintf(config->output,config->bool_true);

	return EXIT_SUCCESS;
}

//TIMESTAMPS
int Read_NSec(frame_structure *frame,const config_structure *config) {
	//2x4 bytes (2xUINT4), timestamps and nanoseconds	MSB
	time_offset = ReadUInt4Swap(frame)+TO_EPOCH;
	strftime(time_str,MAX_FIELD,config->time_format,gmtime(&time_offset));
	//we print as {separator}{timestamp}.{nanoseconds}
	fprintf(config->output,config->nsec_format,time_str,ReadUInt4Swap(frame));

	return EXIT_SUCCESS;
}

int Read_SecNano(frame_structure *frame,const config_structure *config) {
	//2x4 bytes (2xULONG), timestamps and nanoseconds	LSB
	time_offset = ReadUInt4(frame)+TO_EPOCH;
	strftime(time_str,MAX_FIELD,config->time_format,gmtime(&time_offset));
	//we print as {separator}{timestamp}.{nanoseconds}
	fprintf(config->output,config->nsec_format,time_str,ReadUInt4(frame));

	return EXIT_SUCCESS;
}

//ASCII
int Read_ASCII(frame_structure *frame,const config_structure *config) {
	//this is for the ASCII strings, each char on 1 byte
	count=frame->field_options[field_index];
	fprintf(config->output,config->strings_beg);
	for(;count>0;count--) {
		Cresult=frame->raw[frame->cindex++];
		if(Cresult == 0) {//the end of the string has been reached
			frame->cindex += count-1;
			count=0;
		} else
			fprintf(config->output,"%c",Cresult);
	}
	fprintf(config->output,config->strings_end);

	return EXIT_SUCCESS;
}

