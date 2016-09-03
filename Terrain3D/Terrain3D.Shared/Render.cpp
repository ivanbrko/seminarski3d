#include "pch.h"
#include "Game.h"


void CGame::Update()
{
	if (goDown) goRight -= 3;
	else if (goUp) goRight += 3;

	if (goFwd) forward += 3;
	else if(goBack) forward -= 3;
	time += 0.25 + sin(time*50)/5;

}


void CGame::Render()
{
	devcon->OMSetRenderTargets(1, rendertarget.GetAddressOf(), zbuffer.Get());

	float color[4] = { 0.527f, 0.804f, 0.984f, 1.0f };
	devcon->ClearRenderTargetView(rendertarget.Get(), color);
	devcon->ClearDepthStencilView(zbuffer.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);

	
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	devcon->IASetVertexBuffers(0, 1, vertexbuffer.GetAddressOf(), &stride, &offset);
	devcon->IASetIndexBuffer(indexbuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	

	devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	if (horizontal>0.2 || horizontal <-0.2)
		horiz += horizontal;
	if (vertical > 0.2 || vertical < -0.2)
		ver += vertical;
	
	
	XMVECTOR vecCamPosition = XMVectorSet(0.0f + forward, 220, 256.0f - goRight, 0);
	if (XMVectorGetX(vecCamPosition)>0 && XMVectorGetX(vecCamPosition) < 512 && XMVectorGetZ(vecCamPosition) > 0 && XMVectorGetZ(vecCamPosition) < 512 && walkMode)
	{
		float camY = tempHM[512 * (int)XMVectorGetX(vecCamPosition) + (int)XMVectorGetZ(vecCamPosition)] + 13;
		vecCamPosition = XMVectorSetY(vecCamPosition, camY);
	}
	XMVECTOR vecCamLookAt = XMVectorSet(512 + forward, 10 - 7*ver, 256 - goRight - 15 * horiz, 0);
	XMVECTOR vecCamUp = XMVectorSet(0, 1, 0, 0);
	XMMATRIX matView = XMMatrixLookAtLH(vecCamPosition, vecCamLookAt, vecCamUp);

	
	CoreWindow^ Window = CoreWindow::GetForCurrentThread();    
	XMMATRIX matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(45),                                      
		(FLOAT)Window->Bounds.Width / (FLOAT)Window->Bounds.Height,  
		1,                                                           
		5000);                                                        

	CBUFFER cBuffer;
	cBuffer.Final = matView * matProjection;
	cBuffer.DiffuseVector = XMVectorSet(-0.1f, 1.0f, 0.4f, 0.0f);
	cBuffer.DiffuseColor = XMVectorSet(0.5, 0.5, 0.5, 0.5);
	cBuffer.AmbientColor = XMVectorSet(0.2f, 0.2f, 0.2f, 1.0f);
	cBuffer.time = time;
	if (waves) cBuffer.waves = 1;
	else cBuffer.waves = 0;

	
	devcon->UpdateSubresource(constantbuffer.Get(), 0, 0, &cBuffer, 0, 0);
	
	if (wireframe)
		devcon->RSSetState(wireframerasterizerstate.Get());
	else
		devcon->RSSetState(defaultrasterizerstate.Get());


	devcon->PSSetSamplers(0, 1, samplerstate[0].GetAddressOf());
	devcon->PSSetShaderResources(0, 1, texture.GetAddressOf());
	devcon->PSSetShaderResources(1, 1, texture2.GetAddressOf());
	devcon->PSSetShaderResources(2, 1, texture3.GetAddressOf());
	devcon->PSSetShaderResources(3, 1, texture4.GetAddressOf());
	devcon->PSSetShaderResources(4, 1, texture5.GetAddressOf());
	devcon->DrawIndexed(indicesSize, 0, 0);
	
	
	swapchain->Present(1, 0);
}


