#pragma once
#include "ShigUtility.h"
#include "CmdScore.hpp"
#include "tetri.h"

namespace shig {

	class CmdPattern {

	public:
		long long score;
		shig::CmdScore scr;
		int index;
		int srs;
		int kind;
		int ttrp_f;
		int pre_gc;
		bool isSFT;
		std::vector<int> cmd_list;
		Tetri pat;

	public:
		CmdPattern();
		CmdPattern(const Tetri& p, const std::vector<int>& list, const int& d);
		void update();
		void update(long long u);
		void set_ttrpF(const int& sf);
		void set_isSFT(const bool& sf);
		void set(long long& s, const Tetri& p, std::vector<int>& list, int& d, int& r, int& k);
		void set_ts(int r, int k);
		bool operator < (const CmdPattern& ath) const;
		bool operator == (const CmdPattern& ath) const;


	};

}

