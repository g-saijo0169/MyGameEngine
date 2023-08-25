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
		return(this); //������_objName�̃I�u�W�F�N�g�������I
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
/// �ċA�Ăяo����RootJob��T���Ă��̃A�h���X��Ԃ��֐�
/// </summary>
/// <returns>RootJob�̃A�h���X�iGameObject�@*�@�^�j</returns>
GameObject* GameObject::GetRootJob()
{
	if(pParent_ == nullptr)
	return this;

	return pParent_->GetRootJob();
}

GameObject* GameObject::FindObject(string _objName)
{
	//�l���Ă݂�
	//��������݂āA���[�g�W���u��T���āA���̃��[�g�W���u����S���̎q�����ǂ���_objName��T��
	return GetRootJob()->FindChildObject(_objName);


}

void GameObject::AddCollider(SphereCollider* pCollider)
{
	pCollider_ = pCollider;
}

void GameObject::Collision(GameObject* pTarget)
{
	if (pTarget->pCollider_ == nullptr || pTarget == this)
		return; //�^�[�Q�b�g�ɃR���C�_�[���A�^�b�`����Ă��Ȃ�

	//�����ƃ^�[�Q�b�g�̋��� <= R1+R2�Ȃ�
	/*XMVECTOR v{ transform_.position_.x - pTarget->transform_.position_.x,
			   transform_.position_.y - pTarget->transform_.position_.y,
			   transform_.position_.z - pTarget->transform_.position_.z,
				0 };
	XMVECTOR dist = XMVECTOR3D(v, v);*/

	float dist = (transform_.position_.x - pTarget->transform_.position_.x) * (transform_.position_.x - pTarget->transform_.position_.x)
		+ (transform_.position_.y - pTarget->transform_.position_.y) * (transform_.position_.y - pTarget->transform_.position_.y)
		+ (transform_.position_.x - pTarget->transform_.position_.x) * (transform_.position_.z - pTarget->transform_.position_.z);
	float rDist = (this->pCollider_->GetRadius() + pTarget->pCollider_->GetRadius()) * (this->pCollider_->GetRadius() + pTarget->pCollider_->GetRadius());
	// �����A�����̃R���C�_�[�ƃ^�[�Q�b�g���Ԃ����Ă�����
	//onCollision(pTarget)���Ăяo���I
		if (dist <= rDist)
		{
			OnCollision(pTarget);
			
		}
}

void GameObject::RoundRobin(GameObject* pTarget)
{
	if (pCollider_ == nullptr)
		return;
	if (pTarget->pCollider_ != nullptr)//�����ƃ^�[�Q�b�g
		Collision(pTarget);
	//�@�����̎q���S���ƃ^�[�Q�b�g
	for (auto itr : childList_)
		RoundRobin(itr);
}


