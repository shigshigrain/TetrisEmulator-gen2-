
#include "CmdPattern.hpp"

// コマンド入力の内容と情報

namespace shig {

	CmdPattern::CmdPattern() {
		score = 1000000000;
		scr = shig::CmdScore();
		index = -1;
		srs = 0, kind = 0;
		ttrp_f = -1;
		pre_gc = 0;
		isSFT = false;
		cmd_list = {3};
		pat = Tetri();
	}

	CmdPattern::CmdPattern(const Tetri& p, const std::vector<int>& list, const int& d){
		score = 1000000000;
		scr = shig::CmdScore();
		pat = p;
		cmd_list = list;
		index = d;
		srs = 0;
		kind = 0;
		ttrp_f = -1;
		pre_gc = 0;
		isSFT = false;
	};

	void CmdPattern::update() {
		score += scr.get_sum();
		return;
	}

	void CmdPattern::update(long long u) {
		score += u;
		return;
	}

	void CmdPattern::set_ttrpF(const int& sf) {
		ttrp_f = sf;
	}

	void CmdPattern::set_isSFT(const bool& sf) {
		isSFT = sf;
	}

	void CmdPattern::set(long long& s, const Tetri& p, std::vector<int>& list, int& d, int& r, int& k) {
		score = s;
		pat = p;
		cmd_list = list;
		index = d;
		srs = r;
		kind = k;
		//d_line = l;
	}

	void CmdPattern::set_ts(int r, int k) {
		srs = r;
		kind = k;
		return;
	}

	bool CmdPattern::operator<(const CmdPattern& ath) const
	{
		return (pat < ath.pat);
	}

	bool CmdPattern::operator==(const CmdPattern& ath) const
	{
		return (pat == ath.pat);
	}

}
