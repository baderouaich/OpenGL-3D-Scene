#include "pch.hpp"
#include "BufferLayout.hpp"

BufferLayout::BufferLayout()
	:
	m_elements(),
	m_stride(0)
{

}
BufferLayout::~BufferLayout()
{
	m_elements.clear();
}

BufferLayout::BufferLayout(const std::initializer_list<BufferElement>& elements)
	:
	m_elements(elements),
	m_stride(0)
{
	CalculateOffsetsAndStride();
}


void BufferLayout::CalculateOffsetsAndStride() noexcept
{
	std::size_t offset = 0;
	m_stride = 0;
	for (auto& element : m_elements)
	{
		element.offset = offset;
		offset += element.size;
		m_stride += element.size;
	}
}