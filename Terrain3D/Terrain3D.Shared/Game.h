#pragma once

using namespace Microsoft::WRL;
using namespace Windows::UI::Core;
using namespace Platform;
using namespace DirectX;


struct VERTEX
{
	float X, Y, Z;       
	float NX, NY, NZ;    
	float U, V;          
	float tU, tV;
};

struct CBUFFER
{
	XMMATRIX Final;
	XMVECTOR DiffuseVector;
	XMVECTOR DiffuseColor;
	XMVECTOR AmbientColor;
	float time;
	float waves;
};

class CGame
{
public:
	ComPtr<ID3D11Device1> dev;                                
	ComPtr<ID3D11DeviceContext1> devcon;                      
	ComPtr<IDXGISwapChain1> swapchain;                        
	ComPtr<ID3D11RenderTargetView> rendertarget;             
	ComPtr<ID3D11Buffer> vertexbuffer;                       
	ComPtr<ID3D11VertexShader> vertexshader;                  
	ComPtr<ID3D11PixelShader> pixelshader;                    
	ComPtr<ID3D11InputLayout> inputlayout;                    
	ComPtr<ID3D11Buffer> constantbuffer;                      
	ComPtr<ID3D11DepthStencilView> zbuffer;                  
	ComPtr<ID3D11Buffer> indexbuffer;                         
	ComPtr<ID3D11RasterizerState> defaultrasterizerstate;     
	ComPtr<ID3D11RasterizerState> wireframerasterizerstate;   
	ComPtr<ID3D11ShaderResourceView> texture;				  
	ComPtr<ID3D11ShaderResourceView> texture2;
	ComPtr<ID3D11ShaderResourceView> texture3;
	ComPtr<ID3D11ShaderResourceView> texture4;
	ComPtr<ID3D11ShaderResourceView> texture5;
	ComPtr<ID3D11SamplerState> samplerstate[2];

	void Initialize();
	void Update();
	void Render();
	void CreateTerrain(int);
	void InitPipeline();
	void InitStates();
	void Import();

	float tempHM[512 * 512];
	float goLeft, goRight;
	float forward;
	bool wireframe, walkMode;
	int indicesSize;
	bool goDown, goUp, goFwd, goBack;
	String^ dbgRAW = "FAIL";
	String^ dbgInt = "";
	float time;
	bool waves;
	float horiz, horizontal, ver, vertical;
	int tileFactor;
};
