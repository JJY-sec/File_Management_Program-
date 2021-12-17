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
    
	printf("%s", buf);
	
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
		strcat(tag, ",");
		strcat(tag, file_tag);
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

int main() {
	search_db("음악", 'F');
}
