#include "bufferlayout.h"

namespace app { namespace graphics {

	utils::StrongHandle<BufferLayout> BufferLayout::Create() {
		return new BufferLayout();
	}

	unsigned int GetSizeBytes(DataType type) {
		switch (type) {
		case DataType::Int:       return 4;
		case DataType::Int2:      return 4 * 2;
		case DataType::Int3:      return 4 * 3;
		case DataType::Int4:      return 4 * 4;
		case DataType::UInt:      return 4;
		case DataType::UInt2:     return 4 * 2;
		case DataType::UInt3:     return 4 * 3;
		case DataType::UInt4:     return 4 * 4;
		case DataType::Float:     return 4;
		case DataType::Float2:    return 4 * 2;
		case DataType::Float3:    return 4 * 3;
		case DataType::Float4:    return 4 * 4;
		case DataType::Bool:      return 1;
		case DataType::UByte:     return 1;
		case DataType::UByte2:    return 1 * 2;
		case DataType::UByte3:    return 1 * 3;
		case DataType::UByte4:    return 1 * 4;
		case DataType::Byte:      return 1;
		case DataType::Byte2:     return 1 * 2;
		case DataType::Byte3:     return 1 * 3;
		case DataType::Byte4:     return 1 * 4;
		}
		ASSERTM(false, "Inavlid data type");
		return 0;
	}

	BufferLayout::BufferLayout() : m_Entries(), m_IsConstructing(false), m_Stride(0u), m_IsAutoStride(false) {}

	void BufferLayout::BeginEntries(int stride) {
		m_IsConstructing = true;
		if (stride == -1) {
			m_IsAutoStride = true;
		} else {
			m_Stride = (unsigned int)stride;
			m_IsAutoStride = false;
		}
	}

	void BufferLayout::AddEntry(DataType type, unsigned int offsetBytes, std::string semanticName, bool normalize) {
		ASSERTM(m_IsConstructing, "Forgot to call BeginEntries");

		Entry entry;
		entry.Type          = type;
		entry.OffsetBytes   = offsetBytes;
		entry.Normalize     = normalize;
		entry.Name          = semanticName;

		if (m_IsAutoStride) {
			if (offsetBytes + GetSizeBytes(type) > m_Stride) {
				m_Stride = offsetBytes + GetSizeBytes(type);
			}
		}
		
		m_Entries.push_back(entry);
	}

	void BufferLayout::EndEntries() {
		ASSERTM(m_IsConstructing, "Forgot to call BeginEntries");
		m_IsConstructing = false;
	}

	unsigned int Entry::GetNumElements() const {
		switch (Type) {
		case DataType::Int:       return 1;
		case DataType::Int2:      return 2;
		case DataType::Int3:      return 3;
		case DataType::Int4:      return 4;
		case DataType::UInt:      return 1;
		case DataType::UInt2:     return 2;
		case DataType::UInt3:     return 3;
		case DataType::UInt4:     return 4;
		case DataType::Float:     return 1;
		case DataType::Float2:    return 2;
		case DataType::Float3:    return 3;
		case DataType::Float4:    return 4;
		case DataType::Bool:      return 1;
		case DataType::UByte:     return 1;
		case DataType::UByte2:    return 2;
		case DataType::UByte3:    return 3;
		case DataType::UByte4:    return 4;
		case DataType::Byte:      return 1;
		case DataType::Byte2:     return 2;
		case DataType::Byte3:     return 3;
		case DataType::Byte4:     return 4;
		}

		ASSERTM(false, "Inavlid data type");
		return 0;
	}

} }