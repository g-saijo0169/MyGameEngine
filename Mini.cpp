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
}

//更新
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