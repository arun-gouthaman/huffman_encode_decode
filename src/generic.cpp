#include "generic.h"

const std::string sanitize_file_path(std::string& file_path, const std::string& ext)
{
	std::filesystem::path file_sys_path = std::filesystem::path(file_path);

	// Check if parent path is directory
	if (!std::filesystem::is_directory(file_sys_path.parent_path()))
	{
		std::cout << "Invalid Directory " << file_sys_path.parent_path() << "\n";
		return std::string("");
	}

	// Replace extension if provided
	if (!ext.empty())
	{
		file_sys_path.replace_extension(ext);
	}

	return file_sys_path.string();
}