#pragma once
#include "shig_utility.h"
#include "CmdPattern.hpp"
#include "TetriPlate.hpp"

namespace shig {

	class GameContainer
	{
	public:
		//private:
		int slot_id;
		LL pre_score;
		int hold_AI;
		int current_AI;
		int pc_cnt;
		int SRS_kind;
		int TS_kind;
		int combo;
		int btb;
		int height_sum;
		int height_mxm;
		int ttrp_size;
		int ttrp_ofsY;
		int total_s;
		bool ttrp_able;
		VI next_AI;
		deque<int> q_next_AI;
		VVI field_AI;
		VVI p_field_AI;
		VI height;
		VI cmd_list;
		set<CmdPattern> cp;
		vector<CmdPattern> cv;
		TetriPlate gc_ttrp;
		vector<CmdPattern> ec;
	public:
		GameContainer();
		~GameContainer();
		bool set_gc_int(const VI& si);
		bool set_gc_bool(const vector<bool>& sb);
		bool set_gc_VI(const vector<VI>& sv);
		bool set_gc_next(const VI& nx, const deque<int>& qnx);
		bool set_gc_field(const vector<VVI>& sf);
		bool set_gc_ttrp(const TetriPlate& st);
		bool add_gc_cmdp(const CmdPattern& cg);
		CmdPattern get_gc_cmdp(int itr);

	};



}
