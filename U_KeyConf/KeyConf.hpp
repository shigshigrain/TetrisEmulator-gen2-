#pragma once
#include "stdafx.h"
#include<Siv3D.hpp>
constexpr int KeyVal_size = 22;

enum KeyVal {
	R,
	C,
	X,
	Z,
	Space,
	Left,
	Right,
	Up,
	Down,
	A,
	W,
	S,
	D,
	Q,
	U,
	I,
	O,
	P,
	N,
	M,
	G,
	size
};

class KeyConf
{
private:
	Input keyconf_R;
	Input keyconf_C;
	Input keyconf_X;
	Input keyconf_Z;
	Input keyconf_Space;
	Input keyconf_Left;
	Input keyconf_Right;
	Input keyconf_Up;
	Input keyconf_Down;
	Input keyconf_A;
	Input keyconf_W;
	Input keyconf_S;
	Input keyconf_D;
	Input keyconf_Q;
	Input keyconf_U;
	Input keyconf_I;
	Input keyconf_O;
	Input keyconf_P;
	Input keyconf_N;
	Input keyconf_M;
	Input keyconf_G;
	std::vector<Input> Keyconf_list;
public:

	KeyConf();
	void SetDefault();
	void SetKey(const KeyVal& key, const Input& s3d_key);
	Input GetKey(const KeyVal& keyStr);
	//void load_key();

	~KeyConf();

};

// 非メンバ関数
bool IsPressedKeyAnd(KeyConf& kc, const KeyVal& a, const KeyVal& b);

// KeyConf と KeyValを指定して.pressed()の返り値を取得する
bool IsKeyVP(KeyConf& kc, const KeyVal& kv);
