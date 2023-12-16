#pragma once

#include "shig_utility.h"
#include "tetri.h"
#include "KeyConf.h"

const int fh = 45;

// 乱数生成用 
static std::random_device rd;
static std::mt19937 Rengine(rd());

//
namespace shig {
	class TetriEngine {
	private:
		Tetri now_mino;
		VVI field, p_field;
		VI sev;
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
		int garbage_cmd;
		int delay_flame;

		bool hold_f;
		bool l_erasef;
		bool dead_f;

		VS ts_state;
		deque<string> mino_his;

	public:
		int id;

		TetriEngine();
		TetriEngine(int id);
		void add_next_que(deque<int>& que, VI& sev);
		int get_rnd(int l, int r);
		bool get_TF(double p);
		int shig_floorI(int i, int target);
		int shig_ceilI(int i, int target);
		bool add_garbage(int line);
		bool make_garbage(int X, int Y, bool sft);
		void tspin_check(int toX, int toY, Tetri& ts);
		bool move_check(int toX, int toY);
		bool move_check(int toX, int toY, Tetri& check);
		void SRS_rot(int lr);
		void print_mino(int p);
		void print_ghost(int p);
		set<int> erase_check();
		int line_erase();
		bool pc_check();
		void act_hold(int& hold, int& current, int& m_env);
		void act_soft();
		void act_hard();
		void ghost();
		void act_left();
		void act_right();
		void act_rotL();
		void act_rotR();
		void run();
		void set_field();
		void copy_pfield();
		void reset_pfield();
		int game(int action, int evn);
		bool do_advance();
		void set_KeyInput();

		// 読み取り関数
		int get_field_state(int i, int j, int m) const;
		pair<int, deque<int>> get_mino_state() const;
		Tetri get_current() const;
		int get_delayF() const;
		void edit_garbage_cmd(int i);
		VI get_game_state() const;
		VS get_ts_state() const;
		pair<int, int> get_combo() const;
		deque<string> get_mino_his() const;

		~TetriEngine();
	};

}

template <typename T>
int binaryS(const vector<T>& v, const T key);

bool vfind_sorted(const vector<int>& v, const int key);

