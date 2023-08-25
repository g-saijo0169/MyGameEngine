#pragma once
 
#include "Transform.h"
#include <list>
#include <string>

using namespace std;
class SphereCollider;

class GameObject
{
protected:
	std::list<GameObject*>  childList_;
	Transform				transform_;
	GameObject*				pParent_;
	std::string				objectName_;
	SphereCollider*         pCollider_;

public:
	GameObject();
	GameObject(GameObject* parent, const std::string& name);
	~GameObject();

	virtual void Initialize() = 0;
	virtual void Update()= 0;
	virtual void Draw() = 0;
	virtual void Release() = 0;
	void DrawSub();
	void UpdateSub();
	void ReleaseSub();

	bool isDead();
	void KillMe();

	void setPosition(XMFLOAT3 position);
	XMFLOAT3 getPosition();
	GameObject* FindChildObject(string _objName);
	GameObject* GetRootJob();
	GameObject* FindObject(string _objName);
	void AddCollider(SphereCollider* pCollider);
	void Collision(GameObject* pTarget);
	void RoundRobin(GameObject* pTarget);
	virtual void OnCollision(GameObject* pTarget) {};

	//テンプレートの定義
	template <class T>
	T* Instantiate(GameObject* parent)
	{
		T* pObject;
		pObject = new T(parent);
		pObject->Initialize();
		childList_.push_back(pObject);
		return pObject;
	}

private:
	struct OBJECT_STATE
	{
		unsigned dead : 1;			//削除するか
	};
	OBJECT_STATE state_;

	XMFLOAT3 position;
};