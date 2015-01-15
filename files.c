/**************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*             Copyright SLF/WSL, 05/2007 - www.slf.ch                     *
**************************************************************************/
/* Here, we put all functions related to files operations */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "limits.h"
#include "structures.h"
#include "frame_read.h"
#include "files.h"
extern sig_atomic_t flag_stop;

int read_header_line6(char types[NB_MAX_FIELDS][MAX_FIELD],frame_structure *frame){
/* This function builts a table that contains the type of each elements of a frame, according to the header that has been read */
/* It also sets the lenght (in bytes) of a data line */
	int i, j, tempo, frame_length=0;
	int field_length;
	
	/* decoding the types */
	for(i=1; i<=frame->nb_fields; i++) {
	//frame[i] goes from 0 (timestamp) to nb_types while types[j] goes from 0 (first field) to nb_types-1
		j=i-1;
		frame->data_type[i]=0;
		field_length=0;
		if(strncmp(types[j],"IEEE4",5) == 0) { //for IEEE4, IEEE4L
			frame->data_type[i]=IEEE4;
			if(strncmp(types[j],"IEEE4B",6) == 0) { //for IEEE4B
				frame->data_type[i]=IEEE4B;
			}
			field_length=4;
		}
		if(strncmp(types[j],"FP2",3) == 0) {
			frame->data_type[i]=FP2;
			field_length=2;
		}
		if(strncmp(types[j],"FP4",3) == 0) {
			frame->data_type[i]=FP4;
			field_length=4;
		}
		if(strncmp(types[j],"ULONG",5) == 0) {
			frame->data_type[i]=ULONG;
			field_length=4;
		}
		if(strncmp(types[j],"LONG",4) == 0) {
			frame->data_type[i]=LONG;
			field_length=4;
		}
		if(strncmp(types[j],"USHORT",6) == 0) {
			frame->data_type[i]=USHORT;
			field_length=2;
		}
		if(strncmp(types[j],"SHORT",5) == 0) {
			frame->data_type[i]=SHORT;
			field_length=2;
		}
		if(strncmp(types[j],"UINT2",5) == 0) {
			frame->data_type[i]=UINT2;
			field_length=2;
		}
		if(strncmp(types[j],"INT2",4) == 0) {
			frame->data_type[i]=INT2;
			field_length=2;
		}
		if(strncmp(types[j],"UINT4",5) == 0) {
			frame->data_type[i]=UINT4;
			field_length=4;
		}
		if(strncmp(types[j],"INT4",4) == 0) {
			frame->data_type[i]=INT4;
			field_length=4;
		}
		if(strncmp(types[j],"BOOL",4) == 0) {
			frame->data_type[i]=BOOL;
			field_length=1;
		}
		if(strncmp(types[j],"BOOL2",5) == 0) {
			frame->data_type[i]=BOOL2;
			field_length=2;
		}
		if(strncmp(types[j],"BOOL4",5) == 0) {
			frame->data_type[i]=BOOL4;
			field_length=4;
		}
		if(strncmp(types[j],"NSec",4) == 0) {
			frame->data_type[i]=NSec;
			field_length=4+4;
		}
		if(strncmp(types[j],"SecNano",7) == 0) {
			frame->data_type[i]=SecNano;
			field_length=4+4;
		}
		if(strncmp(types[j],"ASCII",5) == 0) {
			frame->data_type[i]=ASCII;
			if(sscanf(types[j],"ASCII(%d)",&tempo) != 1) tempo=1;
			frame->field_options[i]=tempo;
			field_length=tempo;
		}
		
		frame_length+=field_length;

		if(frame->data_type[i] == 0) {
			fprintf(stderr,"ERROR (%s:%d): field %s not identified\n",__FILE__,__LINE__,types[j]);
			return EXIT_FAILURE;
		}
	}
	frame->data_length=frame_length;

	return EXIT_SUCCESS;
}

int read_header_line2(char fields[NB_MAX_FIELDS][MAX_FIELD],int nb_fields,frame_structure *frame){
/* This function sets up the table recording parameters, according to the header that has been read */
	char str[MAX_FIELD];
	double multiplier=0;

	if(frame->type==IS_TOB1) {
		//strncpy(frame->table_name,"tob1_table",MAX_FIELD);	
		frame->notsp_res=0;
		frame->size=0;
		frame->intended_size=0;
		frame->val_stamp=0;
		frame->comp_val_stamp=0;
		frame->subframe_res=0;
		return EXIT_SUCCESS;
	}
	if(frame->type==IS_TOB2) {
		if(nb_fields<6) {
			fprintf(stderr,"ERROR (%s:%d): not enough fields at header line 2 for TOB2! Expected 6 fields, found %d.\n",__FILE__,__LINE__, nb_fields);
			return EXIT_FAILURE;
		}
		if(nb_fields>6) fprintf(stderr,"WARNING (%s:%d): too many fields at header line 2 for TOB2! Expected 6 fields, found %d.\n",__FILE__,__LINE__, nb_fields);
	}
	if(frame->type==IS_TOB3) {
		if(nb_fields<8) {
			fprintf(stderr,"ERROR (%s:%d): not enough fields at header line 2 for TOB3! Expected 8 fields, found %d.\n",__FILE__,__LINE__, nb_fields);
			return EXIT_FAILURE;
		}
		if(nb_fields>8) fprintf(stderr,"WARNING (%s:%d): too many fields at header line 2 for TOB3! Expected 8 fields, found %d.\n",__FILE__,__LINE__, nb_fields);
	}

	//strncpy(frame->table_name,fields[0],MAX_FIELD);			//name of the table
	frame->notsp_res=strtod(fields[1],NULL);			//sampling resolution for data lines within a frame
		if(strstr(fields[1],"HOUR") != NULL) multiplier=3600.0;
		if(strstr(fields[1],"MIN") != NULL) multiplier=60.0;
		if(strstr(fields[1],"SEC") != NULL) multiplier=1.0;
		if(strstr(fields[1],"MSEC") != NULL) multiplier=1e-3;
		if(strstr(fields[1],"USEC") != NULL) multiplier=1e-6;
		if(strstr(fields[1],"NSEC") != NULL) multiplier=1e-9;
	if(multiplier!=0) frame->notsp_res *= multiplier;
		else fprintf(stderr,"ERROR (%s:%d): sampling resolution can not be fully decoded!\n",__FILE__,__LINE__);
	frame->size=(int)strtod(fields[2],NULL);			//size in bits of the whole frame
	frame->intended_size=(int)strtod(fields[3],NULL);		//max size declared in datalogger program
	frame->val_stamp=(int)strtod(fields[4],NULL);			//to validate the frame when reading the footer
	frame->comp_val_stamp=(int)(0xFFFF^frame->val_stamp);		//idem, but complement
	strncpy(str,fields[5],MAX_FIELD);				//resolution of data lines within the frame
		if(strncmp(str,"SecMsec",MAX_FIELD)==0) frame->subframe_res=1e-3;
		if(strncmp(str,"Sec100Usec",MAX_FIELD)==0) frame->subframe_res=100e-6;
		if(strncmp(str,"Sec10Usec",MAX_FIELD)==0) frame->subframe_res=10e-6;
		if(strncmp(str,"SecUsec",MAX_FIELD)==0) frame->subframe_res=1e-6;
	

	if(frame->type==IS_TOB3) {
		frame->ringrecord=(int)strtod(fields[6],NULL);		//record number last time the file was rung
		frame->tremoval=(int)strtod(fields[7],NULL);		//time of last removal of card
	}
	
	return EXIT_SUCCESS;
}


int read_file_header(FILE *data_file, header_structure *h) {
/* This function reads the file header and splits each line by fields */

	h->nb_environment=read_ascii_fields(data_file,h->file_environment);
	if(strcmp(h->file_environment[0],"TOB1") != 0) //no table line for TOB1
		h->nb_table=read_ascii_fields(data_file,h->file_table);
	h->nb_names=read_ascii_fields(data_file,h->file_names);
	h->nb_units=read_ascii_fields(data_file,h->file_units);
	h->nb_processing=read_ascii_fields(data_file,h->file_processing);
	h->nb_types=read_ascii_fields(data_file,h->file_types);
	
	if((h->nb_names+h->nb_units+h->nb_processing+h->nb_types) != (4*h->nb_types)) {
		fprintf(stderr,"ERROR (%s:%d): Input file header is corrupted (number of columns is not constant)\n",__FILE__,__LINE__);
		fprintf(stderr,"%d names, %d units, %d processings, %d types\n",h->nb_names,h->nb_units,h->nb_processing,h->nb_types);
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

int analyze_file_header(header_structure header,frame_structure *frame) {
/* This function populates the structure frame with informations from */
/* the file header */
	struct tm t0;
	int data_segment_size;
	double padding;

	frame->nb_fields=header.nb_names;

	/* header line 1*/
	frame->footer_size=-1;
	frame->header_size=-1;
	if(strcmp(header.file_environment[0],"TOB1") == 0) {
		frame->type=IS_TOB1;
		frame->header_size=0;
		frame->footer_size=0;
	}
	if(strcmp(header.file_environment[0],"TOB2") == 0) {
		frame->type=IS_TOB2;
		frame->header_size=8;
		frame->footer_size=4;
	}
	if(strcmp(header.file_environment[0],"TOB3") == 0) {
		frame->type=IS_TOB3;
		frame->header_size=12;
		frame->footer_size=4;
	}
	if(frame->footer_size==-1) {
		fprintf(stderr,"ERROR (%s:%d): Frame sizes unknown for this file version\n",__FILE__,__LINE__);
		return EXIT_FAILURE;
	}
	if(frame->type == IS_TOB1) {
		frame->t0=0;
	} else {
		sscanf(header.file_environment[7],"%d-%d-%d %d:%d:%d",&t0.tm_year,&t0.tm_mon,&t0.tm_mday,&t0.tm_hour,&t0.tm_min,&t0.tm_sec);
		t0.tm_year -= TM_Y0;	//since it should be the number of years since 1900
		t0.tm_isdst=0;		//we don't use Daylight Saving Time
 		if((frame->t0=(int)mktime(&t0))==-1) fprintf(stderr,"WARNING (%s:%d): File creation date \"%s\" can not be decoded!\n",__FILE__,__LINE__,header.file_environment[7]);
	}

	//Nans definitions
	frame->fp2_nan=FP2_NAN;
	frame->fp4_nan=FP4_NAN;
	frame->uint2_nan=UINT2_NAN;
	if(strcmp(header.file_environment[2],"CR10") == 0) {
		frame->fp2_nan=CR10_FP2_NAN;
		frame->fp4_nan=CR10_FP4_NAN;
	}
	if(strcmp(header.file_environment[2],"CR1000") == 0) {
		frame->fp2_nan=CR1000_FP2_NAN;
		frame->fp4_nan=CR1000_FP4_NAN;
	}

	/* header line 2 */
	read_header_line2(header.file_table,header.nb_table,frame);

	/* header line 6 */
	read_header_line6(header.file_types,frame);

	/* calculation of dimensions */
	if(frame->type==IS_TOB1) {
		frame->size=frame->data_length;
		frame->nb_data_lines=1;
		frame->data_line_padding=0;
	} else {
		data_segment_size=frame->size-(frame->header_size+frame->footer_size);				//part of the frame containing the data
		frame->nb_data_lines=(int)floor( (double)data_segment_size / (double)frame->data_length );	//how many data lines in the frame?
		padding=(double)(data_segment_size-(frame->nb_data_lines*frame->data_length));			//how much padding for each data line?
		if( floor(padding/(double)frame->nb_data_lines) != padding/(double)frame->nb_data_lines ) {
			fprintf(stderr,"ERROR (%s:%d): calculated frame padding is invalid! %g bytes for %d data lines...\n",__FILE__,__LINE__,padding,frame->nb_data_lines);
			return EXIT_FAILURE;
		}
		frame->data_line_padding=(int)padding/(double)frame->nb_data_lines;
	}

	return EXIT_SUCCESS;
}

int print_headers(header_structure h,config_structure config,frame_structure frame) {
/* prints the output file's header */
	int i;	

	if(config.pass_through != 1) {		//normal ascii output
		//prints line 1: metadata
		fprintf(config.output,"%s\"TOA5\"",config.comments);
		for(i=1;i<(h.nb_environment-1);i++)
			fprintf(config.output,"%s\"%s\"",config.separator,h.file_environment[i]);
		if(frame.type==IS_TOB1) //table name is elsewhere for TOB1
			fprintf(config.output,"%s\"%s\"\n",config.separator,h.file_environment[7]);
		else
			fprintf(config.output,"%s\"%s\"\n",config.separator,h.file_table[0]);
		//prints line 2: names
		if(frame.type==IS_TOB1) //no timestamp for TOB1, so we put the line number
			fprintf(config.output,"%s\"LINE\"",config.comments);
		else
			fprintf(config.output,"%s\"TIMESTAMP\"",config.comments);
		for(i=0;i<(h.nb_names);i++)
			fprintf(config.output,"%s\"%s\"",config.separator,h.file_names[i]);
		fprintf(config.output,"\n");
		//prints line 3: units
		if(frame.type==IS_TOB1) //no timestamp for TOB1, so we put the line number
			fprintf(config.output,"%s\"LN\"",config.comments);
		else
			fprintf(config.output,"%s\"TS\"",config.comments);
		for(i=0;i<(h.nb_units);i++)
			fprintf(config.output,"%s\"%s\"",config.separator,h.file_units[i]);
		fprintf(config.output,"\n");
		//prints line 4: processing
		fprintf(config.output,"%s\"\"",config.comments);
		for(i=0;i<(h.nb_processing);i++)
			fprintf(config.output,"%s\"%s\"",config.separator,h.file_processing[i]);
		fprintf(config.output,"\n");
	} else {				//compacted binary output
		fprintf(config.output,"\"%s\"",h.file_environment[0]);
		for(i=1;i<(h.nb_environment);i++)
			fprintf(config.output,",\"%s\"",h.file_environment[i]);
		if(frame.type==IS_TOB1) {
			fprintf(config.output,",\"%s\"",h.file_environment[7]);
		} else {
			fprintf(config.output,"\n\"%s\"",h.file_table[0]);
			for(i=1;i<(h.nb_table);i++)
				fprintf(config.output,",\"%s\"",h.file_table[i]);
		}
		fprintf(config.output,"\n\"%s\"",h.file_names[0]);
		for(i=1;i<(h.nb_names);i++)
			fprintf(config.output,",\"%s\"",h.file_names[i]);
		fprintf(config.output,"\n\"%s\"",h.file_units[0]);
		for(i=1;i<(h.nb_units);i++)
			fprintf(config.output,",\"%s\"",h.file_units[i]);
		fprintf(config.output,"\n\"%s\"",h.file_processing[0]);
		for(i=1;i<(h.nb_processing);i++)
			fprintf(config.output,",\"%s\"",h.file_processing[i]);
		fprintf(config.output,"\n\"%s\"",h.file_types[0]);
		for(i=1;i<(h.nb_types);i++)
			fprintf(config.output,",\"%s\"",h.file_types[i]);
		fprintf(config.output,"\n");
	}

	return EXIT_SUCCESS;
}

int load_frame(config_structure config,frame_structure *frame) {
	//reading the raw frame

	if(fread(frame->raw,(size_t)frame->size,1,config.input)!=1) {
		if(!feof(config.input))
			fprintf(stderr,"ERROR: Can not read file \"%s\" after %16f\n",config.input_name,frame->timestamp);
		return EXIT_FAILURE;
	}
	
	return EXIT_SUCCESS;
}

int read_data(config_structure *config,frame_structure *frame){
/* Encapsulation for reading the data content of the file */
	fpos_t *pos_beg;			//for storing begining position of data
	int nb_failures;
	unsigned char nb_pass,max_pass;
	pass_types pass;

	//TOB1 processing is specific
	if(frame->type == IS_TOB1) {
		config->nb_lines_read=0;
		nb_failures=0;
		while( ((config->stop_cond==0) || (nb_failures<config->stop_cond)) && !feof(config->input) ){
			if(flag_stop==1) return EXIT_FAILURE;	//the user requests to stop the execution
			if(load_frame(*config,frame)==EXIT_SUCCESS) {
				if(analyze_tob1_frame(frame,config)==EXIT_FAILURE) 
					nb_failures++;
				else
					config->nb_lines_read += 1;	//frames can only contain 1 data lines in tob1
			}
		}
		return EXIT_SUCCESS;
	}

	//TOB2 and TOB3 processing
	//depending on options, type of data file, etc determine how many pass to do
	//default: single pass
	max_pass=1;
	pass=SINGLE;
	//two pass
	if((frame->type == IS_TOB3) && (frame->ringrecord > 0) && (config->order_output==1) ) {
		max_pass=2;
		pass=PASS1;
	}
	//only the first pass for full tob32 compatibility
	if((frame->type == IS_TOB3) && (config->tob32==1)) {
		max_pass=1;
		pass=PASS1;
	}
	
	//recording position in input file
	pos_beg=(fpos_t *)malloc(sizeof(fpos_t));
	fgetpos(config->input,pos_beg);

	//extract and print the frames
	nb_pass=1;
	config->nb_lines_read=0;
	while(nb_pass<=max_pass) {
		nb_failures=0;
		while( ((config->stop_cond==0) || (nb_failures<config->stop_cond)) && !feof(config->input) ){
			if(flag_stop==1) return EXIT_FAILURE;	//the user requests to stop the execution
			if(load_frame(*config,frame)==EXIT_SUCCESS) {
				if(analyze_tob32_frame(frame,config,pass)==EXIT_FAILURE) 
					nb_failures++;
				else
					config->nb_lines_read += frame->line_index;
			}
		}
		if(max_pass==2 && nb_pass==1){	//if we need to do two pass, get ready for pass 2
			pass=PASS2;
			fsetpos(config->input,pos_beg);
		}
		nb_pass++;
	}

	return EXIT_SUCCESS;
}

