#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#import "Watcherdll.tlb" no_namespace named_guids

int main()
{
	Watcherdll *watch = NULL;
	Watcherdll *test = NULL; //
	CoInitialize(NULL); //dll 참조

	HRESULT hr = CoCreateInstance(CLSID_Watcher, NULL, CLSCTX_INPROC_SERVER, IID_Watcherdll, reinterpret_cast<void**>(&watch)); //dll
	
	if (SUCCEEDED(hr)) //여기서 dll함수 사용하는 것 같다. 아마 참조 성공
	{
		watch->InitWatcher();
		string s = test->Send_signal(); //아직 여기서 오류뜸

	}
	else // 참조 실패
	{
		CoUninitialize();</void**>;
	}	
	//printf(test);
	//약간 변형해서 사용
	if (Changed != NULL)
	{
		char path[_MAX_PATH] = File_Created();
		char row[max_row_len];
		char time[64];
		char size[64];

		itoa(fd.time_write, time, 10);
		itoa(fd.size, size, 10);

		char* name = strrchr(path, '\\');
		char* s_row = search_db(name, 'F');

		char new_row[max_row_len] = { 0, };
		char tag[total_tag_len] = { 0, };

		if (isFileOrDir(name) == 1)
		{
			extract_tag(s_row, tag);
			delete_db(path, s_row, 'F');
			strcpy(new_row, path);
			strcat(new_row, "|");
			strcat(new_row, tag);
			strcat(new_row, "|");
			strcat(new_row, time);
			strcat(new_row, "|");
			strcat(new_row, size);
			strcat(new_row, "|F\n");
			add_db(new_row);
		}
	}
	if (File_Created() != 1)
	{
		char path[_MAX_PATH] = File_Created();
		char row[max_row_len];
		char time[64];
		char size[64];

		itoa(fd.time_write, time, 10);
		itoa(fd.size, size, 10);

		strcpy(row, path);
		strcat(row, "|");
		strcat(row, "|");
		strcat(row, time);
		strcat(row, "|");
		strcat(row, size);

		char* name = strrchr(path, '\\');

		if (isFileOrDir(name) == 0)//dir
		{
			strcat(row, "|D\n");
			add_db(row);
		}
		else if (isFileOrDir(name) == 1)
		{
			strcat(row, "|F\n");
			add_db(row);
		}
	}
	if (File_Deleted() == 1)
	{
		char path[_MAX_PATH] = File_Deleted();
		char row[max_row_len];
		char time[64];
		char size[64];

		itoa(fd.time_write, time, 10);
		itoa(fd.size, size, 10);

		char* name = strrchr(path, '\\');

		if (isFileOrDir(name) == 0)//dir
		{
			delete_db(path, row, 'D');
		}
		else if (isFileOrDir(name) == 1)
		{
			delete_db(path, row, 'F');
		}
	}
	if (File_Renamed() == 1)
	{
		char old_path[_MAX_PATH] = strtok(File_Renamed(), "||");
		char new_path[_MAX_PATH] = strrchr(File_Renamed(), "||");
		char row[max_row_len];
		char tag[total_tag_len] = { 0, };
		char time[64];
		char size[64];

		char* oldname = strrchr(old_path, '\\');
		char* newname = strrchr(new_path, '\\');
		char new_row[max_row_len] = { 0, };

		extract_tag(oldname, tag);
		extract_time(oldname, time);
		extract_size(oldname, size);

		if (isFileOrDir(newname) == 0)//dir
		{
			strcpy(new_row, new_path);
			strcat(new_row, "|");
			strcat(new_row, tag);
			strcat(new_row, "|");
			strcat(new_row, time);
			strcat(new_row, "|");
			strcat(new_row, size);
			strcat(new_row, "|D\n");
			add_db(new_row);
		}
		else if (isFileOrDir(newname) == 1)
		{
			strcpy(new_row, new_path);
			strcat(new_row, "|");
			strcat(new_row, tag);
			strcat(new_row, "|");
			strcat(new_row, time);
			strcat(new_row, "|");
			strcat(new_row, size);
			strcat(new_row, "|F\n");
			add_db(new_row);
		}
	}
}
