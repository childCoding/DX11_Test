#include "stdafx.h"
#include "DX11SpriteDemo.h"

Sprite::Sprite():
	m_pos(0.0,0.0),
	m_scale(1.0,1.0),
	m_rotation(0.0)
{
}

Sprite::~Sprite()
{
}
XMMATRIX Sprite::GetWorldMatrix()
{
	XMMATRIX translatiton = XMMatrixTranslation(m_pos.x,m_pos.y,0.0);
	XMMATRIX rotationZ =  XMMatrixRotationZ(m_rotation);
	XMMATRIX scale = XMMatrixScaling(m_scale.x,m_scale.y,1.0);

	return translatiton * rotationZ * scale;
}
void Sprite::SetPosition(XMFLOAT2& pos)
{
	m_pos = pos;
}
void Sprite::SetRotation(float rot)
{
	m_rotation = rot;
}
void Sprite::SetScale(XMFLOAT2& scale)
{
	m_scale = scale;
}



DX11SpriteDemo::DX11SpriteDemo(void):
	vertexShader_(nullptr),
	pixelShader_(nullptr),
	inputLayout_(nullptr),
	vertexBuffer_(nullptr),
	colorMap_(nullptr),
	colorMapSampler_(nullptr),
	blendState_(nullptr),
	mvpBuffer_(nullptr)	
{
}


DX11SpriteDemo::~DX11SpriteDemo(void)
{
	UnloadContent();
}


bool DX11SpriteDemo::LoadContent()
{
	//加载定点着色器
	ID3DBlob* vsBuffer = 0;
	bool compileResult = CompileD3DShader( "Shader/DX11SpriteDemo.fx", "VS_Main", "vs_4_0", &vsBuffer );
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
	compileResult = CompileD3DShader( "Shader/DX11SpriteDemo.fx", "PS_Main", "ps_4_0", &psBuffer );
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

	//加载纹理
	d3dResult = D3DX11CreateShaderResourceViewFromFile(D3D11Device_,"img/sprite.dds",0,0,&colorMap_,0);
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

	ID3D11Resource* colorTex;
	colorMap_->GetResource(&colorTex);

	D3D11_TEXTURE2D_DESC descTex;
	((ID3D11Texture2D*)colorTex)->GetDesc(&descTex);
	colorTex->Release();

	float halfWidth = descTex.Width / 2.0f;
	float halfHeight = descTex.Height / 2.0f;
	
	//初始化定点序列
	Vertex vertexs[]={
		{ XMFLOAT3( halfWidth, halfHeight, 1.0f ), XMFLOAT2( 1.0f, 0.0f ) },
		{ XMFLOAT3( halfWidth, -halfHeight, 1.0f ), XMFLOAT2( 1.0f, 1.0f ) },
		{ XMFLOAT3( -halfWidth, -halfHeight, 1.0f ), XMFLOAT2( 0.0f, 1.0f ) },
		{ XMFLOAT3( -halfWidth, -halfHeight, 1.0f ), XMFLOAT2( 0.0f, 1.0f ) },
		{ XMFLOAT3( -halfWidth, halfHeight, 1.0f ), XMFLOAT2( 0.0f, 0.0f ) },
		{ XMFLOAT3( halfWidth, halfHeight, 1.0f ), XMFLOAT2( 1.0f, 0.0f ) },
	};
	D3D11_BUFFER_DESC descBuffer;
	ZeroMemory(&descBuffer,sizeof(descBuffer));
	descBuffer.Usage = D3D11_USAGE_DEFAULT;
	descBuffer.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	descBuffer.ByteWidth = sizeof(Vertex) * 6;

	D3D11_SUBRESOURCE_DATA descData;
	ZeroMemory(&descData,sizeof(descData));
	descData.pSysMem =  vertexs;

	d3dResult = D3D11Device_->CreateBuffer(&descBuffer,&descData,&vertexBuffer_);
	if (FAILED(d3dResult))
	{
		DXTRACE_MSG( "Failed to create vertex buffer!" );
		return false;
	}

	D3D11_BUFFER_DESC constDesc;
	ZeroMemory(&constDesc,sizeof(constDesc));
	constDesc.Usage = D3D11_USAGE_DEFAULT;
	constDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constDesc.ByteWidth = sizeof(XMMATRIX);

	d3dResult = D3D11Device_->CreateBuffer(&constDesc,0,&mvpBuffer_);
	if (FAILED(d3dResult))
	{	
		return false;
	}

	XMFLOAT2 sprite1Pos( 100.0f, 300.0f );
	sprites_[0].SetPosition( sprite1Pos );

	XMFLOAT2 sprite2Pos( 400.0f, 100.0f );
	sprites_[1].SetPosition( sprite2Pos );

	XMMATRIX view = XMMatrixIdentity( );
	XMMATRIX projection = XMMatrixOrthographicOffCenterLH( 0.0f, GetClientWidth(),0.0f,GetClientHeight(), 0.1f, 100.0f );

	vpMatrix_ = XMMatrixMultiply( view, projection );

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
void DX11SpriteDemo::UnloadContent()
{
	if (vertexShader_) vertexShader_->Release();
	if (pixelShader_) pixelShader_->Release();
	if (inputLayout_) inputLayout_->Release();
	if (vertexBuffer_) vertexBuffer_->Release();
	if (colorMap_) colorMap_->Release();
	if (colorMapSampler_) colorMapSampler_->Release();
	if (blendState_) blendState_->Release();
	if (mvpBuffer_) mvpBuffer_->Release();


	blendState_ = nullptr;
	mvpBuffer_ = nullptr;
	vertexShader_ = nullptr;
	pixelShader_ = nullptr;
	inputLayout_ = nullptr;
	vertexBuffer_ = nullptr;
	colorMap_ = nullptr;
	colorMapSampler_ = nullptr;
}

void DX11SpriteDemo::Render()
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

	for (int i=0;i<2;i++)
	{
		XMMATRIX world = sprites_[i].GetWorldMatrix();
		XMMATRIX mvp = XMMatrixMultiply(world,vpMatrix_);
		mvp = XMMatrixTranspose(mvp);
		D3D11DeviceContext_->UpdateSubresource(mvpBuffer_,0,0,&mvp,0,0);
		D3D11DeviceContext_->VSSetConstantBuffers(0,1,&mvpBuffer_);
		D3D11DeviceContext_->Draw(6,0);
	}
	

	DXGISwapChain_->Present(0,0);
}
void DX11SpriteDemo::Update(float dtime)
{

}