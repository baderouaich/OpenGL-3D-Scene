#pragma once
#include <Utility/OpenGLUtils.hpp>
#include <array>
#include <span> // to handle all stl dynamic containers

class IndexBuffer
{
public:
	template<const std::size_t Size>
	IndexBuffer(const std::array<std::uint32_t, Size>& indices)
		:
		IndexBuffer(indices.data(), static_cast<std::uint32_t>(indices.size()))
	{
	}
	IndexBuffer(const std::span<std::uint32_t>& indices)
		:
		IndexBuffer(indices.data(), static_cast<std::uint32_t>(indices.size()))
	{
	}

	IndexBuffer(const std::uint32_t* indices, const std::uint32_t count);
	~IndexBuffer();

public:
	void Bind() const noexcept;
	void Unbind() const noexcept;
	
public: 
	std::uint32_t GetCount() const noexcept { return m_count; }

private:
	GLuint m_id{};
	std::uint32_t m_count{};
};

