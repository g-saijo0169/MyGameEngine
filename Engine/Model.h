#pragma once
//�K�v�ȃC���N���[�h������
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
	 //�g�����X�t�H�[��
	};
	int Load(std::string filename);
	void SetTransform(int hModel, Transform transform);
	void Draw(int hModel_);
	void Relase();
	//���f���̃|�C���^���Ԃ�����ł����x�N�^

}

