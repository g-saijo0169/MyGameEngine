#include "PlayScene.h"
#include "Engine/Fbx.h"
#include "Player.h"

//コンストラクタ
PlayScene::PlayScene(GameObject* parent)
{
}

//初期化
void PlayScene::Initialize()
{
	//Player* pPlayer;
	//pPlayer = new Player(this);   //プレイヤーの親は、自分(PlayScene)
	//pPlayer->Initialize();
	//childList_.push_back(pPlayer);//PlaySceneの子として、プレイヤーを登録
	Instantiate<Player>(this);

}

//更新
void PlayScene::Update()
{
	this->transform_.rotate_.y += 1;
}

//描画
void PlayScene::Draw()
{
}

//開放
void PlayScene::Release()
{

}
