#pragma once
#include "../graphics/Graphics.h"
#include <src\graphics\Font.h>

class ResourceManager {
public:
	ResourceManager();
	void Init(std::string contentPath, Graphics& grp);

	STRB::Ref <Texture>			LoadTexture(std::string path);
	STRB::Ref <Model>			LoadModel(std::string path);
	STRB::Ref <Font>			LoadFont(std::string path);
	/*Material>		LoadMaterial(std::string path);*/

	STRB::Ref <PixelShader>		LoadPixelShader(std::string path);
	STRB::Ref <VertexShader>	LoadVertexShader(std::string path);

private:

	std::map<std::string, STRB::Ref<Texture> > m_textures;
	std::map<std::string, STRB::Ref<Model> > m_models;
	std::map<std::string, STRB::Ref<Material> > m_materials;
	std::map<std::string, STRB::Ref<PixelShader> > m_pixelShaders;
	std::map<std::string, STRB::Ref<VertexShader> > m_vertexShaders;
	std::map<std::string, STRB::Ref<Font> > m_fonts;

	std::filesystem::path m_contentPath;

	Graphics* m_graphicsRef;

	bool m_init = false;
};