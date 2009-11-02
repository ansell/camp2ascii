/**************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*             Copyright SLF/WSL, 03/2007 - www.slf.ch                     *
**************************************************************************/
/* Here, we put all functions related to the files operations */

#ifndef FILES_H
#define FILES_H

#define TM_Y0 1900		//when using the mktime() function, year should be the number of years since 1900

int read_header_line6(char types[NB_MAX_FIELDS][MAX_FIELD],frame_structure *frame);
int read_header_line2(char fields[NB_MAX_FIELDS][MAX_FIELD],int nb_fields,frame_structure *frame);
int read_file_header(FILE *data_file, header_structure *h);
int analyze_file_header(header_structure header,frame_structure *frame);
int print_headers(header_structure h,config_structure config,frame_structure frame);
int load_frame(config_structure config,frame_structure *frame);
int read_data(config_structure *config,frame_structure *frame);


#endif
