#pragma once
#include "dx11base.h"
#include <d3dx11effect.h>
class DX113DBase :
	public DX11Base
{
public:
	struct Vertex
	{
		XMFLOAT3 pos;
		XMFLOAT2 tex;
	};
	DX113DBase(void);
	~DX113DBase(void);

	virtual bool Initialize(HINSTANCE hInstance,HWND hWnd);
	virtual void ShutDown();

	virtual void Update(float dtime) ;
	
	virtual void OnMouseDown(WPARAM btnState, int x, int y);
	virtual void OnMouseUp(WPARAM btnState, int x, int y);
	virtual void OnMouseMove(WPARAM btnState, int x, int y);
	virtual void OnMouseWheel(WPARAM btnState, int x, int y){}
protected:
	ID3D11Texture2D* D3D11Texture2d_;
	ID3D11DepthStencilView* depthview_;

	XMFLOAT4X4 viewMatrix_;
	XMFLOAT4X4 projMatrix_;
	XMFLOAT4X4 worldMat_;

	POINT	m_lastPos;			//上一次鼠标位置，用于跟踪鼠标位移

	float	m_theta, m_phy;		//用于控制相机位置（相机处于一个球面上）
	float	m_radius;			//相机所处球面半径
};

