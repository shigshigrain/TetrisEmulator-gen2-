﻿#pragma once

#include "ShigUtility.h"
#include "tetri.h"
//#include "KeyConf.hpp"

constexpr int fh = 45;

// 乱数生成用 
static std::random_device rd;
static std::mt19937 Rengine(rd());


namespace shig {
	enum Rotate {
		Clockwise,
		CounterClockwise,
	};

	const std::vector<int> next_rotateClock = { 1, 2, 3, 0 };
	const std::vector<int> next_rotateCounter = { 3, 0, 1, 2 };

	const std::vector<int> sev_seed = { 1, 2, 3, 4, 5, 6, 7 };

	//SRS 時計回り 
	const std::vector<std::vector<std::pair<int, int>>> WallKick_clockW = {
		{ { 0, 0}, {-1, 0}, {-1,  1}, {0, -2}, {-1, -2} }, // 0>>1 
		{ { 0, 0}, { 1, 0}, { 1, -1}, {0,  2}, { 1,  2} }, // 1>>2 
		{ { 0, 0}, { 1, 0}, { 1,  1}, {0, -2}, { 1, -2} }, // 2>>3 
		{ { 0, 0}, {-1, 0}, {-1, -1}, {0,  2}, {-1,  2} }, // 3>>0 
	};

	//SRS 反時計回り 
	const std::vector<std::vector<std::pair<int, int>>> WallKick_counterW = {
		{ { 0, 0}, { 1, 0}, { 1,  1}, {0, -2}, { 1, -2} }, // 0>>3 
		{ { 0, 0}, { 1, 0}, { 1, -1}, {0,  2}, { 1,  2} }, // 1>>0 
		{ { 0, 0}, {-1, 0}, {-1,  1}, {0, -2}, {-1, -2} }, // 2>>1 
		{ { 0, 0}, {-1, 0}, {-1, -1}, {0,  2}, {-1,  2} }, // 3>>2 
	};

	//SRS 時計回りI  
	const std::vector<std::vector<std::pair<int, int>>> WallKick_clockI = {
		{ { 0, 0}, {-2, 0}, { 1,  0}, {-2, -1}, { 1,  2} }, // 0>>1 
		{ { 0, 0}, {-1, 0}, { 2,  0}, {-1,  2}, { 2, -1} }, // 1>>2 
		{ { 0, 0}, { 2, 0}, {-1,  0}, { 2,  1}, {-1, -2} }, // 2>>3 
		{ { 0, 0}, { 1, 0}, {-2,  0}, { 1, -2}, {-2,  1} }, // 3>>0 
	};

	//SRS 反時計回りI 
	const std::vector<std::vector<std::pair<int, int>>> WallKick_counterI = {
		{ { 0, 0}, {-1, 0}, { 2,  0}, {-1,  2}, { 2, -1} }, // 0>>3 
		{ { 0, 0}, { 2, 0}, {-1,  0}, { 2,  1}, {-1, -2} }, // 1>>0 
		{ { 0, 0}, { 1, 0}, {-2,  0}, { 1, -2}, {-2,  1} }, // 2>>1 
		{ { 0, 0}, {-2, 0}, { 1,  0}, {-2, -1}, { 1,  2} }, // 3>>2 
	};

	const std::vector<int> garbage = { 8, 8, 8, 8, 8, 8, 8, 8, 8, 8 };

	const std::vector<std::vector<int>> fire_list = {
		{ 0, 0, 1, 2, 4 },
		{ 0, 2, 4, 6, 0 },
		{ 0, 0, 1, 2, 0 },
	};

	const std::vector<int> combo_fire = { 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 4, 5};

	/*static const std::vector<std::vector<int>> baseField = {

		{0, 0, 0, 0, 0, 0, 0 ,0, 0 ,0},
		{0, 0, 0, 0, 0, 0, 0 ,0, 0 ,0},
		{0, 0, 0, 0, 0, 0, 0 ,0, 0 ,0},
		{0, 0, 0, 0, 0, 0, 0 ,0, 0 ,0},
		{0, 0, 0, 0, 0, 0, 0 ,0, 0 ,0},
		{0, 0, 0, 0, 0, 0, 0 ,0, 0 ,0},
		{0, 0, 0, 0, 0, 0, 0 ,0, 0 ,0},
		{0, 0, 0, 0, 0, 0, 0 ,0, 0 ,0},
		{0, 0, 0, 0, 0, 0, 0 ,0, 0 ,0},
		{0, 0, 0, 0, 0, 0, 0 ,0, 0 ,0},
		{0, 0, 0, 0, 0, 0, 0 ,0, 0 ,0},
		{0, 0, 0, 0, 0, 0, 0 ,0, 0 ,0},
		{0, 0, 0, 0, 0, 0, 0 ,0, 0 ,0},
		{0, 0, 0, 0, 0, 0, 0 ,0, 0 ,0},
		{0, 0, 0, 0, 0, 0, 0 ,0, 0 ,0},
		{0, 0, 0, 0, 0, 0, 0 ,0, 0 ,0},
		{0, 0, 0, 0, 0, 0, 0 ,0, 0 ,0},
		{0, 0, 0, 0, 0, 0, 0 ,0, 0 ,0},
		{0, 0, 0, 0, 0, 0, 0 ,0, 0 ,0},
		{0, 0, 0, 0, 0, 0, 0 ,0, 0 ,0},
		{0, 0, 0, 0, 0, 0, 0 ,0, 0 ,0},
		{0, 0, 0, 0, 0, 0, 0 ,0, 0 ,0},
		{0, 0, 0, 0, 0, 0, 0 ,0, 0 ,0},
		{0, 0, 0, 0, 0, 0, 0 ,0, 0 ,0},
		{0, 0, 0, 0, 0, 0, 0 ,0, 0 ,0},
		{0, 0, 0, 0, 0, 0, 0 ,0, 0 ,0},
		{0, 0, 0, 0, 0, 0, 0 ,0, 0 ,0},
		{0, 0, 0, 0, 0, 0, 0 ,0, 0 ,0},
		{0, 0, 0, 0, 0, 0, 0 ,0, 0 ,0},
		{0, 0, 0, 0, 0, 0, 0 ,0, 0 ,0},
		{0, 0, 0, 0, 0, 0, 0 ,0, 0 ,0},
		{0, 0, 0, 0, 0, 0, 0 ,0, 0 ,0},
		{0, 0, 0, 0, 0, 0, 0 ,0, 0 ,0},
		{0, 0, 0, 0, 0, 0, 0 ,0, 0 ,0},
		{0, 0, 0, 0, 0, 0, 0 ,0, 0 ,0},
		{0, 0, 0, 0, 0, 0, 0 ,0, 0 ,0},
		{0, 0, 0, 0, 0, 0, 0 ,0, 0 ,0},
		{0, 0, 0, 0, 0, 0, 0 ,0, 0 ,0},
		{0, 0, 0, 0, 0, 0, 0 ,0, 0 ,0},
		{0, 0, 0, 0, 0, 0, 0 ,0, 0 ,0},
		{0, 0, 0, 0, 0, 0, 0 ,0, 0 ,0},
		{0, 0, 0, 0, 0, 0, 0 ,0, 0 ,0},
		{0, 0, 0, 0, 0, 0, 0 ,0, 0 ,0},
		{0, 0, 0, 0, 0, 0, 0 ,0, 0 ,0},
		{0, 0, 0, 0, 0, 0, 0 ,0, 0 ,0},

	};*/

}



//
namespace shig {
	class TetriEngine {
	private:
		Tetri now_mino;
		std::vector<std::vector<int>> field;
		std::vector<std::vector<int>> p_field;
		std::vector<int> sev;
		deque<int> q_next;
		int hold;
		int current;
		int counter;
		int m_evn;
		int hold_kind;
		int hd_cnt;
		int ts_kind;
		int p_srs;
		int btb;
		int combo;
		int garbage_flag;
		int garbage_stack;
		int garbage_send;
		int delay_flame;
		bool hold_f;
		bool l_erasef;
		bool dead_f;

		std::vector<std::string> ts_state;
		deque<string> mino_his;

	public:
		int id;

		TetriEngine();
		TetriEngine(int _id);
		bool Init(int _id);
	private:
		void AddNextQue(deque<int>& que, std::vector<int>& sev);
		int get_rnd(int l, int r);
		bool get_TF(double p);
		int shig_floorI(int i, int target);
		int shig_ceilI(int i, int target);
		bool AddGarbage(int line);
		bool make_garbage(int X, int Y, bool sft);
		void tspin_check(int toX, int toY, Tetri& ts);
		bool CheckMove(int toX, int toY);
		bool CheckMove(int toX, int toY, Tetri& check);
		int  NextRotate(int n_rot, Rotate rt);
		void SRS_rot(int lr);
		void SRS_Clockwise();
		void SRS_CounterClockwise();
	public:
		void PrintMino(int p);
		void PrintGhost(int p);
		std::set<int> erase_check();
		int EraseLine();
		bool CheckPC();
	private:
		void act_hold(int& hold, int& current, int& m_env);
		void act_soft();
		void act_hard();
		void ghost();
		void act_left();
		void act_right();
		void act_rotL();
		void act_rotR();
		void run();
	public:
		void SetField();
		void CopyFiledP();
		void ResetFieldP();
		int Game(int action, int evn);
		bool Advance();
		bool StackGarbage(int line);// line>0で指定値、line==0で乱数増加
		int getGarbage();
		//void set_KeyInput();

		// 読み取り関数
	public:
		int get_field_state(int i, int j, int m) const;
		std::vector<std::vector<int>> GetField() const;
		bool GetField(std::vector<std::vector<int>>& _field) const;
		pair<int, deque<int>> get_mino_state() const;
		Tetri get_current() const;
		int get_delayF() const;
		void edit_garbage_cmd(int i);
		std::vector<int> get_game_state() const;
		std::vector<std::string> get_ts_state() const;
		pair<int, int> get_combo() const;
		deque<string> get_mino_his() const;

		~TetriEngine();
	};
}

template <typename T>
int binaryS(const vector<T>& v, const T key);

bool vfind_sorted(const vector<int>& v, const int key);

