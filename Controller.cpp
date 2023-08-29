#include "Controller.h"
#include "Engine/Input.h"
#include "Engine/Camera.h"

//コンストラクタ
Controller::Controller(GameObject* parent)
    :GameObject(parent, "Controller")
{
}

//デストラクタ
Controller::~Controller()
{
}

//初期化
void Controller::Initialize()
{
}

//更新
void Controller::Update()
{
    XMVECTOR vPos = XMLoadFloat3(&transform_.position_);
    XMVECTOR mMove = { 0.0f, 0.0f, 0.1f, 0.0f };//奥に0.1m
    XMMATRIX mRotY = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));//Y軸で90°回転させる行列
    mMove = XMVector3TransformCoord(mMove, mRotY);

    if (Input::IsKey(DIK_S))
    {
        vPos += mMove;
        XMStoreFloat3(&transform_.position_, vPos);
    }

    if (Input::IsKey(DIK_W))
    {
        vPos -= mMove;
        XMStoreFloat3(&transform_.position_, vPos);
    }

    if (Input::IsKey(DIK_D))
    {
        transform_.position_.x -= 0.1f;
    }

    if (Input::IsKey(DIK_A))
    {
        transform_.position_.x += 0.1f;
    }

    if (Input::IsKey(DIK_LEFT))
    {
        transform_.rotate_.y -= 0.8f;
    }

    if (Input::IsKey(DIK_RIGHT))
    {
        transform_.rotate_.y += 0.8f;
    }

    Camera::SetTarget(transform_.position_);
    XMVECTOR vCam = { 0, 5, 15, 0 };
    vCam = XMVector3TransformCoord(vCam, mRotY);
    XMFLOAT3 camPos;
    XMStoreFloat3(&camPos, vPos + vCam);
    Camera::SetPosition(camPos);
}

//描画
void Controller::Draw()
{
}

//開放
void Controller::Release()
{
}