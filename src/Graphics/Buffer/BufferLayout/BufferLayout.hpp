#pragma once
#include "BufferElement.hpp"
#include <initializer_list>

class BufferLayout
{
public:
	BufferLayout();
	explicit BufferLayout(const std::initializer_list<BufferElement>& elements);
	~BufferLayout();

public: /*Accesors*/
	const std::vector<BufferElement>& GetElements() const noexcept { return m_elements; }
	std::uint32_t GetStride() const noexcept { return m_stride; }

public: /* foreach iterators access */
	std::vector<BufferElement>::iterator begin() noexcept { return m_elements.begin(); }
	std::vector<BufferElement>::iterator end() noexcept { return m_elements.end(); }
	std::vector<BufferElement>::const_iterator begin() const noexcept { return m_elements.cbegin(); }
	std::vector<BufferElement>::const_iterator end() const noexcept { return m_elements.cend(); }
	std::vector<BufferElement>::reverse_iterator rbegin() noexcept { return m_elements.rbegin(); }
	std::vector<BufferElement>::reverse_iterator rend() noexcept { return m_elements.rend(); }
	std::vector<BufferElement>::const_reverse_iterator rbegin() const noexcept { return m_elements.crbegin(); }
	std::vector<BufferElement>::const_reverse_iterator rend() const noexcept { return m_elements.crend(); }

public: /*Modifiers*/


private:
	void CalculateOffsetsAndStride() noexcept;

private:
	std::vector<BufferElement> m_elements;
	std::uint32_t m_stride{};
};


