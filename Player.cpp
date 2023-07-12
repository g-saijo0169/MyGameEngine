#include "Player.h"
#include "Engine/Fbx.h"
#include "Engine/Input.h"
#include "Mini.h"

//コンストラクタ
Player::Player(GameObject* parent)
	:GameObject(parent,"Player"),pFbx(nullptr)
{
}

//初期化
void Player::Initialize()
{
	pFbx = new Fbx;
	pFbx->Load("Assets\\oden.fbx");
	Instantiate<Mini>(this);
	GameObject* pMN2 = Instantiate<Mini>(this);
	pMN2->setPosition(XMFLOAT3(-2.0f,1.0f,0));
}

//更新
void Player::Update()
{
	transform_.rotate_.y += 1;
	if (Input::IsKeyDown(DIK_K))
	{
		KillMe();
	}
}

//描画
void Player::Draw()
{
	pFbx->Draw(transform_);
}

//開放
void Player::Release()
{
	pFbx->Release();
	delete pFbx;
}