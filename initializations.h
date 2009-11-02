/**************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*             Copyright SLF/WSL, 03/2007 - www.slf.ch                     *
**************************************************************************/
/* reading command line parameters, reading default values, ... */

#ifndef INIT_H
#define INIT_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int check_endianness();
int load_defaults(config_structure *config);
int read_cmd_line(int argc, char **argv, config_structure *config);
int open_files(config_structure *config);
int check_types_sizes();
int style_outputs(config_structure *config, char *style);

#endif
