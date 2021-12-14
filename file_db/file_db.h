#ifndef __FILEDB_H__
#define __FILEDB_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <Windows.h>

#define total_tag_len 100
#define max_row_len _MAX_PATH+total_tag_len+128

struct _finddata_t fd;
char file_path[_MAX_PATH] = "C:";
char file_db[_MAX_PATH] = "C:\\Users\\file_db.txt";
FILE* fp;

#endif
