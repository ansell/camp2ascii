/**************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*             Copyright SLF/WSL, 03/2007 - www.slf.ch                     *
**************************************************************************/
/* a few generic functions that are usefull for us */

#ifndef GENERIC_H
#define GENERIC_H

#include <signal.h>		/* signal management */
#include <inttypes.h>		/* C99 fixed size types (uint_32, etc) */
#include "limits.h"
#include "structures.h"

extern sig_atomic_t flag_stop;

void sig_stop(int sig);	//for catching SIG_INT and SIG_TERM, thus closing in a clean way
int stop_catch_init();
int read_ascii_fields(FILE *data_file,char fields[NB_MAX_FIELDS][MAX_FIELD]);
void types_sizes();
void print_frame_struct(frame_structure *frame);


#endif
