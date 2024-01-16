#pragma once
#include "ShigUtility.h"
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
		std::vector<int> next_AI;
		deque<int> q_next_AI;
		std::vector<std::vector<int>> field_AI;
		std::vector<std::vector<int>> p_field_AI;
		std::vector<int> height;
		std::vector<int> cmd_list;
		set<CmdPattern> cp;
		TetriPlate gc_ttrp;
		vector<CmdPattern> ec;
	public:
		GameContainer();
		~GameContainer();
		bool set_gc_int(const std::vector<int>& si);
		bool set_gc_bool(const vector<bool>& sb);
		bool set_gc_VI(const vector<std::vector<int>>& sv);
		bool set_gc_next(const std::vector<int>& nx, const deque<int>& qnx);
		bool set_gc_field(const vector<std::vector<std::vector<int>>>& sf);
		bool set_gc_ttrp(const TetriPlate& st);
		bool add_gc_cmdp(const CmdPattern& cg);
		CmdPattern get_gc_cmdp(int itr);

	};



}
