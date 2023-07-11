#include "GameObject.h"
#include "Direct3D.h"

GameObject::GameObject(GameObject* parent, const std::string& name)
{
}

GameObject::GameObject()
{
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
