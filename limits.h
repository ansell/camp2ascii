/**************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*             Copyright SLF/WSL, 03/2007 - www.slf.ch                     *
**************************************************************************/
/* definition of various parameters used as maximums for tables, strings, etc */

#ifndef LIMITS_H
#define LIMITS_H

#define MAX_FILE_NAME 200		//maximum length of the input file name
#define MAX_FIELD 40			//maximum length of a field, in characters
#define NB_MAX_FIELDS 200		//maximum number of different fields in a file
#define MAX_LINE 2048			//maximum length of a line, in characters
#define TO_EPOCH 631152000		//number of seconds between Unix Epoch and Campbell Epoch
#define MAX_FRAME_LINES	40		//maximum number data lines in a frame
#define MAX_FORMAT 40			//maximum length of format strings
#define CR10_FP2_NAN 6999		//any FP2 number >= this is a NaN (in absolute value)
#define CR10_FP4_NAN 99999		//any FP4 number >= this is a NaN (in absolute value)
#define CR1000_FP2_NAN 7999		//any FP2 number >= this is a NaN (in absolute value)
#define CR1000_FP4_NAN 99999		//any FP4 number >= this is a NaN (in absolute value)
#define FP2_NAN 7999			//any FP2 number >= this is a NaN (in absolute value, default)
#define FP4_NAN 99999			//any FP4 number >= this is a NaN (in absolute value, default)
#define UINT2_NAN 65535			//any UNIT2 number >= this is a NaN (in absolute value)

#endif
