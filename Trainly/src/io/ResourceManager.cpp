#include "ResourceManager.h"

ResourceManager::ResourceManager()
{

}

void ResourceManager::Init(std::string contentPath, Graphics& grp)
{
	m_contentPath = contentPath;
	m_graphicsRef = &grp;

	m_init = true;
}

STRB::Ref<Texture> ResourceManager::LoadTexture(std::string path)
{

	std::replace(path.begin(), path.end(), '/', '\\');

	if (m_textures.find(path) == m_textures.end())
	{
		m_textures[path] = STRB::CreateRef<Texture>();
		m_textures[path]->Create(m_graphicsRef->GetDevice(), (m_contentPath / (path + ".stx")).string());
	}

	return m_textures[path];
}

STRB::Ref < Model> ResourceManager::LoadModel(std::string path)
{
	std::replace(path.begin(), path.end(), '/', '\\');


	if (m_models.find(path) == m_models.end())
	{
		m_models[path] = STRB::CreateRef<Model>();
		m_models[path]->Create(m_graphicsRef->GetDevice(), (m_contentPath / (path + ".smo")).string());
	}

	return m_models[path];
}

STRB::Ref < Font> ResourceManager::LoadFont(std::string path)
{
	std::replace(path.begin(), path.end(), '/', '\\');


	if (m_fonts.find(path) == m_fonts.end())
	{
		m_fonts[path] = STRB::CreateRef<Font>();
		m_fonts[path]->Create(m_graphicsRef->GetDevice(), (m_contentPath / (path + ".sft")).string());
	}

	return m_fonts[path];
}

//Material> ResourceManager::LoadMaterial(std::string path)
//{
//	return Material();
//}
//
STRB::Ref < PixelShader> ResourceManager::LoadPixelShader(std::string path)
{
	std::replace(path.begin(), path.end(), '/', '\\');


	if (m_pixelShaders.find(path) == m_pixelShaders.end())
	{
		m_pixelShaders[path] = STRB::CreateRef < PixelShader>();
		m_pixelShaders[path]->Create(m_graphicsRef->GetDevice(), StringConverter::StringToWide( (m_contentPath / (path + ".scsh")).string() ));
	}

	return m_pixelShaders[path];
}

STRB::Ref<VertexShader> ResourceManager::LoadVertexShader(std::string path)
{
	std::replace(path.begin(), path.end(), '/', '\\');


	if (m_vertexShaders.find(path) == m_vertexShaders.end())
	{
		m_vertexShaders[path] = STRB::CreateRef <VertexShader>();
		m_vertexShaders[path]->Create(m_graphicsRef->GetDevice(), StringConverter::StringToWide((m_contentPath / (path + ".scsh")).string()));
	}

	return m_vertexShaders[path];
}
