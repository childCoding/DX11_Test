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

	POINT	m_lastPos;			//��һ�����λ�ã����ڸ������λ��

	float	m_theta, m_phy;		//���ڿ������λ�ã��������һ�������ϣ�
	float	m_radius;			//�����������뾶
};

