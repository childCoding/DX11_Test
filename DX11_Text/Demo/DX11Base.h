#pragma once
#include <d3d11.h>
#include <d3dx11.h>
#include <DXErr.h>
#include <d3dcompiler.h>
#include <xnamath.h>
class DX11Base
{
public:
	DX11Base(void);
	virtual ~DX11Base(void);

	virtual bool Initialize(HINSTANCE hInstance,HWND hWnd);
	virtual void ShutDown();

	bool CompileD3DShader( char* filePath, char* entry, char*shaderModel, ID3DBlob** buffer );

	virtual bool LoadContent(){return true;}
	virtual void UnloadContent(){}

	virtual void Render()=0 ;
	virtual void Update(float dtime) =0;

	unsigned int GetClientWidth(){return ClientRect_.right - ClientRect_.left;}
	unsigned int GetClientHeight(){return ClientRect_.bottom - ClientRect_.top;}
	RECT GetClientRect(){return ClientRect_;}

	virtual void OnMouseDown(WPARAM btnState, int x, int y){}
	virtual void OnMouseUp(WPARAM btnState, int x, int y)  { }
	virtual void OnMouseMove(WPARAM btnState, int x, int y){ }
	virtual void OnMouseWheel(WPARAM btnState, int x, int y){}
	virtual void OnSized(WPARAM btnState, int x, int y){ ::GetClientRect(hWnd_,&ClientRect_); }
protected:
	RECT ClientRect_;

	HINSTANCE hInstance_;
	HWND hWnd_;

	D3D_DRIVER_TYPE driverType_;
	D3D_FEATURE_LEVEL featureLevel_;

	ID3D11Device* D3D11Device_;
	ID3D11DeviceContext* D3D11DeviceContext_;
	ID3D11RenderTargetView* D3D11RenderTargetView_;
	IDXGISwapChain* DXGISwapChain_;

	ID3D11BlendState* blendState_;
};
template<typename T>
T Clamp(T vMin, T vMax, T value)
{
	value = max(vMin,value);
	value = min(vMax,value);

	return value;
}

