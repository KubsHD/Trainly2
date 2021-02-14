#include "Font.h"

void Font::Create(ID3D11Device* device, std::string path)
{
	m_font = STRB::CreateScope<DirectX::SpriteFont>(device, StringConverter::StringToWide(path).c_str());
}
