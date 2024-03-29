#pragma once
#include <common.h>
#include <io/FileHandle.h>

namespace spruce {
	namespace io {
		[[nodiscard]] string readFileTxt(const FileHandle& file);
		[[nodiscard]] buffer<uint8> readFileBin(const FileHandle& file);
		void writeFileTxt(const FileHandle& file, string& string);
		void writeFileBin(const FileHandle& file, buffer<uint8> data);
	}
}
