#pragma once
#include <version>  // version include has yvals_core.h include so we can use __cpp_lib_filesystem & __cpp_lib_experimental_filesystem defines
#include <fstream>
#include <functional>

#if defined(PLATFORM_WINDOWS)
	#include <Windows.h>
#endif

#if defined(__cpp_lib_filesystem)
	#include <filesystem>
	namespace fs = std::filesystem;
#elif defined(__cpp_lib_experimental_filesystem)
	#include <experimental/filesystem>
	namespace fs = std::experimental::filesystem;
#else
	#error compiler does not support std::filesystem
#endif
#include <iostream>

class FileUtils final
{
public:
	/*
	*	Returns path to current .exe file
	*/
	static fs::path GetExecutableFilePath()
	{
#if defined(PLATFORM_WINDOWS)
		wchar_t path[MAX_PATH]{ 0 };
		::GetModuleFileName(NULL, path, MAX_PATH);
		return fs::path(path);
#else
		char result[PATH_MAX]{ 0 };
		const ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
		const std::string path(result, (count > 0) ? count : 0);
		return fs::path{path};
#endif	
	}


	/*
	*	Returns dir which contains .exe file 
	*/
	static fs::path GetExecutableDir()
	{
		return GetExecutableFilePath().parent_path();
	}


public:
	static bool Read(const fs::path& filename, std::string& buffer)
	{
		if (std::ifstream ifs{ filename, std::ios::binary | std::ios::ate }) // ate: open at the end
		{
			const std::size_t file_size = static_cast<std::size_t>(ifs.tellg());
			buffer.resize(file_size, '\000');
			ifs.seekg(0, std::ios::beg);
			ifs.read(buffer.data(), file_size);
			ifs.close();
			return true;
		}
		else
		{
			std::cerr << "Failed to read file " << filename.string() << std::endl;
			return false;
		}
	}

	static bool Write(const fs::path& filename, const std::string_view& buffer)
	{
		if (std::ofstream ofs{ filename, std::ios::binary })
		{
			ofs.write(buffer.data(), buffer.size());
			ofs.close();
			return true;
		}
		else
		{
			std::cerr << "Failed to write file " << filename.string() << std::endl;
			return false;
		}
	}


	/*
	*	Reads a file chunk by chunk
	*/
	static void ReadChunks(const fs::path &filename, const std::size_t max_chunk_size, const std::function<bool(std::vector<unsigned char>&&)>& callback)
	{
		if (std::ifstream ifs{ filename, std::ios::binary }) 
		{
			while (!ifs.eof())
			{
				std::vector<std::uint8_t> chunk(max_chunk_size, '\000');
				ifs.read((char*)chunk.data(), chunk.size());

				// resize chunk if we read bytes less than max_chunk_size
				const auto bytes_read = static_cast<std::size_t>(ifs.gcount());
				if (bytes_read < max_chunk_size)
					chunk.resize(bytes_read);

				// serve chunk and see if true was returned from callback to stop reading loop.
				const bool should_stop = callback(std::move(chunk));
				if (should_stop)
					break;
			}
			ifs.close();
		}
		else
		{
			std::cerr << "Failed to read file file chunks " << filename.string() << std::endl;
		}
	}

};
