#include "Mini.h"
#include "Engine/Fbx.h"
#include "Engine/Input.h"

//�R���X�g���N�^
Mini::Mini(GameObject* parent)
	:GameObject(parent, "Mini"), pFbx(nullptr)
{
}

//������
void Mini::Initialize()
{
	pFbx = new Fbx;
	pFbx->Load("Assets\\oden.fbx");

	transform_.position_.x = 2.0f;
	transform_.position_.y = 1.0f;

	transform_.scale_.x = 0.2f;
	transform_.scale_.y = 0.2f;
	transform_.scale_.z = 0.2f;
}

//�X�V
void Mini::Update()
{
	//transform_.rotate_.y += 1;

	if (Input::IsKeyDown(DIK_C))
	{
		KillMe();
	}
}

//�`��
void Mini::Draw()
{
	pFbx->Draw(transform_);
}

//�J��
void Mini::Release()
{
	pFbx->Release();
	delete pFbx;
}