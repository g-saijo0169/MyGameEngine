#include "Mini.h"
#include "Engine/Fbx.h"
#include "Engine/Input.h"
#include "Engine/Model.h"
#include "Engine/SphereCollider.h"

//コンストラクタ
Mini::Mini(GameObject* parent)
	:GameObject(parent, "Mini"), hModel_(-1)
{
}

//初期化
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

//更新
void Mini::Update()
{
	transform_.rotate_.y += 1;
	transform_.position_.z += 0.5f;

	if (transform_.position_.z > 50)
	{
		KillMe();
	}
}

//描画
void Mini::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

//開放
void Mini::Release()
{
}