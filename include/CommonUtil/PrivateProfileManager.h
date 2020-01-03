#pragma once
#include <string>
#include "CommonDllDefine.h"
#include "TUtil.h"
#define MAX_SECTION_CNT 30
#define MAX_NAME_LEN 50
#define MAX_VALUE_STR_LEN 200
#define MAX_ENTRY_CNT 100
#define MAX_FILENAME_LEN 512

struct PrivateProfile
{
	char _entry_str[MAX_NAME_LEN];
	char _value_str[MAX_VALUE_STR_LEN];
};

struct PrivateProfileSection
{
	char _section_name[MAX_NAME_LEN];
	unsigned short _private_profile_cnt;
	PrivateProfile _private_profile[MAX_ENTRY_CNT];
};

/***********************************/
/*
ini형식의 파일을 읽고, 쓰기 위한 클래스ㅏㅐ
*/
class PrivateProfileManager
{
private:
	char _file_name[MAX_FILENAME_LEN];
	unsigned short _section_cnt;
	PrivateProfileSection private_profile_table[MAX_SECTION_CNT];

public:

	UTIL_DLLFunction PrivateProfileManager(char *file_name);
	UTIL_DLLFunction virtual ~PrivateProfileManager();
	UTIL_DLLFunction void write_file();
	UTIL_DLLFunction void flush();
	UTIL_DLLFunction void read_file();
	UTIL_DLLFunction bool set_entry_value(char * section_name, char * entry_name, char * entry_value);
	UTIL_DLLFunction bool set_entry_value(char * section_name, char * entry_name, int * entry_value);
	UTIL_DLLFunction bool set_entry_value(char * section_name, char * entry_name, unsigned int * entry_value);
	UTIL_DLLFunction bool set_entry_value(char * section_name, char * entry_name, float * entry_value);

	UTIL_DLLFunction bool get_entry_value(char * section_name, char * entry_name, char * entry_value);
	UTIL_DLLFunction bool get_entry_value(char * section_name, char * entry_name, int *entry_value, int default_value = 0);
	UTIL_DLLFunction bool get_entry_value(char * section_name, char * entry_name, unsigned int *entry_value, unsigned int default_value = 0);
	UTIL_DLLFunction bool get_entry_value(char * section_name, char * entry_name, float *entry_value, float default_value = 0.0f);

private:
	PrivateProfile create_private_profile(char * buf);

	unsigned short get_section_name(char *bp, char *section_name);
	int get_entry_idx(unsigned int section_idx, char *entry_name);
	bool is_section(char * bp);
	void add_entry_value(char * buf);
	bool add_entry_value(unsigned int section_idx, PrivateProfile private_profile);
	void update_entry_value(unsigned int section_idx, unsigned int entry_idx, char *entry_value);
	bool add_section(char * buf);
	void write_file(FILE *fp, unsigned int section_idx);
	void write_file(FILE *fp, unsigned int section_idx, unsigned int entry_idx);
	int get_section_idx(char *section_name);
};



