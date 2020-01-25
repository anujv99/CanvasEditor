
/*
 *  BufferLayout is used to describe the layout of the vertexbuffer
 *  This layout is just for the users convinience. Vertex Array is actually responsible for feeding data properly to the rendering pipeline
 */

#ifndef __BUFFER_LAYOUT_H__
#define __BUFFER_LAYOUT_H__

#include <string>
#include <vector>

#include <utils/handledobject.h>
#include <utils/stronghandle.h>

namespace app { namespace graphics {

    enum class DataType {
        None,
        Int, Int2, Int3, Int4,
        UInt, UInt2, UInt3, UInt4,
        Float, Float2, Float3, Float4,
        UByte, UByte2, UByte3, UByte4,
        Byte, Byte2, Byte3, Byte4,
        Bool
    };

    unsigned int GetSizeBytes(DataType type);

    struct Entry {
        std::string Name;
        DataType Type;
        unsigned int OffsetBytes;
        bool Normalize;

        unsigned int GetNumElements() const;
    };

    class BufferLayout final : public utils::HandledObject<BufferLayout> {
        friend class utils::StrongHandle<BufferLayout>;
    private:
        BufferLayout();
        ~BufferLayout() {}
    public:
        void BeginEntries();
        void AddEntry(DataType type, unsigned int offsetBytes, std::string semanticName, bool normalize = false);
        void EndEntries();

        inline size_t GetNumEntries() const { return m_Entries.size(); }

        inline std::vector<Entry>::const_iterator begin() const { return m_Entries.begin(); }
        inline std::vector<Entry>::const_iterator end() const { return m_Entries.end(); }
    public:
        static utils::StrongHandle<BufferLayout> Create();
    private:
        std::vector<Entry> m_Entries;
        bool m_IsConstructing;
    };

} }

#endif //__BUFFER_LAYOUT_H__
