#ifndef __FILEDB_H__
#define __FILEDB_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <Windows.h>

#define max_time		15
#define max_size		15
#define total_tag_len	100
#define max_row_len _MAX_PATH + total_tag_len + max_time + max_size
#define cw 100

struct _finddata_t fd;
char file_path[_MAX_PATH] = "C:";
char file_db[_MAX_PATH] = "C:\\file_db.txt";
FILE* fp;

FILE* file_open(char* path, char* mode) {
	fp = fopen(path, mode);
	if(fp == NULL) {
		printf("파일 open 실패!");
		return NULL;
	}
	return fp;
}


#endif
