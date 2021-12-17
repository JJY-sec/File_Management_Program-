#import "Watcherdll.tlb" //no_namespace named_guids
#include "file_db.h"

void extract_path(char* row, char* file_path);
//void extract_tag(char* row, char* file_tag);
void extract_time(char* row, char* file_time);
void extract_size(char* row, char* file_size);
//int is_tag(char* file_tag, char*tag);
void add_db(char* row);
char* search_db(char* name, char mode);
char* delete_db(char* file_path, char* deleted_row, char mode);
//void add_tag(char* file_path, char* file_tag);
//void delete_tag(char* file_path, char* file_tag);

int isFileOrDir(char* s)
{
	if (fd.attrib & _A_SUBDIR)
		return 0;
	else
		return 1;
}