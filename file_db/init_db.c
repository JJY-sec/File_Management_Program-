#include "file_db.h"

// mode는 반드시 F혹은 D로 입력해야 한다. 
void extract_path(char* row, char* file_path) {
	int end = 0;
	while(1) {
		if(row[end] == '|') {
			strncpy(file_path, row, end);
			break;
		}
		end++;
	}
	return;
}

void extract_tag(char* row, char* file_tag) {
	int start = 0;
	int end = 0;
	while(1) {
		if(row[end] == '|') {
			end++;
			break;
		}
		end++;
	}
	start = end;

	while(1) {
		if(row[end] == '|') {
			strncpy(file_tag, row+start, end-start);
			break;
		}
		end++;
	}
	return;
}

void extract_time(char* row, char* file_time) {
	int start = 0;
	int end = 0;
	while(1) {
		if(row[end] == '|') {
			end++;
			break;
		}
		end++;
	}
	start = end;

	while(1) {
		if(row[end] == '|') {
			end++;
			break;
		}
		end++;
	}
	start = end;
	
	while(1) {
		if(row[end] == '|') {
			strncpy(file_time, row+start, end-start);
			break;
		}
		end++;
	}
	return;
}

void extract_size(char* row, char* file_size) {
	int start = 0;
	int end = 0;
	while(1) {
		if(row[end] == '|') {
			end++;
			break;
		}
		end++;
	}
	start = end;

	while(1) {
		if(row[end] == '|') {
			end++;
			break;
		}
		end++;
	}
	start = end;
	
	while(1) {
		if(row[end] == '|') {
			end++;
			break;
		}
		end++;
	}
	start = end;
	
	while(1) {
		if(row[end] == '|') {
			strncpy(file_size, row+start, end-start);
			break;
		}
		end++;
	}
	return;
}

void print_show(char* search) {
	char file_path[_MAX_PATH];
	char file_tag[total_tag_len];
	char file_time[max_time];
	char file_size[max_size];

	char* arr = (char*)malloc(strlen(search)*sizeof(char));
	strcpy(arr, search);
	
	char* result;
	char* next;
	result = strtok_r(arr, "\n", &next);
	while(result != NULL) {
		char file_path[_MAX_PATH] = {0,};
		char file_tag[total_tag_len] = {0,};
		char file_time[max_time] = {0,};
		char file_size[max_size] = {0,};
	
		extract_path(result, file_path);
		extract_tag(result, file_tag);
		extract_time(result, file_time);
		extract_size(result, file_size);
		printf("경로명: %s 태그: %s 생성시간: %s 크기: %s\n", file_path, file_tag, file_time, file_size); 
		
		result = strtok_r(NULL, "\n", &next);
	}
	free(arr);
}

int is_tag(char* file_tag, char* tag) {
	char temp[total_tag_len] = {0,};
	strcpy(temp, file_tag);
	
	char* result;
	char* next;
	int is_tag = 0;
	
	result = strtok_r(temp, ",", &next);
	
	while(result != NULL) {
		if(strcmp(tag, result) != 0)
			result = strtok_r(NULL, ",", &next);
		else
			is_tag = 1;
	}
	return is_tag;
}

void add_db(char* row) {// 아무 조건 없이 row를 그대로 파일에 반영한다. 
	fp = file_open(file_db, "a");
	if(fp == NULL)
		return;
	fwrite(row, strlen(row), 1, fp);
	fclose(fp);
	return;
}


char* search_db(char* name, char mode) {
// mode에 맞는 row를 선택해 path와 tag를 name과 매칭해 일치하는 것을 전부 찾는다.
// 나중에 반드시 리턴값을 free해야 한다. 
    char row[max_row_len];
    int count=0;
    
	fp = file_open(file_db, "r");
	
	if(fp == NULL)
		return NULL;

    fseek(fp, 0, SEEK_END);
    long int file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    
 	char* buf = (char*)malloc(file_size*sizeof(char));
 	buf[0] = 0;
 	
 	long int cum = 0;
 	
    // 문자열 검색
    
    
    while (!feof(fp)) {
    	row[0] = 0;
        fgets(row, max_row_len, fp);// str에 한 행씩 문자열을 입력받습니다

		char path[_MAX_PATH] = {0,};
		char tag[total_tag_len] = {0,};
		
		extract_path(row, path);
		extract_tag(row, tag);

		char is_exists = row[strlen(row)-2];
		
        if (strstr(path, name) != NULL && is_exists == mode) {
        	strcpy(buf+cum, row);
        	cum += strlen(row);
            count = 1; 
        }
        else if(strstr(tag, name) != NULL && is_exists == mode) {
        	strcpy(buf+cum, row);
        	cum += strlen(row);
            count = 1;
        }
    }
    
    fclose(fp);
	
    if(count == 0) {
    	free(buf);
		return NULL;
	}
	return buf;
}

char* delete_db(char* file_path, char* deleted_row, char mode) {
// mode를 기준으로 경로를 찾아 mode를 고려하여 삭제 표시를 한 후 원래 row를 deleted_file에 적는다. 
// 일치하는 row가 있으면 deleted_row를 리턴하고 일치하는 것이 없을 경우 NULL을 리턴한다.  
    char row[max_row_len];

    fp = file_open(file_db, "r+");
    if(fp == NULL)
    	return NULL;
 	
    // 문자열 검색
    while (!feof(fp)) {
        fgets(row, max_row_len, fp);// str에 한 행씩 문자열을 입력받습니다

		char path[_MAX_PATH] = {0,};
		
		extract_path(row, path);

		char is_exists = row[strlen(row)-2];
		
        if(strcmp(path, file_path) == 0 && is_exists == mode) {
        	strcpy(deleted_row, row);
        	fseek(fp, -3, SEEK_CUR);
        	char temp[1];
        	temp[0] = mode+32;
        	fwrite(temp, 1, 1, fp);
            fclose(fp);
            return deleted_row;
        }
    }
    
    fclose(fp);
	printf("삭제를 진행할 수 없습니다.");
	return NULL;
}

void add_tag(char* file_path, char* file_tag) {
// 일치하는 파일 경로명에 새로운 태그를 추가한다.(중복여부를 체크한다.) 
	char deleted_row[max_row_len] = {0,};
	char new_row[max_row_len] = {0,};
	
	char path[_MAX_PATH] = {0,};
	char tag[total_tag_len] = {0,};
	char time[max_time] = {0,};
	char size[max_size] = {0,};
	
	delete_db(file_path, deleted_row, 'F');
	
	extract_path(deleted_row, path);
	extract_tag(deleted_row, tag);
	
	if(is_tag(tag, file_tag) == 0) {
		if(tag[0] == 0)
			strcat(tag, file_tag);
		else {
			strcat(tag, ",");
			strcat(tag, file_tag);
		}
	}
	extract_time(deleted_row, time);
	extract_size(deleted_row, size);
	
	strcat(new_row, path);
	strcat(new_row, "|");
	strcat(new_row, tag);
	strcat(new_row, "|");
	strcat(new_row, time);
	strcat(new_row, "|");
	strcat(new_row, size);
	strcat(new_row, "|");
	strcat(new_row, "|F\n");
	add_db(new_row);
	
	return;
}


void delete_tag(char* file_path, char* file_tag) {
// 일치하는 파일 경로명에 태그를 삭제한다. 
	char deleted_row[max_row_len] = {0,};
	char new_row[max_row_len] = {0,};
	
	char path[_MAX_PATH] = {0,};
	char tag[total_tag_len] = {0,};
	char time[max_time] = {0,};
	char size[max_size] = {0,};
	
	delete_db(file_path, deleted_row, 'F');
	
	extract_path(deleted_row, path);
	extract_tag(deleted_row, tag);
	extract_time(deleted_row, time);
	extract_size(deleted_row, size);
	
	char new_tag[total_tag_len] = {0,};
	
	char* result;
	char* next;
	
	result = strtok_r(tag, ",", &next);
	if(strcmp(file_tag, result) != 0) {
		strcat(new_tag, result);
		result = strtok_r(NULL, ",", &next);
	}
	
	while(result != NULL) {
		if(strcmp(file_tag, result) != 0) {
			strcat(new_tag, ",");
			strcat(new_tag, result);
			result = strtok_r(NULL, ",", &next);
		}
	}
	
	strcat(new_row, path);
	strcat(new_row, "|");
	strcat(new_row, new_tag);
	strcat(new_row, "|");
	strcat(new_row, time);
	strcat(new_row, "|");
	strcat(new_row, size);
	strcat(new_row, "|");
	strcat(new_row, "|F\n");
	add_db(new_row);
	
	return;
}

void get_ext(char* filename, char* extension) {
    char* ptr = NULL;
    ptr = strrchr(filename, '.');
    
    if (ptr == NULL) {
    	strcpy(extension, "\0");
		return;
    }
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

int is_file() {
    if (fd.attrib & _A_SUBDIR)
        return 0;// 디렉터리일 경우 0을 리턴 
    else
        return 1;// 디렉터리가 아닐경우 0을 리턴
}

void file_search(char file_path[]) {
    intptr_t handle;
    int check = 0;
    char file_path2[_MAX_PATH];
 
    strcat(file_path, "\\");
    strcpy(file_path2, file_path);
    strcat(file_path, "*");
 
    if ((handle = _findfirst(file_path, &fd)) == -1) {
        printf("No such file or directory\n");
        return;
    }
 
    while (_findnext(handle, &fd) == 0) {
        char file_pt[_MAX_PATH];
        strcpy(file_pt, file_path2);
        strcat(file_pt, fd.name);
 
        check = is_file();// 파일인지 디렉토리 인지 식별
 
        if (check == 0 && fd.name[0] != '.') {
        	char time[64];
        	itoa(fd.time_write, time, 10);
        	
        	char size[64];
        	itoa(fd.size, size, 10);
        	
        	char row[max_row_len];
        	strcpy(row, file_pt);
        	strcat(row, "|");
        	strcat(row, "|");
        	strcat(row, time);
        	strcat(row, "|");
        	strcat(row, size);
        	strcat(row, "|D\n");
        	fwrite(row, strlen(row), 1, fp);
        	
            file_search(file_pt);// 하위 디렉토리 검색 재귀함수
        }
            
        else if (check == 1 && fd.size != 0 && fd.name[0] != '.') {
        	char extension[_MAX_PATH];
        	get_ext(file_pt, extension);
        	
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
        	strcat(row, "|F");
        	strcat(row, "\n");
        	fwrite(row, strlen(row), 1, fp);
        }
    }
    _findclose(handle);
}

void init_db() {
	fp = file_open(file_db, "w+");
	if(fp == NULL)
		return;
    file_search(file_path);
    fclose(fp);
    return;
}

void optimize_db() {
    char row[max_row_len];

    fp = file_open(file_db, "r");
    if(fp == NULL)
    	return;

    fseek(fp, 0, SEEK_END);
    long int file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    
 	char* buf = (char*)malloc(file_size*sizeof(char));
 	buf[0] = 0;
 	
 	long int cum = 0;
 	
    while (!feof(fp)) {
        fgets(row, max_row_len, fp);// str에 한 행씩 문자열을 입력받습니다
		char is_exists = row[strlen(row)-2];
		
        if(is_exists == 'F' || is_exists == 'D') {
        	strcpy(buf+cum, row);
        	cum += strlen(row);
        }
    }
    
    fclose(fp);
    
    fp = file_open(file_db, "w+");
    if(fp == NULL)
    	return;
    	
    fwrite(buf, strlen(buf), 1, fp);
    fclose(fp);
    free(buf);

	return;
}

int main() {
	init_db();
	int choose;
	int mode;
	char search[max_row_len];
	char file_path[_MAX_PATH];
	char file_tag[total_tag_len];
	char deleted_row[max_row_len];
	
	int count_while = 0;
	while(1) {
		count_while++;
		if(count_while > cw) {
			printf("최적화를 진행중입니다."); 
			count_while = 0;
			optimize_db();	
		}
		
		fflush(stdin);
		printf("1 검색, 2 태그추가, 3 태그삭제, 4 파일삭제, 5 종료: ");
		scanf("%d", &choose);
		fflush(stdin);
		switch(choose) {
			case 1:
				printf("1 파일, 2 디렉터리: ");
				scanf("%d", &mode);
				fflush(stdin);
				if(mode == 1) {
					printf("이름 혹은 태그를 입력하세요: ");
					gets(search);
					char* show = search_db(search, 'F');
					if(show == NULL)
						printf("존재하지 않습니다.\n");
					else {
						print_show(show);
						//printf("%s", show);
						free(show);
					}
				}
				else if(mode == 2) {
					printf("이름 혹은 태그를 입력하세요: ");
					gets(search);
					char* show = search_db(search, 'D');
					if(show == NULL)
						printf("존재하지 않습니다.\n");
					else { 
						print_show(show);
						//printf("%s", show);
						free(show);
					}
				}
				else
					printf("올바르지 않습니다. 처음부터 다시입력하세요\n");	
				break;
			case 2:
				printf("경로명을 입력하세요: ");
				gets(file_path);
				printf("태그명을 입력하세요: ");
				gets(file_tag);
				add_tag(file_path, file_tag);
				break;
	
			case 3:
				printf("경로명을 입력하세요: ");
				gets(file_path);
				printf("태그명을 입력하세요: ");
				gets(file_tag);
				delete_tag(file_path, file_tag);
				break;
			
			case 4:
				printf("경로명을 입력하세요: ");
				gets(file_path);
				delete_db(file_path, deleted_row, 'F');
				break;
			default:
				printf("올바른 숫자를 입력하세요.\n");
				break;
		}
	}
}
