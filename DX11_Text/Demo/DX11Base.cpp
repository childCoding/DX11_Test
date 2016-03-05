#include "stdafx.h"
#include "DX11Base.h"


DX11Base::DX11Base():D3D11Device_(nullptr),
	D3D11DeviceContext_(nullptr),
	D3D11RenderTargetView_(nullptr),
	DXGISwapChain_(nullptr),
	driverType_(D3D_DRIVER_TYPE_NULL),
	featureLevel_(D3D_FEATURE_LEVEL_11_0)
{
}


DX11Base::~DX11Base(void)
{
	ShutDown();
}

bool DX11Base::Initialize(HINSTANCE hInstance,HWND hWnd)
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

	D3D11DeviceContext_->OMSetRenderTargets(1,&D3D11RenderTargetView_,0);

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
bool DX11Base::CompileD3DShader( char* filePath, char* entry, char*shaderModel, ID3DBlob** buffer )
{
	DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	shaderFlags |= D3DCOMPILE_DEBUG;
#endif
	ID3DBlob* errorBuffer = 0;
	HRESULT result;
	result = D3DX11CompileFromFile( filePath, 0, 0, entry, shaderModel,
		shaderFlags, 0, 0, buffer, &errorBuffer, 0 );
	if( FAILED( result ) )
	{
		if( errorBuffer != 0 )
		{
			OutputDebugStringA( ( char* )errorBuffer->GetBufferPointer( ) );
			errorBuffer->Release( );
		}
		return false;
	}
	if( errorBuffer != 0 )
		errorBuffer->Release( );
	return true;
}
void DX11Base::ShutDown()
{
	UnloadContent();

	if(D3D11Device_) D3D11Device_->Release();
	if(D3D11DeviceContext_) D3D11DeviceContext_->Release();
	if(D3D11RenderTargetView_) D3D11RenderTargetView_->Release();
	if(DXGISwapChain_) DXGISwapChain_->Release();

	D3D11Device_ = nullptr;
	D3D11DeviceContext_ = nullptr;
	D3D11RenderTargetView_ = nullptr;
	DXGISwapChain_ = nullptr;
}

