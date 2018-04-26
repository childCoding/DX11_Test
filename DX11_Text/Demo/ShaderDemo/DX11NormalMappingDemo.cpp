#include "DX11NormalMappingDemo.h"

inline int KeyDown(int vKey)
{
	return GetAsyncKeyState(vKey) & 0x8000;
}

DX11NormalMappingDemo::DX11NormalMappingDemo(void):
	inputLayout_(nullptr),
	vertexBuffer_(nullptr),
	indexBuffer_(nullptr),
	m_floorSVR(nullptr),
	m_floorN(nullptr),
	colorMapSampler_(nullptr),
	effect_(nullptr)
{
	//3 directional lights
	//Main Light
	m_dirLights[0].ambient  =	XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	m_dirLights[0].diffuse  =	XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	m_dirLights[0].specular =	XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	m_dirLights[0].dir		 =	XMFLOAT3(0.57735f, -0.57735f, 0.57735f);
	//light2
	m_dirLights[1].ambient  =	XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	m_dirLights[1].diffuse  =	XMFLOAT4(0.20f, 0.20f, 0.20f, 1.0f);
	m_dirLights[1].specular =	XMFLOAT4(0.25f, 0.25f, 0.25f, 1.0f);
	m_dirLights[1].dir		 =	XMFLOAT3(-0.57735f, -0.57735f, 0.57735f);
	//light3
	m_dirLights[2].ambient  =	XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	m_dirLights[2].diffuse  =	XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	m_dirLights[2].specular =	XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	m_dirLights[2].dir	     =	XMFLOAT3(0.0f, -0.707f, -0.707f);

	m_material.ambient = XMFLOAT4(0.5f,0.5f,0.5f,1.f);
	m_material.diffuse = XMFLOAT4(1.f,1.f,1.f,1.f);
	m_material.specular = XMFLOAT4(0.3f,0.3f,0.3f,10.f);
	m_material.reflection = XMFLOAT4(0.8f,0.8f,0.8f,0.8f);
}


DX11NormalMappingDemo::~DX11NormalMappingDemo(void)
{
	UnloadContent();
}


bool DX11NormalMappingDemo::initInputLayout()
{
	D3D11_INPUT_ELEMENT_DESC solidColorLayout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TANGENT",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	unsigned int totalLayoutElements = ARRAYSIZE( solidColorLayout );

	ID3DX11EffectTechnique* colorTechnique;
	colorTechnique = effect_->GetTechniqueByName("Light3");
	ID3DX11EffectPass* colorPass = colorTechnique->GetPassByIndex(0);

	D3DX11_PASS_SHADER_DESC PASS_DESC;
	D3DX11_EFFECT_SHADER_DESC EFFECT_DESC;
	colorPass->GetVertexShaderDesc(&PASS_DESC);
	PASS_DESC.pShaderVariable->GetShaderDesc(PASS_DESC.ShaderIndex,&EFFECT_DESC);

	HRESULT d3dResult = D3D11Device_->CreateInputLayout( solidColorLayout,
		totalLayoutElements,EFFECT_DESC.pBytecode,
		EFFECT_DESC.BytecodeLength, &inputLayout_ );

	if( FAILED( d3dResult ) )
	{
		return false;
	}

	return true;
}
bool DX11NormalMappingDemo::initEffect()
{
	//加载定点着色器
	ID3DBlob* vsBuffer = 0;
	bool compileResult = CompileD3DShader( "../Resources/Shader/DX11NormalMappingDemo.fx", 0, "fx_5_0", &vsBuffer );
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
	vsBuffer->Release( );
	return true;
}
bool DX11NormalMappingDemo::initResourceView()
{
	//加载纹理资源
	HRESULT d3dResult = D3DX11CreateShaderResourceViewFromFile(D3D11Device_,"../Resources/img/stones.dds",0,0,&m_floorSVR,0);
	if (FAILED(d3dResult))
	{
		DXTRACE_MSG("failed to load texture file!");
		return false;
	}
	d3dResult = D3DX11CreateShaderResourceViewFromFile(D3D11Device_,"../Resources/img/stones_nmap.dds",0,0,&m_floorN,0);
	if (FAILED(d3dResult))
	{
		DXTRACE_MSG("failed to load texture file!");
		return false;
	}
	return true;
}
bool DX11NormalMappingDemo::initBuffer()
{
	//初始化定点序列
	GeoGen::CreateGrid(5,5,20,20,m_floor);

	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc,sizeof(desc));
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.ByteWidth = sizeof(GeoGen::Vertex) * m_floor.vertices.size();

	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data,sizeof(data));
	data.pSysMem = &m_floor.vertices[0];

	HRESULT d3dResult = D3D11Device_->CreateBuffer(&desc,&data,&vertexBuffer_);
	if (FAILED(d3dResult))
	{
		return false;
	}

	D3D11_BUFFER_DESC idesc;
	ZeroMemory(&idesc,sizeof(idesc));
	idesc.Usage = D3D11_USAGE_DEFAULT;
	idesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	idesc.ByteWidth = sizeof(UINT) * m_floor.indices.size();

	D3D11_SUBRESOURCE_DATA idata;
	ZeroMemory(&idata,sizeof(idata));
	idata.pSysMem = &m_floor.indices[0];

	d3dResult = D3D11Device_->CreateBuffer(&idesc,&idata,&indexBuffer_);
	if (FAILED(d3dResult))
	{
		return false;
	}

	return true;
}

bool DX11NormalMappingDemo::LoadContent()
{
	if (!initEffect())
	{
		return false;
	}
	if (!initInputLayout())
	{
		return false;
	}
	if (!initBuffer())
	{
		return false;
	}
	if (!initResourceView())
	{
		return false;
	}


	//初始化采样状态
	D3D11_SAMPLER_DESC mapDesc;
	ZeroMemory(&mapDesc,sizeof(mapDesc));
	mapDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	mapDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	mapDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	mapDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	mapDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	mapDesc.MaxLOD = D3D11_FLOAT32_MAX;

	HRESULT d3dResult = D3D11Device_->CreateSamplerState(&mapDesc,&colorMapSampler_);
	if (FAILED(d3dResult))
	{
		DXTRACE_MSG("failed to create the sampler state!");
		return false;
	}

	m_tech =  effect_->GetTechniqueByName( "Light3" );
	return true;
}
void DX11NormalMappingDemo::UnloadContent()
{
	if (effect_)effect_->Release();
	if (inputLayout_) inputLayout_->Release();
	if (vertexBuffer_) vertexBuffer_->Release();
	if (m_floorSVR) m_floorSVR->Release();
	if (m_floorN) m_floorN->Release();
	if (colorMapSampler_) colorMapSampler_->Release();


	inputLayout_ = nullptr;
	vertexBuffer_ = nullptr;
	m_floorSVR = nullptr;
	m_floorN = nullptr;
	colorMapSampler_ = nullptr;
	effect_ = nullptr;
}

void DX11NormalMappingDemo::Render()
{
	if (!D3D11Device_) return;

	float color[] ={0.0,0.0,0.25,1.0};
	D3D11DeviceContext_->ClearRenderTargetView(D3D11RenderTargetView_,color);
	D3D11DeviceContext_->ClearDepthStencilView( depthview_, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0 );

	unsigned int stride = sizeof(GeoGen::Vertex);
	unsigned int offset = 0;

	D3D11DeviceContext_->IASetInputLayout(inputLayout_);
	D3D11DeviceContext_->IASetVertexBuffers(0,1,&vertexBuffer_,&stride,&offset);
	D3D11DeviceContext_->IASetIndexBuffer(indexBuffer_,DXGI_FORMAT_R32_UINT,0);
	D3D11DeviceContext_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	ID3DX11EffectShaderResourceVariable *floor;
	floor = effect_->GetVariableByName("g_texture")->AsShaderResource();
	floor->SetResource(m_floorSVR);
	floor = effect_->GetVariableByName("g_normalMap")->AsShaderResource();
	floor->SetResource(m_floorN);

	//采集状态
	ID3DX11EffectSamplerVariable* colorMapSampler;
	colorMapSampler = effect_->GetVariableByName("samplerTex")->AsSampler( );
	colorMapSampler->SetSampler( 0, colorMapSampler_ );

	//世界矩阵
	ID3DX11EffectMatrixVariable* worldMatrix;
	worldMatrix = effect_->GetVariableByName( "g_world" )->AsMatrix( );
	XMMATRIX worldMat = XMLoadFloat4x4(&worldMat_);
	worldMatrix->SetMatrix(reinterpret_cast<float*>(&worldMat) );

	//视图矩阵
	ID3DX11EffectMatrixVariable* viewMatrix;
	viewMatrix = effect_->GetVariableByName( "g_worldViewProj" )->AsMatrix( );
	XMMATRIX viewMat = XMLoadFloat4x4(&viewMatrix_) * XMLoadFloat4x4(&projMatrix_);
	viewMatrix->SetMatrix( reinterpret_cast<float*>(&viewMat) );

	//世界矩阵的转置
	ID3DX11EffectMatrixVariable* projMatrix;
	projMatrix = effect_->GetVariableByName( "g_worldInvTranspose" )->AsMatrix( );
	XMMATRIX projMat = XMLoadFloat4x4(&worldMat_);
	projMat = XMMatrixTranspose(projMat);
	projMatrix->SetMatrix( reinterpret_cast<float*>(&projMat) );

	//纹理变换
	ID3DX11EffectMatrixVariable *textran;
	textran = effect_->GetVariableByName("g_texTrans")->AsMatrix();
	textran->SetMatrix(reinterpret_cast<const float*>(&XMMatrixScaling(2.f,2.f,1.f)));

	//材质
	ID3DX11EffectVariable *material;
	material = effect_->GetVariableByName("g_material");
	material->SetRawValue(&m_material,0,sizeof(m_material));

	//偏移
	ID3DX11EffectVariable *texOffsetScale;
	texOffsetScale = effect_->GetVariableByName("g_texOffsetScale");
	texOffsetScale->SetRawValue(&XMFLOAT2(0.4f,0.4f),0,sizeof(XMFLOAT2));

	//scalar
	ID3DX11EffectScalarVariable *heightScalar;
	heightScalar = effect_->GetVariableByName("g_heightScalar")->AsScalar();
	heightScalar->SetFloat(0.08f);

	D3DX11_TECHNIQUE_DESC techDesc;
	m_tech->GetDesc( &techDesc );
	for( unsigned int p = 0; p < techDesc.Passes; p++ )
	{
		ID3DX11EffectPass* pass = m_tech->GetPassByIndex( p );
		if( pass != 0 )
		{
			pass->Apply( 0, D3D11DeviceContext_ );
			D3D11DeviceContext_->DrawIndexed( m_floor.indices.size(), 0, 0 );
		}
	}
	DXGISwapChain_->Present(0,0);
}
void DX11NormalMappingDemo::Update(float dtime)
{
	DX113DBase::Update(dtime);

	//灯光
	int count =sizeof(m_dirLights);
	ID3DX11EffectVariable *fxDirLights;
	fxDirLights = effect_->GetVariableByName("g_dirLights");
	fxDirLights->SetRawValue(m_dirLights,0,count);

	//眼睛（摄像机）的位置
	XMFLOAT3 eye = XMFLOAT3(m_radius*sin(m_phy)*cos(m_theta),m_radius*cos(m_phy),m_radius*sin(m_phy)*sin(m_theta));
	ID3DX11EffectVariable *fxEyePos;
	fxEyePos = effect_->GetVariableByName("g_eyePos");
	fxEyePos->SetRawValue(&eye,0,sizeof(eye));

	
	if(KeyDown('1'))
		m_tech =  effect_->GetTechniqueByName( "Light3" );
	else if(KeyDown('2'))
		m_tech =  effect_->GetTechniqueByName( "Light3NormalMapping" );
	else if(KeyDown('3'))
		m_tech =  effect_->GetTechniqueByName( "Light3NormalParallaxMapping" );
	else if(KeyDown('4'))
		m_tech =  effect_->GetTechniqueByName( "Light3Texture" );
	else if(KeyDown('5'))
		m_tech =  effect_->GetTechniqueByName( "Light3TexNormalMapping" );
	else if(KeyDown('6'))
		m_tech =  effect_->GetTechniqueByName( "Light3TexNormalParallaxMapping" );
	
}