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
	data.SysMemPitch = iWidth * 4 ;

	HRESULT hr = D3D11Device_->CreateTexture2D(&tex_desc, &data, &fonttex_);
	if (FAILED(hr))
		std::cout<<"Failed to create texture from memory buffer."<<std::endl;
}
bool DX11FreeTypeFontDemo::LoadContent()
{




	//draw_text(face,L"AVA");


	return DX11FontDemo::LoadContent();

}
void DX11FreeTypeFontDemo::UnloadContent()
{
	DX11FontDemo::UnloadContent();
}

void DX11FreeTypeFontDemo::Render()
{

}
void DX11FreeTypeFontDemo::Update(float dtime)
{

}