#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>

namespace FileSystem {
	bool CreateDirectories(const std::string& directory);
	std::string GetAbsoluteDirectory(const std::string& filename);
	std::string GetParentDirectory(const std::string& filename);
	bool Exists(const std::string& filename);
	bool Remove(const std::string& filename);
}
