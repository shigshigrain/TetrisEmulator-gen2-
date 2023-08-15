
#include "tetris_engine.h"

static random_device rd;

template <typename T>
int binaryS(const vector<T> &v, const T key) {
    int ng = -1; int ok = (int)v.size(); int mid = -1;
    while (abs(ok - ng) > 1) {
        mid = (ok + ng) / 2;
        if ([&]() {return v.at(mid) >= key; }())ok = mid;
        else ng = mid;
    }
    return ok;
}

bool vfind_sorted(const vector<int> &v, const int key) {
    int itr = binaryS<int>(v, key);
    if (itr < 0 || itr >= v.size())return false;
    if (v.at(itr) == key)return true;
    else return false;
}

void TetriEngine::add_next_que(deque<int>&que, VI &_sev) {
    //VI sev = { 1, 2, 3, 4, 5, 6, 7 };
    //random_device seed_gen;
    mt19937 engine(rd());
    shuffle(_sev.begin(), _sev.end(), engine);
	for (auto i : _sev) {
		que.push_back(i);
	}
	return;
}

TetriEngine::TetriEngine() {
	TetriEngine::now_mino = Tetri();
	TetriEngine::id = 0;
	TetriEngine::sev = { 1,2,3,4,5,6,7 };
	TetriEngine::ts_state = vector <std::string>(2);
	set_field();
}

TetriEngine::TetriEngine(int id) {
	TetriEngine::now_mino = Tetri();
	TetriEngine::id = id;
	TetriEngine::sev = { 1,2,3,4,5,6,7 };
	TetriEngine::ts_state = vector <std::string>(2);
	set_field();
}

int TetriEngine::get_rnd(int l, int r) {
    //random_device randomD;
    //vector<uint32_t> randomSeedVec(10);
    //enerate(randomSeedVec.begin(), randomSeedVec.end(), ref(randomD));
    //seed_seq randomSeed(randomSeedVec.begin(), randomSeedVec.end());
    mt19937 engine(rd());
    uniform_int_distribution<int> randomIntDist(l, r);
    return randomIntDist(engine);
}

bool TetriEngine::get_TF(double p) {
    mt19937 engine(rd());
    //double p = (double)(num / den);
    bernoulli_distribution uid(p);
    return uid(engine);
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

bool TetriEngine::add_garbage(int line) {
    VVI pre_field(fh);
    VI garbage(10, 8);
    int g = get_rnd(0, 9);
    shig_rep(i, line) {
        if(!get_TF(0.7)) g = get_rnd(0, 9);
        if (g < 0 || g > 9)g = 0;
        pre_field[i] = garbage;
        pre_field[i][g] = 0;
    }
    shig_rep(i, fh - line) {
        if (i >= field.size())return false;
        pre_field[i + line] = field[i];
    }

    field = pre_field;

    return true;
}

bool TetriEngine::make_garbage(int X, int Y, bool sft) {
    VVI pre_field(fh);
    VI garbage(10, 8);
    if (sft) {
        shig_rep(i, Y) {
            pre_field[i] = field[i];
        }
        shig_rep(i, field.size() - Y - 1) {
            pre_field[i + Y + 1] = field[i + Y];
        }
    }
    field[Y] = garbage;
    field[Y][X] = 0;

    return true;
}

void TetriEngine::tspin_check(int toX, int toY, Tetri& ts) {
    if (ts.id != 6) {
        ts_kind = 0;
        return;
    }
    int ts_cnt = 0;
    int rot = ts.rot;

    VI testX = { 0, 0, 2, 2 };
    VI testY = { 0, 2, 0, 2 };

    pairI2 check = {0, 0};

    shig_rep(i, 4) {
        int sX = ts.X + testX[i] + toX;
        if (sX < 0 || sX >= 10) {
            ts_cnt++;
            continue;
        }
        int sY = ts.Y - testY[i] + toY - 1;
        if (sY < 0 || sY >= (field.size() - 1)) {
            ts_cnt++;
            continue;
        }
        if (field[sY][sX] != 0)ts_cnt++;
        else check = { testX[i], testY[i] };
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

bool TetriEngine::move_check(int toX, int toY) {
    int cnt = 0, cntt = 4;
	auto [rot, size, H, W] = getTS(now_mino);
    shig_rep(i, H) {
        shig_rep(j, W) {
            if (now_mino.mino[rot][i][j] != 0) {
                int sX = now_mino.X + j + toX;
                if (sX < 0 || sX >= 10)continue;
                int sY = now_mino.Y - i + toY;
                if (sY <= 0 || sY >= (field.size() - 1))continue;
                if (field[sY - 1LL][sX] == 0)cnt++;
            }
        }
    }
    if (cnt == cntt) return true;
    else return false;
}

bool TetriEngine::move_check(int toX, int toY, Tetri& check) {
    int cnt = 0, cntt = 4;
	auto [rot, size, H, W] = getTS(check);
    shig_rep(i, H) {
        shig_rep(j, W) {
            if (check.mino[rot][i][j] != 0) {
                int sX = check.X + j + toX;
                if (sX < 0 || sX >= 10)continue;
                int sY = check.Y - i + toY;
                if (sY <= 0 || sY >= (field.size() - 1))continue;
                if (field[sY - 1LL][sX] == 0)cnt++;
            }
        }
    }
    if (cnt == cntt) return true;
    else return false;
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
                if (move_check(0, 0, test)) {
                    p_srs = 0;
                }
                else if (move_check(-1, 0, test)) {
                    p_srs = 1;
                    to_X = -1;
                }
                else if (move_check(2, 0, test)) {
                    p_srs = 2;
                    to_X = 2;
                }
                else if (move_check(-1, 2, test)) {
                    p_srs = 3;
                    to_X = -1;
                    to_Y = 2;
                }
                else if (move_check(2, -1, test)) {
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
                if (move_check(0, 0, test)) {
                    p_srs = 0;
                }
                else if (move_check(-2, 0, test)) {
                    p_srs = 1;
                    to_X = -2;
                }
                else if (move_check(1, 0, test)) {
                    p_srs = 2;
                    to_X = 1;
                }
                else if (move_check(-2, -1, test)) {
                    p_srs = 3;
                    to_X = -2;
                    to_Y = -1;
                }
                else if (move_check(1, 2, test)) {
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
                if (move_check(0, 0, test)) {
                    p_srs = 0;
                }
                else if (move_check(2, 0, test)) {
                    p_srs = 1;
                    to_X = 2;
                }
                else if (move_check(-1, 0, test)) {
                    p_srs = 2;
                    to_X = -1;
                }
                else if (move_check(2, 1, test)) {
                    p_srs = 3;
                    to_X = 2;
                    to_Y = 1;
                }
                else if (move_check(-1, -2, test)) {
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
                if (move_check(0, 0, test)) {
                    p_srs = 0;
                }
                else if (move_check(-1, 0, test)) {
                    p_srs = 1;
                    to_X = -1;
                }
                else if (move_check(2, 0, test)) {
                    p_srs = 2;
                    to_X = 2;
                }
                else if (move_check(-1, 2, test)) {
                    to_X = -1;
                    to_Y = 2;
                }
                else if (move_check(2, -1, test)) {
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
                if (move_check(0, 0, test)) {
                    p_srs = 0;
                }
                else if (move_check(1, 0, test)) {
                    to_X = 1;
                    p_srs = 1;
                }
                else if (move_check(-2, 0, test)) {
                    to_X = -2;
                    p_srs = 2;
                }
                else if (move_check(1, -2, test)) {
                    to_X = 1;
                    to_Y = -2;
                    p_srs = 3;
                }
                else if (move_check(-2, 1, test)) {
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
                if (move_check(0, 0, test)) {
                    p_srs = 0;
                }
                else if (move_check(2, 0, test)) {
                    to_X = 2;
                    p_srs = 1;
                }
                else if (move_check(-1, 0, test)) {
                    to_X = -1;
                    p_srs = 2;
                }
                else if (move_check(2, 1, test)) {
                    to_X = 2;
                    to_Y = 1;
                    p_srs = 3;
                }
                else if (move_check(-1, -2, test)) {
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
                if (move_check(0, 0, test)) {
                    p_srs = 0;
                }
                else if (move_check(-2, 0, test)) {
                    to_X = -2;
                    p_srs = 1;
                }
                else if (move_check(1, 0, test)) {
                    to_X = 1;
                    p_srs = 2;
                }
                else if (move_check(-2, -1, test)) {
                    to_X = -2;
                    to_Y = -1;
                    p_srs = 3;
                }
                else if (move_check(1, 2, test)) {
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
                if (move_check(0, 0, test)) {
                    p_srs = 0;
                }
                else if (move_check(1, 0, test)) {
                    to_X = 1;
                    p_srs = 1;
                }
                else if (move_check(-2, 0, test)) {
                    to_X = -2;
                    p_srs = 2;
                }
                else if (move_check(1, -2, test)) {
                    to_X = 1;
                    to_Y = -2;
                    p_srs = 3;
                }
                else if (move_check(-2, 1, test)) {
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
                if (move_check(0, 0, test)) {
                    p_srs = 0;
                }
                else if (move_check(1, 0, test)) {
                    p_srs = 1;
                    to_X = 1;
                }
                else if (move_check(1, 1, test)) {
                    p_srs = 2;
                    to_X = 1;
                    to_Y = 1;
                }
                else if (move_check(0, -2, test)) {
                    p_srs = 3;
                    to_Y = -2;
                }
                else if (move_check(1, -2, test)) {
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
                if (move_check(0, 0, test)) {
                    p_srs = 0;
                }
                else if (move_check(-1, 0, test)) {
                    p_srs = 1;
                    to_X = -1;
                }
                else if (move_check(-1, 1, test)) {
                    p_srs = 2;
                    to_X = -1;
                    to_Y = 1;
                }
                else if (move_check(0, -2, test)) {
                    p_srs = 3;
                    to_Y = -2;
                }
                else if (move_check(-1, -2, test)) {
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
                if (move_check(0, 0, test)) {
                    p_srs = 0;
                }
                else if (move_check(1, 0, test)) {
                    p_srs = 1;
                    to_X = 1;
                }
                else if (move_check(1, -1, test)) {
                    p_srs = 2;
                    to_X = 1;
                    to_Y = -1;
                }
                else if (move_check(0, 2, test)) {
                    p_srs = 3;
                    to_Y = 2;
                }
                else if (move_check(1, 2, test)) {
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
                if (move_check(0, 0, test)) {
                    p_srs = 0;
                }
                else if (move_check(1, 0, test)) {
                    p_srs = 1;
                    to_X = 1;
                }
                else if (move_check(1, -1, test)) {
                    p_srs = 2;
                    to_X = 1;
                    to_Y = -1;
                }
                else if (move_check(0, 2, test)) {
                    p_srs = 3;
                    to_Y = 2;
                }
                else if (move_check(1, 2, test)) {
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
                if (move_check(0, 0, test)) {
                    p_srs = 0;
                }
                else if (move_check(-1, 0, test)) {
                    p_srs = 1;
                    to_X = -1;
                }
                else if (move_check(-1, 1, test)) {
                    p_srs = 2;
                    to_X = -1;
                    to_Y = 1;
                }
                else if (move_check(0, -2, test)) {
                    p_srs = 3;
                    to_Y = -2;
                }
                else if (move_check(-1, -2, test)) {
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
                if (move_check(0, 0, test)) {
                    p_srs = 0;
                }
                else if (move_check(1, 0, test)) {
                    p_srs = 1;
                    to_X = 1;
                }
                else if (move_check(1, 1, test)) {
                    p_srs = 2;
                    to_X = 1;
                    to_Y = 1;
                }
                else if (move_check(0, -2, test)) {
                    p_srs = 3;
                    to_Y = -2;
                }
                else if (move_check(1, -2, test)) {
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
                if (move_check(0, 0, test)) {
                    p_srs = 0;
                }
                else if (move_check(-1, 0, test)) {
                    p_srs = 1;
                    to_X = -1;
                }
                else if (move_check(-1, -1, test)) {
                    p_srs = 2;
                    to_X = -1;
                    to_Y = -1;
                }
                else if (move_check(0, 2, test)) {
                    p_srs = 3;
                    to_Y = 2;
                }
                else if (move_check(-1, 2, test)) {
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
                if (move_check(0, 0, test)) {
                    p_srs = 0;
                }
                else if (move_check(-1, 0, test)) {
                    p_srs = 1;
                    to_X = -1;
                }
                else if (move_check(-1, -1, test)) {
                    p_srs = 2;
                    to_X = -1;
                    to_Y = -1;
                }
                else if (move_check(0, 2, test)) {
                    p_srs = 3;
                    to_Y = 2;
                }
                else if (move_check(-1, 2, test)) {
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

void TetriEngine::print_mino(int p) {
	auto [rot, size, H, W] = getTS(now_mino);
    shig_rep(i, H) {
        shig_rep(j, W) {
            if (now_mino.mino[rot][i][j] != 0) {
                int sX = now_mino.X + j;
                int sY = now_mino.Y - i;
                if (sX < 0 || sX >= 10)continue;
                if (sY - 1 < 0 || sY - 1 > 45)continue;
                if (p == 1) {
                    field[sY - 1LL][sX] = now_mino.id;
                }
                else {
                    p_field[sY - 1LL][sX] = now_mino.id;
                }
            }
        }
    }
}

void TetriEngine::print_ghost(int p) {
    Tetri ghost_mino = now_mino;
    int sft = -1;
    while (move_check(0, sft, ghost_mino))ghost_mino.addY(sft);
	int gID = ghost_mino.id;
	auto [rot, size, H, W] = getTS(ghost_mino);
    shig_rep(i, H) {
        shig_rep(j, W) {
            if (ghost_mino.mino[rot][i][j] != 0) {
                int sX = ghost_mino.X + j;
                int sY = ghost_mino.Y - i;
                if (sX < 0 || sX >= 10)continue;
                if (sY - 1 < 0 || sY - 1 > 45)continue;
                if (p == 1) {
                    field[sY - 1LL][sX] = gID + 8;
                }
                else {
                    p_field[sY - 1LL][sX] = gID + 8;
                }
            }
        }
    }
}

set<int> TetriEngine::erase_check() {
    set<int> erase_itr;
    print_mino(2);
    int rot = now_mino.rot;
    int H = (int)now_mino.mino[rot].size();
    int pW = (int)field[0].size();

    shig_rep(i, H) {
        int sY = now_mino.Y - i;
        int itr = sY - 1;
        int cnt = 0;
        if (itr < 0 || itr >= fh)continue;
        shig_rep(j, pW) {
            if (field[itr][j] != 0) cnt++;
            else break;
        }
        if (cnt == pW) erase_itr.insert(itr);

    }
    int size_e = (int)erase_itr.size();
	if (size_e == 0) {
		delay_flame = 2;
	}else if (size_e == 1) {
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

int TetriEngine::line_erase() {
    set<int> itr = erase_check();
    VI empty = { 0, 0, 0, 0 ,0 ,0 ,0 ,0 ,0 ,0 };
    VVI proxy(0); proxy.reserve(45);
    VVI pre(0); pre.reserve(45);

    shig_rep(i, fh) {
        decltype(itr)::iterator it = itr.find(i);
        if (it == itr.end()) pre.push_back(field[i]);
        else pre.push_back(empty);
    }

    p_field = pre;
    if (itr.size() != 0) l_erasef = 1;
    

    shig_rep(i, fh) {
        decltype(itr)::iterator it = itr.find(i);
        if (it == itr.end()) proxy.push_back(field[i]);
    }
    while (proxy.size() < fh) proxy.push_back(empty);

    field = proxy;

    int s = (int)itr.size();

    if (s > 0 && ts_kind > 0) {
        combo++;
        btb++;
        if (s == 1)ts_state[0] = "T-spin Single";
        else if (s == 2)ts_state[0] = "T-spin Double";
        else if (s == 3)ts_state[0] = "T-spin Triple";

        if (ts_kind == 2)ts_state[1] = "mini";
        else ts_state[1] = "    ";
    }
    else if (s == 4) {
        combo++;
        btb++;
        ts_state[0] = "4-LINE-eraser";
        ts_state[1] = "    ";
    }
    else if(s > 0){
        combo++;
        btb = -1;
        ts_state[0] = "    ";
        ts_state[1] = "    ";
    }
    else {
        combo = 0;
    }
    

    return s;
}

bool TetriEngine::pc_check() {
    int cnt = 0;
    shig_rep(i, 10) {
        cnt += field[0][i];
    }
    if (cnt > 0)return false;
    else {
        delay_flame = 0;
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
    int sft = -1;
    if (move_check(0, sft)) {
        now_mino.addY(sft);
        ts_kind = 0;
    }
    delay_flame = 2;
    return;
}

void TetriEngine::act_hard() {
    int sft = -1;
    hd_cnt = 0;
    while (move_check(0, sft)) {
        now_mino.addY(sft);
        hd_cnt++;
    }
    if (move_check(0, 0))print_mino(1);

    mino_his.push_front(now_mino.get_mino_str());
    if (mino_his.size() > 7)mino_his.pop_back();
    line_erase();
    pc_check();
    if (garbage_cmd == 1) add_garbage(get_rnd(1, 6));
    garbage_cmd = 0;

    return;
}

void TetriEngine::ghost() {
    int sft = -1;
    while (move_check(0, sft))now_mino.addY(sft);
    if (move_check(0, 0))print_mino(1);
    line_erase();
    return;
}

void TetriEngine::act_left() {
    int sft = -1;
    if (move_check(sft, 0)) {
        now_mino.addX(sft);
        ts_kind = 0;
    }
    delay_flame = 2;
    return;
}

void TetriEngine::act_right() {
    int sft = 1;
    if (move_check(sft, 0)) {
        now_mino.addX(sft);
        ts_kind = 0;
    }
    delay_flame = 2;
    return;
}

void TetriEngine::act_rotL() {
    SRS_rot(-1);
    delay_flame = 2;
    return;
}

void TetriEngine::act_rotR() {
    SRS_rot(1);
    delay_flame = 2;
    return;
}

void TetriEngine::run() {
    return;
}

void TetriEngine::set_field() {

    //shig_rep(i, fh)field[i] = { 0, 0, 0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 };
    //shig_rep(i, fh)p_field[i] = { 0, 0, 0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 };
	field = vector<vector<int>>(fh, vector<int>(10, 0));
	p_field = vector<vector<int>>(fh, vector<int>(10, 0));
    //while (!q_next.empty())q_next.pop_front();
	q_next.clear();
    add_next_que(q_next, sev);
    current = q_next.front();
    q_next.pop_front();
	now_mino = Tetri(current);
    //now_mino.set_mino(current);
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
    garbage_cmd = 0;
	dead_f = false;
    ts_state[0] = "             ";
    ts_state[1] = "    ";
    mino_his.clear();
    return;
}

void TetriEngine::copy_pfield() {
    p_field = field;
    print_ghost(2);
    print_mino(2);
    return;
}

void TetriEngine::reset_pfield() {
    p_field = field;
    return;
}

int TetriEngine::game(int action, int evn) {

	switch (action)
	{
	case 1:
		if (hold_f == 0) {
			act_hold(hold, current, m_evn);
			hold_f = true;
			dead_f = do_advance();
		}
		break;

	case 2:
		act_soft();
		break;

	case 3:
		act_hard();
		hold_f = false;
		m_evn = -1;
		dead_f = do_advance();
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

    //if (m_evn == 1) {
    //    if (action == 1) {//hold action
    //        if (hold_f == 0) {
    //            act_hold(hold, current, m_evn);
    //            hold_f = 1;
    //        }
    //    }
    //    else if (action == 2) {//soft drop
    //        act_soft();
    //    }
    //    else if (action == 3) {//hard drop
    //        act_hard();
    //        hold_f = 0;
    //        m_evn = -1;
    //    }
    //    else if (action == 4) {//left rot Z
    //        act_rotL();
    //    }
    //    else if (action == 5) {//right rot up
    //        act_rotR();
    //    }
    //    else if (action == 6) {//left move
    //        act_left();
    //    }
    //    else if (action == 7) {//right move
    //        act_right();
    //    }
    //}

    /*if (m_evn == -1 || m_evn == -2) {
        if (q_next.size() <= 6)add_next_que(q_next, sev);
        if (m_evn == -1) {
            current = q_next.front();
            q_next.pop_front();
        }
        m_evn = 1;
        now_mino.set_mino(current);
        if (!(move_check(0, 0))) {
            return 1;
        }
    }
    if (l_erasef == 1) {
        l_erasef = 0;
        return 2;
    }*/
    
}

bool TetriEngine::do_advance() {
	if (q_next.size() <= 6)add_next_que(q_next, sev);
	if (m_evn == -1) {
		current = q_next.front();
		q_next.pop_front();
	}
	m_evn = 1;
	now_mino.set_mino(current);
	if (!(move_check(0, 0))) {
		return true;
	}
	return false;
}

int TetriEngine::get_field_state(int i, int j, int m) const {
    
    if(m == 0) return p_field[i][j];
    else if(m == 1) return field[i][j];
    return 0;
}

pair<int, deque<int>> TetriEngine::get_mino_state() const {
    return make_pair(hold, q_next);
}

Tetri TetriEngine::get_current() const {
    return now_mino;
}

int TetriEngine::get_delayF() const {
    return delay_flame;
}

void TetriEngine::edit_garbage_cmd(int i) {
    garbage_cmd = i;
    return;
}

VI TetriEngine::get_game_state() const {
    VI state(13);
    state[0] = hold_f;
    state[1] = hold;
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

VS TetriEngine::get_ts_state() const {
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




