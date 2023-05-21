#include "pch.hpp"
#include "Texture.hpp"



Texture::Texture(const std::string& filename, const TextureType type)
	:
	m_type(type)
{
	// Enable flipping image on load. because stb_image reads image 
	// from top to bottom, OpenGL read images from bottom to top.
	stbi_set_flip_vertically_on_load(true);
	std::uint8_t* pixels = stbi_load(filename.c_str(), &m_width, &m_height, &m_channels, 0);
	if (!pixels)
	{
		std::cerr << "Failed to create texture from " << filename << ", Reason: " << stbi_failure_reason() << std::endl;
		return;
	}



	// Generate texture
	glAssert(glGenTextures(1, &m_id));

	// bind texture
	glAssert(glBindTexture(static_cast<GLenum>(type), m_id));

	//Options
	//	Note: glTextureParameteri is standard in OpenGL 4.5, use glTexParameteri
	glAssert(glTexParameteri(static_cast<GLenum>(type), GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
	glAssert(glTexParameteri(static_cast<GLenum>(type), GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	//glTexParameteri(m_id, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(m_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glAssert(glTexParameteri(static_cast<GLenum>(type), GL_TEXTURE_WRAP_S, GL_REPEAT));
	glAssert(glTexParameteri(static_cast<GLenum>(type), GL_TEXTURE_WRAP_T, GL_REPEAT));

	// Upload image bytes to gpu
	// configure format according to image channels (bpp)
	GLint internal_format = GL_RGBA;
	switch (m_channels)
	{
		case 1: internal_format = GL_RED; break;
		case 3: internal_format = GL_RGB; break;
		case 4: internal_format = GL_RGBA; break;
	}

	glAssert(glTexImage2D(static_cast<GLenum>(type), 0, internal_format, m_width, m_height, 0, internal_format, GL_UNSIGNED_BYTE, pixels));
	glAssert(glGenerateMipmap(static_cast<GLenum>(type)));

	// Unbind texture
	this->Unbind();

	// Free image pixels from RAM once uploaded to the GPU
	stbi_image_free(pixels);
}

Texture::~Texture()
{
	glAssert(glDeleteTextures(1, &m_id));
}

void Texture::Bind(const GLint texture_unit) const noexcept
{
	glAssert(glActiveTexture(GL_TEXTURE0 + texture_unit)); // GL_TEXTURE0...GL_TEXTURE31
	glAssert(glBindTexture(static_cast<GLenum>(m_type), m_id));
}

void Texture::Unbind() const noexcept
{
	//glActiveTexture(0); // gl 4.5 TODO: fix error id: 1280 INVALID_ENUM (texture doesnt need to be deactivated)
	glAssert(glBindTexture(static_cast<GLenum>(m_type), 0));
}

