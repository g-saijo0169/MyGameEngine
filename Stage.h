#pragma once
#include "Engine/GameObject.h"
#include <iostream>
#include "windows.h"
#include <fstream>


namespace {
    const int MODEL_NUM = 5;
    const int XSIZE{ 15 };
    const int YSIZE{ 15 };
    const int ZSIZE{ 15 };
    enum BLOCKTYPE
    {
        DEFAULT, BRICK, GRASS, SAND, WATER
    };

}

//���������Ǘ�����N���X
class Stage : public GameObject
{
    int hModel_[MODEL_NUM];    //���f���ԍ�
    //int table_[XSIZE][ZSIZE];

    struct {
        int type;
        int height;

    }table_[XSIZE][ZSIZE];

    int mode_;      //0:�グ�� 1:������ 2:��ނ�ς���
    int select_;    //���


public:
   
    //�R���X�g���N�^
    Stage(GameObject* parent);

    //�f�X�g���N�^
    ~Stage();

    //������
    void Initialize() override;

    //�X�V
    void Update() override;

    //�`��
    void Draw() override;

    //�J��
    void Release() override;

    BOOL DialogProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp);
    void Save();
    void Load();


    void SetBlock(int _x, int _z, BLOCKTYPE _type);
    void SetBlockHeght(int _x, int _z, int _height);
};

