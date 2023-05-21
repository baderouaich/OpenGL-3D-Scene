#pragma once
#include <Utility/OpenGLUtils.hpp>
#include <glad/glad.h>

class Texture
{
public:
	enum class TextureType : GLenum
	{
		Texture2D = GL_TEXTURE_2D,
		Texture3D = GL_TEXTURE_3D
	};

public:
	Texture(const std::string& filename, const TextureType type = TextureType::Texture2D);
	~Texture();

public:
	void Bind(const GLint texture_unit) const noexcept;
	void Unbind() const noexcept;

public: /*Accessors*/
	GLuint GetID() const noexcept { return m_id; }
	std::int32_t GetWidth() const noexcept { return m_width; }
	std::int32_t GetHeight() const noexcept { return m_height; }
	TextureType GetType() const noexcept { return m_type; }

	// Bits per pixel
	std::int32_t GetChannels() const noexcept { return m_channels; }

public: /*Operators*/
	constexpr bool operator==(const Texture& other) const noexcept { return m_id == other.m_id; }

private:
	GLuint m_id{};
	std::int32_t m_width;
	std::int32_t m_height;
	std::int32_t m_channels;
	TextureType m_type{};

};
