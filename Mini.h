#pragma once
#include "Engine/GameObject.h"

//前方宣言 
class Fbx;

//■■シーンを管理するクラス
class Mini : public GameObject
{
	Fbx* pFbx;
public:
	//コンストラクタ
	//引数：parent  親オブジェクト（SceneManager）
	Mini(GameObject* parent);

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;
};