#include "stdafx.h"
#include "GameContainer.hpp"

namespace shig {

	GameContainer::GameContainer() {

		slot_id = 0;
		pre_score = 0;
		hold_AI = 0;
		current_AI = 0;
		next_AI = VI(0);
		q_next_AI = deque<int>();
		pc_cnt = 0;
		SRS_kind = 0;
		TS_kind = 0;
		combo = 0;
		btb = -1;
		height_sum = 0;
		height_mxm = 0;
		ttrp_size = 0;
		ttrp_ofsY = 0;
		total_s = 0;
		ttrp_able = false;
		field_AI = VVI(45, (VI(10, 0)));
		p_field_AI = VVI(45, (VI(10, 0)));
		height = VI(10, 0);
		cv = vector<CmdPattern>(0);
		cv.reserve(600);
		ec = vector<CmdPattern>(0);
		ec.reserve(10);
		int z = 0;
		string zs = "nothing";
		gc_ttrp.set(z, z, z, z, zs, z);
		VI zv(0, 0);
		gc_ttrp.set_id_list(zv);
		vector<pairI2> zp(0, make_pair(0, 0));
		gc_ttrp.set_terms(zp);
	}

	bool GameContainer::set_gc_int(const VI& si) {
		if (si.size() != 12) return false;
		hold_AI = si[0];
		current_AI = si[1];
		pc_cnt = si[2];
		SRS_kind = si[3];
		TS_kind = si[4];
		combo = si[5];
		btb = si[6];
		height_sum = si[7];
		height_mxm = si[8];
		ttrp_size = si[9];
		ttrp_ofsY = si[10];
		total_s = si[11];
		return true;

	}

	bool GameContainer::set_gc_VI(const vector<VI>& sv) {
		if (sv.size() != 1) return false;
		height = sv[0];
		return true;
	}

	bool GameContainer::set_gc_bool(const vector<bool>& sb) {
		if (sb.size() != 1) return false;
		ttrp_able = sb[0];
		return true;
	}

	bool GameContainer::set_gc_next(const VI& nx, const deque<int>& qnx) {
		next_AI = nx;
		q_next_AI = qnx;
		return true;
	}

	bool GameContainer::set_gc_field(const vector<VVI>& sf) {
		if (sf.size() != 3) return false;
		field_AI = sf[0];
		p_field_AI = sf[1];
		//strategy_map = sf[2];
		return true;
	}

	bool GameContainer::set_gc_ttrp(const TetriPlate& st) {
		gc_ttrp = st;
		return true;
	}

	bool GameContainer::add_gc_cmdp(const CmdPattern& cg) {

		ec.push_back(cg);

		return true;
	}

	CmdPattern GameContainer::get_gc_cmdp(int itr) {
		if (itr < 0 || itr >= ec.size()) {
			return CmdPattern();
		}
		else {
			return ec.at(itr);
		}
	}

	GameContainer::~GameContainer() {

	}



}
