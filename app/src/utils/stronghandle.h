
#ifndef __STRING_HANDLE_H__
#define __STRING_HANDLE_H__

namespace paint { namespace utils {

	template<typename T>
	class StrongHandle {
	public:
		StrongHandle(T * val = nullptr);
		StrongHandle(const StrongHandle & val);
		~StrongHandle();

		inline T * Get() const { return m_PointerValue; }
		inline T & Ref() const { return *m_PointerValue; }
		inline bool IsNull() const;

		inline T * operator->();
		inline const T * operator->() const;
		inline T & operator*();
		inline const T & operator*() const;

		inline operator T * ();
		inline operator const T * () const;

		StrongHandle & operator=(T * val);
		StrongHandle & operator=(const StrongHandle & other);

		bool operator==(const StrongHandle & other) const;
		bool operator==(const T * val) const;
		bool operator!=(const StrongHandle & other) const;
		bool operator!=(const T * val) const;
	private:
		T * m_PointerValue;

		inline void AddRef();
		inline void ReleaseRef();
	};

	template<typename T>
	void StrongHandle<T>::ReleaseRef() {
		if (m_PointerValue) {
			m_PointerValue->ReleaseRef();

			if (m_PointerValue->RefCount() == 0) {
				delete m_PointerValue;
				m_PointerValue = nullptr;
			}
		}
	}

	template<typename T>
	void StrongHandle<T>::AddRef() {
		if (m_PointerValue) m_PointerValue->AddRef();
	}

	template<typename T>
	bool StrongHandle<T>::operator!=(const T * other) const {
		return !(*this == other);
	}

	template<typename T>
	bool StrongHandle<T>::operator!=(const StrongHandle & other) const {
		return !(*this == other);
	}

	template<typename T>
	bool StrongHandle<T>::operator==(const T * other) const {
		return m_PointerValue == other;
	}

	template<typename T>
	bool StrongHandle<T>::operator==(const StrongHandle & other) const {
		return m_PointerValue == other.m_PointerValue;
	}

	template<typename T>
	StrongHandle<T> & StrongHandle<T>::operator=(const StrongHandle & other) {
		if (m_PointerValue == other.m_PointerValue) return *this;

		ReleaseRef();
		m_PointerValue = other.m_PointerValue;
		AddRef();

		return *this;
	}

	template<typename T>
	StrongHandle<T> & StrongHandle<T>::operator=(T * val) {
		if (m_PointerValue == val) return *this;

		ReleaseRef();
		m_PointerValue = val;
		AddRef();

		return *this;
	}

	template<typename T>
	StrongHandle<T>::operator const T * () const {
		return m_PointerValue;
	}

	template<typename T>
	StrongHandle<T>::operator T * () {
		return m_PointerValue;
	}

	template<typename T>
	const T & StrongHandle<T>::operator*() const {
		return *m_PointerValue;
	}

	template<typename T>
	T & StrongHandle<T>::operator*() {
		return *m_PointerValue;
	}

	template<typename T>
	const T * StrongHandle<T>::operator->() const {
		return m_PointerValue;
	}

	template<typename T>
	T * StrongHandle<T>::operator->() {
		return m_PointerValue;
	}

	template<typename T>
	bool StrongHandle<T>::IsNull() const {
		return m_PointerValue == nullptr;
	}

	template<typename T>
	StrongHandle<T>::~StrongHandle() {
		ReleaseRef();
	}

	template<typename T>
	StrongHandle<T>::StrongHandle(const StrongHandle & val) {
		m_PointerValue = val.m_PointerValue;
		AddRef();
	}

	template<typename T>
	StrongHandle<T>::StrongHandle(T * val /*= nullptr*/) : m_PointerValue(val) {
		AddRef();
	}

} }

#endif //__STRING_HANDLE_H__