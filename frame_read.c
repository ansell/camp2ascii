/**************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*             Copyright SLF/WSL, 03/2007 - www.slf.ch                     *
**************************************************************************/
/* Here, we put all functions related to the details of reading a frame */
/* the structures "frame" and "config" are *always* passed by reference */
/* because this is much faster than making and passing a copy of them!  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>		/* for time calculations */
#include <math.h>
#include <inttypes.h>		/* C99 portable data types */
#include "limits.h"
#include "types_processing.h"
#include "frame_read.h"

int read_frame_data(frame_structure *frame,const config_structure *config) {
/* This function reads the fields of the current line (buffered in "frame" structure) */
//the variable "field_index" is global in order to exchange it with the ASCII reading function
//(this way, we can still use function pointers if we want to and this is more efficient than using a field in "frame")

	/* decoding each field one by one */
	for(field_index=1;field_index<=frame->nb_fields;field_index++) {
		fprintf(config->output,"%s",config->separator);
		switch(frame->data_type[field_index]) {
			case IEEE4:	Read_IEEE4(frame,config);
					break;
			case IEEE4B:	Read_IEEE4B(frame,config);
					break;
			case FP2:	Read_FP2(frame,config);
					break;
			case FP4:	Read_FP4(frame,config);
					break;
			case USHORT:	Read_USHORT(frame,config);
					break;
			case SHORT:	Read_SHORT(frame,config);
					break;
			case UINT2:	Read_UINT2(frame,config);
					break;
			case INT2:	Read_INT2(frame,config);
					break;
			case UINT4:	Read_UINT4(frame,config);
					break;
			case INT4:	Read_INT4(frame,config);
					break;
			case BOOL:	Read_BOOL(frame,config);
					break;
			case BOOL2:	Read_BOOL2(frame,config);
					break;
			case BOOL4:	Read_BOOL4(frame,config);
					break;
			case ULONG:	Read_ULONG(frame,config);
					break;
			case LONG:	Read_LONG(frame,config);
					break;
			case NSec:	Read_NSec(frame,config);
					break;
			case SecNano:	Read_SecNano(frame,config);
					break;
			case ASCII:	Read_ASCII(frame,config);
					break;
			default:	fprintf(stderr,"ERROR (%s:%d): no matching processing for data type %d",__FILE__,__LINE__,frame->data_type[field_index]);
					return EXIT_FAILURE;
			}
	}
	fprintf(config->output,"\n");
	return EXIT_SUCCESS;
}

int read_frame_header(frame_structure *frame) {
/* This function reads the frame header */
/*  it sets up the timestamp */
	time_t time_offset,subseconds;

	time_offset = ReadInt4(frame)+TO_EPOCH;
	subseconds  = ReadInt4(frame);

	//subsecond is an offset in ms. subseconds*subframe_resolution gives an offset in s
	frame->timestamp = (double)time_offset + ((double)subseconds)*frame->subframe_res;
	if(frame->type==IS_TOB3) {		//for TOB3, there is a third field
		frame->beg_record=ReadInt4(frame);
	}
	#ifdef DEBUG
		fprintf(stderr,"*** timestamp=0x%x record_number=0x%x\n",(int)(time_offset-TO_EPOCH),frame->beg_record);
	#endif

	return EXIT_SUCCESS;
}

int read_frame_footer(frame_structure *frame) {
/* This function reads the frame footer */
/* It reads all the validation informations */
	int32_t content;

	frame->cindex=frame->size-4;
	content=ReadInt4(frame);
	
	frame->footer_offset     = ((0x000007FF & content)       );
	frame->flag_f            = ((0x00000800 & content) >> 12 );
	frame->flag_r            = ((0x00001000 & content) >> 13 );
	frame->flag_e            = ((0x00002000 & content) >> 14 );
	frame->flag_m            = ((0x00004000 & content) >> 15 );
	frame->footer_validation = ((0xFFFF0000 & content) >> 16 );
	#ifdef DEBUG
		fprintf(stderr,"*** full footer= 0x%x offset=0x%x\n",content,frame->footer_offset);
		fprintf(stderr,"*** header_validation=0x%x footer_validation=0x%x\n",frame->val_stamp,frame->footer_validation);
	#endif

	return EXIT_SUCCESS;
}

int analyze_tob32_frame(frame_structure *frame,const config_structure *config,pass_types pass) {
/* This function coordinates all operations in order to read a tob2/tob3 frame */
/* pass represents the type of processing to be done: single pass, pass1 or pass2 */
	int i;

	//reading frame footer
	read_frame_footer(frame);

	//frame validation: is it a good frame or not?
	if(frame->val_stamp != frame->footer_validation) {
		if(frame->comp_val_stamp != frame->footer_validation) {
			#ifdef VERBOSE
				fprintf(stderr,"WARNING: At %16f, invalid frame, rejected\n",frame->timestamp);
			#endif
			return EXIT_FAILURE;
		}
	}
	if(frame->flag_e == 1) {
		#ifdef VERBOSE
			fprintf(stderr,"WARNING: At %16f, empty frame, rejected\n",frame->timestamp);
		#endif
		return EXIT_FAILURE;
	}
	if(frame->flag_m == 1) {
		#ifdef VERBOSE
			fprintf(stderr,"WARNING: At %16f, minor frame, rejected\n",frame->timestamp);
		#endif
		return EXIT_FAILURE;
	}
	/*if(frame->flag_r == 1) {
		#ifdef VERBOSE
			fprintf(stderr,"WARNING: At %16f, card was removed after writing frame, rejected\n",frame->timestamp);
		#endif
		return EXIT_FAILURE;
	}*/

	//reading data segment
	if(config->pass_through==0) {	//regular, interpreted output
		frame->cindex=0;
		read_frame_header(frame);				//reading frame header
		if(pass==PASS1 && (frame->beg_record >= frame->ringrecord)) return EXIT_SUCCESS;
		if(pass==PASS2 && (frame->beg_record < frame->ringrecord)) return EXIT_SUCCESS;
	
		for(frame->line_index=1;frame->line_index <= frame->nb_data_lines; frame->line_index++) {
			#ifdef DEBUG
				fprintf(stderr,"Line #%d -> ",frame->line_index);
			#endif
			print_timestamp(frame,config);		//first, we print the timestamp
			read_frame_data(frame,config);		//now we read and print the data
			frame->cindex += frame->data_line_padding;	//the data line might have some padding, we go over it
		}
		return EXIT_SUCCESS;
	} else {	//pass-through output, no need to analyze frames
		for(i=0;i<frame->size;i++)
			fprintf(config->output,"%c",frame->raw[i]);
		return EXIT_SUCCESS;
	}
}

int analyze_tob1_frame(frame_structure *frame,const config_structure *config) {
/* This function coordinates all operations in order to read a data line in tob1 format */
	int i;

	if(config->pass_through==0) {	//regular, interpreted output
		frame->cindex=0;
		fprintf(config->output,"%ld",config->nb_lines_read+1);	//we print the *current* line number
		read_frame_data(frame,config);	//and then we read and print the data line (only 1 per frame in tob1)
		return EXIT_SUCCESS;
	} else {	//pass-through output, no need to analyze frames
		for(i=0;i<frame->size;i++)
			fprintf(config->output,"%c",frame->raw[i]);
		return EXIT_SUCCESS;
	}
}

int print_timestamp(const frame_structure *frame,const config_structure *config) {
/* This function prints the data content of one frame */
	time_t time_offset,subseconds;
	double timestamp=frame->timestamp;
	char time_str[MAX_FIELD];
	
	/* Calculating the timestamp for the current line */
	timestamp += frame->notsp_res*(double)(frame->line_index-1); 		//lines are counted from 1 to n
	time_offset=(time_t)(floor(timestamp));
	subseconds=(time_t)( (timestamp-floor(timestamp))*1000*TRUNC_FACTOR );	//to have an offset in ms - for round off errors
	strftime(time_str,MAX_FIELD,config->time_format,gmtime(&time_offset));
	if(subseconds>0 || config->smart_subsec==0)
		fprintf(config->output,config->timestamp_format,time_str,(int)subseconds);
	else
		fprintf(config->output,config->timestp_nodec_format,time_str);

	return EXIT_SUCCESS;
}

