#pragma once

#include <string>
#include "CommonDllDefine.h"

#define MAX_LINE_LENGTH 1024
#define MAX_PROCESS_NAME 128
#define MAX_PAYLOAD_SIZE 64*1024	//MAX_PAYLOAD_SIZE 는 FRAGMENT_MSG_SIZE 보다 크게 설정

#ifdef WIN32
#define msleep(msec)   Sleep((unsigned int)msec)
#else
#include <unistd.h>
#define msleep(usec)   usleep(usec*1000)
#endif

#define htonc_t htonss_t 
#define ntohc_t ntohss_t 

#ifdef WIN32
#define err_num GetLastError()
#else
#define	err_num errno
#endif

//private_profile_ini을 위한 함수들
UTIL_DLLFunction void ignore_blank(char *bp);
UTIL_DLLFunction int read_line(FILE *fp, char *bp);
UTIL_DLLFunction void getParam(char *buff, char *param, int &offset);
bool isSection(char *buff);
bool isDefinedSection(FILE *fp, const char *section);
char *getEntryStr(FILE *fp, const char *entry, char *buff);
UTIL_DLLFunction void remove_comment(char *buf);
UTIL_DLLFunction bool is_comment(char *buf);
#ifdef WIN32
char* parse_file_name(char *file_path);
#endif

UTIL_DLLFunction void miilisec_sleep(unsigned int miliseconds);
UTIL_DLLFunction const char *get_current_process_name();
UTIL_DLLFunction int get_process_id();

#ifdef WIN32
UTIL_DLLFunction int get_private_profile_int(char *section, char *entry, int def, char *file_name);
UTIL_DLLFunction float get_private_profile_float(char *section, char *entry, float def, char *file_name);
UTIL_DLLFunction int get_private_profile_string(char *section, char *entry, char *def, char *buffer, int buffer_len, char *file_name);
#else
UTIL_DLLFunction int get_private_profile_int(const char *section, const char *entry, int def, const char *file_name);
UTIL_DLLFunction float get_private_profile_float(const char *section, const char *entry, float def, const char *file_name);
UTIL_DLLFunction int get_private_profile_string(const char *section, const char *entry, const char *def, char *buffer, int buffer_len, const char *file_name);
#endif
UTIL_DLLFunction int ntohss_t(const char* src, char* dst);
UTIL_DLLFunction int htonss_t(const char* src, char* dst);

UTIL_DLLFunction int ntohs_t(const char* src, char* dst);
UTIL_DLLFunction int htons_t(const char* src, char* dst);

UTIL_DLLFunction int ntohl_t(const char* src, char* dst);
UTIL_DLLFunction int htonl_t(const char* src, char* dst);

UTIL_DLLFunction int ntohll_t(const char* src, char* dst);
UTIL_DLLFunction int htonll_t(const char* src, char* dst);

UTIL_DLLFunction int ntohstr_t(const char* src, char* dst, unsigned int str_len);
UTIL_DLLFunction int htonstr_t(const char* src, char* dst, unsigned int str_len);


UTIL_DLLFunction int ntoh_t(char &value, char *payload);
UTIL_DLLFunction int ntoh_t(unsigned char &value, char *payload);
UTIL_DLLFunction int ntoh_t(short &value, char *payload);
UTIL_DLLFunction int ntoh_t(unsigned short &value, char *payload);
UTIL_DLLFunction int ntoh_t(int &value, char *payload);
UTIL_DLLFunction int ntoh_t(unsigned int &value, char *payload);
UTIL_DLLFunction int ntoh_t(long  &value, char *payload);
UTIL_DLLFunction int ntoh_t(unsigned long &value, char *payload);
UTIL_DLLFunction int ntoh_t(long long &value, char *payload);
UTIL_DLLFunction int ntoh_t(float &value, char *payload);
UTIL_DLLFunction int ntoh_t(double &value, char *payload);
UTIL_DLLFunction int ntoh_t(char *str, unsigned int str_len, char *payload);

UTIL_DLLFunction int hton_t(char &value, char *payload);
UTIL_DLLFunction int hton_t(unsigned char &value, char *payload);
UTIL_DLLFunction int hton_t(short &value, char *payload);
UTIL_DLLFunction int hton_t(unsigned short &value, char *payload);
UTIL_DLLFunction int hton_t(int &value, char *payload);
UTIL_DLLFunction int hton_t(unsigned int &value, char *payload);
UTIL_DLLFunction int hton_t(long &value, char *payload);
UTIL_DLLFunction int hton_t(unsigned long &value, char *payload);
UTIL_DLLFunction int hton_t(long long &value, char *payload);
UTIL_DLLFunction int hton_t(float &value, char *payload);
UTIL_DLLFunction int hton_t(double &value, char *payload);
UTIL_DLLFunction int hton_t(char *str, unsigned int str_len, char *payload);


UTIL_DLLFunction char ntoh_t(char &value);
UTIL_DLLFunction unsigned char ntoh_t(unsigned char &value);
UTIL_DLLFunction short ntoh_t(short &value);
UTIL_DLLFunction unsigned short ntoh_t(unsigned short &value);
UTIL_DLLFunction int ntoh_t(int &value);
UTIL_DLLFunction unsigned int ntoh_t(unsigned int &value);
UTIL_DLLFunction long ntoh_t(long  &value);
UTIL_DLLFunction unsigned long ntoh_t(unsigned long &value);
UTIL_DLLFunction long long ntoh_t(long long &value);
UTIL_DLLFunction float ntoh_t(float &value);
UTIL_DLLFunction double ntoh_t(double &value);

UTIL_DLLFunction char hton_t(char &value);
UTIL_DLLFunction unsigned char hton_t(unsigned char &value);
UTIL_DLLFunction short hton_t(short &value);
UTIL_DLLFunction unsigned short hton_t(unsigned short &value);
UTIL_DLLFunction int hton_t(int &value);
UTIL_DLLFunction unsigned int hton_t(unsigned int &value);
UTIL_DLLFunction long hton_t(long  &value);
UTIL_DLLFunction unsigned long hton_t(unsigned long &value);
UTIL_DLLFunction long long hton_t(long long &value);
UTIL_DLLFunction float hton_t(float &value);
UTIL_DLLFunction double hton_t(double &value);


void swap2bytes(const char* src, char* dst);
void swap4bytes(const char* src, char* dst);
void swap8bytes(const char* src, char* dst);

bool get_system_endian();
UTIL_DLLFunction void wsa_startup();

#ifdef LINUX
int get_process_name_by_pid(int pid, char *name);
#endif


