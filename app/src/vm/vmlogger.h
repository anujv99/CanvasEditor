
#ifndef __VM_LOGGER_H__
#define __VM_LOGGER_H__

#include <core/logger.h>

#define LUA_LOG(...)	app::core::Logger::ChangeColor(app::core::LoggerColor::RED);\
						app::core::Logger::Log("[LUA]" __VA_ARGS__);\
						app::core::Logger::Log("\n");\
						app::core::Logger::ChangeColor(app::core::LoggerColor::ORIGINAL);

#define LUA_DEBUG_LOG(...)	app::core::Logger::ChangeColor(app::core::LoggerColor::RED);\
							{\
								lua_Debug ar;\
								lua_getstack(L, 1, &ar);\
								lua_getinfo(L, "nSl", &ar);\
								app::core::Logger::Log("[LUA %s:%d]", ar.source, ar.currentline);\
							}\
							app::core::Logger::Log( __VA_ARGS__);\
							app::core::Logger::Log("\n");\
							app::core::Logger::ChangeColor(app::core::LoggerColor::ORIGINAL);

#endif //__VM_LOGGER_H__
