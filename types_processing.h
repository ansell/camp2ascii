/**************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*             Copyright SLF/WSL, 03/2007 - www.slf.ch                     *
**************************************************************************/
/* Here, we put all functions related to the details of reading a given data type */

#ifndef TYPES_PROCESSING_H
#define TYPES_PROCESSING_H

#include "generic_functions.h"
#include "structures.h"

//typedefs needed before full C99 support
typedef float float32_t;	//in C99, this could be removed (being a supported type)

#define TRUNC_FACTOR 1.00001	//float->int conversions will be multiplied by this factor in order to account for trunck errors
				//see http://docs.sun.com/source/806-3568/ncg_goldberg.html for more details (Godlberg paper)

//some needed global variables
int count;
int field_index;
int8_t Cresult;
int exponent, mantissa, sign;
int32_t Iresult;
float Fresult;
int16_t SIresult;
time_t time_offset;
char time_str[MAX_FIELD];

//functions prototypes
//general reading
int16_t ReadInt2(frame_structure *frame);
uint16_t ReadUInt2(frame_structure *frame);
int16_t ReadInt2Swap(frame_structure *frame);
uint16_t ReadUInt2Swap(frame_structure *frame);
int32_t ReadInt4(frame_structure *frame);
uint32_t ReadUInt4(frame_structure *frame);
int32_t ReadInt4Swap(frame_structure *frame);
uint32_t ReadUInt4Swap(frame_structure *frame);
float32_t ReadFloat4(frame_structure *frame);
float32_t ReadFloat4Swap(frame_structure *frame);

//types processing
int Read_FP2(frame_structure *frame,const config_structure *config);
int Read_IEEE4(frame_structure *frame,const config_structure *config);
int Read_IEEE4B(frame_structure *frame,const config_structure *config);
int Read_USHORT(frame_structure *frame,const config_structure *config);
int Read_SHORT(frame_structure *frame,const config_structure *config);
int Read_UINT2(frame_structure *frame,const config_structure *config);
int Read_INT4(frame_structure *frame,const config_structure *config);
int Read_ULONG(frame_structure *frame,const config_structure *config);
int Read_LONG(frame_structure *frame,const config_structure *config);
int Read_BOOL(frame_structure *frame,const config_structure *config);
int Read_BOOL4(frame_structure *frame,const config_structure *config);
int Read_NSec(frame_structure *frame,const config_structure *config);
int Read_SecNano(frame_structure *frame,const config_structure *config);
int Read_ASCII(frame_structure *frame,const config_structure *config);

//Still to do/check:
int Read_FP4(frame_structure *frame,const config_structure *config);
int Read_INT2(frame_structure *frame,const config_structure *config);
int Read_UINT4(frame_structure *frame,const config_structure *config);
int Read_BOOL2(frame_structure *frame,const config_structure *config);

#endif
