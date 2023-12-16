#pragma once
#include "tetri.h"

namespace shig {


	Tetri::Tetri() {
		rot = 0;
		X = 0, Y = 0;
		id = 0;
		px_size = 30;
		mino = minoT;
	};

	Tetri::Tetri(int id) {
		rot = 0;
		X = 0, Y = 0;
		Tetri::id = id;
		px_size = 30;
		mino = minoT;
		set_mino(id);
	};

	Tetri::Tetri(int rot, int X, int Y, int id) {
		Tetri::rot = rot;
		Tetri::X = X;
		Tetri::Y = Y;
		Tetri::id = id;
		Tetri::px_size = 30;
		//Tetri::set_mino(id);

	};

	void Tetri::set(int a, int b, int c, int d) {
		Tetri::rot = a; Tetri::X = b; Tetri::Y = c; Tetri::id = d;
		return;
	}

	void Tetri::addX(int add) {
		Tetri::X += add;
		return;
	}

	void Tetri::addY(int add) {
		Tetri::Y += add;
		return;
	}

	void Tetri::set_rot(int to) {
		Tetri::rot = to;
		return;
	}

	void Tetri::set_id(int d) {
		Tetri::id = d;
		return;
	}

	string Tetri::get_mino_str() {
		string gms;
		gms = to_string(rot) + " " + to_string(X) + " " + to_string(Y) + " " + to_string(id);
		return gms;
	}

	void Tetri::set_mino(int i) {
		if (i < 1 || i > 7)id = 6;
		else id = i;
		Tetri::rot = 0; Tetri::X = 3; Tetri::Y = 21;

		switch (Tetri::id)
		{
		case 1:
			Tetri::mino = minoI;
			break;
		case 2:
			Tetri::mino = minoJ;
			break;
		case 3:
			Tetri::mino = minoL;
			break;
		case 4:
			Tetri::mino = minoO;
			break;
		case 5:
			Tetri::mino = minoS;
			break;
		case 6:
			Tetri::mino = minoT;
			break;
		case 7:
			Tetri::mino = minoZ;
			break;
		default:
			Tetri::mino = minoT;
			break;
		}


		return;

	}

	bool Tetri::operator< (const Tetri& ath) const {
		if (id < ath.id)return true;
		else if (id > ath.id)return false;
		else {
			if (X < ath.X)return true;
			else if (X > ath.X)return false;
			else {
				if (Y < ath.Y)return true;
				else if (Y > ath.Y)return false;
				else {
					if (rot < ath.rot)return true;
					else return false;
					//return rot < ath.rot;
				}
			}
		}
	}

	bool Tetri::operator == (const Tetri& ath) const {
		if (id == 4 && ath.id == 4)return (X == ath.X && Y == ath.Y);
		return (id == ath.id && X == ath.X && Y == ath.Y && rot == ath.rot);
	}

	// rot px_size H W (get 4 stats)
	std::tuple<int, int, int, int> getTS(Tetri& t) {
		return { t.rot, t.px_size, (int)t.mino.at(t.rot).size(), (int)t.mino.at(t.rot).at(0).size() };
	}

}
