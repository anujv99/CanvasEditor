
#ifndef __LUA_TYPES_H__
#define __LUA_TYPES_H__

namespace app {

	struct TypeID {
		template<typename T>
		static inline unsigned int value() {
			static unsigned int id = ++counter;
			return id;
		}
	private:
		static unsigned int counter;
	};

}

#endif //__LUA_TYPES_H__
