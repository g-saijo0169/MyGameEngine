#include "SceneManager.h"
#include "../TestScene.h"

SceneManager::SceneManager(GameObject* parent)
	:GameObject(parent, "SceneManager")
{
}

SceneManager::~SceneManager()
{
}

void SceneManager::Initialize()
{
	currentSceneID_ = SCENE_ID_TEST;
	nextSceneID_ = currentSceneID_;


	Instantiate<TestScene>(this);
}

void SceneManager::Update()
{
	//�V�[�������ۂɐ؂�ւ���
	//���݂̃V�[���ƃl�N�X�g�V�[�����ʂ�������؂�ւ�
	if (currentSceneID_ != nextSceneID_)
	{
		auto scene = childList_.begin();
		(*scene)->ReleaseSub();
	}
	//���݂̃V�[���̌�Еt��
	//�V�����V�[���̏���
}

void SceneManager::Draw()
{
}

void SceneManager::Release()
{
}

void SceneManager::ChangeScene(SCENE_ID_NEXT)
{
	nextSceneID_ = next_;
}

