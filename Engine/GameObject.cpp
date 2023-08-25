#include "GameObject.h"
#include "Direct3D.h"
#include "SphereCollider.h"

GameObject::GameObject():pParent_(nullptr)
{
}

GameObject::GameObject(GameObject* parent, const std::string& name)
	:pParent_(parent),objectName_(name)
{
	if (parent != nullptr)
		this->transform_.pParent_ = &(parent->transform_);
}

GameObject::~GameObject()
{
}

void GameObject::DrawSub()
{
	Draw();
	for (auto itr = childList_.begin(); itr != childList_.end(); itr++)
		(*itr)->DrawSub();
}

void GameObject::UpdateSub()
{
	Update();

	RoundRobin(GetRootJob());
	for (auto itr = childList_.begin(); itr != childList_.end(); itr++)
		(*itr)->UpdateSub();

	for (auto it = childList_.begin(); it != childList_.end();)
	{
		if ((*it)->isDead() == true)
		{
			(*it)->ReleaseSub();
			SAFE_DELETE(*it);
			it = childList_.erase(it);

		}
		else
		{
			it++;
		}
	}
}

void GameObject::ReleaseSub()
{

	for (auto it = childList_.begin(); it != childList_.end(); it++)
	{
		(*it)->ReleaseSub();
		SAFE_DELETE(*it);
	}
	Release();
}

bool GameObject::isDead()
{
	return (state_.dead != 0);
}

void GameObject::KillMe()
{
	state_.dead = 1;
}

void GameObject::setPosition(XMFLOAT3 position)
{
	transform_.position_ = position;
}

XMFLOAT3 GameObject::getPosition()
{
	return transform_.position_;
}

GameObject* GameObject::FindChildObject(string _objName)
{
	if (_objName == this->objectName_)
	{
		return(this); //自分が_objNameのオブジェクトだった！
	}
	else
	{
		//for (auto itr = childlList_.begin(); itr != childlList_.end(); itr++)
		for (auto itr: childList_)
		{
			GameObject* obj = itr->FindChildObject(_objName);
			if (obj != nullptr)
				return obj;
		}
	}
	return nullptr;
}


/// <summary>
/// 再帰呼び出しでRootJobを探してそのアドレスを返す関数
/// </summary>
/// <returns>RootJobのアドレス（GameObject　*　型）</returns>
GameObject* GameObject::GetRootJob()
{
	if(pParent_ == nullptr)
	return this;

	return pParent_->GetRootJob();
}

GameObject* GameObject::FindObject(string _objName)
{
	//考えてみて
	//自分からみて、ルートジョブを探して、そのルートジョブから全部の子をたどって_objNameを探す
	return GetRootJob()->FindChildObject(_objName);


}

void GameObject::AddCollider(SphereCollider* pCollider)
{
	pCollider_ = pCollider;
}

void GameObject::Collision(GameObject* pTarget)
{
	if (pTarget->pCollider_ == nullptr || pTarget == this)
		return; //ターゲットにコライダーがアタッチされていない

	//自分とターゲットの距離 <= R1+R2なら
	/*XMVECTOR v{ transform_.position_.x - pTarget->transform_.position_.x,
			   transform_.position_.y - pTarget->transform_.position_.y,
			   transform_.position_.z - pTarget->transform_.position_.z,
				0 };
	XMVECTOR dist = XMVECTOR3D(v, v);*/

	float dist = (transform_.position_.x - pTarget->transform_.position_.x) * (transform_.position_.x - pTarget->transform_.position_.x)
		+ (transform_.position_.y - pTarget->transform_.position_.y) * (transform_.position_.y - pTarget->transform_.position_.y)
		+ (transform_.position_.x - pTarget->transform_.position_.x) * (transform_.position_.z - pTarget->transform_.position_.z);
	float rDist = (this->pCollider_->GetRadius() + pTarget->pCollider_->GetRadius()) * (this->pCollider_->GetRadius() + pTarget->pCollider_->GetRadius());
	// もし、自分のコライダーとターゲットがぶつかっていたら
	//onCollision(pTarget)を呼び出す！
		if (dist <= rDist)
		{
			OnCollision(pTarget);
			
		}
}

void GameObject::RoundRobin(GameObject* pTarget)
{
	if (pCollider_ == nullptr)
		return;
	if (pTarget->pCollider_ != nullptr)//自分とターゲット
		Collision(pTarget);
	//　自分の子供全部とターゲット
	for (auto itr : childList_)
		RoundRobin(itr);
}


