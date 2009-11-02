/**************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*             Copyright SLF/WSL, 03/2007 - www.slf.ch                     *
**************************************************************************/
/* reading command line parameters, reading default values, ... */
#include <stdlib.h>
#include "structures.h"
#include "initializations.h"
#include "camp2ascii.h"
#include <getopt.h>

int check_endianness()
{
	int16_t word = 0x4321;
	#ifdef BIGENDIAN
		int8_t endian=1;
	#else
		int8_t endian=0;
	#endif

	if((*(int8_t *)& word) != 0x21 ) {
		//host is big endian
		if(endian==1) return EXIT_SUCCESS;
		fprintf(stderr,"ERROR (%s:%d): this system is big endian, please recompile with BIGENDIAN compilation flag defined\n",__FILE__,__LINE__);
		return EXIT_FAILURE;
	} else {
		//host is little endian
		if(endian==0) return EXIT_SUCCESS;
		fprintf(stderr,"ERROR (%s:%d): this system is little endian, please recompile removing BIGENDIAN compilation flag\n",__FILE__,__LINE__);
		return EXIT_FAILURE;
	}
}

int load_defaults(config_structure *config){
/* loads the default values into the config structure */
	snprintf(config->input_name,MAX_FILE_NAME,"test.dat");
	snprintf(config->separator,MAX_FORMAT,",");
	config->comments[0]='\0';
	snprintf(config->time_format,MAX_FORMAT,"%%Y-%%m-%%d %%H:%%M:%%S");
	snprintf(config->timestamp_format,MAX_FORMAT,"\"%%s.%%03d\"");
	snprintf(config->timestp_nodec_format,MAX_FORMAT,"\"%%s\"");
	snprintf(config->fp2_format,MAX_FORMAT,"%%g");
	snprintf(config->floats_format,MAX_FORMAT,"%%g");
	snprintf(config->ints_format,MAX_FORMAT,"%%d");
	snprintf(config->strings_beg,MAX_FORMAT,"\"");
	snprintf(config->strings_end,MAX_FORMAT,"\"");
	snprintf(config->bool_true,MAX_FORMAT,"1");
	snprintf(config->bool_false,MAX_FORMAT,"0");
	snprintf(config->nsec_format,MAX_FORMAT,"\"%%s.%%09d\"");
	snprintf(config->NAN,MAX_FORMAT,"\"NAN\"");
	snprintf(config->output_name,MAX_FORMAT,STDOUT);
	config->stop_cond=0;
	config->tob32=0;
	config->pass_through=0;
	config->smart_subsec=0;
	config->order_output=0;

	return EXIT_SUCCESS;
}

int style_outputs(config_structure *config,char *style){
/* sets the output to a given style in order to make using data easier */
/* in other applications */

	if(strncmp(style,"grace",5)==0) {
		snprintf(config->separator,MAX_FORMAT," ");
		snprintf(config->comments,MAX_FORMAT,"#");
		snprintf(config->time_format,MAX_FORMAT,"%%Y-%%m-%%dT%%H:%%M:%%S");
		snprintf(config->timestamp_format,MAX_FORMAT,"%%s.%%03d");
		snprintf(config->timestp_nodec_format,MAX_FORMAT,"%%s");
		snprintf(config->fp2_format,MAX_FORMAT,"%%g");
		snprintf(config->floats_format,MAX_FORMAT,"%%g");
		snprintf(config->ints_format,MAX_FORMAT,"%%d");
		snprintf(config->strings_beg,MAX_FORMAT,"\"");
		snprintf(config->strings_end,MAX_FORMAT,"\"");
		snprintf(config->bool_true,MAX_FORMAT,"1");
		snprintf(config->bool_false,MAX_FORMAT,"0");
		snprintf(config->nsec_format,MAX_FORMAT,"%%s.%%09d");
		snprintf(config->NAN,MAX_FORMAT,"-9999");
	}
	if(strncmp(style,"tob32",5)==0) {
		snprintf(config->separator,MAX_FORMAT,",");
		snprintf(config->time_format,MAX_FORMAT,"%%Y-%%m-%%d %%H:%%M:%%S");
		snprintf(config->timestamp_format,MAX_FORMAT,"\"%%s.%%03d\"");
		snprintf(config->timestp_nodec_format,MAX_FORMAT,"\"%%s\"");
		snprintf(config->fp2_format,MAX_FORMAT,"%%g");
		snprintf(config->floats_format,MAX_FORMAT,"%%g");
		snprintf(config->ints_format,MAX_FORMAT,"%%d");
		snprintf(config->strings_beg,MAX_FORMAT,"\"");
		snprintf(config->strings_end,MAX_FORMAT,"\"");
		snprintf(config->bool_true,MAX_FORMAT,"1");
		snprintf(config->bool_false,MAX_FORMAT,"0");
		snprintf(config->nsec_format,MAX_FORMAT,"\"%%s.%%09d\"");
		snprintf(config->NAN,MAX_FORMAT,"\"NAN\"");
	}
	if(strncmp(style,"excel",5)==0) {
		snprintf(config->separator,MAX_FORMAT,",");
		snprintf(config->time_format,MAX_FORMAT,"%%Y%s%%m%s%%d%s%%H%s%%M%s%%S",config->separator,config->separator,config->separator,config->separator,config->separator);
		snprintf(config->timestamp_format,MAX_FORMAT,"%%s.%%03d");
		snprintf(config->timestp_nodec_format,MAX_FORMAT,"%%s");
		snprintf(config->fp2_format,MAX_FORMAT,"%%g");
		snprintf(config->floats_format,MAX_FORMAT,"%%g");
		snprintf(config->ints_format,MAX_FORMAT,"%%d");
		snprintf(config->strings_beg,MAX_FORMAT,"\"");
		snprintf(config->strings_end,MAX_FORMAT,"\"");
		snprintf(config->bool_true,MAX_FORMAT,"1");
		snprintf(config->bool_false,MAX_FORMAT,"0");	
		snprintf(config->nsec_format,MAX_FORMAT,"%%s.%%09d");
		snprintf(config->NAN,MAX_FORMAT,"NaN");
	}
	if(strncmp(style,"sql",3)==0) {
		snprintf(config->separator,MAX_FORMAT,",");
		snprintf(config->comments,MAX_FORMAT,"#");
		snprintf(config->time_format,MAX_FORMAT,"'%%Y-%%m-%%d %%H:%%M:%%S'");
		snprintf(config->timestamp_format,MAX_FORMAT,"%%s%s%%03d",config->separator);
		snprintf(config->timestp_nodec_format,MAX_FORMAT,"%%s");
		snprintf(config->fp2_format,MAX_FORMAT,"%%g");
		snprintf(config->floats_format,MAX_FORMAT,"%%g");
		snprintf(config->ints_format,MAX_FORMAT,"%%d");
		snprintf(config->strings_beg,MAX_FORMAT,"\"");
		snprintf(config->strings_end,MAX_FORMAT,"\"");
		snprintf(config->bool_true,MAX_FORMAT,"TRUE");
		snprintf(config->bool_false,MAX_FORMAT,"FALSE");	
		snprintf(config->nsec_format,MAX_FORMAT,"%%s.%%09d");
		snprintf(config->NAN,MAX_FORMAT,"'NULL'");
	}

	return EXIT_SUCCESS;
}

int read_cmd_line(int argc,char **argv,config_structure *config){
/* read parameters from the command line */
/* and sets the config structure accordingly */
	int c;
	int status=0;
	
	if (argc == 1) {
		fprintf(stderr,"%s: use -help for help\n",argv[0]);
		return EXIT_FAILURE;
	}

	while ((c=getopt(argc, argv, "cn:d:lho:spt:zk")) != -1) {
		switch (c) {
			case 'n':
				sscanf(optarg,"%d",&(config->stop_cond));
				break;
			case 'c':
				config->tob32=1;
				config->smart_subsec=1;
				break;
			case 'd':
				snprintf(config->separator,MAX_FORMAT,optarg);
				break;
			case 'o':
				snprintf(config->output_name,MAX_FORMAT,optarg);
				break;
			case 's':
				config->order_output=1;
				break;
			case 'h':
				printf("%s-%s (%s), a tool for extracting ascii data from Campbell binary files\n",argv[0],RELEASE,DATE);
				printf("Usage:\n");
				printf(" -c\t\t\tCompatibility: produces the same amount of output as tob32\n");
				printf(" -d [delimiter]\t\tUse [delimiter] as a field delimiter in the output\n");
				printf(" -h\t\t\tShow this help\n");
				printf(" -l\t\t\tShow the license\n");
				printf(" -n\t\t\tNumber of invalid frames triggering a full stop (0=never stop)\n");
				printf(" -o [output]\t\tOutput data to file [output]\n");
				printf(" -p \t\t\tUse stdin as input data (for pipes)\n");
				printf(" -s\t\t\tSort the output in chronological order (if necessary)\n");
				printf(" -t\t\t\tStyle to use for the output (grace, tob32, excel, sql)\n");
				printf(" -k\t\t\tDo not output sub-seconds if equal zero\n");
				printf(" -z\t\t\tCopy valid frames to the output, as binary\n");
				printf("---\n");
				printf("Mathias Bavay - WSL/SLF - %s\n",DATE);
				return EXIT_FAILURE;
				break;
			case 'l':
				printf("This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.\n");
				printf("This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License in file \"COPYING\" for more details, or write to\n");
				printf("\tFree Software Foundation, Inc. 675 Mass Ave, Cambridge, MA 02139, USA\nto obtain a copy of the license.\n");
				return EXIT_FAILURE;
				break;
			case 'k':
				config->smart_subsec=1;
				break;
			case 'p':
				snprintf(config->input_name,MAX_FILE_NAME,STDIN);
				break;
			case 't':
				style_outputs(config,optarg);
				break;
			case 'z':
				config->pass_through=1;
				break;
			default:
				status++;
				break;
		}
	}
	if(status>0) return EXIT_FAILURE;
	if(argc-optind>1) fprintf(stderr,"WARNING: multiple input files not supported yet! Only the first one will be treated\n");
	if(optind < argc) snprintf(config->input_name,MAX_FILE_NAME,argv[optind]);
	/*for ( ; optind < argc; optind++) {
		printf ("Non-option argument[%d] %s\n",optind, argv[optind]);
	}*/

	return EXIT_SUCCESS;
}

int open_files(config_structure *config){
/* open the input and output files according to the configuration settings */
/* returns EXIT_FAILURE if it is not possible, closing what should be closed in such a case */

	//output file
	if(strncmp(config->output_name,STDOUT,strlen(STDOUT)) == 0) {
		config->output=stdout;
	} else {
		#ifdef GNU
		if((config->output=fopen(config->output_name,"wm")) == NULL) { /* the m is a gnu extension */
		#else
		if((config->output=fopen(config->output_name,"w")) == NULL) {
		#endif
			fprintf(stderr,"ERROR (%s:%d): Output file %s cannot be openned\n",__FILE__,__LINE__,config->output_name);
			return EXIT_FAILURE;
		}
	}

	//input file
	if(strncmp(config->input_name,STDIN,strlen(STDIN)) == 0) {
		config->input=stdin;
	} else {
		#ifdef GNU
		if((config->input=fopen(config->input_name,"rmb")) == NULL) { // the m is a gnu extension
		#else
		if((config->input=fopen(config->input_name,"rb")) == NULL) {
		#endif
			fprintf(stderr,"ERROR (%s:%d): Input file %s cannot be openned\n",__FILE__,__LINE__,config->input_name);
			fclose(config->output);
			return EXIT_FAILURE;
		}
	}
	
	//HACK
	//if(setvbuf(config->input, NULL, _IOFBF, (size_t)(8192*8192)) != EXIT_SUCCESS) fprintf(stderr,"Error allocating buffer\n");
	//if(setvbuf(config->output, NULL, _IOFBF, (size_t)(8192*8192)) != EXIT_SUCCESS) fprintf(stderr,"Error allocating buffer\n");

	return EXIT_SUCCESS;
}

int check_types_sizes() {
/* This function checks the sizes of common types (as used in this program) */

	if(sizeof(short int)!=2) {
		fprintf(stderr,"ERROR (%s:%d): sizeof(short int)=%d, it should be 2!\n",__FILE__,__LINE__,sizeof(short int));
		return EXIT_FAILURE;
	}
	if(sizeof(int)!=4) {
		fprintf(stderr,"ERROR (%s:%d): sizeof(int)=%d, it should be 4!\n",__FILE__,__LINE__,sizeof(int));
		return EXIT_FAILURE;
	}
	if(sizeof(long long int)!=8) {
		fprintf(stderr,"ERROR (%s:%d): sizeof(long long int)=%d, it should be 8!\n",__FILE__,__LINE__,sizeof(long long int));
		return EXIT_FAILURE;
	}
	if(sizeof(float)!=4) {
		fprintf(stderr,"ERROR (%s:%d): sizeof(float)=%d, it should be 4!\n",__FILE__,__LINE__,sizeof(float));
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
