/**************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*             Copyright SLF/WSL, 03/2007 - www.slf.ch                     *
**************************************************************************/
/* a few generic functions that are usefull for us */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "generic_functions.h"

void sig_stop(int sig) {
/* function called when a SIGINT/SIGTERM signal is caught */
/* taken from gm4lin, http://mathias.bavay.free.fr/software/gm4lin/index.html (GPL) */
	flag_stop=1;
	fprintf(stderr,"*** Signal %d received, program stopping...\n",sig);
}

int stop_catch_init() {
/* this is the initialization of signal catching for the STOP and TERM signals */
/* we want to catch the signal SIGINT to stop the program in a clean way */
/* return EXIT_SUCCES or EXIT_FAILURE */
/* taken from gm4lin, http://mathias.bavay.free.fr/software/gm4lin/index.html (GPL) */
	struct sigaction stop_act;			//for signal catching (for the end of extraction)
	
    	stop_act.sa_handler=(void *)sig_stop;
    	sigemptyset(&stop_act.sa_mask);			//We don't want to block any other signals
	stop_act.sa_flags=0;
	flag_stop=0;
	if (sigaction(SIGINT,&stop_act,NULL) != EXIT_SUCCESS) {
		fprintf(stderr,"WARNING (%s:%d): signal catching failed for SIGINT\n",__FILE__,__LINE__);
		return EXIT_FAILURE;
	}
	if (sigaction(SIGTERM,&stop_act,NULL) != EXIT_SUCCESS) {
		fprintf(stderr,"WARNING (%s:%d): signal catching failed for SIGTERM",__FILE__,__LINE__);
		return EXIT_FAILURE;
	}
	
	return EXIT_SUCCESS;
}

int read_ascii_fields(FILE *data_file,char fields[NB_MAX_FIELDS][MAX_FIELD]) {
/* This function reads the next line in the opened file and splits it in its fields, separated by delimiter "delimiter"*/
/* It returns a table containing strings, one cell per field */
	char delimiter='"';
	char *line;
	char *field;
	char *pos;
	int i=0;
	
	line=(char *)calloc(sizeof(char),(size_t)MAX_LINE);
	field=(char *)calloc(sizeof(char),(size_t)MAX_FIELD);
	if(fgets(line,MAX_LINE,data_file) == NULL) {
		printf("ERROR (%s:%d): Cannot read line\n",__FILE__,__LINE__);
		return EXIT_FAILURE;
	}
	
	//printf("Line read: %s\n",line);
	if(line[0]=='"')
		line=(line+1);	//the line starts with a " that we don't want
	
	while((pos=strchr(line,delimiter)) != NULL) {
		strncpy(fields[i],line,(pos-line));
		fields[i][(pos-line)]='\0';
		if(strcmp(fields[i],",") != 0) i++;
		line=(pos+1);
	}
	
	free(field);
	return(i);
}

void types_sizes() {
/* This function prints the sizes of common types (as used in this program */
	printf("Sizes of used types:\n");
	printf("char is %d\t",sizeof(char));
	printf("unsigned short int is %d\n",sizeof(unsigned short int));
	printf("short int is %d\t",sizeof(short int));
	printf("int is %d\n",sizeof(int));
	printf("int64_t is %d\n",sizeof(int64_t));
	printf("long int is %d\t",sizeof(long int));
	printf("long long int is %d\n",sizeof(long long int));
	printf("float is %d\t",sizeof(float));
	printf("double is %d\n",sizeof(double));

}

void print_frame_struct(frame_structure *frame) {
/* Prints the fields of frame */
	printf("Content of frame:\n");
	printf("\ttype:%d\n",frame->type);
	printf("\ttimestamp:%f\n",frame->timestamp);
	printf("\tbeg_record:%d\n",frame->beg_record);
	printf("\tnotsp_res:%g\n",frame->notsp_res);
	printf("\tsize:%d\n",frame->size);
	printf("\tintended_size:%d\n",frame->intended_size);

	printf("\tline_index:%d\n",frame->line_index);
	printf("\tdata_length:%d\n",frame->data_length);
	printf("\tnb_fields:%d\n",frame->nb_fields);

	printf("\tval_stamp:0x%0x\t",frame->val_stamp);
	printf("\tcomp_val_stamp:0x%0x\n",frame->comp_val_stamp);
	printf("\tsubframe_res:%g\n",frame->subframe_res);
	printf("\tt0:%d\n",frame->t0);
	printf("\tring_record:%d\n",frame->ringrecord);
	printf("\ttremoval:%d\n",frame->tremoval);

	printf("\tfooter_validation:0x%0x\n",frame->footer_validation);
	printf("\tfooter_offset:0x%0x\n",frame->footer_offset);
	printf("\tflag_f:%d\n",frame->flag_f);
	printf("\tflag_r:%d\n",frame->flag_r);
	printf("\tflag_e:%d\n",frame->flag_e);
	printf("\tflag_m:%d\n",frame->flag_m);
	
}
