#include "Transform.h"

Transform::Transform()
{
    position_ = { 0,0,0 };	//�ʒu
     rotate_ = { 0,0,0 };	//����
     scale_ = { 1,1,1 };	//�g�嗦

     matTranslate_ = XMMatrixIdentity();
     matRotate_ = XMMatrixIdentity();
     matScale_ = XMMatrixIdentity();
}

Transform::~Transform()
{
}

void Transform::Calclation()
{
    //�ړ��s��쐬
    matTranslate_ = XMMatrixTranslation(position_.x, position_.y, position_.z);
    matRotate_ = XMMatrixRotationZ(XMConvertToRadians(rotate_.z)) * XMMatrixRotationX(XMConvertToRadians(rotate_.x)) * XMMatrixRotationY(XMConvertToRadians(rotate_.y));
    matScale_ = XMMatrixScaling(scale_.x, scale_.y, scale_.z);

}

XMMATRIX Transform::GetWorldMatrix()
{
    if (pParent_ != nullptr)
    {
     //�e����������e�̃��[���h�}�g���N�X���|����
     return  matScale_ * matRotate_ * matTranslate_ * pParent_->GetWorldMatrix();
    }else //�e��nullptr�̎��́A���̕ϊ�����
     return matScale_ * matRotate_ * matTranslate_;
}

XMMATRIX Transform::GetNormalMatrix()
{
    //��]�ƃX�P�[���̋t�s��
    return matRotate_ * XMMatrixInverse(nullptr, matScale_);
}