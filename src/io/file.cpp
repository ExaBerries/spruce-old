#include <io/file.h>

namespace spruce {
	namespace io {
		string readFileTxt(const FileHandle& file) {
			FILE* cfile = fopen(file.absolutePath.c_str(), "rt");
			if (cfile != NULL) {
				fseek(cfile, 0, SEEK_END);
				uint64 length = ftell(cfile);
				char* data = new char[length + 1];
				memset(data, 0, length + 1);
				fseek(cfile, 0, SEEK_SET);
				fread(data, sizeof(char), length, cfile);
				fclose(cfile);
				string result(data);
				delete[] data;
				return result;
			}
			return "";
		}

		uint8* readFileBin(const FileHandle& file, uint32& count) {
			FILE* cfile = fopen(file.absolutePath.c_str(), "rb");
			if (cfile != NULL) {
				fseek(cfile, 0, SEEK_END);
				uint64 length = ftell(cfile);
				uint8* data = new uint8[length + 1];
				memset(data, 0, length + 1);
				fseek(cfile, 0, SEEK_SET);
				fread(data, sizeof(uint8), length, cfile);
				fclose(cfile);
				count = length;
				return data;
			}
			return nullptr;
		}

		void writeFileTxt(const FileHandle& file, string& string){
			FILE* cfile = fopen(file.absolutePath.c_str(), "wt");
			if (cfile != NULL) {
				fputs(string.c_str(), cfile);
				fclose(cfile);
			}
		}

		void writeFileBin(const FileHandle& file, uint8* data, uint32& count) {
			FILE* cfile = fopen(file.absolutePath.c_str(), "wb");
			if (cfile != NULL) {
				fwrite(data, sizeof(uint8), count, cfile);
				fclose(cfile);
			}
		}
	}
}
