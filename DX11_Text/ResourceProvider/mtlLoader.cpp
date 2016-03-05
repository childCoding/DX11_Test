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
	ifstream infile(path.c_str()); //��ָ���ļ�
	string sline;//ÿһ��

	string value,name,texture;
	while(getline(infile,sline)) {//��ָ���ļ����ж�ȡ
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

