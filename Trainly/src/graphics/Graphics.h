#pragma once

#include "../stdafx.h"
#include "Model.h"
#include "Camera.h"
#include "Font.h"

#include "ConstantBuffer.h"

#include "IndexBuffer.h"
#include "VertexBuffer.h"

#include "shaders/PixelShader.h"
#include "shaders/VertexShader.h"
#include "material/Material.h"


struct Pipeline
{
	STRB::Ref<VertexShader> vShader;
	STRB::Ref<PixelShader> pShader;
};

struct TransformConstantBuffer
{
	DirectX::SimpleMath::Matrix transform;
};

using namespace Microsoft::WRL;

class Graphics
{

public:
	Graphics() {};
	~Graphics() {};

	void Init(HWND app, SDL_Window* win);

	void Clear(float r, float g, float b);
	void Present();

	void SetActiveCamera(STRB::Ref<Camera> cam);
	void BindPipeline(Pipeline& pip);

	void Resize(int w, int h);

	// Creating functions

	template<class T>
	VertexBuffer<T> CreateVertexBuffer(T* data, UINT vertexCount);
	IndexBuffer CreateIndexBuffer(uint32_t* indices, UINT indiceCount);

	// Drawing functions

	void DrawModel(Model& mod, DirectX::SimpleMath::Vector3 position = { 0,0,0 }, DirectX::SimpleMath::Quaternion rotation = DirectX::SimpleMath::Quaternion::Identity);
	void DrawString(std::string text, DirectX::SimpleMath::Vector2 position, Font& font);

	void DrawLine(DirectX::SimpleMath::Vector3 beginPos, DirectX::SimpleMath::Vector3 endPos);

	ID3D11Device* GetDevice() { return m_device.Get(); }
private:

	STRB::Ref<Camera> m_activeCamera;

	HWND m_hwnd = NULL;

	ComPtr<ID3D11Device> m_device;
	ComPtr<ID3D11DeviceContext> m_devContext;
	ComPtr<IDXGISwapChain> m_swapChain;
	ComPtr<ID3D11RenderTargetView> m_renderViewTarget;
	ComPtr<ID3D11SamplerState> m_samplerState;

	ComPtr<ID3D11Texture2D> m_depthBuffer;
	ComPtr<ID3D11DepthStencilView> m_depthStencil;
	ComPtr<ID3D11DepthStencilState> m_depthStencilState;

	// buffers

	ConstantBuffer<TransformConstantBuffer> m_transformBuffer;


	STRB::Ref<DirectX::SpriteBatch> m_spriteBatch;
};

template<class T>
VertexBuffer<T> Graphics::CreateVertexBuffer(T* data, UINT vertexCount)
{
	auto vb = VertexBuffer<T>();
	vb.Create(m_device.Get(), data, vertexCount);
	return vb;
}

