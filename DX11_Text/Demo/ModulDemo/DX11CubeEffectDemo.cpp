#include "DX11CubeEffectDemo.h"


DX11CubeEffectDemo::DX11CubeEffectDemo(void):	
	inputLayout_(nullptr),
	vertexBuffer_(nullptr),
	indexBuffer_(nullptr),
	colorMap_(nullptr),
	colorMapSampler_(nullptr)
{
}


DX11CubeEffectDemo::~DX11CubeEffectDemo(void)
{
}

bool DX11CubeEffectDemo::LoadContent()
{
	//加载定点着色器
	ID3DBlob* vsBuffer = 0;
	bool compileResult = CompileD3DShader( "../Resources/Shader/DX11CubeEffectDemo.fx", 0, "fx_5_0", &vsBuffer );
	if( compileResult == false )
	{
		MessageBox( 0, "Error loading vertex shader!", "Compile Error", MB_OK );
		return false;
	}
	HRESULT d3dResult;
	d3dResult = D3DX11CreateEffectFromMemory(vsBuffer->GetBufferPointer(),vsBuffer->GetBufferSize(),0,D3D11Device_,&effect_);
	if( FAILED( d3dResult ) )
	{
			MessageBox( 0, "Error creating the effect shader!", "Compile Error", MB_OK );
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

	ID3DX11EffectTechnique* colorTechnique;
	colorTechnique = effect_->GetTechniqueByName("ColorShift");
	ID3DX11EffectPass* colorPass = colorTechnique->GetPassByIndex(0);

	D3DX11_PASS_SHADER_DESC PASS_DESC;
	D3DX11_EFFECT_SHADER_DESC EFFECT_DESC;
	colorPass->GetVertexShaderDesc(&PASS_DESC);
	PASS_DESC.pShaderVariable->GetShaderDesc(PASS_DESC.ShaderIndex,&EFFECT_DESC);

	d3dResult = D3D11Device_->CreateInputLayout( solidColorLayout,
		totalLayoutElements,EFFECT_DESC.pBytecode,
		EFFECT_DESC.BytecodeLength, &inputLayout_ );
	vsBuffer->Release();
	if( FAILED( d3dResult ) )
	{
		return false;
	}

	struct Vertex
	{
		XMFLOAT3 pos;
		XMFLOAT2 tex;
	};
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
	d3dResult = D3DX11CreateShaderResourceViewFromFile( D3D11Device_, "../Resources/img/tex.dds", 0, 0, &colorMap_, 0 );
	if( FAILED( d3dResult ) )
	{
		DXTRACE_MSG( "Failed to load the texture image!" );
		return false;
	}
	//加载贴图
	d3dResult = D3DX11CreateShaderResourceViewFromFile( D3D11Device_, "../Resources/img/heart.dds", 0, 0, &secondMap_, 0 );
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

	return true;
}
void DX11CubeEffectDemo::UnloadContent()
{
	if (inputLayout_) inputLayout_->Release();
	if (vertexBuffer_) vertexBuffer_->Release();
	if (colorMap_) colorMap_->Release();
	if (colorMapSampler_) colorMapSampler_->Release();

	if( indexBuffer_ ) indexBuffer_->Release( );



	inputLayout_ = nullptr;
	vertexBuffer_ = nullptr;
	colorMap_ = nullptr;
	colorMapSampler_ = nullptr;
	indexBuffer_ = nullptr;

}

void DX11CubeEffectDemo::Render()
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


	ID3DX11EffectShaderResourceVariable* colorMap;
	colorMap = effect_->GetVariableByName( "colorMap_" )->AsShaderResource( );
	colorMap->SetResource( colorMap_ );
	colorMap = effect_->GetVariableByName( "secondMap_" )->AsShaderResource( );
	colorMap->SetResource( secondMap_ );

	ID3DX11EffectSamplerVariable* colorMapSampler;
	colorMapSampler = effect_->GetVariableByName("colorSampler_")->AsSampler( );
	colorMapSampler->SetSampler( 0, colorMapSampler_ );

	ID3DX11EffectMatrixVariable* worldMatrix;
	worldMatrix = effect_->GetVariableByName( "worldMatrix" )->AsMatrix( );
	XMMATRIX worldMat = XMLoadFloat4x4(&worldMat_);
	worldMatrix->SetMatrix(reinterpret_cast<float*>(&worldMat) );

	ID3DX11EffectMatrixVariable* viewMatrix;
	viewMatrix = effect_->GetVariableByName( "viewMatrix" )->AsMatrix( );
	XMMATRIX viewMat = XMLoadFloat4x4(&viewMatrix_);
	viewMatrix->SetMatrix( reinterpret_cast<float*>(&viewMat) );

	ID3DX11EffectMatrixVariable* projMatrix;
	projMatrix = effect_->GetVariableByName( "projMatrix" )->AsMatrix( );
	XMMATRIX projMat = XMLoadFloat4x4(&projMatrix_);
	projMatrix->SetMatrix( reinterpret_cast<float*>(&projMat) );

	ID3DX11EffectTechnique* colorInvTechnique;
	colorInvTechnique = effect_->GetTechniqueByName( "ColorShift" );


	D3DX11_TECHNIQUE_DESC techDesc;
	colorInvTechnique->GetDesc( &techDesc );
	for( unsigned int p = 0; p < techDesc.Passes; p++ )
	{
		ID3DX11EffectPass* pass = colorInvTechnique->GetPassByIndex( p );
		if( pass != 0 )
		{
			pass->Apply( 0, D3D11DeviceContext_ );
			D3D11DeviceContext_->DrawIndexed( 36, 0, 0 );
		}
	}

	DXGISwapChain_->Present(0,0);
}
void DX11CubeEffectDemo::Update(float dtime)
{
	DX113DBase::Update(dtime);
	
	
}
