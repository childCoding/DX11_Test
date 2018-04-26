#include "stdafx.h"
#include "DX11Light.h"


DX11Light::DX11Light(void)
{
}


DX11Light::~DX11Light(void)
{
}
bool DX11Light::LoadContent()
{
	//加载着色器
	if(!LoadShader("../Resources/Shader/DX11Light.fx"))
	{
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

	HRESULT d3dResult = D3D11Device_->CreateInputLayout( solidColorLayout,
		totalLayoutElements,EFFECT_DESC.pBytecode,
		EFFECT_DESC.BytecodeLength, &inputLayout_ );
	
	if( FAILED( d3dResult ) )
	{
		return false;
	}

	m_radius = 20;
	//加载模型数据
	model_ = new ObjModel(new ModelObj("..\\Resources\\model\\Female.obj"),D3D11Device_);		//ren 
	//model_ = new ObjModel(new ModelObj("..\\Resources\\model\\XJC\\model.obj"),D3D11Device_);		//树
	//model_ = new ObjModel(new ModelObj("..\\Resources\\model\\09\\saloon.obj"),D3D11Device_);	//汽车

	
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
void DX11Light::UnloadContent()
{
	if (inputLayout_) inputLayout_->Release();

	if (colorMapSampler_) colorMapSampler_->Release();




	inputLayout_ = nullptr;
	colorMapSampler_ = nullptr;


}

void DX11Light::Render()
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

	SetEffectSampler("colorSampler_",0,colorMapSampler_);
	SetEffectMatrix("g_world",worldMat_);
	SetEffectMatrix("g_view",viewMatrix_);
	SetEffectMatrix("g_project",projMatrix_);


	model_->render(D3D11DeviceContext_,effect_);

	DXGISwapChain_->Present(0,0);
}
void DX11Light::Update(float dtime)
{
	DX11Light::Update(dtime);


}
