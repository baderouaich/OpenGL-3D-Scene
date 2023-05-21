#pragma once
#include <glad/glad.h>
#include <cstring>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <csignal>

class OpenGLUtils final
{
public:

	/*
	*	Convert GL Error Enum macro name to a String, returns "Unknown GLenum" on non detected macros 
	*/
	static constexpr const char* StringifyGLErrorEnum(const GLenum _enum) noexcept
	{
#define CASE_ENUM(e) case e: return #e
		switch (_enum)
		{
			// errors
			CASE_ENUM(GL_NONE);
			CASE_ENUM(GL_INVALID_ENUM);
			CASE_ENUM(GL_INVALID_VALUE);
			CASE_ENUM(GL_INVALID_OPERATION);
			CASE_ENUM(GL_INVALID_FRAMEBUFFER_OPERATION);
			CASE_ENUM(GL_OUT_OF_MEMORY);
			default: return "<unknown GLenum>";
		};
#undef CASE_ENUM
	}

	/*
	*	Checks wether an extension available for example: GL_NVX_gpu_memory_info
	*	returns extension index of glGetStringi() if found, -1 otherwise
	*/
	static GLint IsExtensionSupported(const char* extension_name) noexcept
	{
		GLint ext_count{};
		glGetIntegerv(GL_NUM_EXTENSIONS, &ext_count);
		if (ext_count < 1)
			return -1;

		for (GLint i = 0; i < ext_count; ++i)
		{
			const char* extension = reinterpret_cast<const char*>(glGetStringi(GL_EXTENSIONS, i));
			if (std::strcmp(extension, extension_name) == 0)
			{
				// Found
				return i;
			}
		}
		// Not found
		return -1;
	}

};

/// Multiplatform Debug Break
#ifdef _MSC_VER
	#define DEBUG_BREAK() DebugBreak()
#else  
	#define DEBUG_BREAK() std::raise(SIGTRAP)
#endif 

/// glAssert to handle opengl calls errors (for opengl versions less than 4.3 with no error callback func to handle errors)
#ifdef _DEBUG
	#define glAssert(call) \
			do \
			{ \
				(call); \
				const GLenum err = glGetError(); \
				if (err != GL_NO_ERROR) \
				{  \
					std::cerr << "============= [OpenGL Error] =============\n" \
						<< "Error: " << OpenGLUtils::StringifyGLErrorEnum(err) << '\n' \
						<< "File: " << __FILE__ << '\n' \
						<< "Function: " << __PRETTY_FUNCTION__ << '\n' \
						<< "Line: " << __LINE__ << '\n' \
						<< "=========================================="; \
						DEBUG_BREAK(); \
				} \
			} while (false)
#else // for full performance
	#define glAssert(call) (call)
#endif // DEBUG


///