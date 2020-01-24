
/*
 *  Interface to logger
 *  Actual platform sepcific implementation is in external/win32 and external/unix
 *  Support multi-color logging
 */

#ifndef __LOGGER_H__
#define __LOGGER_H__

namespace app { namespace core {

    enum class LoggerColor {
		RED,
		WHITE,
		BLUE,
		GREEN,
		YELLOW,
		CYAN,
		MAGENTA,
		ORIGINAL
    };

	class Logger {
	public:
		static void ChangeColor(LoggerColor color);
		static void Log(const char * fmt, ...);
	};

} }

#endif //__LOGGER_H__
