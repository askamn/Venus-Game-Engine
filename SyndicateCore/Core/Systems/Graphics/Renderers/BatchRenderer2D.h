#ifndef BATCH_RENDERER2D_H_
#define BATCH_RENDERER2D_H_

#include <Common.h>

#include <Utilities/Logger.h>

#include <Systems/Graphics/Renderers/Renderer2D.h>
#include <Systems/Graphics/Color.h>
#include <Systems/Graphics/Sprites/Renderable2D.h>
#include <Systems/Window.h>
#include <map>
#include <string>
#include <freetype-gl.h>

#include <GLM/vec2.hpp>
#include <GLM/vec3.hpp>
#include <GLM/gtc/matrix_transform.hpp>

#define SHADER_VERTEX_POSITION_LOCATION 0
#define SHADER_VERTEX_COLOR_LOCATION    1
#define SHADER_VERTEX_UV_LOCATION		2
#define SHADER_VERTEX_TID_LOCATION		3

#define RENDERER2D_MAX_SPRITES		60000
#define RENDERER2D_VERTEX_SIZE		sizeof(VertexData)
#define RENDERER2D_SPRITE_SIZE		RENDERER2D_VERTEX_SIZE * 4
#define RENDERER2D_BUFFER_SIZE		RENDERER2D_SPRITE_SIZE * RENDERER2D_MAX_SPRITES
#define RENDERER2D_INDICES_SIZE		RENDERER2D_MAX_SPRITES * 6

namespace Syndicate { namespace Graphics {
	
class SYNDICATE_API BatchRenderer2D : public Renderer2D
{
private:
	GLuint  m_VAO;
	GLuint  m_VBO;
	GLsizei m_IndexCount;

	IndexBuffer* m_IBO;
	VertexData* m_Buffer;

	bool m_RendererStarted;

	std::vector<Texture*> m_Textures;
	std::map<unsigned int, std::string> m_TextureAtlasCache;

public:
	BatchRenderer2D();

	float GetTexture(Texture*);

	void Start() override;
	void Submit(const Renderable2D* renderable) override;
	void DrawString(const std::string& text, glm::vec2 position, const Font& font) override;
	void End() override;
	void Render() override;

	~BatchRenderer2D() override;
};

} }

#endif