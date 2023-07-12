#include "Mini.h"
#include "Engine/Fbx.h"
#include "Engine/Input.h"

//コンストラクタ
Mini::Mini(GameObject* parent)
	:GameObject(parent, "Mini"), pFbx(nullptr)
{
}

//初期化
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

//更新
void Mini::Update()
{
	//transform_.rotate_.y += 1;

	if (Input::IsKeyDown(DIK_C))
	{
		KillMe();
	}
}

//描画
void Mini::Draw()
{
	pFbx->Draw(transform_);
}

//開放
void Mini::Release()
{
	pFbx->Release();
	delete pFbx;
}