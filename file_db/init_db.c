#include "file_db.h"

FILE* file_open(char* path, char* mode) {
	fp = fopen(path, mode);
	if(fp == NULL) {
		printf("파일 open 실패!");
		return NULL;
	}
	return fp;
}

void getExt(char* filename, char* extension) {
    char* ptr = NULL;
    ptr = strrchr(filename, '.');
    
    if (ptr == NULL)
        return;
    strcpy(extension, ptr + 1);
	return;
}

char* check_basic_extension(char* extension) {	
	if(strcmp(extension, "avi") == 0 || strcmp(extension, "mp4") == 0)
		return "영상";
	else if(strcmp(extension, "mp3") == 0)
		return "음악";
	else if(strcmp(extension, "jpg") == 0)
		return "사진";
	else if(strcmp(extension, "pdf") == 0)
		return "문서";
	else
		return NULL;
}

int isFileOrDir() {
    if (fd.attrib & _A_SUBDIR)
        return 0;// 디렉터리일 경우 0을 리턴 
    else
        return 1;// 디렉터리가 아닐경우 0을 리턴
}

void FileSearch(char file_path[]) {
    intptr_t handle;
    int check = 0;
    char file_path2[_MAX_PATH];
 
    strcat(file_path, "\\");
    strcpy(file_path2, file_path);
    strcat(file_path, "*");
 
    if ((handle = _findfirst(file_path, &fd)) == -1) {
        //printf("No such file or directory\n");
        return;
    }
 
    while (_findnext(handle, &fd) == 0) {
        char file_pt[_MAX_PATH];
        strcpy(file_pt, file_path2);
        strcat(file_pt, fd.name);
 
        check = isFileOrDir();// 파일인지 디렉토리 인지 식별
 
        if (check == 0 && fd.name[0] != '.')
            FileSearch(file_pt);// 하위 디렉토리 검색 재귀함수
            
        else if (check == 1 && fd.size != 0 && fd.name[0] != '.') {
        	char extension[_MAX_PATH];
        	getExt(file_pt, extension);
        	
        	char* tag = check_basic_extension(extension);
        	
        	char time[64];
        	itoa(fd.time_write, time, 10);
        	
        	char size[64];
        	itoa(fd.size, size, 10);
        	
        	char row[max_row_len];
        	strcpy(row, file_pt);
        	strcat(row, "|");
        	if(tag == NULL)
        		strcat(row, "|");
        	else {
        		strcat(row, tag);
        		strcat(row, "|");
        	}
        	strcat(row, time);
        	strcat(row, "|");
        	strcat(row, size);
        	strcat(row, "|y");
        	strcat(row, "\n");
        	fwrite(row, strlen(row), 1, fp);
        	//printf("%s", row);
        }
    }
    _findclose(handle);
}

void init_db() {
	fp = file_open(file_db, "w+");
	if(fp == NULL)
		return;
    FileSearch(file_path);
    fclose(fp);
    return;
}

int main() {
	init_db();
}
