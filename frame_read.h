/**************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*             Copyright SLF/WSL, 03/2007 - www.slf.ch                     *
**************************************************************************/
/* Here, we put all functions related to the details of reading a frame */

#ifndef FRAME_READ_H
#define FRAME_READ_H

#include "types_processing.h"
#include "structures.h"

#define MAX_FIELD 40
#define NB_MAX_FIELDS 200
#define MAX_LINE 2048

int read_frame_data(frame_structure *frame,const config_structure *config);
int read_frame_header(frame_structure *frame);
int read_frame_footer(frame_structure *frame);
int analyze_tob32_frame(frame_structure *frame,const config_structure *config,pass_types pass);
int analyze_tob1_frame(frame_structure *frame,const config_structure *config);
int print_timestamp(const frame_structure *frame,const config_structure *config);

#endif
