#include "pch.hpp"
#include "Shader.hpp"

Shader::Shader(const std::initializer_list<ShaderProgramData>& shaders_data)
{
	assert(shaders_data.size() >= 2 && "At least vertex & fragment shaders should be provided");

	using enum ShaderSourceType;
	using enum ShaderType;

	//std::string vertex_source, fragment_source, geometry_source;
	std::unordered_map<GLenum, std::string> shader_sources{};

	// Read shaders from file if needed, or assign sources
	for (const ShaderProgramData& data : shaders_data)
	{
		switch (data.source_type)
		{
			// its a raw memory text
			case String:
			{
				shader_sources[static_cast<GLenum>(data.type)] = data.filename_or_source;
				break;
			}

			// its a files
			case File:
			{
				std::string source{};
				assert(FileUtils::Read(data.filename_or_source, source) && "Failed to read shader file");
				shader_sources[static_cast<GLenum>(data.type)] = source;
				break;
			}
		}

	}

	// Compile loaded shaders
	if (!this->Compile(shader_sources))
		throw std::runtime_error("Failed to compile shader");
}

Shader::~Shader()
{
	glAssert(glDeleteProgram(m_id));
}

void Shader::Bind() const noexcept
{
	glAssert(glUseProgram(m_id));
}

void Shader::Unbind() const noexcept
{
	glAssert(glUseProgram(0));
}

void Shader::SetInteger(const GLchar* name, GLint v) noexcept
{
	glAssert(glProgramUniform1i(m_id, this->GetUniformLocation(name), v));
}

void Shader::SetFloat(const GLchar* name, GLfloat v) noexcept
{
	glAssert(glProgramUniform1f(m_id, this->GetUniformLocation(name), v));
}

void Shader::SetVec2f(const GLchar* name, const glm::fvec2& v) noexcept
{
	glAssert(glProgramUniform2fv(m_id, this->GetUniformLocation(name), 1, glm::value_ptr(v)));
}

void Shader::SetVec3f(const GLchar* name, const glm::fvec3& v) noexcept
{
	glAssert(glProgramUniform3fv(m_id, this->GetUniformLocation(name), 1, glm::value_ptr(v)));
}

void Shader::SetVec4f(const GLchar* name, const glm::fvec4& v) noexcept
{
	glAssert(glProgramUniform4fv(m_id, this->GetUniformLocation(name), 1, glm::value_ptr(v)));
}

void Shader::SetMat3(const GLchar* name, const glm::mat3& v, GLboolean transpose) noexcept
{
	glAssert(glProgramUniformMatrix3fv(m_id, this->GetUniformLocation(name), 1, transpose, glm::value_ptr(v)));
}

void Shader::SetMat4(const GLchar* name, const glm::mat4& v, GLboolean transpose) noexcept
{
	glAssert(glProgramUniformMatrix4fv(m_id, this->GetUniformLocation(name), 1, transpose, glm::value_ptr(v)));
}

void Shader::SetIntegerArray(const GLchar* name, const GLint* v, GLsizei count) noexcept
{
	glAssert(glProgramUniform1iv(m_id, this->GetUniformLocation(name), count, v));
}

GLint Shader::GetUniformLocation(const std::string& uniform_name) noexcept
{
	if (m_uniform_locations_cache.find(uniform_name) == m_uniform_locations_cache.end())
	{
		GLint location = glGetUniformLocation(m_id, uniform_name.c_str());
		if (location == -1)
		{
			std::cout << "WARNING! Uniform " << uniform_name << " not found in shader #" << m_id << '\n';
		}
		m_uniform_locations_cache[uniform_name] = location;
	}
	return m_uniform_locations_cache[uniform_name];
}

std::uint32_t Shader::GetDataTypeSize(ShaderDataType type) noexcept
{
	switch (type)
	{
	case ShaderDataType::Float:		return 4;
	case ShaderDataType::Vec2f:		return 4 * 2;
	case ShaderDataType::Vec3f:		return 4 * 3; //Todo: maybe use sizeof(glm::vec3)
	case ShaderDataType::Vec4f:		return 4 * 4;
	case ShaderDataType::Mat3:		return 4 * 3 * 3;
	case ShaderDataType::Mat4:		return 4 * 4 * 4;
	case ShaderDataType::Int:		return 4;
	case ShaderDataType::Vec2i:		return 4 * 2;
	case ShaderDataType::Vec3i:		return 4 * 3;
	case ShaderDataType::Vec4i:		return 4 * 4;
	case ShaderDataType::Bool:		return 1;
	default:
		break;
	}

	assert(false && "<unsupported Shader Data Type>");
	return 0;
}

bool Shader::Compile(const std::unordered_map<GLenum, std::string>& shader_sources) noexcept
{
	GLuint program = glCreateProgram();
	if (program == 0)
		return false;

	std::vector<GLenum> gl_shader_IDs;
	gl_shader_IDs.reserve(shader_sources.size());
	for (const auto& [shaderType, source] : shader_sources)
	{
		// Create an empty vertex shader handle
		GLuint shader = glCreateShader(shaderType);
		if (shader == GL_INVALID_ENUM)
		{
			glDeleteProgram(program);
			return false;
		}

		// Send the vertex shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		const GLchar* sourceCStr = source.c_str();
		glAssert(glShaderSource(shader, 1, &sourceCStr, 0));

		// Compile the vertex shader
		glAssert(glCompileShader(shader));

		GLint isCompiled = 0;
		glAssert(glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled));
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glAssert(glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength));

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glAssert(glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]));
			// We don't need the shader anymore.
			glAssert(glDeleteShader(shader));

			// Use the infoLog as you see fit.
			std::cerr << (infoLog.data()) << std::endl;

			// we'll just leave
			return false;
		}
		// Attach our shaders to our program
		glAssert(glAttachShader(program, shader));
		gl_shader_IDs.emplace_back(shader);
	}


	// Link our program
	glAssert(glLinkProgram(program));

	// Note the different functions here: glGetProgram* instead of glGetShader*.
	GLint isLinked = 0;
	glAssert(glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked));
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glAssert(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength));

		// The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glAssert(glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]));

		// We don't need the program anymore.
		glAssert(glDeleteProgram(program));

		// Don't leak shaders either.
		for (GLenum shader : gl_shader_IDs)
			glAssert(glDeleteShader(shader));

		// Use the infoLog as you see fit.
		std::cerr << (infoLog.data()) << std::endl;

		// we'll just leave
		return false;
	}

	// Always detach shaders after a successful link. like an .obj file with cpp, one its linked to the program. u can delete the source code program
	for (GLenum shader : gl_shader_IDs)
	{
		glAssert(glDetachShader(program, shader));
		glAssert(glDeleteShader(shader));
	}

	// At least Vertex and fragment shaders are successfully compiled.
	// Now time to link them together into a program.
	// Get a program object.
	m_id = program;

	return true;
}

GLenum Shader::ShaderDataTypeToOpenGLBaseType(const ShaderDataType type) noexcept
{
	switch (type)
	{
	case ShaderDataType::Float:	 return GL_FLOAT;
	case ShaderDataType::Vec2f:	 return GL_FLOAT;
	case ShaderDataType::Vec3f:	 return GL_FLOAT;
	case ShaderDataType::Vec4f:	 return GL_FLOAT;
	case ShaderDataType::Mat3:		 return GL_FLOAT;
	case ShaderDataType::Mat4:		 return GL_FLOAT;
	case ShaderDataType::Int:	     return GL_INT;
	case ShaderDataType::Vec2i:		return GL_INT;
	case ShaderDataType::Vec3i:		return GL_INT;
	case ShaderDataType::Vec4i:		return GL_INT;
	case ShaderDataType::Bool:		 return GL_BOOL;
	default: 
		assert(false && "<unknown ShaderDataType>");
		break;
	}

	
	return 0;
}

//void Shader::SetTexture(const GLchar *name, const std::shared_ptr<Texture> &texture) noexcept {
//  texture->Bind();
//  glAssert(glProgramUniform1i(m_id, GetUniformLocation(name), texture->GetTextureUnit()));
//}
