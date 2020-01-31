
#ifdef __UNIX__

#include <core/logger.h>

namespace app { namespace core {

		void Logger::ChangeColor(LoggerColor color) {
		}

		void Logger::Log(const char * fmt, ...) {
			va_list args;
			va_start(args, fmt);
			vprintf(fmt, args);
			va_end(args);
		}

} }

#endif //__LINUX__
