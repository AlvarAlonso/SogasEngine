#pragma once

#include <vector>
#include "defines.h"
#include "../core/assertions.h"
#include "../external/GLEW/glew-2.1.0/include/GL/glew.h"

struct VertexBufferElement
{
	u32 type;
	u32 count;
	u8 normalized;

	static u32 getTypeSize(u32 type)
	{
		switch (type)
		{
		case GL_FLOAT: return 4;
		case GL_UNSIGNED_INT: return 4;
		case GL_UNSIGNED_BYTE: return 1;
		}

		SGSASSERT(false, "Wrong type size!");
		return 0;
	}
};

class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement> m_elements;
	u32 m_stride;
public:
	VertexBufferLayout()
		: m_stride(0) {}

	template<typename T>
	void Push(int count)
	{
		static_assert(false);
	}

	template<>
	void Push<f32>(i32 count)
	{
		m_elements.push_back({ GL_FLOAT, count, GL_FALSE });
		m_stride += count * VertexBufferElement::getTypeSize(GL_FLOAT);
	}

	template<> 
	void Push<u32>(i32 count)
	{
		m_elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		m_stride += count * VertexBufferElement::getTypeSize(GL_UNSIGNED_INT);
	}

	template<>
	void Push<u8>(i32 count)
	{
		m_elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		m_stride += count * VertexBufferElement::getTypeSize(GL_UNSIGNED_BYTE);
	}

	inline const std::vector<VertexBufferElement> getElements() const { return m_elements; }
	inline u32 getStride() const { return m_stride; }
};