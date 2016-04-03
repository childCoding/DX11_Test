#include "DX11TexDemo.h"


DX11TexDemo::DX11TexDemo(void):
	vertexShader_(nullptr),
	pixelShader_(nullptr),
	inputLayout_(nullptr),
	vertexBuffer_(nullptr),
	colorMap_(nullptr),
	colorMapSampler_(nullptr)
{
}


DX11TexDemo::~DX11TexDemo(void)
{
	UnloadContent();
}


bool DX11TexDemo::LoadContent()
{
	//加载定点着色器
	ID3DBlob* vsBuffer = 0;
	bool compileResult = CompileD3DShader( "Shader/DX11TexDemo.fx", "VS_Main", "vs_4_0", &vsBuffer );
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
		0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,
			0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
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
	compileResult = CompileD3DShader( "Shader/DX11TexDemo.fx", "PS_Main", "ps_4_0", &psBuffer );
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
		{ XMFLOAT3( 1.0f, 1.0f, 1.0f ), XMFLOAT2( 1.0f, 0.0f ) },
		{ XMFLOAT3( 1.0f, -1.0f, 1.0f ), XMFLOAT2( 1.0f, 1.0f ) },
		{ XMFLOAT3( -1.0f, -1.0f, 1.0f ), XMFLOAT2( 0.0f, 1.0f ) },
		{ XMFLOAT3( -1.0f, -1.0f, 1.0f ), XMFLOAT2( 0.0f, 1.0f ) },
		{ XMFLOAT3( -1.0f, 1.0f, 1.0f ), XMFLOAT2( 0.0f, 0.0f ) },
		{ XMFLOAT3( 1.0f, 1.0f, 1.0f ), XMFLOAT2( 1.0f, 0.0f ) },
	};
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc,sizeof(desc));
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.ByteWidth = sizeof(Vertex) * 6;

	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data,sizeof(data));
	data.pSysMem = vertexs;

	d3dResult = D3D11Device_->CreateBuffer(&desc,&data,&vertexBuffer_);
	if (FAILED(d3dResult))
	{
		return false;
	}

	d3dResult = D3DX11CreateShaderResourceViewFromFile(D3D11Device_,"img/1002.map.dds",0,0,&colorMap_,0);
	if (FAILED(d3dResult))
	{
		DXTRACE_MSG("failed to load texture file!");
		return false;
	}

	D3D11_SAMPLER_DESC mapDesc;
	ZeroMemory(&mapDesc,sizeof(mapDesc));
	mapDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	mapDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	mapDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	mapDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	mapDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	mapDesc.MaxLOD = D3D11_FLOAT32_MAX;

	d3dResult = D3D11Device_->CreateSamplerState(&mapDesc,&colorMapSampler_);
	if (FAILED(d3dResult))
	{
		DXTRACE_MSG("failed to create the sampler state!");
		return false;
	}


	return true;
}
void DX11TexDemo::UnloadContent()
{
	if (vertexShader_) vertexShader_->Release();
	if (pixelShader_) pixelShader_->Release();
	if (inputLayout_) inputLayout_->Release();
	if (vertexBuffer_) vertexBuffer_->Release();
	if (colorMap_) colorMap_->Release();
	if (colorMapSampler_) colorMapSampler_->Release();

	vertexShader_ = nullptr;
	pixelShader_ = nullptr;
	inputLayout_ = nullptr;
	vertexBuffer_ = nullptr;
	colorMap_ = nullptr;
	colorMapSampler_ = nullptr;
}

void DX11TexDemo::Render()
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
	D3D11DeviceContext_->PSSetShaderResources(0,1,&colorMap_);
	D3D11DeviceContext_->PSSetSamplers(0,1,&colorMapSampler_);
	D3D11DeviceContext_->Draw(6,0);

	DXGISwapChain_->Present(0,0);
}
void DX11TexDemo::Update(float dtime)
{

}