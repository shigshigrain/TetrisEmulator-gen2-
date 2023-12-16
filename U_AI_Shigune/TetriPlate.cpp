
#include "TetriPlate.hpp"

namespace shig {

	TetriPlate::TetriPlate() {
		avalable = true;
		mino_num = 7;
		list_size = 0;
		tp_id = 0;
		terms_num = 0;
		bgn_f = 0;
		terms = std::vector<std::pair<int, int>>(0);
		list_id = std::vector<int>(0);
		mino_check = std::vector<bool>(0);
		temp_name = "template";
		list_mino = std::vector<Tetri>(0);
		list_mino_s = std::vector<int>(0);
	}

	bool TetriPlate::set_def() {
		mino_num = 0;
		mino_check = vector<bool>(mino_num, false);
		list_size = 0;
		tp_id = 0;
		terms_num = 0;
		temp_name = "nothing";
		bgn_f = 0;
		terms = vector<pairI2>(0);
		list_id = vector<int>(0);
		mino_check = vector<bool>(0);
		list_mino = vector<Tetri>(0);
		list_mino_s = vector<int>(0);

		return true;
	}

	bool TetriPlate::set(int& a, int& b, int& c, int& d, string& e, int& f) {
		mino_num = a;
		mino_check = vector<bool>(mino_num, false);
		list_size = b;
		tp_id = c;
		terms_num = d;
		temp_name = e;
		bgn_f = f;

		return true;
	}

	bool TetriPlate::set_id_list(VI& l) {
		list_id = l;
		return true;
	}

	bool TetriPlate::set_list(vector<Tetri>& rnd, VI& rs) {
		list_mino = rnd;
		list_mino_s = rs;
		return true;
	}


	bool TetriPlate::set_terms(vector<pairI2>& tr) {
		terms = tr;
		return true;
	}


	TetriPlate::~TetriPlate() {
	}

}
