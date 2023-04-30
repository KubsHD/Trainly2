#include "Graphics.h"

#include "../stdafx.h"

#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "ConstantBuffer.h"
#include "shaders/PixelShader.h"
#include "shaders/VertexShader.h"
#include "Texture.h"
#include "Font.h"
#include "Camera.h"
#include "Mesh.h"
#include "Model.h"

#pragma comment( lib, "user32" )          // link against the win32 library
#pragma comment( lib, "d3d11.lib" )       // direct3D library
#pragma comment( lib, "dxgi.lib" )        // directx graphics interface
#pragma comment( lib, "d3dcompiler.lib" ) // shader compiler

using namespace DirectX;
using namespace DirectX::SimpleMath;

SDL_Window* winTmp;

void Graphics::Init(HWND app, SDL_Window* win)
{
	m_hwnd = app;
	winTmp = win;

	HRESULT hr;

	DXGI_ADAPTER_DESC adapterDesc = {};

	D3D_FEATURE_LEVEL lvl;

	hr = D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, 0, D3D11_CREATE_DEVICE_DEBUG, NULL, 0, D3D11_SDK_VERSION, &m_device, &lvl, &m_devContext);

	if (FAILED(hr))
	{
		printf("The creation of the Direct3D device and its context failed!\n");
		throw std::runtime_error("Unable to create the Direct3D device and its context!");
	}
	else if (lvl < D3D_FEATURE_LEVEL_11_0)
	{
		printf("Critical error: DirectX 11 is not supported by your GPU!\n");
		throw std::runtime_error("Unable to create the Direct3D device and its context!");
	}
	else
	{
		printf("Created DirectX!\n");
	}

	// swapchain

	DXGI_SWAP_CHAIN_DESC scd = {};
	scd.BufferDesc.Width = 0;												// width of the back buffer
	scd.BufferDesc.Height = 0;													// height
	scd.BufferDesc.RefreshRate.Numerator = 0;									// refresh rate: 0 -> do not care
	scd.BufferDesc.RefreshRate.Denominator = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;								// the color palette to use								
	scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;		// unspecified scan line ordering
	scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;						// unspecified scaling
	scd.SampleDesc.Count = 1;													// disable msaa
	scd.SampleDesc.Quality = 0;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;							// use back buffer as render target
	scd.BufferCount = 2;														// the number of buffers in the swap chain (including the front buffer)
	scd.OutputWindow = app;				// set the main window as output target
	scd.Windowed = true;														// windowed, not fullscreen$
	scd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;								// flip mode and discared buffer after presentation
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;							// allow mode switching

	Microsoft::WRL::ComPtr<IDXGIDevice> dxgiDevice;
	Microsoft::WRL::ComPtr<IDXGIAdapter> dxgiAdapter;
	Microsoft::WRL::ComPtr<IDXGIFactory> dxgiFactory;

	hr = m_device.As(&dxgiDevice);

	if (FAILED(hr))
		throw std::runtime_error("The Direct3D device was unable to retrieve the underlying DXGI device!");


	hr = dxgiDevice->GetAdapter(dxgiAdapter.GetAddressOf());

	if (FAILED(hr))
		throw std::runtime_error("The DXGI Device was unable to get the GPU adapter!");


	hr = dxgiAdapter->GetParent(__uuidof(IDXGIFactory), &dxgiFactory);

	if (FAILED(hr))
		throw std::runtime_error("The DXGI Adapter was unable to get the factory!");

	hr = dxgiFactory->CreateSwapChain(m_device.Get(), &scd, m_swapChain.GetAddressOf());
	if (FAILED(hr))
		throw std::runtime_error("The Swap Chain couldn't be created!");



	ID3D11Texture2D* backBuffer;
	DX::ThrowIfFailed(m_swapChain.Get()->GetBuffer(0, IID_PPV_ARGS(&backBuffer)));
	DX::ThrowIfFailed(m_device.Get()->CreateRenderTargetView(backBuffer, NULL, m_renderViewTarget.ReleaseAndGetAddressOf()));
	
	backBuffer->Release();

	m_devContext.Get()->OMSetRenderTargets(1, m_renderViewTarget.GetAddressOf(), NULL);

	//init sampler state

	D3D11_SAMPLER_DESC sampDesc = {};

	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MaxAnisotropy = 0;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	sampDesc.MipLODBias = 0;
	DX::ThrowIfFailed(m_device->CreateSamplerState(&sampDesc, m_samplerState.GetAddressOf()));

	RECT winRect;
	GetClientRect(m_hwnd, &winRect);

	ID3D11Texture2D* pDepthStencil = NULL;
	D3D11_TEXTURE2D_DESC descDepth = {};

	descDepth.Width = (FLOAT)(winRect.right - winRect.left);
	descDepth.Height = (FLOAT)(winRect.bottom - winRect.top);
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;

	DX::ThrowIfFailed(m_device->CreateTexture2D(&descDepth, NULL, m_depthBuffer.GetAddressOf()));
	DX::ThrowIfFailed(m_device->CreateDepthStencilView(m_depthBuffer.Get(), NULL, m_depthStencil.GetAddressOf()));


	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer backends
	ImGui_ImplSDL2_InitForD3D(win);
	ImGui_ImplDX11_Init(m_device.Get(), m_devContext.Get());

	// init sprite batch

	m_spriteBatch = STRB::CreateRef<DirectX::SpriteBatch>(m_devContext.Get());
	//m_spriteFont = STRB::CreateScope<DirectX::SpriteFont>();

	// init viewport

	GetClientRect(m_hwnd, &winRect);
	D3D11_VIEWPORT viewport = {
	  .TopLeftX = 0.0f,
	  .TopLeftY = 0.0f,
	  .Width = (FLOAT)(winRect.right - winRect.left),
	  .Height = (FLOAT)(winRect.bottom - winRect.top),
	  .MinDepth = 0.0f,
	  .MaxDepth = 1.0f };


	m_devContext->RSSetViewports(1, &viewport);

	m_transformBuffer.Create(m_device.Get());

}

void Graphics::Clear(float r, float g, float b)
{
	m_spriteBatch->Begin(SpriteSortMode_Deferred, nullptr, m_samplerState.Get(), m_depthStencilState.Get());


	float c[] = { r / 255.0f, g / 255.0f,b / 255.0f, 1.0f };
	m_devContext.Get()->ClearRenderTargetView(m_renderViewTarget.Get(), c);
	m_devContext.Get()->ClearDepthStencilView(m_depthStencil.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplSDL2_NewFrame(winTmp);
	ImGui::NewFrame();


}

void Graphics::Present()
{

	m_spriteBatch->End();

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	DX::ThrowIfFailed(m_swapChain->Present(1, 0));
}



void Graphics::SetActiveCamera(STRB::Ref<Camera> cam)
{
	m_activeCamera = cam;
}

//void Graphics::DrawTraingle(float angle, Input& in)
//{	
//
//	//{
//	//	ImGui::Begin("Camera");
//	//	ImGui::DragFloat3("translation", translation, 0.1f, -99999.0f, 99999.0f);
//	//	ImGui::DragFloat3("rotation", rotation, 0.1f, -99999.0f, 99999.0f);
//	//	ImGui::End();
//	//}
//
//	/*cam.SetPosition({ translation[0], translation[1], translation[2] });
//	cam.SetRotation({ rotation[0], rotation[1], rotation[2] });*/
//}

void Graphics::BindPipeline(Pipeline& pip)
{
	m_devContext->VSSetShader(pip.vShader->Get(), 0, 0);
	m_devContext->PSSetShader(pip.pShader->Get(), 0, 0);
	m_devContext->IASetInputLayout(pip.vShader->GetLayout());
}

void Graphics::Resize(int w, int h)
{
	m_devContext->OMSetRenderTargets(0, 0, 0);
	m_renderViewTarget->Release();
	
	DX::ThrowIfFailed(m_swapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0));

	ID3D11Texture2D* backBuffer;
	DX::ThrowIfFailed(m_swapChain.Get()->GetBuffer(0, IID_PPV_ARGS(&backBuffer)));
	DX::ThrowIfFailed(m_device.Get()->CreateRenderTargetView(backBuffer, NULL, m_renderViewTarget.GetAddressOf()));
	
	backBuffer->Release();

	// resize depth stencil

	ID3D11Texture2D* pDepthStencil = NULL;
	D3D11_TEXTURE2D_DESC descDepth = {};

	descDepth.Width = w;
	descDepth.Height = h;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;

	DX::ThrowIfFailed(m_device->CreateTexture2D(&descDepth, NULL, m_depthBuffer.GetAddressOf()));
	DX::ThrowIfFailed(m_device->CreateDepthStencilView(m_depthBuffer.Get(), NULL, m_depthStencil.GetAddressOf()));


	
	m_devContext->OMSetRenderTargets(1, m_renderViewTarget.GetAddressOf(), NULL);

	// Set up the viewport.
	RECT winRect;
	GetClientRect(m_hwnd, &winRect);
	D3D11_VIEWPORT viewport = {
	  .TopLeftX = 0.0f,
	  .TopLeftY = 0.0f,
	  .Width = (FLOAT)(winRect.right - winRect.left),
	  .Height = (FLOAT)(winRect.bottom - winRect.top),
	  .MinDepth = 0.0f,
	  .MaxDepth = 1.0f };
	m_devContext->RSSetViewports(1, &viewport);
	
	m_activeCamera->Resize(w, h);
}

float deg2rad(float angle)
{
	return XMConvertToRadians(angle);
}

IndexBuffer Graphics::CreateIndexBuffer(uint32_t* indices, UINT indiceCount)
{
	auto ib = IndexBuffer();
	ib.Create(m_device.Get(), indices, indiceCount);
	return ib;
}


void Graphics::DrawModel(Model& mod, DirectX::SimpleMath::Vector3 position, DirectX::SimpleMath::Quaternion rotation)
{
	m_devContext->OMSetRenderTargets(1, m_renderViewTarget.GetAddressOf(), m_depthStencil.Get());
	m_devContext->OMSetDepthStencilState(m_depthStencilState.Get(), 0);

	m_devContext->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_devContext->PSSetSamplers(0, 1, m_samplerState.GetAddressOf());


	Matrix model;
	model = model.CreateFromYawPitchRoll(deg2rad(rotation.z), deg2rad(rotation.y), deg2rad(rotation.x)) * model.CreateTranslation(position);

	TransformConstantBuffer transform = {
		.transform = (model * m_activeCamera->GetView() * m_activeCamera->GetProjection()).Transpose()
	};


	m_transformBuffer.UpdateBuffer(m_devContext.Get(), &transform);

	m_devContext->VSSetConstantBuffers(0, 1, m_transformBuffer.GetAddressOf());

	for(int i = 0; i < mod.GetMeshes().size(); i++)
	{
		if (mod.GetTextures().size() > 0)
			// TODO: support multiple materials/textures
			m_devContext->PSSetShaderResources(0, 1, mod.GetTextures()[0].pGetShaderResource());

		m_devContext->IASetIndexBuffer(mod.GetMeshes()[i].GetIndexBuffer()->Get(), DXGI_FORMAT_R32_UINT, 0);
		m_devContext->IASetVertexBuffers(
			0,
			1,
			mod.GetMeshes()[i].GetVertexBuffer()->GetAddressOf(),
			mod.GetMeshes()[i].GetVertexBuffer()->pStride(),
			mod.GetMeshes()[i].GetVertexBuffer()->pOffset());

		m_devContext->DrawIndexed(mod.GetMeshes()[i].GetIndexBuffer()->Count(), 0, 0);
	}
}

void Graphics::DrawString(std::string text, DirectX::SimpleMath::Vector2 position, Font& font)
{
	font.GetFont()->DrawString(m_spriteBatch.get(), text.c_str(), position, DirectX::Colors::White, 0.0f, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f });
}

void Graphics::DrawLine(Vector3 beginPos, Vector3 endPos)
{
	m_devContext->OMSetRenderTargets(1, m_renderViewTarget.GetAddressOf(), m_depthStencil.Get());
	m_devContext->OMSetDepthStencilState(m_depthStencilState.Get(), 0);

	m_devContext->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
