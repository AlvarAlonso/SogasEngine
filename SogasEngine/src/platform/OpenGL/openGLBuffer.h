#pragma once

#include "defines.h"

#include "renderer/buffer.h"

class OpenGLVertexBuffer : public VertexBuffer
{
private:
	u32 m_ID;
	VertexBufferLayout m_layout;
	u32 m_size;
public:
	OpenGLVertexBuffer(f32* data, u32 size);
	virtual ~OpenGLVertexBuffer();

	virtual void bind() const override;
	virtual void unbind() const override;

	virtual const VertexBufferLayout& getLayout() const override { return m_layout; }
	virtual void setLayout(const VertexBufferLayout& layout) override { m_layout = layout; }

	virtual u32 getSize() const override { return m_size; }
};

class OpenGLIndexBuffer : public IndexBuffer
{
private:
	u32 m_ID;
	u32 m_count;
public:
	OpenGLIndexBuffer(u32* data, u32 count);
	virtual ~OpenGLIndexBuffer();

	virtual void bind() const override;
	virtual void unbind() const override;

	u32 getCount() const { return m_count; };
};