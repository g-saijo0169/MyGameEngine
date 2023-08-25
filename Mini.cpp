#include "Mini.h"
#include "Engine/Fbx.h"
#include "Engine/Input.h"
#include "Engine/Model.h"
#include "Engine/SphereCollider.h"

//�R���X�g���N�^
Mini::Mini(GameObject* parent)
	:GameObject(parent, "Mini"), hModel_(-1)
{
}

//������
void Mini::Initialize()
{
	hModel_ = Model::Load("Assets/oden.fbx");
	assert(hModel_ >= 0);

	//transform_.position_.x = 2.0f;
	//transform_.position_.y = 1.0f;

	transform_.scale_.x = 0.2f;
	transform_.scale_.y = 0.2f;
	transform_.scale_.z = 0.2f;

	SphereCollider* collision = new SphereCollider(1.2f);
	AddCollider(collision);
}

//�X�V
void Mini::Update()
{
	transform_.rotate_.y += 1;
	transform_.position_.z += 0.5f;

	if (transform_.position_.z > 50)
	{
		KillMe();
	}
}

//�`��
void Mini::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

//�J��
void Mini::Release()
{
}