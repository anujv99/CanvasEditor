
#ifndef __FILE_UTILS_H__
#define __FILE_UTILS_H__ 

#include <fstream>
#include <string>
#include <ios>

#include "handledobject.h"
#include "assert.h"

#define ROOT_PATH "../../../../"

namespace app { namespace utils {

	struct DataFile final : public HandledObject<DataFile> {
		friend class StrongHandle<DataFile>;
		friend class FileUtils;
	private:
		DataFile() {}
		~DataFile() {}
	public:
		std::string & GetContent() {
			if (m_Content.size() > 0) return m_Content;
			m_File.seekg(0, std::ios::end);
			m_Content.resize(m_File.tellg());
			m_File.seekg(0, std::ios::beg);
			m_File.read(m_Content.data(), m_Content.size());
			m_File.close();
			return m_Content;
		}
	private:
		std::string m_FilePath;
		std::string m_Content;
		std::fstream m_File;
	};

	class FileUtils final {
	private:
		FileUtils() {}
		~FileUtils() {}
	public:
		inline static bool FileExists(const char * path) {
			return std::ifstream(path).operator bool();
		}

		inline static std::string ConvertToRelativePath(const char * path) {
			return std::string(ROOT_PATH) + path;
		}

		inline static StrongHandle<DataFile> OpenFile(const char * path) {
			ASSERTM(FileExists(path), "File not found");
			DataFile * file = new DataFile();
			file->m_File = std::fstream(path);
			file->m_FilePath = path;
			ASSERTM(file->m_File, "Failed to open file");
			return file;
		}

	};

} }

#endif //__FILE_UTILS_H__
