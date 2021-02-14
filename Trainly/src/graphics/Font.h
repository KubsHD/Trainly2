#pragma once

#include "../stdafx.h"

class Font {
public:
	Font() {};

	void Create(ID3D11Device* device, std::string path);

	DirectX::SpriteFont* GetFont() { return m_font.get(); }

private:
	STRB::Scope<DirectX::SpriteFont> m_font;
};