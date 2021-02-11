#include "Graphics.h"

#include "../stdafx.h"

#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "ConstantBuffer.h"
#include "shaders/PixelShader.h"
#include "shaders/VertexShader.h"
#include "Texture.h"
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

	DXGI_ADAPTER_DESC adapterDesc;

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

	DXGI_SWAP_CHAIN_DESC scd;
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



	ID3D11Resource* backBuffer;
	m_swapChain.Get()->GetBuffer(0, __uuidof(ID3D11Resource), reinterpret_cast<void**>(&backBuffer));
	m_device.Get()->CreateRenderTargetView(backBuffer, NULL, m_renderViewTarget.GetAddressOf());
	m_devContext.Get()->OMSetRenderTargets(1, m_renderViewTarget.GetAddressOf(), NULL);

	//init sampler state

	D3D11_SAMPLER_DESC sampDesc;

	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
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
	D3D11_TEXTURE2D_DESC descDepth;

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

	m_spriteBatch = STRB::CreateScope<DirectX::SpriteBatch>(m_devContext.Get());
	//m_spriteFont = STRB::CreateScope<DirectX::SpriteFont>();

}

void Graphics::Clear(float r, float g, float b)
{
	float c[] = { r / 255.0f, g / 255.0f,b / 255.0f, 1.0f };
	m_devContext.Get()->ClearRenderTargetView(m_renderViewTarget.Get(), c);
	m_devContext.Get()->ClearDepthStencilView(m_depthStencil.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplSDL2_NewFrame(winTmp);
	ImGui::NewFrame();

	//m_spriteBatch->Begin();

}

void Graphics::Present()
{

	//m_spriteBatch->End();

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	HRESULT hr = m_swapChain->Present(1, 0);
}

Model mod;
Model train;


struct TransformConstantBuffer
{
	Matrix transform;
};

VertexShader vertex_shader_ptr;
PixelShader pixel_shader_ptr;

STRB::Ref<Camera> cam;

void Graphics::CreateTriangle()
{
	

	std::vector<D3D11_INPUT_ELEMENT_DESC> inputElementDesc = {
	  { "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	  { "TexCoord", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	  { "Normal", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	vertex_shader_ptr.Create(m_device.Get(), inputElementDesc, L"data/shader/vertex.scsh");
	pixel_shader_ptr.Create(m_device.Get(), L"data/shader/pixel.scsh");

	mod.Create(m_device.Get(), "data/model/chunk/city/mdl_city.smo");
	train.Create(m_device.Get(), "data/model/train/mdl_train.smo");

	float aspectRatio = 16.0 / 9.0f;
	cam = STRB::CreateRef<Camera>();
	cam->Init(60.0f, aspectRatio, 10.0f, 100000.0f);
	cam->SetPosition({ 0.0f, 30.0f, -30.0f });
	cam->SetLookTargetPosition({ 0.0f, 0.0f, 0.0f });

	m_activeCamera = cam;

}

void Graphics::DrawTraingle(float angle, Input& in)
{	
	

	float cameraSpeed = 10.0f;

	if (in.IsKeyPressed(SDLK_LSHIFT))
	{
		cam->AppendPosition(cam->GetUpVector() * cameraSpeed);
	}

	if (in.IsKeyPressed(SDLK_LCTRL))
	{
		cam->AppendPosition(cam->GetDownVector() * cameraSpeed);
	}

	if (in.IsKeyPressed(SDLK_w))
	{
		cam->AppendPosition(cam->GetForwardVector() * cameraSpeed);
	}

	if (in.IsKeyPressed(SDLK_s))
	{
		cam->AppendPosition(cam->GetBackwardVector() * cameraSpeed);
	}

	if (in.IsKeyPressed(SDLK_a))
	{
		cam->AppendPosition(cam->GetLeftVector() * cameraSpeed);
	}

	if (in.IsKeyPressed(SDLK_d))
	{
		cam->AppendPosition(cam->GetRightVector() * cameraSpeed);
	}

	//cam.SetLookTargetPosition({ 0.0f, 10.0f, 0.0f });




	if (in.IsMouseButtonPressed(SDL_BUTTON_RIGHT))
		cam->AppendRotation({(in.GetDeltaMousePos().y * 0.005f), (in.GetDeltaMousePos().x * 0.005f), 0.0f});



	//{
	//	ImGui::Begin("Camera");
	//	ImGui::DragFloat3("translation", translation, 0.1f, -99999.0f, 99999.0f);
	//	ImGui::DragFloat3("rotation", rotation, 0.1f, -99999.0f, 99999.0f);
	//	ImGui::End();
	//}

	/*cam.SetPosition({ translation[0], translation[1], translation[2] });
	cam.SetRotation({ rotation[0], rotation[1], rotation[2] });*/



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

	m_devContext->OMSetRenderTargets(1, m_renderViewTarget.GetAddressOf(), m_depthStencil.Get());

	m_devContext->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_devContext->PSSetSamplers(0, 1, m_samplerState.GetAddressOf());
	m_devContext->IASetInputLayout(vertex_shader_ptr.GetLayout());

	m_devContext->VSSetShader(vertex_shader_ptr.Get(), 0, 0);
	m_devContext->PSSetShader(pixel_shader_ptr.Get(), 0, 0);

	DrawModel(mod);
	DrawModel(train);
}

void Graphics::DrawModel(Model& mod, DirectX::SimpleMath::Vector3 position, DirectX::SimpleMath::Quaternion rotation)
{
	ConstantBuffer<TransformConstantBuffer> cbd;

	cbd.Create(m_device.Get());

	Matrix world = Matrix::Identity.CreateFromQuaternion(rotation).CreateTranslation(position);
	
	TransformConstantBuffer cb = {
		.transform = (world * m_activeCamera->GetView() * m_activeCamera->GetProjection()).Transpose()
	};


	cbd.UpdateBuffer(m_devContext.Get(), &cb);

	m_devContext->VSSetConstantBuffers(0, 1, cbd.GetAddressOf());

	for(int i = 0; i < mod.GetMeshes().size(); i++)
	{
		if (mod.GetTextures().size() > 0)
			m_devContext->PSSetShaderResources(0, 1, mod.GetTextures()[i].pGetShaderResource());

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

void Graphics::DrawString(std::string text, DirectX::SimpleMath::Vector2 position)
{
}
