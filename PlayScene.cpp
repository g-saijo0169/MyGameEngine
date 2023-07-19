#include "PlayScene.h"
#include "Engine/Fbx.h"
#include "Player.h"

//コンストラクタ
PlayScene::PlayScene(GameObject* parent)
	:GameObject(parent, "PlayScene")
{
}

//初期化
void PlayScene::Initialize()
{
	//Player* pPlayer;
	//pPlayer = new Player(this);   //プレイヤーの親は、自分(PlayScene)
	//pPlayer->Initialize();
	//childList_.push_back(pPlayer);//PlaySceneの子として、プレイヤーを登録
	GameObject* pPlayer = Instantiate<Player>(this);
}

//更新
void PlayScene::Update()
{
}

//描画
void PlayScene::Draw()
{
}

//開放
void PlayScene::Release()
{

}
