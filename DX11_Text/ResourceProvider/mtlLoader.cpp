#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "mtlLoader.h"
using namespace std;

MtlObj::MtlObj() {
	path="";
}

MtlObj::MtlObj(string p) {
	path=p;
}

MtlObj::~MtlObj() {
	mtls.clear();
	cout<<"delMTL"<<endl;
}



void MtlObj::readfile() {
	ifstream infile(path.c_str()); //打开指定文件
	string sline;//每一行

	string value,name,texture;
	while(getline(infile,sline)) {//从指定文件逐行读取
		if(sline!="") {
			istringstream ins(sline);
			ins>>value;
			if(value=="newmtl") {
				ins>>name;
				mtls[name] = mtlTexture(name);

			} else if(value=="map_Kd") {
				ins>>texture;
				mtls[name].map_kd = texture;
			}
		}
	}
	infile.close();
}

string MtlObj::getTextureByName(string name) {
	auto it = mtls.find(name) ;
	if ( it != mtls.end())
	{
		return it->second.map_kd;
	}
	 
	return "";
}


int MtlObj::size() {
	return mtls.size();
}

