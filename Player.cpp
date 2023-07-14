#include "Player.h"
#include "Engine/Fbx.h"
#include "Engine/Input.h"
#include "Mini.h"

//�R���X�g���N�^
Player::Player(GameObject* parent)
	:GameObject(parent,"Player"),pFbx(nullptr)
{
}

//������
void Player::Initialize()
{
	pFbx = new Fbx;
	pFbx->Load("Assets\\oden.fbx");
	//Instantiate<Mini>(this);
	//GameObject* pMN2 = Instantiate<Mini>(this);
	//pMN2->setPosition(XMFLOAT3(-2.0f,1.0f,0));
}

//�X�V
void Player::Update()
{
	transform_.rotate_.y += 1;
	//if (Input::IsKeyDown(DIK_K))
	//{
	//	KillMe();
	//}
	if (Input::IsKey(DIK_LEFT))
	{
		transform_.position_.x -= 0.1f;
	}
	if (Input::IsKey(DIK_RIGHT))
	{
		transform_.position_.x += 0.1f;
	}
	if (Input::IsKey(DIK_SPACE))
	{
		GameObject* pBullet = Instantiate<Mini>(pParent_);
		pBullet->setPosition(transform_.position_);
	}
}

//�`��
void Player::Draw()
{
	pFbx->Draw(transform_);
}

//�J��
void Player::Release()
{
	//pFbx->Release();
	//delete pFbx;
}