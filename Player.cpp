#include "Player.h"
#include "Engine/Fbx.h"

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
}

//�X�V
void Player::Update()
{
}

//�`��
void Player::Draw()
{
	pFbx->Draw(transform_);
}

//�J��
void Player::Release()
{
	pFbx->Release();
	delete pFbx;
}