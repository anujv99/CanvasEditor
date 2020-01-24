
#ifndef __HANDLED_OBJECT_H__
#define __HANDLED_OBJECT_H__

#include "assert.h"
#include "stronghandle.h"

namespace app { namespace utils {

	template<typename T>
	class HandledObject {
	public:
		HandledObject();

		StrongHandle<T> GetHandle();

		inline int RefCount() const { return m_ReferenceCount; }
		inline void AddRef();
		inline void ReleaseRef();
	private:
		int m_ReferenceCount;
	};

	template<typename T>
	void HandledObject<T>::ReleaseRef() {
		m_ReferenceCount--;
		ASSERT(m_ReferenceCount >= 0);
	}

	template<typename T>
	void HandledObject<T>::AddRef() {
		m_ReferenceCount++;
		ASSERT(m_ReferenceCount >= 0);
	}

	template<typename T>
	StrongHandle<T> HandledObject<T>::GetHandle() {
		ASSERT(m_ReferenceCount >= 0);
		return StrongHandle<T>((T *)this);
	}

	template<typename T>
	HandledObject<T>::HandledObject() : m_ReferenceCount(0) {}

} }

#endif //__HANDLED_OBJECT_H__