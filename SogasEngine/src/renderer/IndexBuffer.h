#pragma once

#include "defines.h"

class IndexBuffer
{
private:
	u32 m_ID;
	u32 m_count;
public:
	IndexBuffer(const void* data, u32 count);
	~IndexBuffer();

	void bind() const;
	void unbind() const;

	u32 getCount() const;
};

