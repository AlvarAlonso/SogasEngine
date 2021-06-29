#pragma once

#include "core/assertions.h"
#include <string>
#include <vector>

namespace Sogas 
{
	enum class ShaderDataType
	{
		None = 0, Int, Int2, Int3, Int4, Float, Float2, Float3, Float4, Mat3, Mat4, Bool
	};

	static u32 ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Int:		return 4;
		case ShaderDataType::Int2:		return 4 * 2;
		case ShaderDataType::Int3:		return 4 * 3;
		case ShaderDataType::Int4:		return 4 * 4;
		case ShaderDataType::Float:		return 4;
		case ShaderDataType::Float2:	return 4 * 2;
		case ShaderDataType::Float3:	return 4 * 3;
		case ShaderDataType::Float4:	return 4 * 4;
		case ShaderDataType::Mat3:		return 4 * 3 * 3;
		case ShaderDataType::Mat4:		return 4 * 4 * 4;
		case ShaderDataType::Bool:		return 1;
		}

		SGSASSERT_MSG(false, "Unknown shader data type!");
		return 0;
	}

	struct VertexBufferAttribute
	{
		std::string name;
		ShaderDataType type{ ShaderDataType::None };
		u32 size{ 0 };
		u32 offset{ 0 };
		bool normalized{ false };

		VertexBufferAttribute() {}

		VertexBufferAttribute(ShaderDataType _type, const std::string& _name, bool _normalized = false)
			: name(_name), type(_type), size(ShaderDataTypeSize(_type)), offset(0), normalized(_normalized)
		{
		}

		u32 getComponentCount() const
		{
			switch (type)
			{
			case ShaderDataType::Int:	 return 1;
			case ShaderDataType::Int2:	 return 2;
			case ShaderDataType::Int3:	 return 3;
			case ShaderDataType::Int4:	 return 4;
			case ShaderDataType::Float:  return 1;
			case ShaderDataType::Float2: return 2;
			case ShaderDataType::Float3: return 3;
			case ShaderDataType::Float4: return 4;
			case ShaderDataType::Mat3:	 return 3 * 3;
			case ShaderDataType::Mat4:	 return 4 * 4;
			case ShaderDataType::Bool:   return 1;
			}

			SGSASSERT_MSG(false, "Unknown shader data type!");
			return 0;
		}
	};

	class VertexBufferLayout
	{
	private:
		std::vector<VertexBufferAttribute> m_vertexAttributes;
		u32 m_stride = 0;
	public:
		VertexBufferLayout() {}

		VertexBufferLayout(const std::initializer_list<VertexBufferAttribute>& vertexAttributes)
			: m_vertexAttributes(vertexAttributes)
		{
			calculateOffsetsAndStride();
		}

		inline u32 getStride() const { return m_stride; }
		inline const std::vector<VertexBufferAttribute>& getVertexAttributes() const { return m_vertexAttributes; }

		std::vector<VertexBufferAttribute>::iterator begin() { return m_vertexAttributes.begin(); }
		std::vector<VertexBufferAttribute>::iterator end() { return m_vertexAttributes.end(); }
		std::vector<VertexBufferAttribute>::const_iterator begin() const { return m_vertexAttributes.begin(); }
		std::vector<VertexBufferAttribute>::const_iterator end() const { return m_vertexAttributes.end(); }
	private:
		void calculateOffsetsAndStride()
		{
			u32 offset = 0;
			m_stride = 0;
			for (auto& vertexAttribute : m_vertexAttributes)
			{
				vertexAttribute.offset = offset;
				offset += vertexAttribute.size;
				m_stride += vertexAttribute.size;
			}
		}
	};

	class SGS VertexBuffer
	{
	public:
		virtual ~VertexBuffer() = default;

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		virtual const VertexBufferLayout& getLayout() const = 0;
		virtual void setLayout(const VertexBufferLayout& layout) = 0;

		virtual u32 getSize() const = 0;

		static VertexBuffer* create(f32* vertices, u32 size);
	};

	class SGS IndexBuffer
	{
	public:
		virtual ~IndexBuffer() = default;

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		virtual u32 getCount() const = 0;

		static IndexBuffer* create(u32* indices, u32 count);
	};
}