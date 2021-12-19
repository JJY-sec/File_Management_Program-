#include "file_db.h"

// mode�� �ݵ�� FȤ�� D�� �Է��ؾ� �Ѵ�. 
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
		printf("��θ�: %s �±�: %s �����ð�: %s ũ��: %s\n", file_path, file_tag, file_time, file_size); 
		
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

void add_db(char* row) {// �ƹ� ���� ���� row�� �״�� ���Ͽ� �ݿ��Ѵ�. 
	fp = file_open(file_db, "a");
	if(fp == NULL)
		return;
	fwrite(row, strlen(row), 1, fp);
	fclose(fp);
	return;
}


char* search_db(char* name, char mode) {
// mode�� �´� row�� ������ path�� tag�� name�� ��Ī�� ��ġ�ϴ� ���� ���� ã�´�.
// ���߿� �ݵ�� ���ϰ��� free�ؾ� �Ѵ�. 
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
 	
    // ���ڿ� �˻�
    
    
    while (!feof(fp)) {
    	row[0] = 0;
        fgets(row, max_row_len, fp);// str�� �� �྿ ���ڿ��� �Է¹޽��ϴ�

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
// mode�� �������� ��θ� ã�� mode�� ����Ͽ� ���� ǥ�ø� �� �� ���� row�� deleted_file�� ���´�. 
// ��ġ�ϴ� row�� ������ deleted_row�� �����ϰ� ��ġ�ϴ� ���� ���� ��� NULL�� �����Ѵ�.  
    char row[max_row_len];

    fp = file_open(file_db, "r+");
    if(fp == NULL)
    	return NULL;
 	
    // ���ڿ� �˻�
    while (!feof(fp)) {
        fgets(row, max_row_len, fp);// str�� �� �྿ ���ڿ��� �Է¹޽��ϴ�

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
	printf("������ ������ �� �����ϴ�.");
	return NULL;
}

void add_tag(char* file_path, char* file_tag) {
// ��ġ�ϴ� ���� ��θ� ���ο� �±׸� �߰��Ѵ�.(�ߺ����θ� üũ�Ѵ�.) 
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
// ��ġ�ϴ� ���� ��θ� �±׸� �����Ѵ�. 
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
		return "����";
	else if(strcmp(extension, "mp3") == 0)
		return "����";
	else if(strcmp(extension, "jpg") == 0)
		return "����";
	else if(strcmp(extension, "pdf") == 0)
		return "����";
	else
		return NULL;
}

int is_file() {
    if (fd.attrib & _A_SUBDIR)
        return 0;// ���͸��� ��� 0�� ���� 
    else
        return 1;// ���͸��� �ƴҰ�� 0�� ����
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
 
        check = is_file();// �������� ���丮 ���� �ĺ�
 
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
        	
            file_search(file_pt);// ���� ���丮 �˻� ����Լ�
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
        fgets(row, max_row_len, fp);// str�� �� �྿ ���ڿ��� �Է¹޽��ϴ�
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
			printf("����ȭ�� �������Դϴ�."); 
			count_while = 0;
			optimize_db();	
		}
		
		fflush(stdin);
		printf("1 �˻�, 2 �±��߰�, 3 �±׻���, 4 ���ϻ���, 5 ����: ");
		scanf("%d", &choose);
		fflush(stdin);
		switch(choose) {
			case 1:
				printf("1 ����, 2 ���͸�: ");
				scanf("%d", &mode);
				fflush(stdin);
				if(mode == 1) {
					printf("�̸� Ȥ�� �±׸� �Է��ϼ���: ");
					gets(search);
					char* show = search_db(search, 'F');
					if(show == NULL)
						printf("�������� �ʽ��ϴ�.\n");
					else {
						print_show(show);
						//printf("%s", show);
						free(show);
					}
				}
				else if(mode == 2) {
					printf("�̸� Ȥ�� �±׸� �Է��ϼ���: ");
					gets(search);
					char* show = search_db(search, 'D');
					if(show == NULL)
						printf("�������� �ʽ��ϴ�.\n");
					else { 
						print_show(show);
						//printf("%s", show);
						free(show);
					}
				}
				else
					printf("�ùٸ��� �ʽ��ϴ�. ó������ �ٽ��Է��ϼ���\n");	
				break;
			case 2:
				printf("��θ��� �Է��ϼ���: ");
				gets(file_path);
				printf("�±׸��� �Է��ϼ���: ");
				gets(file_tag);
				add_tag(file_path, file_tag);
				break;
	
			case 3:
				printf("��θ��� �Է��ϼ���: ");
				gets(file_path);
				printf("�±׸��� �Է��ϼ���: ");
				gets(file_tag);
				delete_tag(file_path, file_tag);
				break;
			
			case 4:
				printf("��θ��� �Է��ϼ���: ");
				gets(file_path);
				delete_db(file_path, deleted_row, 'F');
				break;
			default:
				printf("�ùٸ� ���ڸ� �Է��ϼ���.\n");
				break;
		}
	}
}
