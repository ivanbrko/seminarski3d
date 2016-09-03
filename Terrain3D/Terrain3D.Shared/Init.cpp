#include "pch.h"
#include "Game.h"
#include "WICTextureLoader.h"



void CGame::Initialize()
{
	// Define temporary pointers to a device and a device context
	ComPtr<ID3D11Device> dev11;
	ComPtr<ID3D11DeviceContext> devcon11;

	// Create the device and device context objects
	D3D11CreateDevice(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&dev11,
		nullptr,
		&devcon11);

	// Convert the pointers from the DirectX 11 versions to the DirectX 11.1 versions
	dev11.As(&dev);
	devcon11.As(&devcon);


	// obtain the DXGI factory
	ComPtr<IDXGIDevice1> dxgiDevice;
	dev.As(&dxgiDevice);
	ComPtr<IDXGIAdapter> dxgiAdapter;
	dxgiDevice->GetAdapter(&dxgiAdapter);
	ComPtr<IDXGIFactory2> dxgiFactory;
	dxgiAdapter->GetParent(__uuidof(IDXGIFactory2), &dxgiFactory);

	// set up the swap chain description
	DXGI_SWAP_CHAIN_DESC1 scd = { 0 };
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;    // how the swap chain should be used
	scd.BufferCount = 2;                                  // a front buffer and a back buffer
	scd.Format = DXGI_FORMAT_B8G8R8A8_UNORM;              // the most common swap chain format
	scd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;    // the recommended flip mode
	scd.SampleDesc.Count = 1;                             // disable anti-aliasing

	CoreWindow^ Window = CoreWindow::GetForCurrentThread();    // get the window pointer

	// create the swap chain
	dxgiFactory->CreateSwapChainForCoreWindow(
		dev.Get(),                                  // address of the device
		reinterpret_cast<IUnknown*>(Window),        // address of the window
		&scd,                                       // address of the swap chain description
		nullptr,                                    // advanced
		&swapchain);                                // address of the new swap chain pointer


	// get a pointer directly to the back buffer
	ComPtr<ID3D11Texture2D> backbuffer;
	swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), &backbuffer);

	// create a render target pointing to the back buffer
	dev->CreateRenderTargetView(backbuffer.Get(), nullptr, &rendertarget);


	// create a zbuffer
	D3D11_TEXTURE2D_DESC texd = { 0 };

	texd.Width = Window->Bounds.Width;
	texd.Height = Window->Bounds.Height;
	texd.ArraySize = 1;
	texd.MipLevels = 1;
	texd.SampleDesc.Count = 1;
	texd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	texd.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	ComPtr<ID3D11Texture2D> zbuffertexture;
	dev->CreateTexture2D(&texd, nullptr, &zbuffertexture);

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
	ZeroMemory(&dsvd, sizeof(dsvd));

	dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

	dev->CreateDepthStencilView(zbuffertexture.Get(), &dsvd, &zbuffer);


	// set the viewport
	D3D11_VIEWPORT viewport = { 0 };

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = Window->Bounds.Width;
	viewport.Height = Window->Bounds.Height;
	viewport.MinDepth = 0;    // the closest an object can be on the depth buffer is 0.0
	viewport.MaxDepth = 1;    // the farthest an object can be on the depth buffer is 1.0

	devcon->RSSetViewports(1, &viewport);

	HRESULT hr = CreateWICTextureFromFile(
		dev.Get(),
		nullptr,
		//L"coastal2.png",
		//L"mountainBM.png",
		L"mountainBM2.png",
		nullptr,
		&texture,
		0);
	hr = CreateWICTextureFromFile(
		dev.Get(),
		nullptr,
		L"grass1.png",
		nullptr,
		&texture2,
		0);
	hr = CreateWICTextureFromFile(
		dev.Get(),
		nullptr,
		L"Cliffs.png",
		nullptr,
		&texture3,
		0);
	hr = CreateWICTextureFromFile(
		dev.Get(),
		nullptr,
		L"Sand.png",
		nullptr,
		&texture4,
		0);
	hr = CreateWICTextureFromFile(
		dev.Get(),
		nullptr,
		L"testMask.png",
		nullptr,
		&texture5,
		0);

	goLeft = 0;
	goRight = 0;
	forward = 0;
	wireframe = false;
	goDown = false;
	goUp = false;
	goFwd = false;
	goBack = false;
	walkMode = false;
	waves = false;
	time = 0;
	horizontal = 0;
	vertical = 0;
	ver = 0;
	horiz = 0;
	tileFactor = 20;

	
	InitPipeline();
	InitStates();
	Import();
	CreateTerrain(512);

}
