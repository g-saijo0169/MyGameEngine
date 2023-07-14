#pragma once
//�K�v�ȃC���N���[�h������
#include <string>
#include <vector>
#include "Engine/Fbx.h"

namespace Model
{
	struct ModelData
	{
		Fbx* pFbx_;
		Transform transform_;
		std::string filename_;
	 //�g�����X�t�H�[��
	};
	int Load(std::string filename);
	void SetTransform(int hModel, Transform transform);
	void Draw(int hModel_);
	//���f���̃|�C���^���Ԃ�����ł����x�N�^
	std::vector<ModelData*> modelList;

}

