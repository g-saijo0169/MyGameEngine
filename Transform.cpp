#include "Transform.h"

Transform::Transform()
{
    position_ = { 0,0,0 };	//à íu
     rotate_ = { 0,0,0 };	//å¸Ç´
     scale_ = { 1,1,1 };	//ägëÂó¶

     matTranslate_ = XMMatrixIdentity();
     matRotate_ = XMMatrixIdentity();
     matScale_ = XMMatrixIdentity();
}

Transform::~Transform()
{
}

void Transform::Calclation()
{
    //à⁄ìÆçsóÒçÏê¨
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
    return matRotate_ * XMMatrixInverse(nullptr, matScale_);
}