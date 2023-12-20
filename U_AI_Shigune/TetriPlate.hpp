#pragma once
#include "shig_utility.h"
#include "tetri.h"

namespace shig {

	class TetriPlate
	{
	public:
		bool avalable;
		int mino_num;
		int list_size;
		int tp_id;
		int terms_num;
		int bgn_f;
		std::vector<std::pair<int, int>> terms;
		std::vector<int> list_id;
		std::vector<bool> mino_check;
		std::string temp_name;
		std::vector<Tetri> list_mino;
		std::vector<int> list_mino_s;

	public:
		TetriPlate();
		bool set_def();
		bool Setup(const int& a, const int& b, const int& c, const int& d, const string& e, const int& f);
		bool set_id_list(const std::vector<int>& l);
		bool set_list(const vector<Tetri>& rnd, const std::vector<int>& rs);
		bool set_terms(const std::vector<std::pair<int, int>>& tr);
		~TetriPlate();
	};

}
