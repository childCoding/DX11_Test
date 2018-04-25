#pragma once
#include "../dx11base.h"


class DX11MapDemo :
	public DX11Base
{
	struct Vertex
	{
		XMFLOAT3 pos;
		XMFLOAT2 tex;
	};
	struct Rect
	{
		float x,y;
		float width,height;
		Rect():x(0),y(0),width(0),height(0){}
		XMFLOAT2 getTopLeft(){ return XMFLOAT2(x,y);} 
		XMFLOAT2 getTopRight(){ return XMFLOAT2(x+width,y);} 
		XMFLOAT2 getBottomLeft(){ return XMFLOAT2(x,y+height);} 
		XMFLOAT2 getBottomRight(){ return XMFLOAT2(x+width,y+height);} 
	};
public:
	DX11MapDemo(void);
	~DX11MapDemo(void);


	virtual bool LoadContent();
	virtual void UnloadContent();

	virtual void Render() ;
	virtual void Update(float dtime) ;

	virtual void OnMouseDown(WPARAM btnState, int x, int y);
	virtual void OnMouseUp(WPARAM btnState, int x, int y);
	virtual void OnMouseMove(WPARAM btnState, int x, int y);
	virtual void OnMouseWheel(WPARAM btnState, int x, int y){}


	void clipMap();
private: 
	ID3D11VertexShader* vertexShader_;
	ID3D11PixelShader* pixelShader_;
	ID3D11InputLayout* inputLayout_;
	ID3D11Buffer* vertexBuffer_;

	ID3D11ShaderResourceView* colorMap_;
	ID3D11SamplerState* colorMapSampler_;


	bool	m_bMouseDown;
	Rect	m_clipRect;			//裁剪区域
	POINT	m_lastPos;			//上一次鼠标位置，用于跟踪鼠标位移
	XMFLOAT2 m_vector;			//与上一点的偏移量
};

