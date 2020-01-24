
#ifdef __WINDOWS__

#include <unordered_map>
#include <core/logger.h>

#include <Windows.h>

namespace app { namespace core {

	static std::unordered_map<LoggerColor, int32_t> LOGGER_COLOR_MAP({
		{ LoggerColor::ORIGINAL,	7  },
		{ LoggerColor::BLUE,		9  },
		{ LoggerColor::GREEN,		10 },
		{ LoggerColor::CYAN,		11 },
		{ LoggerColor::RED,			12 },
		{ LoggerColor::MAGENTA,		13 },
		{ LoggerColor::YELLOW,		14 },
		{ LoggerColor::WHITE,		15 },
	});

	void Logger::ChangeColor(LoggerColor color) {
		HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
		if (console) {
			SetConsoleTextAttribute(console, (WORD)LOGGER_COLOR_MAP[color]);
		}
	}

	void Logger::Log(const char * fmt, ...) {
		va_list args;
		va_start(args, fmt);
		vprintf(fmt, args);
		va_end(args);
	}

} }

#endif //__WINDOWS__