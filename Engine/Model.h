#pragma once
//必要なインクルードを書く
#include <string>
#include <vector>
#include "Fbx.h"

namespace Model
{
	struct ModelData
	{
		Fbx* pFbx_;
		Transform transform_;
		std::string filename_;
	 //トランスフォーム
	};
	int Load(std::string filename);
	void SetTransform(int hModel, Transform transform);
	void Draw(int hModel_);
	void Relase();
	//モデルのポインタをぶち込んでおくベクタ

}

