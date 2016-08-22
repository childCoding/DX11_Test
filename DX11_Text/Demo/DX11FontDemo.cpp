#include "stdafx.h"
#include "DX11FontDemo.h"


DX11FontDemo::DX11FontDemo(void):
	vertexShader_(nullptr),
	pixelShader_(nullptr),
	inputLayout_(nullptr),
	vertexBuffer_(nullptr),
	colorMap_(nullptr),
	colorMapSampler_(nullptr),
	blendState_(nullptr)
{
}


DX11FontDemo::~DX11FontDemo(void)
{
	UnloadContent();
}

bool DX11FontDemo::LoadContent()
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

	d3dResult = D3DX11CreateShaderResourceViewFromFile(D3D11Device_,"img/num.dds",0,0,&colorMap_,0);
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

	D3D11_BUFFER_DESC descBuffer;
	ZeroMemory(&descBuffer,sizeof(descBuffer));
	descBuffer.Usage = D3D11_USAGE_DYNAMIC;
	descBuffer.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	descBuffer.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	descBuffer.ByteWidth = sizeof(Vertex) * 6 * 10;

	d3dResult = D3D11Device_->CreateBuffer(&descBuffer,0,&vertexBuffer_);
	if (FAILED(d3dResult))
	{
		DXTRACE_MSG( "Failed to create vertex buffer!" );
		return false;
	}

	D3D11_BLEND_DESC blendDesc;
	ZeroMemory( &blendDesc, sizeof( blendDesc ) );
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = 0x0F;

	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	D3D11Device_->CreateBlendState( &blendDesc, &blendState_ );
	D3D11DeviceContext_->OMSetBlendState( blendState_, blendFactor, 0xFFFFFFFF );

	return true;
}
void DX11FontDemo::UnloadContent()
{
	if (vertexShader_) vertexShader_->Release();
	if (pixelShader_) pixelShader_->Release();
	if (inputLayout_) inputLayout_->Release();
	if (vertexBuffer_) vertexBuffer_->Release();
	if (colorMap_) colorMap_->Release();
	if (colorMapSampler_) colorMapSampler_->Release();
	if (blendState_) blendState_->Release();


	blendState_ = nullptr;
	vertexShader_ = nullptr;
	pixelShader_ = nullptr;
	inputLayout_ = nullptr;
	vertexBuffer_ = nullptr;
	colorMap_ = nullptr;
	colorMapSampler_ = nullptr;
}

void DX11FontDemo::Render()
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


	

	DXGISwapChain_->Present(0,0);
}
void DX11FontDemo::Update(float dtime)
{
		DrawString("1234567",-0.2f,0.0f);
}

bool DX11FontDemo::DrawString(char* str,float startX,float startY)
{
	const int spriteSize = sizeof(Vertex) * 6;
	const int maxletter  = 10 ;	

	float charWidth = 84.0f / GetClientWidth();
	float charHeight = 76.0f / GetClientHeight();

	float pixelWidth = 83.6f / 836.0f;

	D3D11_MAPPED_SUBRESOURCE subresource;
	HRESULT reslut = D3D11DeviceContext_->Map(vertexBuffer_,0,D3D11_MAP_WRITE_DISCARD,0,&subresource);

	if (FAILED(reslut))
	{
		return false;
	}

	Vertex *spritePtr = ( Vertex* )subresource.pData;
	const int index0 = static_cast<char>( '0' );
	const int index9 = static_cast<char>( '9' );

	int len = strlen(str);
	for (int i=0;i<len;i++)
	{

		
		float sX = startX + charWidth * i;
		float sY = startY;
		float eX = sX + charWidth ;
		float eY = startY + charHeight;
		
		spritePtr[0].pos = XMFLOAT3(eX,eY,1.0);
		spritePtr[1].pos = XMFLOAT3(eX,sY,1.0);
		spritePtr[2].pos = XMFLOAT3(sX,sY,1.0);
		spritePtr[3].pos = XMFLOAT3(sX,sY,1.0);
		spritePtr[4].pos = XMFLOAT3(sX,eY,1.0);
		spritePtr[5].pos = XMFLOAT3(eX,eY,1.0);
		

		unsigned int index = str[i] - '0';
		
		float pixelS = pixelWidth * index ;
		float pixelE = pixelS + pixelWidth;
		
		spritePtr[0].tex = XMFLOAT2( pixelE, 0.0f );
		spritePtr[1].tex = XMFLOAT2( pixelE, 1.0f );
		spritePtr[2].tex = XMFLOAT2( pixelS, 1.0f );
		spritePtr[3].tex = XMFLOAT2( pixelS, 1.0f );
		spritePtr[4].tex = XMFLOAT2( pixelS, 0.0f );
		spritePtr[5].tex = XMFLOAT2( pixelE, 0.0f );


		spritePtr += 6;
	}

	D3D11DeviceContext_->Unmap( vertexBuffer_, 0 );
	D3D11DeviceContext_->Draw( 6 * len, 0 );

	return true;
} 