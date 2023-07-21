#include "Enemy.h"
#include "Engine/Input.h"
#include "Engine/SphereCollider.h"

Enemy::Enemy(GameObject* parent)
	:GameObject(parent, "Enemy")
{
}

void Enemy::Initialize()
{
	pFbx = new Fbx;
	pFbx->Load("Assets/odn.fbx");
	transform_.position_.z = 20.0f;

	SphereCollider
}

void Enemy::Update()
{
}

void Enemy::Draw()
{
	pFbx->Draw(transform_)
}

void Enemy::Release()
{
}