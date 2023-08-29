#include "Controller.h"
#include "Engine/Input.h"
#include "Engine/Camera.h"

//�R���X�g���N�^
Controller::Controller(GameObject* parent)
    :GameObject(parent, "Controller")
{
}

//�f�X�g���N�^
Controller::~Controller()
{
}

//������
void Controller::Initialize()
{
}

//�X�V
void Controller::Update()
{
    XMVECTOR vPos = XMLoadFloat3(&transform_.position_);
    XMVECTOR mMove = { 0.0f, 0.0f, 0.1f, 0.0f };//����0.1m
    XMMATRIX mRotY = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));//Y����90����]������s��
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

//�`��
void Controller::Draw()
{
}

//�J��
void Controller::Release()
{
}