#include "FontManager.h"

namespace Syndicate { namespace Graphics {

std::map<std::string, texture_font_t*> FontManager::m_FontCache;
std::map<std::string, std::string> FontManager::m_FontDataCache;

void FontManager::loadFont(const std::string& fontName, const std::string& fontPath)
{
	auto it = m_FontDataCache.find(fontName);

	// Already inside cache
	if (it != m_FontDataCache.end())
	{
		SYNDICATE_WARNING("Font " + fontName + " already present in cache.");
		return;
	}

	if (!Utilities::File::Exists(fontPath))
	{
		SYNDICATE_WARNING( "Font " + fontName + " at path " + fontPath + " does not exist." );
		return;
	}

	Utilities::File fontData = Utilities::File(fontPath, true);

	std::string data = fontData.Read().getData();
	m_FontDataCache.emplace(fontName, data);
}

void FontManager::loadFontFromPackage(const std::string& identifier, const std::string& package)
{
	auto it = m_FontDataCache.find(identifier);

	// Already inside cache
	if (it != m_FontDataCache.end())
	{
		SYNDICATE_WARNING("Font " + identifier + " already present in cache.");
		return;
	}

	FontData* font;
	font = (package.length()) ? ResourceManager::i()->LoadFont(identifier, package) : ResourceManager::i()->LoadFont(identifier);

	m_FontDataCache.emplace(identifier, font->data);
}

texture_font_t* FontManager::getFont(std::string fontName, float size)
{
	auto it = m_FontCache.find(fontName);

	if (it != m_FontCache.end())
	{
		if(it->second->size == size)
			return it->second;
	}

	auto fontDataIt = m_FontDataCache.find(fontName);
	if (fontDataIt == m_FontDataCache.end())
	{
		return nullptr;
	}

	texture_atlas_t* atlas = texture_atlas_new(ATLAS_WIDTH, ATLAS_HEIGHT, ATLAS_BIT_DEPTH);
	texture_font_t* font = texture_font_new_from_memory(atlas, size, fontDataIt->second.c_str(), fontDataIt->second.size());
	
	m_FontCache.emplace(fontName, font);

	return font;
}

void FontManager::deleteFont(std::string fontName)
{
	auto it = m_FontCache.find(fontName);

	if (it != m_FontCache.end())
	{
		texture_font_delete(it->second);
		m_FontCache.erase(it->first);
	}
}

void FontManager::Clear()
{
	for (auto it = m_FontCache.begin(); it != m_FontCache.end();)
	{
		texture_atlas_delete(it->second->atlas);
		texture_font_delete(it->second);
		m_FontCache.erase(it->first);
		it = m_FontCache.begin();
	}
}

} }