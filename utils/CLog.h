/*
 *  created by simon 2014.11.10
 */

#ifndef _CLOG_H_
#define _CLOG_H_

#pragma warning(disable:4996)

// __GNUC__ 
#ifdef __LINUX_CODE__
#include <sys/syscall.h>  // __NR_gettid
#define gettid() syscall(__NR_gettid)
// _MSC_VER
#else
#include <sys/timeb.h>
#define gettid() ::GetCurrentThreadId()
#define S_ISDIR(mask)  ((mask) & _S_IFDIR)
#endif

#ifndef log_write
#define log_write(...) if( g_log_level ) { CLog::write_log(g_log_level, "debug", __VA_ARGS__);}
#endif
#ifndef log_error
#define log_error(...) if( g_log_level ) { CLog::write_error_log(g_log_level, "debug", __VA_ARGS__);}
#endif
#ifndef log_debug
#define log_debug(...) if( g_log_level ) { CLog::write_debug_log(__FILE__, __LINE__, g_log_level, "debug", __VA_ARGS__);}
#endif

class CLog
{
private:
	static char * m_logdir;
	
public:
	// bitmask
	enum LOG_MASK
	{
		LOG_NORMAL  = 0x0000,   // ����¼��־
		LOG_DETAIL  = 0x0001,   // ��ϸ��־��¼
		LOG_ERROR   = 0x0002,   // ��¼������Ϣ
		LOG_HEXBUFF = 0x0004,   // ��ӡʮ����������
		LOG_DEBUG   = 0x0008,   // ��ӡ�ض���Ϣ
		LOG_COLOR   = 0x0010,   // ��ɫ��ʾ
		
		LOG_STDOUT  = 0x0f00,   // ��д��־��ֻ����Ļ��ӡ
		LOG_FILE    = 0xf000,   // ֻд��־��������Ļ�ϴ�ӡ
		LOG_BOTH    = 0xff00,
	};
	enum Color
	{
		BLACK,
		RED,
		GREEN,
		BROWN,
		BLUE,
		MAGENTA,
		CYAN,
		GREY,
		YELLOW,
		LRED,
		LGREEN,
		LBLUE,
		LMAGENTA,
		LCYAN,
		WHITE
	};
	
public:
	static void set_color(bool show_color, Color color);

	static void reset_color(bool show_color);

	static const char* log_dir(const char* logdir="./logs");
	//===================== version 1==========================
	static int write_log(const char* prefix, const char* fmt,...);
	//==========================================
	static int write_log(int log_mask, const char* prefix, const char* fmt,...);
	// 
	static int write_error_log(int log_mask,const char* prefix, const char* fmt, ...);
	static int write_hex_log(int log_mask, const char* prefix, const char * buff, int buff_len, const char *msg_tip = NULL );
	// #define log_debug(...) write_debug_log( __FILE__, __LINE__, ...)
	static int  write_debug_log(const char* filename, int line, int log_mask,const char* prefix, const char* fmt, ...);
	static void get_time_string(char * time_str);
	static void get_date_string(char * date_str);
	static void get_date_time_string(char * date_str);
	static bool is_file_exist(const char *filepath);
	static bool is_dir_exist(const char *filepath);
protected:
	static void set_default_path(char * fullPath, int path_max_len, const char * deflt_dir="./logs", const char *prefix = NULL);
	
};

#endif
