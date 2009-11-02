/**************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*             Copyright SLF/WSL, 03/2007 - www.slf.ch                     *
**************************************************************************/
/* structures used throughout the program */

#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <stdio.h>
#include <time.h>
#include "limits.h"

#define STDOUT "/dev/stdout"	//location of stdout
#define STDIN "/dev/stdin"	//location of stdin

typedef enum FRAME_TYPES {IS_TOB1, IS_TOB2, IS_TOB3} frame_type;
typedef enum TYPES {NONE, IEEE4, IEEE4B, FP2, FP4, USHORT, SHORT, UINT2, INT2, UINT4, INT4, ULONG, LONG, NSec, SecNano, BOOL, BOOL2, BOOL4, ASCII} numeric_types;

typedef enum PASS {SINGLE, PASS1, PASS2} pass_types;

typedef struct frame_struct {
//General properties of the frame: they come from the file header, they are the same for each frame
	frame_type type;			//type of frame: TOB2 or TOB3
	double notsp_res;			//sampling resolution for data within a frame
	int size;				//size in bytes of the whole frame
	int intended_size;			//intended size from datalogger
	int val_stamp;				//to validate the frame when reading the footer
	int comp_val_stamp;			//sometimes, the complement is used... 
	double subframe_res;			//time resolution for individual lines within the frame
	int t0;					//file creation time (since EPOCH)
	int ringrecord;				//record number written when the file was last rung
	int tremoval;				//timestamps of last card removal (since EPOCH)

	int cindex;				//byte index for position in the frame
	unsigned char *raw;			//raw frame

//Frame header
	char header_size;			//size of the header
	double timestamp;			//frame base timestamp
	int beg_record;				//for TOB3, begining record number of the frame

//Frame data
	int nb_fields;				//number of fields declared in the header
	int field_options[MAX_FIELD];		//optional parameters needed for processing a given field (such as string length, ...)
	numeric_types data_type[NB_MAX_FIELDS];	//numerical types of the various fields
	int data_length;			//length in bits of the data line (the data segment contains several data lines)
	int line_index;				//this is the index of the data line within a frame (starting at 0 for first data line)
	int nb_data_lines;			//maximum number of data lines in a frame
	int data_line_padding;			//after each data line, there might be some padding (calculated from frame size)
	int fp2_nan;				//any FP2 number >= this is a NaN (in absolute value)
	int fp4_nan;				//any FP4 number >= this is a NaN (in absolute value)
	int uint2_nan;				//any UINT2 number >= this is a NaN (in absolute value)

//Frame footer
	char footer_size;			//size of the footer
	int footer_validation;			//to validate the frame, to compare with header footer
	int footer_offset;			//either 0 (TOB3 major frame), total size of the minor frame 
						//or nb of frames without minor frames (TOB2 major frame)
	char flag_f;				//file mark: all records in frame occured before the mark
	char flag_r;				//0 for TOB2, card removed after this frame (TOB3)
	char flag_e;				//frame contains no record
	char flag_m;				//minor frame
} frame_structure;

typedef struct config_struct {
//user selectable parameters
	char input_name[MAX_FILE_NAME];		//input file name
	FILE *input;				//input file
	char output_name[MAX_FILE_NAME];	//output file name
	FILE *output;				//output file

	int stop_cond;				//number of invalid frames to read before stopping (0 does not stop)
	char smart_subsec;			//if the subsecond time is 0, do not show it in timestamp
	char order_output;			//work in 2 passes in order to order output (if necessary)
	char tob32;				//forces to produces the same amount of output as tob32
	char pass_through;			//simply copy valid frames to the output, without any changes

//formatting parameters, not directly selected by the user, but through the theme option
	char separator[MAX_FORMAT];		//field separator
	char comments[MAX_FORMAT];		//marking a comment line
	char time_format[MAX_FORMAT];		//formatting of time
	char timestamp_format[MAX_FORMAT];	//formatting of timestamps
	char timestp_nodec_format[MAX_FORMAT];	//formatting of timestamps when there are no decimals
	char fp2_format[MAX_FORMAT];		//formatting of fp2 floats
	char floats_format[MAX_FORMAT];		//formatting of floats and doubles
	char ints_format[MAX_FORMAT];		//formatting of ints
	char strings_beg[MAX_FORMAT];		//formatting of strings (opening)
	char strings_end[MAX_FORMAT];		//formatting of strings (closing)
	char bool_true[MAX_FORMAT];		//formatting of booleans when true
	char bool_false[MAX_FORMAT];		//formatting of booleans when false
	char nsec_format[MAX_FORMAT];		//formatting of NSec fields

	char NAN[MAX_FORMAT];			//what to display for Not A Number

//to keep track of current processing
	long int nb_lines_read;			//number of data lines read
} config_structure;

typedef struct header_struct {
//the header lines
	char file_environment[NB_MAX_FIELDS][MAX_FIELD];	//header line 1
	char file_table[NB_MAX_FIELDS][MAX_FIELD];		//header line 2
	char file_names[NB_MAX_FIELDS][MAX_FIELD];		//header line 3
	char file_units[NB_MAX_FIELDS][MAX_FIELD];		//header line 4
	char file_processing[NB_MAX_FIELDS][MAX_FIELD];		//header line 5
	char file_types[NB_MAX_FIELDS][MAX_FIELD];		//header line 6
	int nb_environment, nb_table, nb_names, nb_units, nb_processing, nb_types;

//info for the processing of the fields: it would look nicier in frame structure, but it would be a recursive definition!
	//table of function pointers, pointing to the function needed for the correct processing of such a data type
	//int (*field_processing[MAX_FIELD])(frame_structure *,const config_structure *);	
} header_structure;

#endif
