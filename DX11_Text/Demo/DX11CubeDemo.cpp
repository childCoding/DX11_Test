#include "stdafx.h"
#include "DX11CubeDemo.h"


DX11CubeDemo::DX11CubeDemo(void):	
	vertexShader_(nullptr),
	pixelShader_(nullptr),
	inputLayout_(nullptr),
	vertexBuffer_(nullptr),
	indexBuffer_(nullptr),
	colorMap_(nullptr),
	colorMapSampler_(nullptr),
	viewCB_(nullptr),
	projCB_(nullptr),
	worldCB_(nullptr)
{
}


DX11CubeDemo::~DX11CubeDemo(void)
{
}

bool DX11CubeDemo::LoadContent()
{
	//加载定点着色器
	ID3DBlob* vsBuffer = 0;
	bool compileResult = CompileD3DShader( "Shader/DX11CubeDemo.fx", "VS_Main", "vs_4_0", &vsBuffer );
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
	compileResult = CompileD3DShader( "Shader/DX11CubeDemo.fx", "PS_Main", "ps_4_0", &psBuffer );
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
	

	//加载顶点数据
	Vertex vertices[] =
	{
		{ XMFLOAT3( -1.0f, 1.0f, -1.0f ), XMFLOAT2( 0.0f, 0.0f ) },
		{ XMFLOAT3( 1.0f, 1.0f, -1.0f ), XMFLOAT2( 1.0f, 0.0f ) },
		{ XMFLOAT3( 1.0f, 1.0f, 1.0f ), XMFLOAT2( 1.0f, 1.0f ) },
		{ XMFLOAT3( -1.0f, 1.0f, 1.0f ), XMFLOAT2( 0.0f, 1.0f ) },
		{ XMFLOAT3( -1.0f, -1.0f, -1.0f ), XMFLOAT2( 0.0f, 0.0f ) },
		{ XMFLOAT3( 1.0f, -1.0f, -1.0f ), XMFLOAT2( 1.0f, 0.0f ) },
		{ XMFLOAT3( 1.0f, -1.0f, 1.0f ), XMFLOAT2( 1.0f, 1.0f ) },
		{ XMFLOAT3( -1.0f, -1.0f, 1.0f ), XMFLOAT2( 0.0f, 1.0f ) },
		{ XMFLOAT3( -1.0f, -1.0f, 1.0f ), XMFLOAT2( 0.0f, 0.0f ) },
		{ XMFLOAT3( -1.0f, -1.0f, -1.0f ), XMFLOAT2( 1.0f, 0.0f ) },
		{ XMFLOAT3( -1.0f, 1.0f, -1.0f ), XMFLOAT2( 1.0f, 1.0f ) },
		{ XMFLOAT3( -1.0f, 1.0f, 1.0f ), XMFLOAT2( 0.0f, 1.0f ) },
		{ XMFLOAT3( 1.0f, -1.0f, 1.0f ), XMFLOAT2( 0.0f, 0.0f ) },
		{ XMFLOAT3( 1.0f, -1.0f, -1.0f ), XMFLOAT2( 1.0f, 0.0f ) },
		{ XMFLOAT3( 1.0f, 1.0f, -1.0f ), XMFLOAT2( 1.0f, 1.0f ) },
		{ XMFLOAT3( 1.0f, 1.0f, 1.0f ), XMFLOAT2( 0.0f, 1.0f ) },
		{ XMFLOAT3( -1.0f, -1.0f, -1.0f ), XMFLOAT2( 0.0f, 0.0f ) },
		{ XMFLOAT3( 1.0f, -1.0f, -1.0f ), XMFLOAT2( 1.0f, 0.0f ) },
		{ XMFLOAT3( 1.0f, 1.0f, -1.0f ), XMFLOAT2( 1.0f, 1.0f ) },
		{ XMFLOAT3( -1.0f, 1.0f, -1.0f ), XMFLOAT2( 0.0f, 1.0f ) },
		{ XMFLOAT3( -1.0f, -1.0f, 1.0f ), XMFLOAT2( 0.0f, 0.0f ) },
		{ XMFLOAT3( 1.0f, -1.0f, 1.0f ), XMFLOAT2( 1.0f, 0.0f ) },
		{ XMFLOAT3( 1.0f, 1.0f, 1.0f ), XMFLOAT2( 1.0f, 1.0f ) },
		{ XMFLOAT3( -1.0f, 1.0f, 1.0f ), XMFLOAT2( 0.0f, 1.0f ) },
	};

	D3D11_BUFFER_DESC vertexDesc;
	ZeroMemory( &vertexDesc, sizeof( vertexDesc ) );
	vertexDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexDesc.ByteWidth = sizeof( Vertex ) * 24;
	D3D11_SUBRESOURCE_DATA resourceData;
	ZeroMemory( &resourceData, sizeof( resourceData ) );
	resourceData.pSysMem = vertices;
	d3dResult = D3D11Device_->CreateBuffer( &vertexDesc, &resourceData, &vertexBuffer_ );
	if( FAILED( d3dResult ) )
	{
		DXTRACE_MSG( "Failed to create vertex buffer!" );
		return false;
	}

	//加载索引数据
	WORD indices[] =
	{
		3, 1, 0, 2, 1, 3,
		6, 4, 5, 7, 4, 6,
		11, 9, 8, 10, 9, 11,
		14, 12, 13, 15, 12, 14,
		19, 17, 16, 18, 17, 19,
		22, 20, 21, 23, 20, 22
	};
	D3D11_BUFFER_DESC indexDesc;
	ZeroMemory( &indexDesc, sizeof( indexDesc ) );
	indexDesc.Usage = D3D11_USAGE_DEFAULT;
	indexDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexDesc.ByteWidth = sizeof( WORD ) * 36;
	indexDesc.CPUAccessFlags = 0;
	resourceData.pSysMem = indices;
	d3dResult = D3D11Device_->CreateBuffer( &indexDesc, &resourceData, &indexBuffer_ );
	if( FAILED( d3dResult ) )
	{
		DXTRACE_MSG( "Failed to create index buffer!" );
		return false;
	}
	
	//加载贴图
	d3dResult = D3DX11CreateShaderResourceViewFromFile( D3D11Device_, "img/tex.dds", 0, 0, &colorMap_, 0 );
	if( FAILED( d3dResult ) )
	{
		DXTRACE_MSG( "Failed to load the texture image!" );
		return false;
	}
	//加载贴图
	d3dResult = D3DX11CreateShaderResourceViewFromFile( D3D11Device_, "img/heart.dds", 0, 0, &secondMap_, 0 );
	if( FAILED( d3dResult ) )
	{
		DXTRACE_MSG( "Failed to load the texture image!" );
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

	//加载常量
	D3D11_BUFFER_DESC constDesc;
	ZeroMemory( &constDesc, sizeof( constDesc ) );
	constDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constDesc.ByteWidth = sizeof( XMMATRIX );
	constDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dResult = D3D11Device_->CreateBuffer( &constDesc, 0, &viewCB_ );
	if( FAILED( d3dResult ) )
	{
		return false;
	}
	d3dResult = D3D11Device_->CreateBuffer( &constDesc, 0, &projCB_ );
	if( FAILED( d3dResult ) )
	{
		return false;
	}
	d3dResult = D3D11Device_->CreateBuffer( &constDesc, 0, &worldCB_ );
	if( FAILED( d3dResult ) )
	{
		return false;
	}

	return true;
}
void DX11CubeDemo::UnloadContent()
{
	if (vertexShader_) vertexShader_->Release();
	if (pixelShader_) pixelShader_->Release();
	if (inputLayout_) inputLayout_->Release();
	if (vertexBuffer_) vertexBuffer_->Release();
	if (colorMap_) colorMap_->Release();
	if (colorMapSampler_) colorMapSampler_->Release();

	if( indexBuffer_ ) indexBuffer_->Release( );
	if( viewCB_ ) viewCB_->Release( );
	if( projCB_ ) projCB_->Release( );
	if( worldCB_ ) worldCB_->Release( );

	vertexShader_ = nullptr;
	pixelShader_ = nullptr;
	inputLayout_ = nullptr;
	vertexBuffer_ = nullptr;
	colorMap_ = nullptr;
	colorMapSampler_ = nullptr;
	indexBuffer_ = nullptr;
	viewCB_ = nullptr;
	projCB_ = nullptr;
	worldCB_ = nullptr;
}

void DX11CubeDemo::Render()
{
	if (!D3D11Device_) return;

	float color[] ={0.0,0.0,0.25,1.0};
	D3D11DeviceContext_->ClearRenderTargetView(D3D11RenderTargetView_,color);
	D3D11DeviceContext_->ClearDepthStencilView( depthview_, D3D11_CLEAR_DEPTH, 1.0f, 0 );

	unsigned int stride = sizeof(Vertex);
	unsigned int offset = 0;

	D3D11DeviceContext_->IASetInputLayout(inputLayout_);
	D3D11DeviceContext_->IASetVertexBuffers(0,1,&vertexBuffer_,&stride,&offset);
	D3D11DeviceContext_->IASetIndexBuffer(indexBuffer_,DXGI_FORMAT_R16_UINT,0);
	D3D11DeviceContext_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	D3D11DeviceContext_->VSSetShader(vertexShader_,0,0);
	D3D11DeviceContext_->PSSetShader(pixelShader_,0,0);
	D3D11DeviceContext_->PSSetShaderResources(0,1,&colorMap_);  //设置第一个槽的纹理map
	D3D11DeviceContext_->PSSetShaderResources(1,1,&secondMap_); //设置第二个槽的纹理map
	D3D11DeviceContext_->PSSetSamplers(0,1,&colorMapSampler_);

	XMMATRIX worldMat = XMLoadFloat4x4(&worldMat_);
	XMMATRIX viewMat = XMLoadFloat4x4(&viewMatrix_);
	XMMATRIX projMat = XMLoadFloat4x4(&projMatrix_);

	//使用这种方式更新矩阵，需要进行转置。具体为什么？
	worldMat = XMMatrixTranspose(worldMat);
	viewMat = XMMatrixTranspose(viewMat);
	projMat = XMMatrixTranspose(projMat);

	D3D11DeviceContext_->UpdateSubresource( worldCB_, 0, 0, &worldMat, 0, 0 );
	D3D11DeviceContext_->UpdateSubresource( viewCB_, 0, 0, &viewMat, 0, 0 );
	D3D11DeviceContext_->UpdateSubresource( projCB_, 0, 0, &projMat, 0, 0 );
	D3D11DeviceContext_->VSSetConstantBuffers( 0, 1, &worldCB_ );
	D3D11DeviceContext_->VSSetConstantBuffers( 1, 1, &viewCB_ );
	D3D11DeviceContext_->VSSetConstantBuffers( 2, 1, &projCB_ );
	D3D11DeviceContext_->DrawIndexed( 36, 0, 0 );


	DXGISwapChain_->Present(0,0);
}
void DX11CubeDemo::Update(float dtime)
{
	DX113DBase::Update(dtime);

}
