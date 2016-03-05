#include "stdafx.h"
#include "DX11Demo.h"


DX11Demo::DX11Demo(void):
	vertexShader_(nullptr),
	pixelShader_(nullptr),
	inputLayout_(nullptr),
	vertexBuffer_(nullptr)
{
}


DX11Demo::~DX11Demo(void)
{
  UnloadContent();
}

bool DX11Demo::LoadContent()
{
	//加载定点着色器
	ID3DBlob* vsBuffer = 0;
	bool compileResult = CompileD3DShader( "Shader/DX11Demo.fx", "VS_Main", "vs_4_0", &vsBuffer );
	if( compileResult == false )
	{
		MessageBox( 0, "Error loading vertex shader!", "Compile Error", MB_OK );
		return false;
	}
	HRESULT d3dResult;
	d3dResult = D3D11Device_->CreateVertexShader( vsBuffer->GetBufferPointer(),
		vsBuffer->GetBufferSize( ), 0, &vertexShader_ );
	if( FAILED( d3dResult ) )
	{
		if( vsBuffer )
			vsBuffer->Release( );
		return false;
	}

	D3D11_INPUT_ELEMENT_DESC solidColorLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,
		0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	unsigned int totalLayoutElements = ARRAYSIZE( solidColorLayout );
	d3dResult = D3D11Device_->CreateInputLayout( solidColorLayout,
		totalLayoutElements, vsBuffer->GetBufferPointer( ),
		vsBuffer->GetBufferSize( ), &inputLayout_ );
	vsBuffer->Release();
	if( FAILED( d3dResult ) )
	{
		return false;
	}

	//加载像素着色器
	ID3DBlob* psBuffer = 0;
	compileResult = CompileD3DShader( "Shader/DX11Demo.fx", "PS_Main", "ps_4_0", &psBuffer );
	if( compileResult == false )
	{
		MessageBox( 0, "Error loading pixel shader!", "Compile Error", MB_OK );
		return false;
	}
	d3dResult = D3D11Device_->CreatePixelShader( psBuffer->GetBufferPointer( ),
		psBuffer->GetBufferSize( ), 0, &pixelShader_ );
	psBuffer->Release( );
	if( FAILED( d3dResult ) )
	{
		return false;
	}

	//初始化定点序列
	Vertex vertexs[] ={
		XMFLOAT3(0.5f,0.5f,0.5f),
		XMFLOAT3(0.5f,-0.5f,0.5f),
		XMFLOAT3(-0.5f,-0.5f,0.5f),

		XMFLOAT3(-0.5f,-0.5f,0.5f),
		XMFLOAT3(-0.5f,0.5f,0.5f),	
		XMFLOAT3(0.5f,0.5f,0.5f)
	};
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc,sizeof(desc));
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.ByteWidth = sizeof(Vertex) * 6;

	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data,sizeof(data));
	data.pSysMem = vertexs;

	d3dResult = D3D11Device_->CreateBuffer(&desc,0,&vertexBuffer_);
	if (FAILED(d3dResult))
	{
		return false;
	}
	return true;
}
void DX11Demo::UnloadContent()
{
	if (vertexShader_) vertexShader_->Release();
	if (pixelShader_) pixelShader_->Release();
	if (inputLayout_) inputLayout_->Release();
	if (vertexBuffer_) vertexBuffer_->Release();

	vertexShader_ = nullptr;
	pixelShader_ = nullptr;
	inputLayout_ = nullptr;
	vertexBuffer_ = nullptr;
}

void DX11Demo::Render()
{
	if (!D3D11Device_) return;

	float color[] ={0.0,0.0,0.25,1.0};
	D3D11DeviceContext_->ClearRenderTargetView(D3D11RenderTargetView_,color);

	unsigned int stride = sizeof(Vertex);
	unsigned int offset = 0;

	D3D11DeviceContext_->IASetInputLayout(inputLayout_);
	D3D11DeviceContext_->IASetVertexBuffers(0,1,&vertexBuffer_,&stride,&offset);
	D3D11DeviceContext_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	D3D11DeviceContext_->VSSetShader(vertexShader_,0,0);
	D3D11DeviceContext_->PSSetShader(pixelShader_,0,0);

	Vertex vertexs[] ={
		XMFLOAT3(0.5f,0.5f,0.5f),
		XMFLOAT3(0.5f,-0.5f,0.5f),
		XMFLOAT3(-0.5f,-0.5f,0.5f),

		XMFLOAT3(-0.5f,-0.5f,0.5f),
		XMFLOAT3(-0.5f,0.5f,0.5f),	
		XMFLOAT3(0.5f,0.5f,0.5f)
	};

	D3D11_MAPPED_SUBRESOURCE subresource;
	HRESULT reslut = D3D11DeviceContext_->Map(vertexBuffer_,0,D3D11_MAP_WRITE_DISCARD,0,&subresource);
	Vertex *spritePtr = ( Vertex* )subresource.pData;
	for (int i=0;i<ARRAYSIZE(vertexs);i++)
	{
		spritePtr[i] = vertexs[i];
	}
	D3D11DeviceContext_->Unmap( vertexBuffer_, 0 );

	D3D11DeviceContext_->Draw(6,0);

	DXGISwapChain_->Present(0,0);
}
void DX11Demo::Update(float dtime)
{

}