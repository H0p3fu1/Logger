#include "FileSystem.h"

namespace FileSystem {
	bool CreateDirectories(const std::string& directories) {
		return std::filesystem::create_directories(directories);
	}

	std::string GetAbsoluteDirectory(const std::string& filename) {
		return std::filesystem::absolute(filename).string();
	}

	std::string GetParentDirectory(const std::string& filename) {
		return std::filesystem::path(filename).parent_path().string();
	}

	bool Exists(const std::string& filename) {
		return std::filesystem::exists(filename);
	}

	bool Remove(const std::string& filename) {
		return std::filesystem::remove(filename);
	}
}