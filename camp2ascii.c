#include <stdio.h>
#include <stdlib.h>
#include <signal.h>		/* signal management */
#include <time.h> 		/* for timing execution */
#include "camp2ascii.h"
#include "frame_read.h"
#include "files.h"
#include "initializations.h"

//global flag, goes to 1 when the user presses CTRL+C, etc
sig_atomic_t flag_stop;						/* flag for the end of the recording */

int main (int argc, char **argv)
{
	frame_structure frame;			//the structure that will contain the current frame
	config_structure config;		//the configuration (defaults and user's choice)
	header_structure header;		//for input file's header lines
	time_t t0,t2;				//for timing execution

	time(&t0);
	//applying the settings: defaults, then overloaded with user configuration
	//this is done before system's check, so the user can still list options, etc
	//even if his system is not correct for the program
	load_defaults(&config);
	if(read_cmd_line(argc,argv,&config)==EXIT_FAILURE) return EXIT_SUCCESS;

	//since we are currently platform dependent (32 or 64 bits), check that types are ok
	//if(check_types_sizes()==EXIT_FAILURE) return EXIT_FAILURE; //Not Needed Any More! (at least I hope...)
	if(check_endianness()==EXIT_FAILURE) return EXIT_FAILURE; 
	
	stop_catch_init();	//we catch SIG_INT and SIG_TERM

	//Open input and output files
	if(open_files(&config) == EXIT_FAILURE) return EXIT_FAILURE;

	//read file header and initialize the structures
	if(read_file_header(config.input,&header)==EXIT_FAILURE) return EXIT_FAILURE;
	if(analyze_file_header(header,&frame)==EXIT_FAILURE) return EXIT_FAILURE;
	print_headers(header,config,frame);		//print the header to the output file

	//allocating memory for the raw frame content
	frame.raw=(unsigned char *)calloc(sizeof(unsigned char),(size_t)MAX_LINE);

	//finally, reading data (and printing it)
	read_data(&config,&frame);

	//freeing memory from the raw frame
	free(frame.raw);
	//closing the files
	fclose(config.input);
	fclose(config.output);

	time(&t2);
	fprintf(stderr,"*** ");
	if(frame.type == IS_TOB1) fprintf(stderr,"TOB1 file - ");
	if(frame.type == IS_TOB2) fprintf(stderr,"TOB2 file - ");
	if(frame.type == IS_TOB3) fprintf(stderr,"TOB3 file - ");
	fprintf(stderr,"%ld lines read in %g seconds (elapsed time)\n",config.nb_lines_read,(double)(t2-t0));
	return EXIT_SUCCESS;
}
