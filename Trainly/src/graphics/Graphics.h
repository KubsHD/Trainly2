#pragma once

#include "../stdafx.h"
#include "../Input.h"
#include "Model.h"
#include "Camera.h"

#include "shaders/PixelShader.h"
#include "shaders/VertexShader.h"
#include "material/Material.h"




using namespace Microsoft::WRL;

class Graphics
{

public:
	Graphics() {};
	~Graphics() {};

	void Init(HWND app, SDL_Window* win);
	void Clear(float r, float g, float b);
	void Present();

	void CreateTriangle();
	void DrawTraingle(float angle, Input& in);

	// Drawing functions


	void DrawModel(Model& mod, DirectX::SimpleMath::Vector3 position = { 0,0,0 }, DirectX::SimpleMath::Quaternion rotation = { 0,0,0,0 });
	void DrawString(std::string text, DirectX::SimpleMath::Vector2 position);


	// Creating stuff

	Texture			CreateTexture(const char* data, int width, int height);
	Model			CreateModel(const char* data);

	Material		CreateMaterial(const char* data);

	PixelShader		CreatePixelShader(const char* data);
	VertexShader	CreateVertexShader(const char* data);

private:

	STRB::Ref<Camera> m_activeCamera;

	HWND m_hwnd;

	ComPtr<ID3D11Device> m_device;
	ComPtr<ID3D11DeviceContext> m_devContext;
	ComPtr<IDXGISwapChain> m_swapChain;
	ComPtr<ID3D11RenderTargetView> m_renderViewTarget;
	ComPtr<ID3D11SamplerState> m_samplerState;

	ComPtr<ID3D11Texture2D> m_depthBuffer;
	ComPtr<ID3D11DepthStencilView> m_depthStencil;
	ComPtr<ID3D11DepthStencilState> m_depthStencilState;

	STRB::Scope<DirectX::SpriteBatch> m_spriteBatch;
	STRB::Scope<DirectX::SpriteFont> m_spriteFont;
};

