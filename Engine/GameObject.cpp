#include "GameObject.h"
#include "Direct3D.h"

GameObject::GameObject()
	:pParent_(nullptr)
{
}

GameObject::GameObject(GameObject* parent, const std::string& name)
	:pParent_(nullptr),objectName_(name)
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


