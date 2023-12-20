
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
		mino_check = std::vector<bool>(mino_num, false);
		list_size = 0;
		tp_id = 0;
		terms_num = 0;
		temp_name = "nothing";
		bgn_f = 0;
		terms.clear();
		list_id.clear();
		mino_check.clear();
		list_mino.clear();
		list_mino_s.clear();

		return true;
	}

	bool TetriPlate::Setup(const int& a, const int& b, const int& c, const int& d, const string& e, const int& f) {
		mino_num = a;
		mino_check = std::vector<bool>(mino_num, false);
		list_size = b;
		tp_id = c;
		terms_num = d;
		temp_name = e;
		bgn_f = f;

		return true;
	}

	bool TetriPlate::set_id_list(const std::vector<int>& l) {
		list_id = l;
		return true;
	}

	bool TetriPlate::set_list(const std::vector<Tetri>& rnd, const std::vector<int>& rs) {
		list_mino = rnd;
		list_mino_s = rs;
		return true;
	}


	bool TetriPlate::set_terms(const std::vector<std::pair<int, int>>& tr) {
		terms = tr;
		return true;
	}


	TetriPlate::~TetriPlate() {
	}

}
