
#include "tetris_engine.hpp"

// 
template <typename T>
int binaryS(const vector<T>& v, const T key) {
	int ng = -1; int ok = (int)v.size(); int mid = -1;
	while (abs(ok - ng) > 1) {
		mid = (ok + ng) / 2;
		if ([&]() {return v.at(mid) >= key; }())ok = mid;
		else ng = mid;
	}
	return ok;
}

bool vfind_sorted(const vector<int>& v, const int key) {
	int itr = binaryS<int>(v, key);
	if (itr < 0 || itr >= v.size())return false;
	if (v.at(itr) == key)return true;
	else return false;
}

namespace shig {

	void TetriEngine::AddNextQue(std::deque<int>& que, std::vector<int>& _sev) {
		std::shuffle(_sev.begin(), _sev.end(), Rengine);
		for (const auto ni : _sev) {
			que.push_back(ni);
		}
		return;
	}

	TetriEngine::TetriEngine() {
		now_mino = Tetri();
		id = 0;
		sev = { 1,2,3,4,5,6,7 };
		ts_state = std::vector<std::string>(2, "");
		mino_his = std::deque<std::string>(0);
		field = std::vector<std::vector<int>>(45, (std::vector<int>(10, 0)));
		p_field = std::vector<std::vector<int>>(45, (std::vector<int>(10, 0)));
		//SetField();
		current = 0;
		hold_f = 0;
		hold = 0;
		counter = 0;
		m_evn = 1;
		hold_kind = 0;
		hd_cnt = 0;
		delay_flame = 0;
		ts_kind = 0;
		p_srs = -1;
		btb = -1;
		combo = 0;
		garbage_flag = 0;
		garbage_stack = 0;
		garbage_send = 0;
		l_erasef = false;
		dead_f = false;
	}

	TetriEngine::TetriEngine(int _id) {
		now_mino = Tetri();
		id = _id;
		sev = { 1,2,3,4,5,6,7 };
		ts_state = std::vector<std::string>(2);
		mino_his = std::deque<std::string>(0);
		field = std::vector<std::vector<int>>(45, (std::vector<int>(10, 0)));
		p_field = std::vector<std::vector<int>>(45, (std::vector<int>(10, 0)));
		//SetField();
		current = 0;
		hold_f = 0;
		hold = 0;
		counter = 0;
		m_evn = 1;
		hold_kind = 0;
		hd_cnt = 0;
		delay_flame = 0;
		ts_kind = 0;
		p_srs = -1;
		btb = -1;
		combo = 0;
		garbage_flag = 0;
		garbage_stack = 0;
		garbage_send = 0;
		l_erasef = false;
		dead_f = false;
	}

	bool TetriEngine::Init(int _id)
	{
		id = _id;
		SetField();
		return true;
	}

	int TetriEngine::get_rnd(int l, int r) {
		uniform_int_distribution<int> randomIntDist(l, r);
		return randomIntDist(Rengine);
	}

	bool TetriEngine::get_TF(double p) {
		bernoulli_distribution uid(p);
		return uid(Rengine);
	}

	int TetriEngine::shig_floorI(int i, int target) {
		target = abs(target);
		int flr = 0;
		if (i >= 0) {
			flr = i - i % target;
		}
		else {
			flr = i + (abs(i) % target);
			if (abs(i) % target != 0)flr -= target;
		}
		return flr;
	}

	int TetriEngine::shig_ceilI(int i, int target) {
		target = abs(target);
		int ceil = 0;
		if (i >= 0) {
			ceil = i - i % target;
			if (i % target != 0)ceil += target;
		}
		else {
			ceil = i + (abs(i) % target);
		}
		return ceil;
	}

	bool TetriEngine::AddGarbage(int line) {
		std::vector<std::vector<int>> pre_field(fh);
		int g = get_rnd(0, 9);
		shig_rep(i, line) {
			if (!get_TF(0.7)) g = get_rnd(0, 9);
			pre_field.at(i) = garbage;
			pre_field.at(i).at(g) = 0;
		}
		shig_rep(i, fh - line) {
			if (i >= field.size())return false;
			pre_field.at((size_t)i + line) = field.at(i);
		}
		field = pre_field;

		return true;
	}

	bool TetriEngine::make_garbage(int X, int Y, bool sft) {
		std::vector<std::vector<int>> pre_field(fh);
		if (sft) {
			for (size_t i = 0; i < Y; ++i) {
				pre_field.at(i) = field.at(i);
			}
			for (size_t i = 0; i < (field.size() - Y - 1); ++i) {
				pre_field[i + Y + 1] = field[i + Y];
			}
		}
		field.at(Y) = garbage;
		field.at(Y).at(X) = 0;

		return true;
	}

	void TetriEngine::tspin_check(int toX, int toY, Tetri& ts) {
		if (ts.id != 6) {
			ts_kind = 0;
			return;
		}
		int ts_cnt = 0;
		int rot = ts.rot;

		std::vector<int> testX = { 0, 0, 2, 2 };
		std::vector<int> testY = { 0, 2, 0, 2 };

		pairI2 check = { 0, 0 };

		shig_rep(i, 4) {
			int sX = ts.X + testX.at(i) + toX;
			if (sX < 0 || sX >= 10) {
				ts_cnt++;
				continue;
			}
			int sY = ts.Y - testY.at(i) + toY - 1;
			if (sY < 0 || sY >= (field.size() - 1)) {
				ts_cnt++;
				continue;
			}
			if (field[sY].at(sX) != 0)ts_cnt++;
			else check = { testX.at(i), testY.at(i) };
		}

		if (ts_cnt == 3 && p_srs != 4) {
			if (rot == 0) {
				if (check.first == 0 && check.second == 0)ts_kind = 2;
				else if (check.first == 2 && check.second == 0)ts_kind = 2;
				else ts_kind = 1;
			}
			else if (rot == 1) {
				if (check.first == 2 && check.second == 0)ts_kind = 2;
				else if (check.first == 2 && check.second == 2)ts_kind = 2;
				else ts_kind = 1;
			}
			else if (rot == 2) {
				if (check.first == 2 && check.second == 2)ts_kind = 2;
				else if (check.first == 0 && check.second == 2)ts_kind = 2;
				else ts_kind = 1;
			}
			else if (rot == 3) {
				if (check.first == 0 && check.second == 2)ts_kind = 2;
				else if (check.first == 0 && check.second == 0)ts_kind = 2;
				else ts_kind = 1;
			}
		}
		else if (ts_cnt == 3 || ts_cnt == 4)ts_kind = 1;//normal
		else ts_kind = 0;

		return;
	}

	bool TetriEngine::CheckMove(int toX, int toY) {
		int cnt = 0, cntt = 4;
		const auto& [rot, size, H, W] = getTS(now_mino);
		shig_rep(i, H) {
			shig_rep(j, W) {
				if (now_mino.mino.at(rot).at(i).at(j) != 0) {
					int sX = now_mino.X + j + toX;
					if (sX < 0 || sX >= 10)continue;
					int sY = now_mino.Y - i + toY;
					if (sY <= 0 || sY >= (field.size() - 1))continue;
					if (field.at(sY - 1LL).at(sX) == 0)cnt++;
				}
			}
		}
		if (cnt == cntt) return true;
		else return false;
	}

	bool TetriEngine::CheckMove(int toX, int toY, Tetri& check) {
		int cnt = 0, cntt = 4;
		const auto& [rot, size, H, W] = getTS(check);
		shig_rep(i, H) {
			shig_rep(j, W) {
				if (check.mino.at(rot).at(i).at(j) != 0) {
					int sX = check.X + j + toX;
					if (sX < 0 || sX >= 10)continue;
					int sY = check.Y - i + toY;
					if (sY <= 0 || sY >= (field.size() - 1))continue;
					if (field.at(sY - 1LL).at(sX) == 0)cnt++;
				}
			}
		}
		if (cnt == cntt) return true;
		else return false;
	}

	int TetriEngine::NextRotate(int n_rot, Rotate rt)
	{
		switch (rt)
		{
		case shig::Clockwise:
			return next_rotateClock.at(n_rot);
			break;
		case shig::CounterClockwise:
			return next_rotateCounter.at(n_rot);
			break;
		default:
			return 0;
			break;
		}
	}

	void TetriEngine::SRS_rot(int lr) {//l=-1 r=1
		Tetri test = now_mino;
		int to_X = 0, to_Y = 0, rot = test.rot;
		bool can = true;
		if (test.id == 1) {//I-mino SRS
			if (test.rot == 0) {
				if (lr == -1) {
					test.set_rot(3);
					rot = 3;
					if (CheckMove(0, 0, test)) {
						p_srs = 0;
					}
					else if (CheckMove(-1, 0, test)) {
						p_srs = 1;
						to_X = -1;
					}
					else if (CheckMove(2, 0, test)) {
						p_srs = 2;
						to_X = 2;
					}
					else if (CheckMove(-1, 2, test)) {
						p_srs = 3;
						to_X = -1;
						to_Y = 2;
					}
					else if (CheckMove(2, -1, test)) {
						p_srs = 4;
						to_X = 2;
						to_Y = -1;
					}
					else {
						p_srs = -1;
						can = false;
					}
				}
				else if (lr == 1) {
					test.set_rot(1);
					rot = 1;
					if (CheckMove(0, 0, test)) {
						p_srs = 0;
					}
					else if (CheckMove(-2, 0, test)) {
						p_srs = 1;
						to_X = -2;
					}
					else if (CheckMove(1, 0, test)) {
						p_srs = 2;
						to_X = 1;
					}
					else if (CheckMove(-2, -1, test)) {
						p_srs = 3;
						to_X = -2;
						to_Y = -1;
					}
					else if (CheckMove(1, 2, test)) {
						p_srs = 4;
						to_X = 1;
						to_Y = 2;
					}
					else {
						p_srs = -1;
						can = false;
					}
				}
			}
			else if (test.rot == 1) {
				if (lr == -1) {
					test.set_rot(0);
					rot = 0;
					if (CheckMove(0, 0, test)) {
						p_srs = 0;
					}
					else if (CheckMove(2, 0, test)) {
						p_srs = 1;
						to_X = 2;
					}
					else if (CheckMove(-1, 0, test)) {
						p_srs = 2;
						to_X = -1;
					}
					else if (CheckMove(2, 1, test)) {
						p_srs = 3;
						to_X = 2;
						to_Y = 1;
					}
					else if (CheckMove(-1, -2, test)) {
						p_srs = 4;
						to_X = -1;
						to_Y = -2;
					}
					else {
						p_srs = -1;
						can = false;
					}
				}
				else if (lr == 1) {
					test.set_rot(2);
					rot = 2;
					if (CheckMove(0, 0, test)) {
						p_srs = 0;
					}
					else if (CheckMove(-1, 0, test)) {
						p_srs = 1;
						to_X = -1;
					}
					else if (CheckMove(2, 0, test)) {
						p_srs = 2;
						to_X = 2;
					}
					else if (CheckMove(-1, 2, test)) {
						to_X = -1;
						to_Y = 2;
					}
					else if (CheckMove(2, -1, test)) {
						to_X = 2;
						to_Y = -1;
					}
					else {
						p_srs = -1;
						can = false;
					}
				}
			}
			else if (test.rot == 2) {
				if (lr == -1) {
					test.set_rot(1);
					rot = 1;
					if (CheckMove(0, 0, test)) {
						p_srs = 0;
					}
					else if (CheckMove(1, 0, test)) {
						to_X = 1;
						p_srs = 1;
					}
					else if (CheckMove(-2, 0, test)) {
						to_X = -2;
						p_srs = 2;
					}
					else if (CheckMove(1, -2, test)) {
						to_X = 1;
						to_Y = -2;
						p_srs = 3;
					}
					else if (CheckMove(-2, 1, test)) {
						to_X = -2;
						to_Y = 1;
						p_srs = 4;
					}
					else {
						p_srs = -1;
						can = false;
					}
				}
				else if (lr == 1) {
					test.set_rot(3);
					rot = 3;
					if (CheckMove(0, 0, test)) {
						p_srs = 0;
					}
					else if (CheckMove(2, 0, test)) {
						to_X = 2;
						p_srs = 1;
					}
					else if (CheckMove(-1, 0, test)) {
						to_X = -1;
						p_srs = 2;
					}
					else if (CheckMove(2, 1, test)) {
						to_X = 2;
						to_Y = 1;
						p_srs = 3;
					}
					else if (CheckMove(-1, -2, test)) {
						to_X = -1;
						to_Y = -2;
						p_srs = 4;
					}
					else {
						p_srs = -1;
						can = false;
					}
				}
			}
			else if (test.rot == 3) {
				if (lr == -1) {
					test.set_rot(2);
					rot = 2;
					if (CheckMove(0, 0, test)) {
						p_srs = 0;
					}
					else if (CheckMove(-2, 0, test)) {
						to_X = -2;
						p_srs = 1;
					}
					else if (CheckMove(1, 0, test)) {
						to_X = 1;
						p_srs = 2;
					}
					else if (CheckMove(-2, -1, test)) {
						to_X = -2;
						to_Y = -1;
						p_srs = 3;
					}
					else if (CheckMove(1, 2, test)) {
						to_X = 1;
						to_Y = 2;
						p_srs = 4;
					}
					else {
						p_srs = -1;
						can = false;
					}
				}
				else if (lr == 1) {
					test.set_rot(0);
					rot = 0;
					if (CheckMove(0, 0, test)) {
						p_srs = 0;
					}
					else if (CheckMove(1, 0, test)) {
						to_X = 1;
						p_srs = 1;
					}
					else if (CheckMove(-2, 0, test)) {
						to_X = -2;
						p_srs = 2;
					}
					else if (CheckMove(1, -2, test)) {
						to_X = 1;
						to_Y = -2;
						p_srs = 3;
					}
					else if (CheckMove(-2, 1, test)) {
						to_X = -2;
						to_Y = 1;
						p_srs = 4;
					}
					else {
						p_srs = -1;
						can = false;
					}
				}
			}
		}
		else {
			if (test.rot == 0) {
				if (lr == -1) {
					test.set_rot(3);
					rot = 3;
					if (CheckMove(0, 0, test)) {
						p_srs = 0;
					}
					else if (CheckMove(1, 0, test)) {
						p_srs = 1;
						to_X = 1;
					}
					else if (CheckMove(1, 1, test)) {
						p_srs = 2;
						to_X = 1;
						to_Y = 1;
					}
					else if (CheckMove(0, -2, test)) {
						p_srs = 3;
						to_Y = -2;
					}
					else if (CheckMove(1, -2, test)) {
						p_srs = 4;
						to_X = 1;
						to_Y = -2;
					}
					else {
						p_srs = -1;
						can = false;
					}
				}
				else if (lr == 1) {
					test.set_rot(1);
					rot = 1;
					if (CheckMove(0, 0, test)) {
						p_srs = 0;
					}
					else if (CheckMove(-1, 0, test)) {
						p_srs = 1;
						to_X = -1;
					}
					else if (CheckMove(-1, 1, test)) {
						p_srs = 2;
						to_X = -1;
						to_Y = 1;
					}
					else if (CheckMove(0, -2, test)) {
						p_srs = 3;
						to_Y = -2;
					}
					else if (CheckMove(-1, -2, test)) {
						p_srs = 4;
						to_X = -1;
						to_Y = -2;
					}
					else {
						p_srs = -1;
						can = false;
					}
				}
			}
			else if (test.rot == 1) {
				if (lr == -1) {
					test.set_rot(0);
					rot = 0;
					if (CheckMove(0, 0, test)) {
						p_srs = 0;
					}
					else if (CheckMove(1, 0, test)) {
						p_srs = 1;
						to_X = 1;
					}
					else if (CheckMove(1, -1, test)) {
						p_srs = 2;
						to_X = 1;
						to_Y = -1;
					}
					else if (CheckMove(0, 2, test)) {
						p_srs = 3;
						to_Y = 2;
					}
					else if (CheckMove(1, 2, test)) {
						p_srs = 4;
						to_X = 1;
						to_Y = 2;
					}
					else {
						p_srs = -1;
						can = false;
					}
				}
				else if (lr == 1) {
					test.set_rot(2);
					rot = 2;
					if (CheckMove(0, 0, test)) {
						p_srs = 0;
					}
					else if (CheckMove(1, 0, test)) {
						p_srs = 1;
						to_X = 1;
					}
					else if (CheckMove(1, -1, test)) {
						p_srs = 2;
						to_X = 1;
						to_Y = -1;
					}
					else if (CheckMove(0, 2, test)) {
						p_srs = 3;
						to_Y = 2;
					}
					else if (CheckMove(1, 2, test)) {
						p_srs = 4;
						to_X = 1;
						to_Y = 2;
					}
					else {
						p_srs = -1;
						can = false;
					}
				}
			}
			else if (test.rot == 2) {
				if (lr == -1) {
					test.set_rot(1);
					rot = 1;
					if (CheckMove(0, 0, test)) {
						p_srs = 0;
					}
					else if (CheckMove(-1, 0, test)) {
						p_srs = 1;
						to_X = -1;
					}
					else if (CheckMove(-1, 1, test)) {
						p_srs = 2;
						to_X = -1;
						to_Y = 1;
					}
					else if (CheckMove(0, -2, test)) {
						p_srs = 3;
						to_Y = -2;
					}
					else if (CheckMove(-1, -2, test)) {
						p_srs = 4;
						to_X = -1;
						to_Y = -2;
					}
					else {
						p_srs = -1;
						can = false;
					}
				}
				else if (lr == 1) {
					test.set_rot(3);
					rot = 3;
					if (CheckMove(0, 0, test)) {
						p_srs = 0;
					}
					else if (CheckMove(1, 0, test)) {
						p_srs = 1;
						to_X = 1;
					}
					else if (CheckMove(1, 1, test)) {
						p_srs = 2;
						to_X = 1;
						to_Y = 1;
					}
					else if (CheckMove(0, -2, test)) {
						p_srs = 3;
						to_Y = -2;
					}
					else if (CheckMove(1, -2, test)) {
						p_srs = 4;
						to_X = 1;
						to_Y = -2;
					}
					else {
						p_srs = -1;
						can = false;
					}
				}
			}
			else if (test.rot == 3) {
				if (lr == -1) {
					test.set_rot(2);
					rot = 2;
					if (CheckMove(0, 0, test)) {
						p_srs = 0;
					}
					else if (CheckMove(-1, 0, test)) {
						p_srs = 1;
						to_X = -1;
					}
					else if (CheckMove(-1, -1, test)) {
						p_srs = 2;
						to_X = -1;
						to_Y = -1;
					}
					else if (CheckMove(0, 2, test)) {
						p_srs = 3;
						to_Y = 2;
					}
					else if (CheckMove(-1, 2, test)) {
						p_srs = 4;
						to_X = -1;
						to_Y = 2;
					}
					else {
						p_srs = -1;
						can = false;
					}
				}
				else if (lr == 1) {
					test.set_rot(0);
					rot = 0;
					if (CheckMove(0, 0, test)) {
						p_srs = 0;
					}
					else if (CheckMove(-1, 0, test)) {
						p_srs = 1;
						to_X = -1;
					}
					else if (CheckMove(-1, -1, test)) {
						p_srs = 2;
						to_X = -1;
						to_Y = -1;
					}
					else if (CheckMove(0, 2, test)) {
						p_srs = 3;
						to_Y = 2;
					}
					else if (CheckMove(-1, 2, test)) {
						p_srs = 4;
						to_X = -1;
						to_Y = 2;
					}
					else {
						p_srs = -1;
						can = false;
					}
				}
			}
		}

		if (can) {
			tspin_check(to_X, to_Y, test);
			now_mino.set_rot(rot);
			now_mino.addX(to_X);
			now_mino.addY(to_Y);
		}

		return;
	}

	void TetriEngine::SRS_Clockwise()
	{
		Tetri test = now_mino;
		int to_X = 0, to_Y = 0, rot = test.rot;
		bool can = false;
		test.set_rot(NextRotate(rot, Rotate::Clockwise));
		if (test.id == 1) {
			int cnt = 0;
			for (const auto& tester : WallKick_clockI.at(rot)) {
				if (CheckMove(tester.first, tester.second, test)) {
					p_srs = cnt;
					to_X = tester.first;
					to_Y = tester.second;
					can = true;
					break;
				}
				else {
					cnt++;
				}
			}
		}
		else {
			int cnt = 0;
			for (const auto& tester : WallKick_clockW.at(rot)) {
				if (CheckMove(tester.first, tester.second, test)) {
					p_srs = cnt;
					to_X = tester.first;
					to_Y = tester.second;
					can = true;
					break;
				}
				else {
					cnt++;
				}
			}
		}

		if (can) {
			tspin_check(to_X, to_Y, test);
			now_mino.set_rot(test.rot);
			now_mino.addX(to_X);
			now_mino.addY(to_Y);
		}

		return;

	}

	void TetriEngine::SRS_CounterClockwise()
	{
		Tetri test = now_mino;
		int to_X = 0, to_Y = 0, rot = test.rot;
		bool can = false;
		test.set_rot(NextRotate(rot, Rotate::CounterClockwise));
		if (test.id == 1) {
			int cnt = 0;
			for (const auto& tester : WallKick_counterI.at(rot)) {
				if (CheckMove(tester.first, tester.second, test)) {
					p_srs = cnt;
					to_X = tester.first;
					to_Y = tester.second;
					can = true;
					break;
				}
				else {
					cnt++;
				}
			}
		}
		else {
			int cnt = 0;
			for (const auto& tester : WallKick_counterW.at(rot)) {
				if (CheckMove(tester.first, tester.second, test)) {
					p_srs = cnt;
					to_X = tester.first;
					to_Y = tester.second;
					can = true;
					break;
				}
				else {
					cnt++;
				}
			}
		}

		if (can) {
			tspin_check(to_X, to_Y, test);
			now_mino.set_rot(test.rot);
			now_mino.addX(to_X);
			now_mino.addY(to_Y);
		}

		return;
	}

	void TetriEngine::PrintMino(int p) {
		const auto& [rot, size, H, W] = getTS(now_mino);
		shig_rep(i, H) {
			shig_rep(j, W) {
				if (now_mino.mino.at(rot).at(i).at(j) != 0) {
					int sX = now_mino.X + j;
					int sY = now_mino.Y - i;
					if (sX < 0 || sX >= 10)continue;
					if (sY - 1 < 0 || sY - 1 > 45)continue;
					if (p == 1) {
						field.at(sY - 1LL).at(sX) = now_mino.id;
					}
					else {
						p_field.at(sY - 1LL).at(sX) = now_mino.id;
					}
				}
			}
		}
	}

	void TetriEngine::PrintGhost(int p) {
		Tetri ghost_mino = now_mino;
		const int sft = -1;
		while (CheckMove(0, sft, ghost_mino))ghost_mino.addY(sft);
		int gID = ghost_mino.id;
		const auto& [rot, size, H, W] = getTS(ghost_mino);
		shig_rep(i, H) {
			shig_rep(j, W) {
				if (ghost_mino.mino.at(rot).at(i).at(j) != 0) {
					int sX = ghost_mino.X + j;
					int sY = ghost_mino.Y - i;
					if (sX < 0 || sX >= 10)continue;
					if (sY - 1 < 0 || sY - 1 > 45)continue;
					if (p == 1) {
						field.at(sY - 1LL).at(sX) = gID + 8;
					}
					else {
						p_field.at(sY - 1LL).at(sX) = gID + 8;
					}
				}
			}
		}
	}

	std::set<int> TetriEngine::erase_check() {
		std::set<int> erase_itr;
		PrintMino(2);
		int rot = now_mino.rot;
		int H = (int)now_mino.mino.at(rot).size();
		int pW = (int)field.at(0).size();

		shig_rep(i, H) {
			int sY = now_mino.Y - i;
			int itr = sY - 1;
			int cnt = 0;
			if (itr < 0 || itr >= fh)continue;
			shig_rep(j, pW) {
				if (field[itr].at(j) != 0) cnt++;
				else break;
			}
			if (cnt == pW) erase_itr.insert(itr);

		}
		int size_e = (int)erase_itr.size();
		if (size_e == 0) {
			delay_flame = 2;
		}
		else if (size_e == 1) {
			delay_flame = 40;
		}
		else if (size_e == 2 || size_e == 3) {
			delay_flame = 45;
		}
		else {
			delay_flame = 50;
		}
		return erase_itr;
	}

	int TetriEngine::EraseLine() {
		std::set<int> itr = erase_check();
		std::vector<std::vector<int>> proxy(0); proxy.reserve(45);
		std::vector<std::vector<int>> pre(0); pre.reserve(45);

		for (int i = 0; i < fh; ++i) {
			decltype(itr)::iterator it = itr.find(i);
			if (it == itr.end()) pre.push_back(field.at(i));
			else pre.push_back(W_seed);
		}

		//p_field = pre;
		for (size_t i = 0; i < fh; ++i) {
			p_field.at(i) = pre.at(i);
		}

		if (itr.size() != 0) l_erasef = 1;


		for (int i = 0; i < fh; ++i) {
			decltype(itr)::iterator it = itr.find(i);
			if (it == itr.end()) proxy.push_back(field.at(i));
		}
		while (proxy.size() < fh) proxy.push_back(W_seed);

		//field = proxy;
		for (size_t i = 0; i < fh; ++i) {
			field.at(i) = proxy.at(i);
		}

		proxy.clear();

		size_t s = itr.size();
		if (ts_kind < 0 || ts_kind > 3)ts_kind = 0;

		garbage_send += fire_list.at(ts_kind).at(s);

		if (s > 0 && ts_kind > 0) {
			combo++;
			btb++;
			if (btb >= 0)garbage_send += 1;
			if (s == 1) {
				ts_state.at(0) = "T-spin Single";
			}
			else if (s == 2) {
				ts_state.at(0) = "T-spin Double";
			}
			else if (s == 3) {
				ts_state.at(0) = "T-spin Triple";
			}

			if (ts_kind == 2) {
				ts_state.at(1) = "mini";
			}
			else {
				ts_state.at(1) = "    ";
			}

		}
		else if (s == 4) {
			if (btb >= 0)garbage_send += 1;
			combo++;
			btb++;
			ts_state.at(0) = "4-LINE-eraser";
			ts_state.at(1) = "    ";
		}
		else if (s > 0) {
			combo++;
			btb = -1;
			ts_state.at(0) = "    ";
			ts_state.at(1) = "    ";
		}
		else {
			combo = 0;
		}

		return s;
	}

	bool TetriEngine::CheckPC() {
		int cnt = 0;
		shig_rep(i, 10) {
			cnt += field.at(0).at(i);
		}
		if (cnt > 0)return false;
		else {
			delay_flame = 0;
			garbage_send += 10;
			return true;
		}
	}

	void TetriEngine::act_hold(int& hld, int& crt, int& evnM) {
		if (hld == 0) {
			delay_flame = 2 + 5;
			hld = crt;
			evnM = -1;
		}
		else {
			swap(hld, crt);
			evnM = -2;
			delay_flame = 2;
		}

		return;
	}

	void TetriEngine::act_soft() {
		const int sft = -1;
		if (CheckMove(0, sft)) {
			now_mino.addY(sft);
			ts_kind = 0;
		}
		delay_flame = 2;
		return;
	}

	void TetriEngine::act_hard() {
		const int sft = -1;
		hd_cnt = 0;
		while (CheckMove(0, sft)) {
			now_mino.addY(sft);
			hd_cnt++;
		}
		if (CheckMove(0, 0))PrintMino(1);

		mino_his.push_front(now_mino.get_mino_str());
		if ((int)mino_his.size() > 7)mino_his.pop_back();
		EraseLine();
		CheckPC();
		//if (garbage_flag == 1) AddGarbage(get_rnd(1, 6));
		//garbage_flag = 0;
		if (combo >= ((int)combo_fire.size() - 1)) {
			garbage_send += combo_fire.back();
		}
		else {
			combo = std::max(combo, 0);
			garbage_send += combo_fire.at(combo);
		}
		garbage_stack = std::max(garbage_stack, 0);
		garbage_stack -= garbage_send;
		garbage_send = -1 * garbage_stack;
		garbage_send = std::max(garbage_send, 0);
		garbage_stack = std::min(garbage_stack, 22);
		garbage_stack = std::max(garbage_stack, 0);
		AddGarbage(garbage_stack);
		garbage_stack = 0;

		return;
	}

	void TetriEngine::ghost() {
		const int sft = -1;
		while (CheckMove(0, sft))now_mino.addY(sft);
		if (CheckMove(0, 0))PrintMino(1);
		EraseLine();
		return;
	}

	void TetriEngine::act_left() {
		const int sft = -1;
		if (CheckMove(sft, 0)) {
			now_mino.addX(sft);
			ts_kind = 0;
		}
		delay_flame = 2;
		return;
	}

	void TetriEngine::act_right() {
		const int sft = 1;
		if (CheckMove(sft, 0)) {
			now_mino.addX(sft);
			ts_kind = 0;
		}
		delay_flame = 2;
		return;
	}

	void TetriEngine::act_rotL() {
		SRS_CounterClockwise();
		delay_flame = 2;
		return;
	}

	void TetriEngine::act_rotR() {
		SRS_Clockwise();
		delay_flame = 2;
		return;
	}

	void TetriEngine::run() {
		return;
	}

	void TetriEngine::SetField() {

		for (auto&& fH : field) for (auto&& fW : fH)fW = 0;
		for (auto&& fH : p_field)for (auto&& fW : fH)fW = 0;
		
		q_next.clear();
		AddNextQue(q_next, sev);
		current = q_next.front();
		q_next.pop_front();
		now_mino.SetMino(current);
		hold_f = 0;
		hold = 0;
		counter = 0;
		m_evn = 1;
		hold_kind = 0;
		hd_cnt = 0;
		delay_flame = 0;
		ts_kind = 0;
		p_srs = -1;
		btb = -1;
		combo = 0;
		garbage_flag = 0;
		garbage_stack = 0;
		l_erasef = false;
		dead_f = false;
		sev = sev_seed;
		//mino_his.clear();
		return;
	}

	void TetriEngine::CopyFiledP() {
		p_field = field;
		PrintGhost(2);
		PrintMino(2);
		return;
	}

	void TetriEngine::ResetFieldP() {
		p_field = field;
		return;
	}

	int TetriEngine::Game(int action, int evn) {

		switch (action)
		{
		case 1:
			if (hold_f == 0) {
				act_hold(hold, current, m_evn);
				hold_f = true;
				dead_f = Advance();
			}
			break;

		case 2:
			act_soft();
			break;

		case 3:
			act_hard();
			hold_f = false;
			m_evn = -1;
			dead_f = Advance();
			break;

		case 4:
			act_rotL();
			break;

		case 5:
			act_rotR();
			break;

		case 6:
			act_left();
			break;

		case 7:
			act_right();
			break;

		default:
			break;
		}

		if (dead_f)return 1;
		else if (l_erasef) {
			l_erasef = 0;
			return 2;
		}

		return 0;

	}

	bool TetriEngine::Advance() {
		if (q_next.size() <= 6)AddNextQue(q_next, sev);
		if (m_evn == -1) {
			current = q_next.front();
			q_next.pop_front();
		}
		m_evn = 1;
		now_mino.SetMino(current);
		if (!(CheckMove(0, 0))) {
			return true;
		}
		return false;
	}

	bool TetriEngine::StackGarbage(int line)
	{
		if (line >= 0) {
			garbage_stack += line;
		}
		else if(line < 0){
			garbage_stack += get_rnd(1, 3);
		}

		return true;
	}

	int TetriEngine::getGarbage()
	{
		int _gbg = garbage_send;
		garbage_send = 0;
		return _gbg;
	}

	int TetriEngine::get_field_state(int i, int j, int m) const {

		if (m == 0) return p_field.at(i).at(j);
		else if (m == 1) return field.at(i).at(j);
		return 0;
	}

	// fieldのコピーを返す 
	std::vector<std::vector<int>> TetriEngine::GetField() const
	{
		return field;
	}

	bool TetriEngine::GetField(std::vector<std::vector<int>>& _field) const
	{
		_field = field;
		return true;
	}

	std::pair<int, std::deque<int>> TetriEngine::get_mino_state() const {
		return make_pair(hold, q_next);
	}

	Tetri TetriEngine::get_current() const {
		return now_mino;
	}

	int TetriEngine::get_delayF() const {
		return delay_flame;
	}

	void TetriEngine::edit_garbage_cmd(int i) {
		garbage_flag = i;
		return;
	}

	std::vector<int> TetriEngine::get_game_state() const {
		std::vector<int> state(13);
		state.at(0) = hold_f;
		state.at(1) = hold;
		state[2] = current;
		state[3] = counter;
		state[4] = m_evn;
		state[5] = hold_kind;
		state[6] = hd_cnt;
		state[7] = l_erasef;
		state[8] = delay_flame;
		state[9] = ts_kind;
		state[10] = p_srs;
		state[11] = btb;
		state[12] = combo;
		return state;
	}

	std::vector<std::string> TetriEngine::get_ts_state() const {
		return ts_state;
	}

	pair<int, int> TetriEngine::get_combo() const {
		return { combo, btb };
	}

	deque<string> TetriEngine::get_mino_his() const {
		return mino_his;
	}

	TetriEngine::~TetriEngine() {

	}


}
