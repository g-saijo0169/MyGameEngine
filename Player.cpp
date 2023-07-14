#include "Player.h"
#include "Engine/Input.h"
#include "Mini.h"
#include "Engine/Model.h"

//�R���X�g���N�^
Player::Player(GameObject* parent)
	:GameObject(parent,"Player"),hModel_(-1)
{
}

//������
void Player::Initialize()
{
	hModel_ = Model::Load("Assets/oden.fbx");
	assert(hModel_ >= 0);
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
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

//�J��
void Player::Release()
{
}