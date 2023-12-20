
#include "KeyConf.hpp"

KeyConf::KeyConf() {

	keyconf_R = KeyR;
	keyconf_C = KeyC;
	keyconf_X = KeyX;
	keyconf_Z = KeyZ;
	keyconf_Space = KeySpace;
	keyconf_Left = KeyLeft;
	keyconf_Right = KeyRight;
	keyconf_Up = KeyUp;
	keyconf_Down = KeyDown;
	keyconf_A = KeyA;
	keyconf_W = KeyW;
	keyconf_S = KeyS;
	keyconf_D = KeyD;
	keyconf_Q = KeyQ;
	keyconf_U = KeyU;
	keyconf_I = KeyI;
	keyconf_O = KeyO;
	keyconf_P = KeyP;
	keyconf_N = KeyN;
	keyconf_M = KeyM;
	keyconf_G = KeyG;

	Keyconf_list = std::vector<Input>(KeyVal_size, Input());

};

void KeyConf::SetDefault() {

	Keyconf_list.at(0) = KeyR;
	Keyconf_list.at(1) = KeyC;
	Keyconf_list.at(2) = KeyX;
	Keyconf_list.at(3) = KeyZ;
	Keyconf_list.at(4) = KeySpace;
	Keyconf_list.at(5) = KeyLeft;
	Keyconf_list.at(6) = KeyRight;
	Keyconf_list.at(7) = KeyUp;
	Keyconf_list.at(8) = KeyDown;
	Keyconf_list.at(9) = KeyA;
	Keyconf_list.at(10) = KeyW;
	Keyconf_list.at(11) = KeyS;
	Keyconf_list.at(12) = KeyD;
	Keyconf_list.at(13) = KeyQ;
	Keyconf_list.at(14) = KeyU;
	Keyconf_list.at(15) = KeyI;
	Keyconf_list.at(16) = KeyO;
	Keyconf_list.at(17) = KeyP;
	Keyconf_list.at(18) = KeyN;
	Keyconf_list.at(19) = KeyM;
	Keyconf_list.at(20) = KeyG;

	return;
};

void KeyConf::SetKey(const KeyVal& key, const Input& s3d_key){

	/*switch (key)
	{
	case KeyVal::R:
		KeyConf::keyconf_R = s3d_key;
		break;
	case KeyVal::C:
		KeyConf::keyconf_C = s3d_key;
		break;
	case KeyVal::X:
		KeyConf::keyconf_X = s3d_key;
		break;
	case KeyVal::Z:
		KeyConf::keyconf_Z = s3d_key;
		break;
	case KeyVal::Space:
		KeyConf::keyconf_Space = s3d_key;
		break;
	case KeyVal::Left:
		KeyConf::keyconf_Left = s3d_key;
		break;
	case KeyVal::Right:
		KeyConf::keyconf_Right = s3d_key;
		break;
	case KeyVal::Up:
		KeyConf::keyconf_Up = s3d_key;
		break;
	case KeyVal::Down:
		KeyConf::keyconf_Down = s3d_key;
		break;
	case KeyVal::A:
		KeyConf::keyconf_A = s3d_key;
		break;
	case KeyVal::W:
		KeyConf::keyconf_W = s3d_key;
		break;
	case KeyVal::S:
		KeyConf::keyconf_S = s3d_key;
		break;
	case KeyVal::D:
		KeyConf::keyconf_D = s3d_key;
		break;
	case KeyVal::Q:
		KeyConf::keyconf_Q = s3d_key;
		break;
	case KeyVal::U:
		KeyConf::keyconf_U = s3d_key;
		break;
	case KeyVal::I:
		KeyConf::keyconf_I = s3d_key;
		break;
	case KeyVal::O:
		KeyConf::keyconf_O = s3d_key;
		break;
	case KeyVal::P:
		KeyConf::keyconf_P = s3d_key;
		break;
	case KeyVal::N:
		KeyConf::keyconf_N = s3d_key;
		break;
	case KeyVal::M:
		KeyConf::keyconf_M = s3d_key;
		break;


	default:
		break;
	}*/

	Keyconf_list.at(key) = s3d_key;

	return;
}
Input KeyConf::GetKey(const KeyVal& keyStr){

	return Keyconf_list.at(keyStr);
}
;

KeyConf::~KeyConf() {

};

bool IsPressedKeyAnd(KeyConf& kc, const KeyVal& a, const KeyVal& b){
	return kc.GetKey(a).pressed() && kc.GetKey(b).pressed();
}

bool IsKeyVP(KeyConf& kc, const KeyVal& kv)
{
	return kc.GetKey(kv).pressed();
}
