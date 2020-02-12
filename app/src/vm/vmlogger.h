
#ifndef __VM_LOGGER_H__
#define __VM_LOGGER_H__

#include <core/logger.h>

#define LUA_LOG(...)	app::core::Logger::ChangeColor(app::core::LoggerColor::CYAN);\
						app::core::Logger::Log("[LUA]" __VA_ARGS__);\
						app::core::Logger::Log("\n");\
						app::core::Logger::ChangeColor(app::core::LoggerColor::ORIGINAL);

#endif //__VM_LOGGER_H__
