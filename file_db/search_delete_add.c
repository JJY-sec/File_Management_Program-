#include "file_db.h"

FILE* file_open(char* path, char* mode) {
	fp = fopen(path, mode);
	if(fp == NULL) {
		printf("파일 open 실패!");
		return NULL;
	}
	return fp;
}

char* search_db(char* name) {
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

		char file_path[_MAX_PATH] = {0,};
		char tag[total_tag_len];
		tag[0] = 0;
		
		int start = 0;
		int end = 0;
		while(1) {
			if(row[end] == '|') {
				strncpy(file_path, row+start, end-start);
				end++;
				break;
			}
			end++;
		}
		start = end;

		while(1) {
			if(row[end] == '|') {
				strncpy(tag, row+start, end-start);
				end++;
				break;
			}
			end++;
		}

		char is_exists = row[strlen(row)-2];
		
        if (strstr(file_path, name) != NULL && is_exists == 'y') {
        	strcpy(buf+cum, row);
        	cum += strlen(row);
            count = 1;
        }
        else if(strstr(tag, name) != NULL && is_exists == 'y') {
        	strcpy(buf+cum, row);
        	cum += strlen(row);
            count = 1;
        }
 
    }
    
    fclose(fp);
    
	printf("%s", buf);
	
    if(count == 0)
		return NULL;

	return buf;
}


void delete_db(char* name) {
    char row[max_row_len];

    fp = file_open(file_db, "r+");
    if(fp == NULL)
    	return;
 	
    // 문자열 검색
    while (!feof(fp)) {
        fgets(row, max_row_len, fp);// str에 한 행씩 문자열을 입력받습니다

		char file_path[_MAX_PATH] = {0,};
		
		int end = 0;
		while(1) {
			if(row[end] == '|') {
				strncpy(file_path, row, end);
				break;
			}
			end++;
		}

		char is_exists = row[strlen(row)-2];
		
        if(strcmp(file_path, name) == 0 && is_exists == 'y') {
        	fseek(fp, -3, SEEK_CUR);
        	char temp[1];
        	temp[0] = 'n';
        	fwrite(temp, 1, 1, fp);
            fclose(fp);
            return;
        }
    }
    
    fclose(fp);
	printf("존재하지 않는 파일입니다.");
	return;
}

void add_db(char* row) {
	fp = file_open(file_db, "a");
	if(fp == NULL)
		return;
	fwrite(row, strlen(row), 1, fp);
	fclose(fp);
	return;
}

int main() {
	search_db("음악");
}
