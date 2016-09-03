#include "pch.h"
#include "Game.h"
#include "HelperFunction.h"

void CGame::InitPipeline()
{
	Array<byte>^ VSFile = LoadShaderFile("VertexShader.cso");
	Array<byte>^ PSFile = LoadShaderFile("PixelShader.cso");

	dev->CreateVertexShader(VSFile->Data, VSFile->Length, nullptr, &vertexshader);
	dev->CreatePixelShader(PSFile->Data, PSFile->Length, nullptr, &pixelshader);

	devcon->VSSetShader(vertexshader.Get(), nullptr, 0);
	devcon->PSSetShader(pixelshader.Get(), nullptr, 0);

	
	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 1, DXGI_FORMAT_R32G32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};


	dev->CreateInputLayout(ied, ARRAYSIZE(ied), VSFile->Data, VSFile->Length, &inputlayout);
	devcon->IASetInputLayout(inputlayout.Get());

	D3D11_BUFFER_DESC bd = { 0 };
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(CBUFFER);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	dev->CreateBuffer(&bd, nullptr, &constantbuffer);
	devcon->VSSetConstantBuffers(0, 1, constantbuffer.GetAddressOf());
}


void CGame::InitStates()
{
	D3D11_RASTERIZER_DESC rd;
	rd.FillMode = D3D11_FILL_SOLID;
	rd.CullMode = D3D11_CULL_BACK;
	rd.FrontCounterClockwise = FALSE;
	rd.DepthClipEnable = TRUE;
	rd.ScissorEnable = FALSE;
	rd.AntialiasedLineEnable = FALSE;
	rd.MultisampleEnable = FALSE;
	rd.DepthBias = 0;
	rd.DepthBiasClamp = 0.0f;
	rd.SlopeScaledDepthBias = 0.0f;

	dev->CreateRasterizerState(&rd, &defaultrasterizerstate);

	// set the changed values for wireframe mode
	rd.FillMode = D3D11_FILL_WIREFRAME;
	rd.AntialiasedLineEnable = TRUE;

	dev->CreateRasterizerState(&rd, &wireframerasterizerstate);

	D3D11_SAMPLER_DESC sd;
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sd.MaxAnisotropy = 8;
	sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	/*sd.BorderColor[0] = 0.0f;
	sd.BorderColor[1] = 0.0f;
	sd.BorderColor[2] = 0.0f;
	sd.BorderColor[3] = 0.0f;*/
	sd.MinLOD = 0.0f;
	sd.MaxLOD = FLT_MAX;
	sd.MipLODBias = 0.0f;

	dev->CreateSamplerState(&sd, &samplerstate[0]);
}
