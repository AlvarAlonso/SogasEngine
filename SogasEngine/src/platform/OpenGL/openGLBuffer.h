#pragma once

#include "../../renderer/buffer.h"

#include "defines.h"

class OpenGLVertexBuffer : public VertexBuffer
{
private:
	u32 m_ID;
	VertexBufferLayout m_layout;
public:
	OpenGLVertexBuffer(f32* data, u32 size);
	virtual ~OpenGLVertexBuffer();

	virtual void bind() const override;
	virtual void unbind() const override;

	virtual const VertexBufferLayout& getLayout() const override { return m_layout; }
	virtual void setLayout(const VertexBufferLayout& layout) override { m_layout = layout; }
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