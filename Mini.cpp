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
}

//�X�V
void Mini::Update()
{
	transform_.position_.x = 5;
	transform_.rotate_.y += 1;
	transform_.scale_.x = 0.5f;
	transform_.scale_.y = 0.5f;
	transform_.scale_.z = 0.5f;
	if (Input::IsKeyDown(DIK_K))
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