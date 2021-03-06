#ifndef VERTEX_ARRAY_H_
#define VERTEX_ARRAY_H

#include <Common.h>

#include <Systems/Graphics/Buffers/Buffer.h>
#include <vector>

namespace Syndicate { namespace Graphics {

class SYNDICATE_API VertexArray
{
	GLuint m_ArrayID;
	std::vector<Buffer*> m_Buffers;

public:
	VertexArray();
	~VertexArray();

	void addBuffer(Buffer* buffer, GLuint index);

	inline void Bind()   const { glBindVertexArray(this->m_ArrayID); };
	inline void Unbind() const { glBindVertexArray(0); };
};

}}
#endif