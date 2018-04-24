#include "DX11ModelDemo.h"


DX11ModelDemo::DX11ModelDemo(void):
	inputLayout_(nullptr),
	colorMapSampler_(nullptr)
{
}


DX11ModelDemo::~DX11ModelDemo(void)
{
}


bool DX11ModelDemo::LoadContent()
{
	//加载定点着色器
	ID3DBlob* vsBuffer = 0;
	bool compileResult = CompileD3DShader( "Shader/DX11ModelDemo.fx", 0, "fx_5_0", &vsBuffer );
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

	m_radius = 200;
	//加载模型数据
	model_ = new ObjModel(new ModelObj("..\\model\\XJC\\model.obj"),D3D11Device_);		//树
	//model_ = new ObjModel(new ModelObj("..\\model\\09\\saloon.obj"),D3D11Device_);	//汽车

	
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
void DX11ModelDemo::UnloadContent()
{
	if (inputLayout_) inputLayout_->Release();

	if (colorMapSampler_) colorMapSampler_->Release();




	inputLayout_ = nullptr;
	colorMapSampler_ = nullptr;


}

void DX11ModelDemo::Render()
{
	if (!D3D11Device_) return;

	float color[] ={0.0,0.0,0.25,1.0};
	D3D11DeviceContext_->ClearRenderTargetView(D3D11RenderTargetView_,color);
	D3D11DeviceContext_->ClearDepthStencilView( depthview_, D3D11_CLEAR_DEPTH, 1.0f, 0 );
	/*
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
	*/
	/*
	//设置为“线框”绘制模式
	D3D11_RASTERIZER_DESC rsDesc;
	ZeroMemory(&rsDesc,sizeof(rsDesc));
	rsDesc.CullMode = D3D11_CULL_BACK;
	rsDesc.DepthClipEnable = true;
	rsDesc.FillMode = D3D11_FILL_WIREFRAME;		//WireFrame
	rsDesc.FrontCounterClockwise = false;
	ID3D11RasterizerState *rsState(NULL);
	D3D11Device_->CreateRasterizerState(&rsDesc,&rsState);
	D3D11DeviceContext_->RSSetState(rsState);
	*/
	D3D11DeviceContext_->IASetInputLayout(inputLayout_);

	//D3D11DeviceContext_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


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

	model_->render(D3D11DeviceContext_,effect_);

	DXGISwapChain_->Present(0,0);
}
void DX11ModelDemo::Update(float dtime)
{
	DX113DBase::Update(dtime);


}
