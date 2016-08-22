#include "stdafx.h"
#include "DX11FreeTypeFontDemo.h"

DX11FreeTypeFontDemo::DX11FreeTypeFontDemo(std::string pathname)
{
	//初始化库
	FT_Error error = FT_Init_FreeType ( &library );
	if (error)
	{
		std::cout<<"init library error!"<<std::endl;
	}

	//初始化字体 "E:/BaiduYunDownload/CEGUI/cegui-0.8.4/datafiles/fonts/msyh.ttf"
	error = FT_New_Face( library,pathname.c_str(),0,&face );
	if ( error == FT_Err_Unknown_File_Format )
	{
		std::cout<<"do not susppend this format file." <<std::endl;
		//ASSERT(0);
	}
	else if ( error )
	{
		std::cout<<"do not open this file!"<<std::endl;

	}

	//设置字体大小
	error = FT_Set_Char_Size(face,0,12*64,300,300);
	if ( error )
	{
		std::cout<<"set char size failed!..."<<std::endl;
		
	}
}


DX11FreeTypeFontDemo::~DX11FreeTypeFontDemo(void)
{
}
void DX11FreeTypeFontDemo::DrawGlyphToBitmap( UINT* buffer,int buffer_width,FT_GlyphSlot glyph)
{
	if(!glyph)
		return ;

	UINT nPlanes = 1;
	UINT nBitcount = 1;

	FT_Bitmap* bitmap =& glyph->bitmap;
	//start_x += glyph->bitmap_left;
	//start_y -= glyph->bitmap_top;


	switch (bitmap->pixel_mode)
	{
	case  FT_PIXEL_MODE_GRAY:
		{
			for (int row = 0 ; row < bitmap->rows;++row)
			{
				byte* dst = (byte*)buffer;
				byte* src = bitmap->buffer + ( row * bitmap->pitch );
				for (int col = 0 ;col < bitmap->width ; ++col)
				{
					*dst++ = 0xFF;
					*dst++ = 0xFF;
					*dst++ = 0xFF;
					*dst++ = *src++;
				}
				buffer += buffer_width;
			}
		}
		break;
	case  FT_PIXEL_MODE_MONO:
		nBitcount = 1;
		break;
		//case FT_PIXEL_MODE_BGRA:

		//	break;
	default:
		std::cout<<"error pixel format!" <<std::endl;
		break;
	}
	
}
void DX11FreeTypeFontDemo::creatTextureFromFont(wchar_t* str)
{
	UINT* mem_buffer = new UINT[1024*1024];
	memset(mem_buffer,0,1024*1024);
	
	FT_UInt prev_glyph = 0;
	int height_max = 0;
	int start_x = 0,start_y = 80;
	int len = wcslen(str);
	for (int i = 0 ; i < len;++i)
	{
		FT_UInt glyph_index;
		FT_Vector delta;
		glyph_index =  FT_Get_Char_Index(face,str[i]);
		FT_Error error = FT_Load_Glyph(face,glyph_index,FT_LOAD_DEFAULT);
		if ( error )
		{
			std::cout<<"load glyph failed!..."<<std::endl;
			continue;
		}
		error = FT_Render_Glyph(face->glyph,ft_render_mode_normal);
		if ( error )
		{
			std::cout<<"rendered glyph failed!..."<<std::endl;
			continue;
		}



		if (start_x + (face->glyph->advance.x>>6) > 1024 )
		{
			start_x = 0;
			start_y += height_max;
			prev_glyph = 0;
		}

		FT_Get_Kerning(face,prev_glyph,glyph_index,ft_kerning_default,&delta);

		start_x += delta.x>>6;
		start_y += delta.y>>6;

		int draw_x = start_x + face->glyph->bitmap_left;
		int draw_y = start_y - face->glyph->bitmap_top;

		DrawGlyphToBitmap(mem_buffer + (draw_y*1024) + draw_x,1024,face->glyph);
			//drawglyph_bitmap(face->glyph,start_x,start_y);

		height_max = face->glyph->bitmap.rows > height_max ? face->glyph->bitmap.rows : height_max;
		start_x += face->glyph->advance.x>>6;
		start_y += start_x / 1024 * height_max;
		start_x %= 1024;

		prev_glyph = glyph_index;
	}

	Rasterise(mem_buffer,1024,1024);

	D3D11_SHADER_RESOURCE_VIEW_DESC desc;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
	desc.Texture2D.MostDetailedMip = 0;
	desc.Texture2D.MipLevels = 1;
	HRESULT d3dResult = D3D11Device_->CreateShaderResourceView(fonttex_,0,&colorMap_);
	//HRESULT d3dResult = D3DX11CreateShaderResourceViewFromFile(D3D11Device_,"img/1002.map.dds",0,0,&colorMap_,0);

	if (FAILED(d3dResult))
	{
		DXTRACE_MSG("failed to load texture file!");
		return ;
	}
}
void DX11FreeTypeFontDemo::Rasterise(const void* buffer,UINT iWidth,UINT iHeight)
{

	D3D11_TEXTURE2D_DESC tex_desc;
	ZeroMemory(&tex_desc, sizeof(tex_desc));
	tex_desc.Width = static_cast<UINT>(iWidth);
	tex_desc.Height = static_cast<UINT>(iHeight);
	tex_desc.ArraySize = 1;
	tex_desc.SampleDesc.Count = 1;
	tex_desc.SampleDesc.Quality = 0;
	tex_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;  //toD3DPixelFormat(pixel_format);
	tex_desc.Usage = D3D11_USAGE_DEFAULT;
	tex_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	tex_desc.CPUAccessFlags = 0;
	tex_desc.MiscFlags = 0;
	tex_desc.MipLevels = 1;

	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
	data.pSysMem = buffer;
	data.SysMemPitch = iWidth * sizeof(UINT) ;

	HRESULT hr = D3D11Device_->CreateTexture2D(&tex_desc,&data, &fonttex_);
	if (FAILED(hr))
		std::cout<<"Failed to create texture from memory buffer."<<std::endl;


}
bool DX11FreeTypeFontDemo::LoadContent()
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

	creatTextureFromFont(L"大家好我是一个程序员！AVDJEFlskfj e二儿三卜1");


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
	Vertex vertices[] ={
		{ XMFLOAT3( 1.0f, 1.0f, 1.0f ), XMFLOAT2( 1.0f, 0.0f ) },
		{ XMFLOAT3( 1.0f, -1.0f, 1.0f ), XMFLOAT2( 1.0f, 1.0f ) },
		{ XMFLOAT3( -1.0f, -1.0f, 1.0f ), XMFLOAT2( 0.0f, 1.0f ) },
		{ XMFLOAT3( -1.0f, -1.0f, 1.0f ), XMFLOAT2( 0.0f, 1.0f ) },
		{ XMFLOAT3( -1.0f, 1.0f, 1.0f ), XMFLOAT2( 0.0f, 0.0f ) },
		{ XMFLOAT3( 1.0f, 1.0f, 1.0f ), XMFLOAT2( 1.0f, 0.0f ) },
	};

	D3D11_BUFFER_DESC descBuffer;
	ZeroMemory(&descBuffer,sizeof(descBuffer));
	descBuffer.Usage = D3D11_USAGE_DEFAULT;
	//descBuffer.CPUAccessFlags = D3D11_USAGE_DEFAULT;
	descBuffer.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	descBuffer.ByteWidth = sizeof(Vertex) * 6;

	D3D11_SUBRESOURCE_DATA resourceData;
	ZeroMemory( &resourceData, sizeof( resourceData ) );
	resourceData.pSysMem = vertices;

	d3dResult = D3D11Device_->CreateBuffer(&descBuffer,&resourceData,&vertexBuffer_);
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

	

	//draw_text(face,L"AVA");


	//return DX11FontDemo::LoadContent();
	return true;
}
void DX11FreeTypeFontDemo::UnloadContent()
{
	DX11FontDemo::UnloadContent();
}

void DX11FreeTypeFontDemo::Render()
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
void DX11FreeTypeFontDemo::Update(float dtime)
{

}
bool DX11FreeTypeFontDemo::DrawString(wchar_t* str,float x,float y)
{
	FT_UInt prev_glyph = 0;
	int height_max = 0;
	int start_x = 0,start_y = 80;
	int len = wcslen(str);
	for (int i = 0 ; i < len;++i)
	{
		FT_UInt glyph_index;
		FT_Vector delta;
		glyph_index =  FT_Get_Char_Index(face,str[i]);
		FT_Error error = FT_Load_Glyph(face,glyph_index,FT_LOAD_DEFAULT);
		if ( error )
		{
			std::cout<<"load glyph failed!..."<<std::endl;
			continue;
		}
		error = FT_Render_Glyph(face->glyph,ft_render_mode_normal);
		if ( error )
		{
			std::cout<<"rendered glyph failed!..."<<std::endl;
			continue;
		}



		if (start_x + (face->glyph->advance.x>>6) > 512 )
		{
			start_x = 0;
			start_y += height_max;
			prev_glyph = 0;
		}

		FT_Get_Kerning(face,prev_glyph,glyph_index,ft_kerning_default,&delta);

		start_x += delta.x>>6;
		start_y += delta.y>>6;

		//DrawGlyphToBitmap()
		//drawglyph_bitmap(face->glyph,start_x,start_y);

		height_max = face->glyph->bitmap.rows > height_max ? face->glyph->bitmap.rows : height_max;
		start_x += face->glyph->advance.x>>6;
		start_y += start_x / 512 * height_max;
		start_x %= 512;

		prev_glyph = glyph_index;
	}
	
	return true;
}