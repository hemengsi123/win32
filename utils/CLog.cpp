
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/stat.h>
#include <time.h>
#include <direct.h>
#include <errno.h>
#include <windows.h>
#include "CLog.h"


char * CLog::m_logdir = NULL;

int g_log_level = CLog::LOG_STDOUT | CLog::LOG_DETAIL |CLog::LOG_DEBUG;

void CLog::set_color(bool show_color, Color color)
{
	if(show_color)
	{
		static const int Color_count = int(WHITE)+1;
#ifdef __LINUX_CODE__
		enum ANSITextAttr
		{
			TA_NORMAL=0,
			TA_BOLD=1,
			TA_BLINK=5,
			TA_REVERSE=7
		};

		enum ANSIFgTextAttr
		{
			FG_BLACK=30, FG_RED,  FG_GREEN, FG_BROWN, FG_BLUE,
			FG_MAGENTA,  FG_CYAN, FG_WHITE, FG_YELLOW
		};

		enum ANSIBgTextAttr
		{
			BG_BLACK=40, BG_RED,  BG_GREEN, BG_BROWN, BG_BLUE,
			BG_MAGENTA,  BG_CYAN, BG_WHITE
		};
		static unsigned char UnixColorFG[Color_count] =
		{
			FG_BLACK,                                           // BLACK
			FG_RED,                                             // RED
			FG_GREEN,                                           // GREEN
			FG_BROWN,                                           // BROWN
			FG_BLUE,                                            // BLUE
			FG_MAGENTA,                                         // MAGENTA
			FG_CYAN,                                            // CYAN
			FG_WHITE,                                           // WHITE
			FG_YELLOW,                                          // YELLOW
			FG_RED,                                             // LRED
			FG_GREEN,                                           // LGREEN
			FG_BLUE,                                            // LBLUE
			FG_MAGENTA,                                         // LMAGENTA
			FG_CYAN,                                            // LCYAN
			FG_WHITE                                            // LWHITE
		};
	
		fprintf(stdout/*(stdout_stream? stdout : stderr)*/, "\x1b[%d%sm",UnixColorFG[color],(color>=YELLOW&&color<Color_count ?";1":""));
#else
//		 static WORD WinColorFG[Color_count] =
		static  unsigned short WinColorFG[Color_count] =
	    {
	        0,                                                  // BLACK
	        FOREGROUND_RED,                                     // RED
	        FOREGROUND_GREEN,                                   // GREEN
	        FOREGROUND_RED | FOREGROUND_GREEN,                  // BROWN
	        FOREGROUND_BLUE,                                    // BLUE
	        FOREGROUND_RED |                    FOREGROUND_BLUE,// MAGENTA
	        FOREGROUND_GREEN | FOREGROUND_BLUE,                 // CYAN
	        FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,// WHITE
	                                                            // YELLOW
	        FOREGROUND_RED | FOREGROUND_GREEN |                   FOREGROUND_INTENSITY,
	                                                            // RED_BOLD
	        FOREGROUND_RED |                                      FOREGROUND_INTENSITY,
	                                                            // GREEN_BOLD
	        FOREGROUND_GREEN |                   FOREGROUND_INTENSITY,
	        FOREGROUND_BLUE | FOREGROUND_INTENSITY,             // BLUE_BOLD
	                                                            // MAGENTA_BOLD
	        FOREGROUND_RED |                    FOREGROUND_BLUE | FOREGROUND_INTENSITY,
	                                                            // CYAN_BOLD
	        FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
	                                                            // WHITE_BOLD
	        FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY
	    };

	    HANDLE hConsole = ::GetStdHandle(/*stdout_stream ? STD_OUTPUT_HANDLE : STD_ERROR_HANDLE*/ STD_OUTPUT_HANDLE);
	    ::SetConsoleTextAttribute(hConsole, WinColorFG[color]);
#endif
	}
}

void CLog::reset_color(bool show_color)
{
	if(show_color)
	{
#ifdef __LINUX_CODE__
		fprintf(stdout/*( stdout_stream ? stdout : stderr )*/, "\x1b[0m");
#else
		HANDLE hConsole = ::GetStdHandle(/*stdout_stream ? STD_OUTPUT_HANDLE : STD_ERROR_HANDLE */STD_OUTPUT_HANDLE);
		::SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED );
#endif
	}
}

const char* CLog::log_dir(const char* logdir)
{
	if( !m_logdir )
	{
		static char log_dir[255] = {0,};
		strncpy(log_dir, logdir, sizeof(log_dir));
		m_logdir = log_dir;
	}
	return m_logdir;
}
//===================== version 1==========================
int CLog::write_log(const char* prefix, const char* fmt,...)
{
	int ret_len = 0;

	char log_file[255] = {0,};
	set_default_path(log_file, sizeof(log_file), log_dir(), prefix);
	
	FILE * fout = fopen(log_file, "ab");
	if( fout )
	{
		va_list arg_ptr;
		va_start(arg_ptr, fmt);
		int bytes = vfprintf(fout, fmt, arg_ptr);
		va_end(arg_ptr);
		//fwrite("\n", 1, 1, fout);
		fclose(fout);
		ret_len = bytes;
	}
	else
		ret_len = -1;
	
	return ret_len;
}
//==========================================
int CLog::write_log(int log_mask, const char* prefix, const char* fmt,...)
{
	int ret_len = 0;
	// 屏幕打印
	if(log_mask & LOG_STDOUT)
	{
		// 是否屏幕打印
		if( log_mask & LOG_DETAIL )
		{
			va_list arg_ptr;
			va_start(arg_ptr, fmt);
			int bytes = vfprintf(stdout, fmt, arg_ptr);
			va_end(arg_ptr);
			
			ret_len = bytes;
		}
	}
	// 日志文件
	if(log_mask & LOG_FILE)
	{
		if( log_mask & LOG_DETAIL )
		{
			char log_file[255] = {0,};
			set_default_path(log_file, sizeof(log_file), log_dir(), prefix);
			
			FILE * fout = fopen(log_file, "ab");
			if( fout )
			{
				va_list arg_ptr;
				va_start(arg_ptr, fmt);
				int bytes = vfprintf(fout, fmt, arg_ptr);
				va_end(arg_ptr);
				//fwrite("\n", 1, 1, fout);
				fclose(fout);
				ret_len = bytes;
			}
			else
				ret_len = -1;
		}
	}
	
	return ret_len;
}
// 
int CLog::write_error_log(int log_mask,const char* prefix, const char* fmt, ...)
{
	int ret_len = 0;
	
	if(log_mask & LOG_STDOUT)
	{
		// 是否屏幕打印
		if( log_mask & LOG_ERROR || log_mask & LOG_DETAIL )
		{
			set_color(log_mask & LOG_COLOR, RED);
			fwrite("#", 1, 1, stdout);
			va_list arg_ptr;
			va_start(arg_ptr, fmt);
			int bytes = vfprintf(stdout, fmt, arg_ptr);
			va_end(arg_ptr);
			reset_color(log_mask & LOG_COLOR);
			ret_len = bytes;
		}
	}
	// 日志文件
	if(log_mask & LOG_FILE)
	{
		if( log_mask & LOG_ERROR || log_mask & LOG_DETAIL )
		{
			char log_file[255] = {0,};
			set_default_path(log_file, sizeof(log_file), log_dir(), prefix);
			
			FILE * fout = fopen(log_file, "ab");
			if( fout )
			{
				fwrite("#", 1, 1, fout);
				va_list arg_ptr;
				va_start(arg_ptr, fmt);
				int bytes = vfprintf(fout, fmt, arg_ptr);
				va_end(arg_ptr);
				//fwrite("\n", 1, 1, fout);
				fclose(fout);
				ret_len = bytes;
			}
			else
				ret_len = -1;
		}
	}
	
	
	return ret_len;
}

int CLog::write_hex_log(int log_mask, const char* prefix, const char * buff, int buff_len, const char *msg_tip)
{
	int ret_len = 0;
	if(log_mask & LOG_STDOUT)
	{
		// 是否屏幕打印
		
		if( (log_mask & LOG_HEXBUFF) && (buff != NULL) )
		{
			set_color(log_mask & LOG_COLOR, CYAN);
			if( msg_tip != NULL )
			{
				printf("%s HEX: ", msg_tip);
			}
			else
				printf("HEX: ");
			
			for(int i=0; i< buff_len; ++i)
			{
				printf("%02X ", (unsigned char)buff[i]);
			}
			printf("\n");
			reset_color(log_mask & LOG_COLOR);
			ret_len = buff_len;
		}
	}
	// 日志文件
	if(log_mask & LOG_FILE)
	{
		if( (log_mask & LOG_HEXBUFF) && (buff != NULL) )
		{
			char log_file[255] = {0,};
			set_default_path(log_file, sizeof(log_file), log_dir(), prefix);
			
			FILE * fout = fopen(log_file, "ab");
			if( fout )
			{
				if( msg_tip != NULL)
					fprintf(fout, "\n%s HEX: ", msg_tip);
				else
					fprintf(fout, "\nHEX: ");
				
				for(int i=0; i<buff_len; ++i)
				{
					fprintf(fout, "%02X ", (unsigned char)buff[i]);
				}
				fwrite("\n\n", 2, 1, fout);
				fclose(fout);
				
				ret_len = buff_len;
			}
			else
				ret_len = -1;
		}
	}
	
	return ret_len;
}
// #define log_debug(...) write_debug_log( __FILE__, __LINE__, ...)
int CLog::write_debug_log(const char* filename, int line, int log_mask,const char* prefix, const char* fmt, ...)
{
	int ret_len = 0;
//		CCriticalSection log_lock;
	const char * strname = NULL;
	char strtime[20] = {0};
	get_time_string(strtime);
#ifdef __LINUX_CODE__
	if(filename)
	{
		strname = strrchr(filename, '/');
		if(!strname)
			strname = strrchr(filename, '\\');
	}
	
	if( !strname )
		strname = "";
	else
		strname += 1; // skip '/'
#else
	strname = filename;
#endif

	// 屏幕打印
	if(log_mask & LOG_STDOUT)
	{
		// 是否屏幕打印
		if( log_mask & LOG_DEBUG )
		{
//				log_lock.Lock();
			set_color(log_mask & LOG_COLOR, BROWN);
			FILE * fout = stdout;
			//fwrite("\t", 1, 1, fout);
			fprintf(fout, "%s[%s(%d) %d] ", strtime, strname, line, gettid());
			va_list arg_ptr;
			va_start(arg_ptr, fmt);
			int bytes = vfprintf(fout, fmt, arg_ptr);
			va_end(arg_ptr);
			reset_color(log_mask & LOG_COLOR);
//				log_lock.Unlock();
			
			ret_len = bytes;
		}
	}
	// 日志文件
	if(log_mask & LOG_FILE)
	{
		if( log_mask & LOG_DEBUG )
		{
			char log_file[255] = {0,};
			set_default_path(log_file, sizeof(log_file), log_dir(), prefix);
			
//				log_lock.Lock();
			FILE * fout = fopen(log_file, "ab");
			if( fout )
			{
				fwrite("\t", 1, 1, fout);
				fprintf(fout, "%s[%s(%d) %d] ", strtime, strname, line, gettid());
				va_list arg_ptr;
				va_start(arg_ptr, fmt);
				int bytes = vfprintf(fout, fmt, arg_ptr);
				va_end(arg_ptr);
				//fwrite("\n", 1, 1, fout);
				fclose(fout);
				ret_len = bytes;
			}
			else
				ret_len = -1;
//				log_lock.Unlock();
		}
	}
	return ret_len;
}
void CLog::get_time_string(char * time_str)
{
	struct tm *ptm;
//		long       ts;
	time_t     ts;

	ts    =   time(NULL);
	ptm   =   localtime(&ts);
	
	sprintf(time_str, "[%02u:%02u:%02u]", ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
}
void CLog::get_date_string(char * date_str)
{
	struct   tm     *ptm;
//		long       ts;
	time_t     ts;
	int         y,m,d;
	ts    =   time(NULL);
	ptm   =   localtime(&ts);

	y   =   ptm->tm_year + 1900;     //年
	m   =   ptm->tm_mon + 1;         //月
	d   =   ptm->tm_mday;            //日
	
	sprintf(date_str, "%04u-%02u-%02u", y, m, d);
}
void CLog::get_date_time_string(char * date_str)
{
	struct   tm     *ptm;
//		long       ts;
	time_t     ts;
	int         y,m,d;
	ts    =   time(NULL);
	ptm   =   localtime(&ts);

	y   =   ptm->tm_year + 1900;     //年
	m   =   ptm->tm_mon + 1;         //月
	d   =   ptm->tm_mday;            //日
	
	sprintf(date_str, "%04u-%02u-%02u[%02u:%02u:%02u]", y, m, d, ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
}
bool CLog::is_file_exist(const char *filepath)
{
#ifdef __LINUX_CODE__
	struct stat tmp;
	int ret = stat(filepath, &tmp);
#else
	struct _stat tmp;
	int ret = _stat(filepath, &tmp);
#endif
	if( ret == 0)
	{
		if(S_ISDIR(tmp.st_mode))
		{
			printf("%s is a dir.\n", filepath);
		}
		else
		{
			return true;
		}
	}
	else
	{
		if( errno == ENOENT) /* No such file or directory */
		{
			printf("no such file or dir.\n");
		}
	}
	
	return false;
}
bool CLog::is_dir_exist(const char *filepath)
{
#ifdef __LINUX_CODE__
	struct stat tmp;
	int ret = stat(filepath, &tmp);
#else
	struct _stat tmp;
	int ret = _stat(filepath, &tmp);
#endif
	if( ret == 0)
	{
		if(S_ISDIR(tmp.st_mode))
		{
			return true;
		}
		else
		{
			printf("%s is a normal file.\n", filepath);
		}
	}
	else
	{
		if( errno == ENOENT) /* No such file or directory */
		{
			printf("no such file or dir.\n");
		}
	}
	
	return false;
}

void CLog::set_default_path(char * fullPath, int path_max_len, const char * deflt_dir, const char *prefix)
{
	if( !is_dir_exist(deflt_dir) )
	{
		printf("to mkdir: %s\n", deflt_dir);
#ifdef __LINUX_CODE__
		mkdir(deflt_dir, S_IWRITE);
#else
		_mkdir(deflt_dir);
#endif
	}
	
	char date_str[20] = {0,};
	get_date_string(date_str);
	
	if(prefix != NULL)
	{
#ifdef __LINUX_CODE__
		snprintf(&fullPath[0], path_max_len, "%s/%s[%s].log", deflt_dir, prefix, date_str);
#else
		_snprintf(&fullPath[0], path_max_len, "%s/%s[%s].log", deflt_dir, prefix, date_str);
#endif
	}
	else
	{
#ifdef __LINUX_CODE__
		snprintf(&fullPath[0], path_max_len, "%s/%s.log", deflt_dir, date_str);
#else
		_snprintf(&fullPath[0], path_max_len, "%s/%s.log", deflt_dir, date_str);
#endif
	}
}


