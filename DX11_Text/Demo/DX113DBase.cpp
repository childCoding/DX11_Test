#include "stdafx.h"
#include "DX113DBase.h"


DX113DBase::DX113DBase(void):
	D3D11Texture2d_(nullptr),
	depthview_(nullptr),
	m_theta(XM_PI*1.5f),
	m_phy(XM_PI*0.4f),
	m_radius(200.f)
{
}


DX113DBase::~DX113DBase(void)
{
	ShutDown();
}
bool DX113DBase::Initialize(HINSTANCE hInstance,HWND hWnd)
{
	hInstance_ = hInstance;
	hWnd_ = hWnd;

	
	::GetClientRect(hWnd_,&ClientRect_);

	unsigned int width = GetClientWidth();
	unsigned int height = GetClientHeight();

	D3D_DRIVER_TYPE driver_type[] = {
		D3D_DRIVER_TYPE_HARDWARE,D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_SOFTWARE,D3D_DRIVER_TYPE_REFERENCE
	};
	unsigned int totalDriverType = ARRAYSIZE(driver_type);

	D3D_FEATURE_LEVEL feature_level[] = {
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0
	};
	unsigned int totalFeatureLevel = ARRAYSIZE(feature_level);

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc,sizeof(swapChainDesc));
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferDesc.Height = height;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hWnd_;
	swapChainDesc.Windowed = true ;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	
	unsigned int createFlag = 0;
#ifdef _DEBUG
	createFlag |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	HRESULT result;
	for (int i=0; i<totalDriverType;i++)
	{
		result = D3D11CreateDeviceAndSwapChain(0,driver_type[i],0,createFlag,feature_level,
					totalFeatureLevel,D3D11_SDK_VERSION,&swapChainDesc,&DXGISwapChain_,
					&D3D11Device_,&featureLevel_,&D3D11DeviceContext_);
		if (SUCCEEDED(result))
		{
			driverType_ = driver_type[i];
			break;
		}
	}

	if (FAILED(result))
	{
		DXTRACE_MSG("failed to create the device");
		return false;
	}
	
	ID3D11Texture2D* bufferTexture2d;
	result = DXGISwapChain_->GetBuffer(0,__uuidof(ID3D11Texture2D),(LPVOID*)&bufferTexture2d);

	if(FAILED(result))
	{
		DXTRACE_MSG("failed to get the swap chain back buffer!");
		return FALSE;
	}
	result = D3D11Device_->CreateRenderTargetView(bufferTexture2d,0,&D3D11RenderTargetView_);

	if (bufferTexture2d)
	{
		bufferTexture2d->Release();
	}

	if (FAILED(result))
	{
		DXTRACE_MSG("failed to create render target view!");
		return FALSE;
	}
	
	
	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc,sizeof(textureDesc));
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	result = D3D11Device_->CreateTexture2D(&textureDesc,0,&D3D11Texture2d_);
	if (FAILED(result))
	{
		DXTRACE_MSG("failed to create texture2d!");
		return FALSE;
	}

	D3D11_DEPTH_STENCIL_VIEW_DESC depthDesc;
	ZeroMemory(&depthDesc,sizeof(depthDesc));
	depthDesc.Format = textureDesc.Format;
	depthDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthDesc.Texture2D.MipSlice = 0;

	result = D3D11Device_->CreateDepthStencilView(D3D11Texture2d_,&depthDesc,&depthview_);

	if (FAILED(result))
	{
		DXTRACE_MSG("failed to create depth view!");
		return FALSE;
	}

	D3D11DeviceContext_->OMSetRenderTargets(1,&D3D11RenderTargetView_,depthview_);

	D3D11_VIEWPORT viewPort;
	viewPort.Width = static_cast<float>(width);
	viewPort.Height =  static_cast<float>(height);
	viewPort.MinDepth = 0.0;
	viewPort.MaxDepth = 1.0;
	viewPort.TopLeftX = 0;
	viewPort.TopLeftY = 0;

	D3D11DeviceContext_->RSSetViewports(1,&viewPort);
		
	return LoadContent();
}
void DX113DBase::ShutDown()
{
	//UnloadContent();
	//DX11Base::ShutDown();

	if(D3D11Texture2d_) D3D11Texture2d_->Release();
	if(depthview_) depthview_->Release();

	depthview_ = nullptr;
	D3D11Texture2d_ = nullptr;
}
void DX113DBase::Update(float dtime)
{

	XMMATRIX worldMat = XMMatrixIdentity();//rotationMat * translationMat;
	//worldMat = XMMatrixTranspose( worldMat );
	XMStoreFloat4x4(&worldMat_,worldMat);

	//更新视角、投影矩阵
	//相机位置根据球面坐标(m_radius、m_theta、m_phy)来确定
	XMVECTOR pos = XMVectorSet(m_radius*sin(m_phy)*cos(m_theta),m_radius*cos(m_phy),m_radius*sin(m_phy)*sin(m_theta),1.f);
	XMVECTOR target = XMVectorSet(0.f,0.f,0.f,1.f);
	XMVECTOR up = XMVectorSet(0.f,1.f,0.f,0.f);   //y方向朝上
	XMMATRIX viewMatrix = XMMatrixLookAtLH(pos,target,up);
	//viewMatrix = XMMatrixTranspose(viewMatrix);
	XMStoreFloat4x4(&viewMatrix_,viewMatrix);

	XMMATRIX projMatrix = XMMatrixPerspectiveFovLH(XM_PIDIV4,1.f*GetClientWidth()/GetClientHeight(),1.f,10000.f);
	//projMatrix = XMMatrixTranspose(projMatrix);
	XMStoreFloat4x4(&projMatrix_,projMatrix);

}


void DX113DBase::OnMouseDown(WPARAM btnState, int x, int y)
{
	m_lastPos.x = x;
	m_lastPos.y = y;

	SetCapture(hWnd_);
}
void DX113DBase::OnMouseUp(WPARAM btnState, int x, int y)  
{
	ReleaseCapture();
}
void DX113DBase::OnMouseMove(WPARAM btnState, int x, int y)
{
	if((btnState & MK_LBUTTON) != 0)
	{
		float dx = XMConvertToRadians(0.25f*(x - m_lastPos.x));
		float dy = XMConvertToRadians(0.25f*(y - m_lastPos.y));

		m_theta -= dx;
		m_phy -= dy;

		m_phy = Clamp(0.01f,XM_PI-0.01f,m_phy);		
	}
	else if((btnState & MK_RBUTTON) != 0)
	{
		float dRadius = 0.1f * static_cast<float>(x - m_lastPos.x);
		m_radius -= dRadius;

		m_radius = Clamp(3.f,8000.f,m_radius);
	}

	m_lastPos.x = x;
	m_lastPos.y = y;
}
void DX113DBase::OnMouseWheel(WPARAM btnState, int x, int y)
{

}

bool DX113DBase::LoadShader(std::string file)
{
	//加载定点着色器
	ID3DBlob* vsBuffer = 0;
	bool compileResult = CompileD3DShader((char*)file.c_str(), 0, "fx_5_0", &vsBuffer );
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
}
void DX113DBase::SetEffectMatrix(const char * name, XMMATRIX& val)
{
	ID3DX11EffectMatrixVariable* vMatrix = effect_->GetVariableByName( name )->AsMatrix( );
	vMatrix->SetMatrix( reinterpret_cast<float*>(&val) );

}
void DX113DBase::SetEffectMatrix(const char * name, XMFLOAT4X4& val)
{
	XMMATRIX mat = XMLoadFloat4x4(&val);
	SetEffectMatrix(name,mat);
}
void DX113DBase::SetEffectSampler(const char * name,int index, ID3D11SamplerState* val)
{
	ID3DX11EffectSamplerVariable* colorMapSampler = effect_->GetVariableByName(name)->AsSampler( );
	colorMapSampler->SetSampler(index, val );
}