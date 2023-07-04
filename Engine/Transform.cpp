#include "Transform.h"

Transform::Transform()
{
    position_ = { 0,0,0 };	//位置
     rotate_ = { 0,0,0 };	//向き
     scale_ = { 1,1,1 };	//拡大率

     matTranslate_ = XMMatrixIdentity();
     matRotate_ = XMMatrixIdentity();
     matScale_ = XMMatrixIdentity();
}

Transform::~Transform()
{
}

void Transform::Calclation()
{
    //移動行列作成
    matTranslate_ = XMMatrixTranslation(position_.x, position_.y, position_.z);
    matRotate_ = XMMatrixRotationZ(XMConvertToRadians(rotate_.z)) * XMMatrixRotationX(XMConvertToRadians(rotate_.x)) * XMMatrixRotationY(XMConvertToRadians(rotate_.y));
    matScale_ = XMMatrixScaling(scale_.x, scale_.y, scale_.z);

}

XMMATRIX Transform::GetWorldMatrix()
{
    Calclation();
    return matScale_ * matRotate_ * matTranslate_;
}

XMMATRIX Transform::GetNormalMatrix()
{
    //回転とスケールの逆行列
    return matRotate_ * XMMatrixInverse(nullptr, matScale_);
}