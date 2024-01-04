#include "shigune_AI.h"

static int i32_zero = 0;
static int i32_one = 1;
static int i32_minus_one = -1;
constexpr size_t branch_num = 40;//branch_num <= 30
const vector<size_t> expl_width = { 40, 40, 40, 30, 30, 30, 30, 25, 25, 25, 25, 10, 10, 10, 10 };
constexpr int cyc_num = 6;//up to 14

// thread分割数 
constexpr int thd_num = 5;


//using namespace shig;

namespace shig {

	AiShigune::AiShigune() {
		identifier = 0;
		hold_AI = 0;
		current_AI = 0;
		next_AI = std::vector<int>(0);
		q_next_AI = deque<int>(0);
		mind = -1;
		pc_cnt = 0;
		SRS_kind = 0;
		TS_kind = 0;
		combo = 0;
		btb = -1;
		height_sum = 0;
		height_mxm = 0;
		ttrp_size = 0;
		ttrp_ofsY = 0;
		exp_cyc_lim = cyc_num;
		ttrp_able = false;
		field_AI = std::vector<std::vector<int>>(fh, W_seed);
		p_field_AI = std::vector<std::vector<int>>(fh, W_seed);
		s_field_AI = std::vector<std::vector<int>>(fh, W_seed);
		height = std::vector<int>(10, 0);
		cmd_list = std::vector<int>(0);
		cmd_list.reserve(40);
		ttrp_name_list = std::vector<std::string>(0);
		ttrp_id_list = std::vector<int>(0);
		ttrp_list = std::vector<TetriPlate>(0);
		ttrp_bgnF = std::vector<int>(0, 0);
		select_ttrp.set_def();
		gc_slot = std::vector<GameContainer>(branch_num);
		now_gc = GameContainer();
		s_branch = std::vector<CmdPattern>(0);

	}

	AiShigune::AiShigune(int ii) {
        identifier = ii;
		hold_AI = 0;
		current_AI = 0;
        next_AI = std::vector<int>(0);
		q_next_AI = std::deque<int>(0);
		mind = -1;
		pc_cnt = 0;
        SRS_kind = 0;
        TS_kind = 0;
        combo = 0;
        btb = -1;
        height_sum = 0;
        height_mxm = 0;
        ttrp_size = 0;
        ttrp_ofsY = 0;
        exp_cyc_lim = cyc_num;
        ttrp_able = false;
		field_AI = std::vector<std::vector<int>>(45, W_seed);
		p_field_AI = std::vector<std::vector<int>>(45, W_seed);
		s_field_AI = std::vector<std::vector<int>>(45, W_seed);
        height = std::vector<int>(10, 0);
		cmd_list = std::vector<int>(0);  cmd_list.reserve(40);
		ttrp_name_list = std::vector<std::string>(0);
		ttrp_id_list = std::vector<int>(0, 0);
		ttrp_list = std::vector<TetriPlate>(0);
		ttrp_bgnF = std::vector<int>(0, 0);
		select_ttrp.set_def();
		gc_slot = std::vector<GameContainer>(branch_num);
		now_gc = GameContainer();
		s_branch = std::vector<CmdPattern>(0);

	}

	// TetriEngine をloadTEする 
	AiShigune::AiShigune(const TetriEngine& te) {
		identifier = te.id;
		hold_AI = 0;
		current_AI = 0;
		next_AI = std::vector<int>(0);
		q_next_AI = deque<int>();
		mind = -1;
		pc_cnt = 0;
		SRS_kind = 0;
		TS_kind = 0;
		combo = 0;
		btb = -1;
		height_sum = 0;
		height_mxm = 0;
		ttrp_size = 0;
		ttrp_ofsY = 0;
		exp_cyc_lim = cyc_num;
		ttrp_able = false;
		field_AI = std::vector<std::vector<int>>(45, W_seed);
		p_field_AI = std::vector<std::vector<int>>(45, W_seed);
		s_field_AI = std::vector<std::vector<int>>(45, W_seed);
		height = std::vector<int>(10, 0);
		cmd_list = std::vector<int>(0);
		cmd_list.reserve(40);
		ttrp_name_list = std::vector<std::string>(0);
		ttrp_id_list = std::vector<int>(0, 0);
		ttrp_list = std::vector<TetriPlate>(0);
		ttrp_bgnF = std::vector<int>(0, 0);
		select_ttrp.set_def();
		gc_slot = std::vector<GameContainer>(branch_num);
		now_gc = GameContainer();
		s_branch = std::vector<CmdPattern>(0);
		loadTE(te);
	}

	/*AiShigune::AiShigune(const AiShigune& copyAi)
	{
		TE = copyAi.TE;
		identifier = copyAi.identifier;
		mind = copyAi.mind;
		hold_AI = copyAi.hold_AI;
		current_AI = copyAi.current_AI;
		pc_cnt = copyAi.pc_cnt;
		SRS_kind = copyAi.SRS_kind;
		TS_kind = copyAi.TS_kind;
		combo = copyAi.combo;
		btb = copyAi.btb;
		height_sum = copyAi.height_sum;
		height_mxm = copyAi.height_mxm;
		ttrp_size = copyAi.ttrp_size;
		ttrp_ofsY = copyAi.ttrp_ofsY;
		exp_cyc_lim = copyAi.exp_cyc_lim;
		ttrp_able = copyAi.ttrp_able;
		next_AI = copyAi.next_AI;
		q_next_AI = copyAi.q_next_AI;
		field_AI = copyAi.field_AI;
		p_field_AI = copyAi.p_field_AI;
		s_field_AI = copyAi.s_field_AI;
		H_target = copyAi.H_target;
		W_target = copyAi.W_target;
		height = copyAi.height;
		cmd_list = copyAi.cmd_list;
		cp = copyAi.cp;
		ttrp_name_list = copyAi.ttrp_name_list;
		ttrp_id_list = copyAi.ttrp_id_list;
		ttrp_list = copyAi.ttrp_list;
		ttrp_bgnF = copyAi.ttrp_bgnF;
		select_ttrp = copyAi.select_ttrp;
		gc_slot = copyAi.gc_slot;
		now_gc = copyAi.now_gc;
		s_branch = copyAi.s_branch;
	}*/

	// let Ai think about the next move(return True or False)
	bool AiShigune::thinking() {
		SetupState();
		SetupField();
        strategy_mark();

        set_gc(now_gc);

		ChoiceExplore(now_gc);

		next_crr_check();

		Tetri s_pat = s_branch.front().pat;
		std::set<int> el = CheckErase(s_pat, now_gc);
		int els = (int)el.size();

		std::vector<int> mnL = make_order_list();
		
		AiShigune::ttrp_check(s_branch.front(), els, mnL);

		return true;
	}

	// get Ai's suggestion of move cmd (by {int, int, ...})
	std::vector<int> AiShigune::get_recent_cmd() {
		// hold : 1, soft : 2, hard : 3, L_rot : 4, R_rot : 5, l_move : 6, r_move : 7;
        std::vector<int> cmd = {3};

		Tetri s_pat = s_branch.front().pat;

		set<int> el = CheckErase(s_pat, now_gc);
		int els = (int)el.size();

		/*std::vector<std::vector<int>> proxy(0); proxy.reserve(45);
		shig_rep(i, 45) {
			decltype(el)::iterator it = el.find(i);
			if (it == el.end()) proxy.push_back(strategy_map.at(i));
		}
		while (proxy.size() < 45) proxy.push_back(ev_empty);
		strategy_map = proxy;*/

		std::vector<int> mnL = make_order_list();

		next_crr_check();

		AiShigune::ttrp_check(s_branch.front(), els, mnL);

		return s_branch.front().cmd_list;

	}

	bool AiShigune::makeAiSuggestion(){

		std::lock_guard<std::mutex> _lock{ AiMtx };

		for (auto&& fH : s_field_AI)for (auto&& fW : fH)fW = 0;
		//s_field_AI = std::vector<std::vector<int>>(45, W_seed);
		size_t ss = s_branch.size();
		//std::reverse(s_branch.begin(), s_branch.end());

		for (size_t i = 0; i < ss; i++) {
			ApplyMino(s_field_AI, s_branch.at(ss - i - 1).pat);
		}

		return true;
	}

	bool AiShigune::makeAiSuggestion(std::mutex& up)
	{

		std::lock_guard<std::mutex> _lock{ up };

		//s_field_AI = std::vector<std::vector<int>>(45, W_seed);
		size_t ss = s_branch.size();
		//std::reverse(s_branch.begin(), s_branch.end());

		for (size_t i = 0; i < ss; i++) {
			ApplyMino(s_field_AI, s_branch.at(ss - i - 1).pat);
		}

		return true;

	}

	vector<vector<int>> AiShigune::getSuggestionAi() const{

		//std::lock_guard<std::mutex> _lock{ AiMtx };
		//呼び出し側で制御
		return AiShigune::s_field_AI;
	}

	vector<vector<int>> AiShigune::getSuggestionAi(std::mutex& up)
	{
		std::lock_guard<std::mutex> _lock{ up };
		return AiShigune::s_field_AI;
	}

	void AiShigune::SetupField() {
		/*shig_rep(i, 45) {
			shig_rep(j, 10) {
				field_AI.at(i).at(j) = TE.get_field_state(i, j, 1);
                p_field_AI.at(i).at(j) = TE.get_field_state(i, j, 0);
			}
		}*/
		p_field_AI = field_AI;
        height_sum = 0;
        height_mxm = 0;
        shig_rep(j, 10) {
            int h = fh - 1;
            while (h >= 0) {
                if (field_AI.at(h).at(j) == 0) {
                    height.at(j) = h;
                    h--;
                }
                else {
                    break;
                }
            }
            h++;
            height_sum += h;
            height_mxm = max(height_mxm, h);
        }

		return;
	}

	void AiShigune::SetupState() {
		//current_AI = TE.get_current().id;
		//pair<int, deque<int>> temp = TE.get_mino_state();
		//hold_AI = temp.first;
		//q_next_AI = temp.second;
		next_AI.clear();
		for (auto&& _nn : q_next_AI) {
			next_AI.push_back(_nn);
		}
		/*pair<int, int> temp2 = TE.get_combo();
		combo = temp2.first; btb = temp2.second;*/
		return;
	}

	vector<int> AiShigune::make_order_list()
	{
		std::vector<int> mnl(8, 0);
		for (size_t i = 0; i < mnl.size(); i++)
		{
			mnl.at(i) = (int)i + 8;
		}

		mnl.at(current_AI) = 0;
		mnl.at(hold_AI) = -1;
		for (size_t i = 0; i < std::min((size_t)5, next_AI.size()); i++) {
			mnl.at(next_AI.at(i)) = (int)i + 1;
		}

		return mnl;
	}

	vector<int> AiShigune::make_order_list(GameContainer& gc)
	{
		std::vector<int> mnl(8, 0);
		for (size_t i = 0; i < mnl.size(); i++)
		{
			mnl.at(i) = (int)i + 8;
		}

		mnl.at(gc.current_AI) = 0;
		mnl.at(gc.hold_AI) = -1;
		for (size_t i = 0; i < std::min((size_t)5, gc.next_AI.size()); i++) {
			mnl.at(gc.next_AI.at(i)) = (int)i + 1;
		}

		return mnl;
	}

	bool AiShigune::strategy_mark() {

		if (CheckPC())bgn_strategy();

		return true;
	}

	bool AiShigune::CheckPC() {
		shig_rep(i, 10) {
			if (field_AI[0].at(i) != 0)return false;
		}
		return true;
	}

	void AiShigune::bgn_strategy() {
		// I J L O S T Z

        std::vector<int> mnL = make_order_list();
        bool all_TF = next_crr_check();
        
        if (all_TF) {
            ttrp_able = true;
        }
        else {
            ttrp_able = false;
        }

        if (ttrp_able) {

            shig_rep(i, ttrp_size) {

                if (ttrp_list.at(i).bgn_f != 0)continue;
                int tm_cnt = 0;
                shig_rep(j, ttrp_list.at(i).terms_num) {
                    int tL = ttrp_list.at(i).terms.at(j).first;
                    int tR = ttrp_list.at(i).terms.at(j).second;
                    if (mnL[tL] < mnL[tR])tm_cnt++;
                }

                if (tm_cnt >= ttrp_list.at(i).terms_num) {
                    select_ttrp = ttrp_list.at(i);
                    break;
                }

            }

        }
        
		return;
	}

    vector<CmdPattern> AiShigune::search_way(GameContainer gc, int loop) {
        
        constexpr int mxm = 200;
        
        vector<CmdPattern> pcv(0);
        
        if (gc.current_AI == 0)return pcv;
        gc.p_field_AI = gc.field_AI;
        std::vector<int> rsv(0); rsv.reserve(30);
        std::vector<std::vector<int>> search_tree(mxm, rsv);
        std::vector<int> parent_tree(mxm, 0);
        int to = 0;
        gc.cp.clear();

        shig_rep(i, base_cmd.size()) {
            search_tree.at(i) = base_cmd.at(i);
            parent_tree.at(i) = i;
            to++;
        }

        int w = 0;
        Tetri test;
        while (!search_tree[w].empty() && (w < mxm - 1)) {
            bool can = true;  
            if (parent_tree[w] == w) {
                test.SetMino(gc.current_AI);
                shig_rep(i, search_tree[w].size() - 1) {
                    if (!MoveMino(test, search_tree[w].at(i), gc)) {
                        can = false;
                        break;
                    }
                }
                if (!can) {
                    w++;
                    continue;
                }
                int sft = -1, hd_cnt = 0;
                while (CheckMove(0, sft * (hd_cnt + 1), test, gc)) hd_cnt++;
                MoveMino(test, 3, gc);
                CmdPattern c(test, search_tree[w], parent_tree[w]);
                decltype(gc.cp)::iterator it = gc.cp.find(c);
                if (it != gc.cp.end()) {
                    w++;
                    continue;
                }
                CalcScore(c, gc, loop);
                c.pre_gc = gc.slot_id;
                pcv.push_back(c);
                gc.cp.insert(c);
                std::vector<int> w_sft = search_tree[w];
                w_sft.pop_back();
                shig_rep(i, hd_cnt)w_sft.push_back(2);
                search_tree[to] = w_sft; parent_tree[to] = parent_tree[w]; to++;
                w++;
            }
            else {
                if (search_tree[w].back() != 3) {
                    const std::vector<int> test_case = { 6, 7, 4, 5 };
                    shig_rep(h, test_case.size()) {
                        test.SetMino(gc.current_AI);
                        can = true;
                        std::vector<int> add_tree = search_tree[w];
                        add_tree.push_back(test_case.at(h));
                        shig_rep(i, add_tree.size()) {
                            if (!MoveMino(test, add_tree.at(i), gc)) {
                                can = false;
                                break;
                            }
                        }
                        if (!can) continue;

                        int sft = -1; int hd_cnt = 0;
                        while (CheckMove(0, sft * (hd_cnt + 1), test, gc)) hd_cnt++;
                        MoveMino(test, 3, gc);
                        add_tree.push_back(3);
                        CmdPattern c(test, add_tree, w); c.set_isSFT(true);
                        decltype(gc.cp)::iterator it = gc.cp.find(c);
                        if (it != gc.cp.end()) continue;
                        CalcScore(c, gc, loop);
                        gc.cp.insert(c);
                        add_tree.pop_back();
                        c.pre_gc = gc.slot_id;
                        pcv.push_back(c);
                        shig_rep(i, hd_cnt)add_tree.push_back(2);
                        if (to < mxm - 1) {
                            search_tree[to] = add_tree;
                            parent_tree[to] = parent_tree[w];
                            to++;
                        }
                    }
                    w++;
                }
                else {
                    w++;
                }
            }
        }


        return pcv;


    }

    void AiShigune::do_sw(vector<CmdPattern> &ctl, GameContainer gc, size_t loop) {

        std::vector<CmdPattern> pcv(0);

        if (gc.current_AI == 0)return;
        gc.p_field_AI = gc.field_AI;
		Tetri test;
		CmdPattern c;
		gc.cp.clear();

		std::set<Tetri> SearchList;

		std::queue<std::vector<int>> SearchTree;
		std::vector<int> add_tree;
		std::vector<int> soft_tree;

        // 通常探索 
        int w = 0;
		for (auto&& bc : BaseCmdS) {
			SearchTree.push(bc);
		}
		while (!SearchTree.empty()) {
			bool can = true;

			test.SetMino(gc.current_AI);
			soft_tree = SearchTree.front();

			for (size_t i = 0; i < SearchTree.front().size(); i++) {
				if (!MoveMino(test, SearchTree.front().at(i), gc)) {
					can = false;
					break;
				}
			}
			if (!can) {
				w++;
				SearchTree.pop();
				continue;
			}

			int sft = -1, hd_cnt = 0;
			while (CheckMove(0, sft * (hd_cnt + 1), test, gc)) {
				soft_tree.push_back(2);
				hd_cnt++;
			}
			MoveMino(test, 3, gc);
			//SearchList.insert(test);
			c = CmdPattern(test, SearchTree.front(), w);
			c.cmd_list.push_back(3);
			decltype(gc.cp)::iterator it = gc.cp.find(c);
			if (it != gc.cp.end()) {
				w++;
				SearchTree.pop();
				continue;
			}
			CalcScore(c, gc, loop);
			c.pre_gc = gc.slot_id;
			pcv.push_back(c);
			gc.cp.insert(c);

			//SearchTree.push(add_tree);

			// ソフドロ回転入力
			for (auto&& tm_soft : TestMove_soft) {
				test.SetMino(gc.current_AI);
				can = true;
				add_tree = soft_tree;
				add_tree.push_back(tm_soft);
				for (size_t i = 0; i < add_tree.size(); i++) {
					if (!MoveMino(test, add_tree.at(i), gc)) {
						can = false;
						break;
					}
				}
				if (!can) continue;
				SearchTree.push(add_tree);
			}

			w++;
			SearchTree.pop();

		}

		// 横入れ探索 
		w = 0;
		if (gc.ttrp_able == true) {
			for (auto&& bc : BaseCmdD) {
				SearchTree.push(bc);
			}
		}
		while (!SearchTree.empty()) {
			bool can = true;
			test.SetMino(gc.current_AI);
			soft_tree = SearchTree.front();
			for (size_t i = 0; i < SearchTree.front().size(); i++) {
				if (!MoveMino(test, SearchTree.front().at(i), gc)) {
					can = false;
					break;
				}
			}
			if (!can) {
				w++;
				SearchTree.pop();
				continue;
			}
			decltype(SearchList)::iterator isl = SearchList.find(test);
			if (isl != SearchList.end()) {
				w++;
				SearchTree.pop();
				continue;
			}
			SearchList.insert(test);
			// 途中横入れ動作
			for (auto&& tm_side : TestMove_side) {
				test.SetMino(gc.current_AI);
				can = true;
				add_tree = SearchTree.front();
				add_tree.push_back(tm_side);
				for (size_t i = 0; i < add_tree.size(); i++) {
					if (!MoveMino(test, add_tree.at(i), gc)) {
						can = false;
						break;
					}
				}
				if (!can) continue;
				SearchTree.push(add_tree);
			}
			MoveMino(test, 3, gc);
			c = CmdPattern(test, SearchTree.front(), w);
			c.cmd_list.push_back(3);
			decltype(gc.cp)::iterator it = gc.cp.find(c);
			if (it != gc.cp.end()) {
				w++;
				SearchTree.pop();
				continue;
			}
			CalcScore(c, gc, loop);
			c.pre_gc = gc.slot_id;
			pcv.push_back(c);
			gc.cp.insert(c);
			w++;
			SearchTree.pop();
		}

        std::lock_guard<std::mutex> lock(AiMtx);
        for (CmdPattern p : pcv) {
            ctl.push_back(p);
        }
        
        return;
    }

    bool AiShigune::ChoiceExplore(GameContainer gc_org) {

        gc_slot = std::vector<GameContainer>(branch_num);
        vector<vector<CmdPattern>> branch(branch_num, vector<CmdPattern>(0));
        vector<CmdPattern> catalog(0);
        constexpr size_t cls = 60 * branch_num;
        catalog.reserve(cls);

        gc_org.cp.clear();
        //catalog.clear();

		// frt = 25 まで善手選択
		static const size_t frt = 28;

        do_sw(ref(catalog), gc_org, 0);

        std::sort(catalog.begin(), catalog.end(), [&](const CmdPattern& l, const CmdPattern& r) { return r.scr < l.scr; });

        if (catalog.size() <= frt) {

			s_branch = { CmdPattern()};

            return false;
        }
        /*else if (catalog.size() < branch_num && catalog.size() > 0) {
            for (size_t i = 0; i < catalog.size(); i++) {
                branch.at(i).push_back(catalog.at(i));
                gc_slot.at(i) = update_gc(catalog.at(i), gc_org);
                gc_slot.at(i).slot_id = (int)i;
            }
            for (size_t i = catalog.size(); i < branch_num; i++) {
                gc_slot.at(i) = GameContainer();
                gc_slot.at(i).slot_id = (int)i;
            }
        }*/
        else {
			for (size_t i = 0; i < frt; i++) { // 一時的に具体値指定
                branch.at(i).push_back(catalog.at(i));
                gc_slot.at(i) = update_gc(catalog.at(i), gc_org);
                gc_slot.at(i).slot_id = (int)i;
            }

			std::vector<int> RndChoise(catalog.size() - frt, 0);
			std::iota(RndChoise.begin(), RndChoise.end(), 0);
			std::shuffle(RndChoise.begin(), RndChoise.end(), Rengine);

			for (size_t i = 0; i < std::min(branch_num - frt, RndChoise.size()); i++) { //
				branch.at(i + frt).push_back(catalog.at(RndChoise.at(i)));
				gc_slot.at(i + frt) = update_gc(catalog.at(RndChoise.at(i)), gc_org);
				gc_slot.at(i + frt).slot_id = (int)(i + frt);
			}

            /*int mid = (int)catalog.size() / 2;

            for (int i = 0; i < branch_num - frt; i++) {
                branch.at(frt + i).push_back(catalog.at(i + mid));
                gc_slot.at(frt + i) = update_gc(catalog.at(i + mid), gc_org);
                gc_slot.at(frt + i).slot_id = frt + i;
            }*/


        }

		for (size_t n = 0; n < exp_cyc_lim - 1; n++) {
            catalog.clear(); catalog.reserve(cls);

            size_t epwtn = expl_width.at(n);
			if (epwtn < 5)epwtn = 5;
			while (epwtn % thd_num != 0)epwtn--;
            std::vector<std::thread> threads(0);

			//for (size_t i = 0; i < epwtn; i += thd_num) {
			//	//GameContainer gc = gc_slot.at(i);
			//	threads.emplace_back(std::thread([this, i, &catalog, n]() {this->do_sw(ref(catalog), this->gc_slot.at(i), n); }));
			//	threads.emplace_back(std::thread([this, i, &catalog, n]() {this->do_sw(ref(catalog), this->gc_slot.at(i+1), n); }));
			//	threads.emplace_back(std::thread([this, i, &catalog, n]() {this->do_sw(ref(catalog), this->gc_slot.at(i+2), n); }));
			//	threads.emplace_back(std::thread([this, i, &catalog, n]() {this->do_sw(ref(catalog), this->gc_slot.at(i+3), n); }));
			//	threads.emplace_back(std::thread([this, i, &catalog, n]() {this->do_sw(ref(catalog), this->gc_slot.at(i+4), n); }));
			//}

			for (auto&& _gcs : gc_slot) {
				threads.emplace_back(std::thread([this, &_gcs, &catalog, n]() {this->do_sw(ref(catalog), _gcs, n); }));
			}
			for (auto& thr : threads) {
				thr.join();
			}
			
			std::sort(catalog.begin(), catalog.end(), [&](const CmdPattern &l, const CmdPattern &r) { return r.scr < l.scr; });
            vector<GameContainer> proxy_slot(branch_num);
            vector<vector<CmdPattern>> proxy_branch(branch_num, vector<CmdPattern>(0));
			for (size_t i = 0; i < epwtn; i++) {
                if (catalog.size() <= i || catalog.size() == 0) {
                    /*branch.at(i).push_back(null_cmd);
                    proxy_slot.at(i) = update_gc(null_cmd, gc_slot.at(0));
                    proxy_slot.at(i).slot_id = i;*/
					break;
                }
                else {
                    int ci = catalog.at(i).pre_gc;
                    proxy_branch.at(i) = branch.at(ci);
                    proxy_branch.at(i).push_back(catalog.at(i));
                    proxy_slot.at(i) = update_gc(catalog.at(i), gc_slot.at(ci));
                    proxy_slot.at(i).slot_id = (int)i;
                }
                
            }

			std::vector<int> RndChoise(catalog.size() - frt, 0);
			std::iota(RndChoise.begin(), RndChoise.end(), 0);
			std::shuffle(RndChoise.begin(), RndChoise.end(), Rengine);

			for (size_t i = 0; i < std::min(branch_num - frt, RndChoise.size()); i++) { //
				branch.at(i + frt).push_back(catalog.at(RndChoise.at(i)));
				gc_slot.at(i + frt) = update_gc(catalog.at(RndChoise.at(i)), gc_org);
				gc_slot.at(i + frt).slot_id = (int)i + frt;
			}

            gc_slot = proxy_slot;
            branch = proxy_branch;

        }

        if (catalog.size() == 0)return false;
        int ri = catalog.at(0).pre_gc;
		s_branch = branch.at(ri);
        return true;

    }
    
    // 評価値を求める
    void AiShigune::CalcScore(CmdPattern& cd, GameContainer& gcs, size_t loopc) {

		cd.scr.init_s();

		//gcs.p_field_AI = gcs.field_AI;
		//shigune_AI::ApplyMino(gcs.p_field_AI, cd.pat); ↓やってくれていた
		std::set<int> L = CheckErase(cd.pat, gcs);

		LL contact = 0;
		LL touch = 0;
		LL high = 0;
		LL btbc = 0;
		LL Wall = 0;

		const int rot = cd.pat.rot;
		const int idnt = cd.pat.id - 1;
		const int H = (int)cd.pat.mino.at(rot).size();
		const int W = (int)cd.pat.mino.at(rot).at(0).size();
		const int Ls = (int)L.size();

		//shigune_AI::height_calc(gcs);

		cd.set_ts(gcs.SRS_kind, gcs.TS_kind);

		std::vector<std::vector<int>> r_field_AI(0); r_field_AI.reserve(fh);
		if (Ls > 0) {
			for (size_t i = 0; i < fh; ++i) {
				decltype(L)::iterator it = L.find(i);
				if (it == L.end()) r_field_AI.push_back(gcs.p_field_AI.at(i));
			}
			while (r_field_AI.size() < fh) r_field_AI.push_back(ev_empty);
		}
		else {
			r_field_AI = gcs.p_field_AI;
		}

		bool isPC = true;
		if (Ls > 0) {
			for (size_t i = 0; i < 10; i++) {
				if (r_field_AI.at(0).at(i) != 0) {
					isPC = false;
					break;
				}
			}
		}
		else {
			isPC = false;
		}

		if (isPC)cd.scr.sum = shig::secure_add(114514191981000LL, cd.scr.sum);

		constexpr LL ttrp_rate = 200000000000000LL;

		bool chk_f = false;
		Tetri ofs_cdp = cd.pat;
		ofs_cdp.addY(gcs.ttrp_ofsY);

		if (gcs.ttrp_able == true) {
			for (int i = 0; i < (int)gcs.gc_ttrp.list_mino.size(); i++) {
				if (gcs.gc_ttrp.list_mino.at(i) == ofs_cdp) {
					if (cd.pat.id == 6) {
						if (gcs.gc_ttrp.list_mino_s.at(i) == 0 && gcs.TS_kind == 0) {
							chk_f = true;
							cd.set_ttrpF(i);
						}
						else if ((gcs.gc_ttrp.list_mino_s.at(i)) == Ls && gcs.TS_kind == 1) {
							chk_f = true;
							cd.set_ttrpF(i);
						}
						else {
							chk_f = false;
							cd.set_ttrpF(-1);
							//cd.update(ttrp_rate * -1LL);
						}
					}
					else {
						chk_f = true;
						cd.set_ttrpF(i);
					}

					break;

				}
				else {
					chk_f = false;
					cd.set_ttrpF(-1);
				}
			}

		}

		if (chk_f == true) {
			//cd.update(ttrp_rate);
			cd.scr.sum = shig::secure_add(ttrp_rate, cd.scr.sum);
			cd.scr.sum = shig::secure_add(gcs.pre_score, cd.scr.sum);
			cd.score = shig::secure_add(cd.score, cd.scr.sum);
			return;
		}
		else {
			cd.scr.sum = shig::secure_add(cd.scr.sum, -100000000LL);
		}

		//contact.V1
		int cnt_m = 0, cnt_n = 0;
		vector<bool> chk_con(W, false);
		for (size_t j = 0; j < W; j++) {
			if (ch.at(idnt).at(rot).at(j) == 1) {
				cnt_m++;
				int xp = cd.pat.X + (int)j;
				if (xp < 0 || xp >= 10)continue;
				int h = cd.pat.Y - gcs.height.at(xp) - 1;
				if (h >= 0 && h < H) {
					if (cd.pat.mino.at(rot).at(h).at(j) != 0) {
						cnt_n++;
					}
					else {
						if (h >= 1) {
							if (cd.pat.mino.at(rot).at((size_t)h - 1).at(j) != 0) {
								chk_con.at(j) = true;
								contact -= 200;
							}
							else {
								contact -= 5000;
							}
						}

					}
				}
				else if (h >= H) {
					contact -= 10000;
				}

			}
		}

		// contact V2
		constexpr  LL pnl_A = 4000, pnl_B = 1000, pnl_C = -2000;

		if (cd.pat.id == 1) {
			if (cd.pat.rot == 0 || cd.pat.rot == 2) {
				if (cnt_n == 4)contact += pnl_A;
				else if (cnt_n == 3 && (chk_con.at(0) == true || chk_con.at(3) == true))contact += pnl_B;
				else contact += pnl_C;
			}
			else {
				if (cnt_n == 1)contact += pnl_A;
				else contact += pnl_C;
			}
		}
		else if (cd.pat.id == 2) {
			if (cd.pat.rot == 0 || cd.pat.rot == 2) {
				if (cnt_n == 3)contact += pnl_A;
				else if (cnt_n == 2) {
					if (chk_con.at(0) == true || chk_con.at(2) == true) {
						contact += pnl_B;
					}
					else contact += pnl_C;
				}
				else contact += pnl_C;
			}
			else if (cd.pat.rot == 1) {
				if (cnt_n == 2)contact += pnl_A;
				else if (cnt_n == 1) {
					if (chk_con.at(1) == false && chk_con.at(2) == true) {
						contact += pnl_B;
					}
					else contact += pnl_C;
				}
				else contact += pnl_C;
			}
			else if (cd.pat.rot == 3) {
				if (cnt_n == 2)contact += pnl_A;
				else if (cnt_n == 1) {
					if (chk_con.at(1) == false && chk_con.at(0) == true) {
						contact += pnl_B;
					}
					else contact += pnl_C;
				}
				else contact += pnl_C;
			}
		}
		else if (cd.pat.id == 3) {
			if (cd.pat.rot == 0 || cd.pat.rot == 2) {
				if (cnt_n == 3)contact += pnl_A;
				else if (cnt_n == 2) {
					if (chk_con.at(0) == true || chk_con.at(2) == true) {
						contact += pnl_B;
					}
					else contact += pnl_C;
				}
				else contact += pnl_C;
			}
			else if (cd.pat.rot == 1) {
				if (cnt_n == 2)contact += pnl_A;
				else if (cnt_n == 1) {
					if (chk_con.at(1) == false && chk_con.at(2) == true) {
						contact += pnl_B;
					}
					else contact += pnl_C;
				}
				else contact += pnl_C;
			}
			else if (cd.pat.rot == 3) {
				if (cnt_n == 2)contact += pnl_A;
				else if (cnt_n == 1) {
					if (chk_con.at(1) == false && chk_con.at(0) == true) {
						contact += pnl_B;
					}
					else contact += pnl_C;
				}
				else contact += pnl_C;
			}
		}
		else if (cd.pat.id == 4) {
			if (cnt_n == 2)contact += pnl_A;
			else if (cnt_n == 1)contact += pnl_B;
			else contact += pnl_C;
		}
		else if (cd.pat.id == 5) {
			if (cd.pat.rot == 0 || cd.pat.rot == 2) {
				if (cnt_n == 3)contact += pnl_A;
				else if (cnt_n == 2 && chk_con.at(2) == true)contact += pnl_B;
				else contact += pnl_C;
			}
			else {
				if (cnt_n == 2)contact += pnl_A;
				else if (cnt_n == 1 && chk_con.at(1) == true)contact += pnl_B;
				else contact += pnl_C;
			}
		}
		else if (cd.pat.id == 6) {
			if (cd.pat.rot == 0 || cd.pat.rot == 2) {
				if (cnt_n == 3)contact += pnl_A;
				else if (cnt_n == 2 && (chk_con.at(0) == true || chk_con.at(2) == true))contact += pnl_B;
				else contact += pnl_C;
			}
			else if (cd.pat.rot == 1) {
				if (cnt_n == 2)contact += pnl_A;
				else if (cnt_n == 1 && chk_con.at(2) == true)contact += pnl_B;
				else contact += pnl_C;
			}
			else if (cd.pat.rot == 3) {
				if (cnt_n == 2)contact += pnl_A;
				else if (cnt_n == 1 && chk_con.at(0) == true)contact += pnl_B;
				else contact += pnl_C;
			}

		}
		else if (cd.pat.id == 7) {
			if (cd.pat.rot == 0 || cd.pat.rot == 2) {
				if (cnt_n == 3)contact += pnl_A;
				else if (cnt_n == 2 && chk_con.at(0) == true)contact += pnl_B;
				else contact += pnl_C;
			}
			else {
				if (cnt_n == 2)contact += pnl_A;
				else if (cnt_n == 1 && chk_con.at(1) == true)contact += pnl_B;
				else contact += pnl_C;
			}
		}


		//high
		LL cdY = cd.pat.Y;
		high = 5000;
		if (cdY > 15) {
			high = (-20 * cdY * cdY + 2865);
			high -= (cdY - gcs.height_mxm + 1) * 4000;
		}
		else if (cdY > 10) {
			high = (-10 * cdY * cdY + 615);
			high -= (cdY - gcs.height_mxm + 1) * 1000;

		}
		else if (cdY > 5)
		{
			high = (-5LL * cdY * cdY + 115);
			high -= (cdY - gcs.height_mxm + 1) * 250;

		}
		else {
			high = (-2LL * cdY - 0);
			high -= (cdY - gcs.height_mxm + 1) * 50;
		}


		//closure & touch
		int tls = (int)touch_list.at(idnt).at(rot).size();
		LL thc = 0, thcls = 0;
		for (int i = 0; i < tls; i++) {

			int ty = 0, tx = 0;
			ty = touch_list.at(idnt).at(rot).at(i).first;
			tx = touch_list.at(idnt).at(rot).at(i).second;

			tx = cd.pat.X + tx;
			if (tx < 0 || tx >= 10) {
				thc++;
				continue;
			}

			ty = cd.pat.Y - ty - 1;
			if (ty < 0 || ty >= fh - 1) {
				thc++;
				continue;
			}

			if (gcs.p_field_AI.at(ty).at(tx) != 0) {
				thc++;

				if (tx == 0 || tx == 9) {
					touch = shig::secure_add(touch, 10);
				}

				continue;
			}
			else {
				if (tx == 0 || tx == 9) {
					touch = shig::secure_add(touch, -10);
				}

			}


			int closure = 0;
			for (int k = 0; k < cc.size(); k++) {
				int yk = cc.at(k).first + ty;
				int xk = cc.at(k).second + tx;
				if (xk < 0 || xk >= 10) {
					closure++;
					continue;
				}
				if (yk < 0 || yk >= fh - 1) {
					closure++;
					continue;
				}

				if (gcs.p_field_AI.at(yk).at(xk) != 0) {
					closure++;
					continue;
				}
				else {
					continue;
				}

			}

			if (closure >= 4) {
				touch = shig::secure_add(touch, -8000LL);
				thcls++;
			}
			else {
				touch = shig::secure_add(touch, -1000LL * closure);
			}

		}

		touch = shig::secure_add(touch, thc * 1000);

		if (thcls == 0) {
			touch = shig::secure_add(touch, 10000LL);
		}

		//cmb
		constexpr LL cmb_rate = 1000000;
		LL cmb_s = 0;
		if (gcs.combo * Ls > 0) {
			cmb_s = (cmb_rate * 10LL * (gcs.combo + 1LL));
			cd.scr.cmb = shig::secure_add(cd.scr.cmb, cmb_s);
		}
		else if (gcs.combo > 0 && Ls == 0) {
			if (gcs.combo < 5) {
				cmb_s = (cmb_rate * -10LL * (gcs.combo + 1LL));
				cd.scr.cmb = shig::secure_add(cd.scr.cmb, cmb_s);
			}
		}
		else if (gcs.combo == 0 && Ls == 0) {
			cmb_s = cmb_rate;
			cd.scr.cmb = shig::secure_add(cd.scr.cmb, cmb_s);
		}
		else {
			//cd.update(gcs.combo * 0LL);
		}


		LL ve = 0;
		if (Ls == 0) {

			if (gcs.height_sum >= 128 && gcs.height_mxm >= 14) {
				ve += 100;

			}
			else if (gcs.height_sum >= 80 && gcs.height_mxm >= 10) {
				ve += 500;
				if (cd.pat.id == 6) {
					ve += -100;
				}
				else if (cd.pat.id == 1) {
					ve += -100;
				}
			}
			else if (gcs.height_sum >= 48 && gcs.height_mxm >= 6) {
				ve += 1000;
				if (cd.pat.id == 6) {
					ve += -500;
				}
				else if (cd.pat.id == 1) {
					ve += -200;
				}
			}
			else {
				ve += 4000;

				if (cd.pat.id == 6) {
					ve += -1000;
				}
				else if (cd.pat.id == 1) {
					ve += -400;
				}

			}

		}
		else if (Ls == 1) {
			if (gcs.TS_kind == 1) {

				if (gcs.height_sum >= 80 && gcs.height_mxm >= 10) {
					ve += 20000;
				}
				else if (gcs.height_sum >= 48 && gcs.height_mxm >= 6) {
					ve += 100000;
				}
				else {
					ve += 50000;
				}

				if (gcs.btb > -1)btbc += 5000;

			}
			else if (gcs.TS_kind == 2) {

				if (gcs.height_sum >= 80 && gcs.height_mxm >= 10) {
					ve += 500;
				}
				else {
					ve += 1000;
				}

				if (gcs.btb > -1)btbc += 50;
			}
			else {

				if (gcs.height_sum >= 80 && gcs.height_mxm >= 10) {
					ve += 1000;
					if (cd.pat.id == 1) {
						ve += 1000;
					}
				}
				else if (gcs.height_sum >= 48 && gcs.height_mxm >= 6) {
					ve += -10000;
					if (cd.pat.id == 6) {
						ve += -1000;
					}
					else if (cd.pat.id == 1) {
						ve += -1000;
					}
				}
				else {
					ve += -500000;
					if (cd.pat.id == 6) {
						ve += -200000;
					}
					else if (cd.pat.id == 1) {
						ve += -100000;
					}
				}

				if (gcs.btb > -1)btbc += -4000;

			}
		}
		else if (Ls == 2) {
			if (gcs.TS_kind == 1) {

				if (gcs.height_sum >= 80 && gcs.height_mxm >= 10) {
					ve +=   50000000;
				}
				else if (gcs.height_sum >= 48 && gcs.height_mxm >= 6) {
					ve += 1000000000;
				}
				else {
					ve += 2000000000;
				}

				if (gcs.btb > -1)btbc += 10000000;

			}
			else if (gcs.TS_kind == 2) {
				ve += 200;
				if (gcs.height_sum >= 48 && gcs.height_mxm >= 6) {
					ve -= 500;
				}

				if (gcs.btb > -1)btbc += 100;

			}
			else {

				if (gcs.height_sum >= 80 && gcs.height_mxm >= 10) {
					ve += 4000;

					if (cd.pat.id == 6) {
						ve += 200;
					}
					else if (cd.pat.id == 1) {
						ve += 1000;
					}

				}
				else if (gcs.height_sum >= 48 && gcs.height_mxm >= 6) {
					ve += -12000;
					if (cd.pat.id == 6) {
						ve += -8000;
					}
					else if (cd.pat.id == 1) {
						ve += -4000;
					}
				}
				else {
					ve += -60000;
					if (cd.pat.id == 6) {
						ve += -30000;
					}
					else if (cd.pat.id == 1) {
						ve += -10000;
					}

				}

				if (gcs.btb > -1)btbc += -20000;

			}
		}
		else if (Ls == 3) {
			if (gcs.TS_kind == 1) {
				if (gcs.height_sum >= 80 && gcs.height_mxm >= 10) {
					ve += 200000;
				}
				else {
					ve += 700000;
				}

				if (gcs.btb > -1)btbc += 50000;

			}
			else if (gcs.TS_kind == 2) {
				ve += 0;
				if (gcs.height_sum >= 80 && gcs.height_mxm >= 10) {
					ve -= 0;
				}
			}
			else {

				if (gcs.height_sum >= 80 && gcs.height_mxm >= 10) {
					ve += 10000;
					if (cd.pat.id == 1) {
						ve += 4000;
					}
				}
				else if (gcs.height_sum >= 48 && gcs.height_mxm >= 6) {
					ve += -8000;
					if (cd.pat.id == 6) {
						ve += -1000;
					}
					else if (cd.pat.id == 1) {
						ve += 2000;
					}
				}
				else {
					ve += -16000;
					if (cd.pat.id == 1) {
						ve += -4000;
					}
				}

				if (gcs.btb > -1)btbc += -10000;

			}
		}
		else if (Ls == 4) {
			if (gcs.height_sum >= 80 && gcs.height_mxm >= 10) {
				ve += 500000;
			}
			else if (gcs.height_sum >= 48 && gcs.height_mxm >= 6) {
				ve += 600000;
			}
			else {
				ve += 800000;
			}

			if (gcs.btb > -1)btbc += 80000;

		}

		constexpr LL ve_rate = 500;
		ve *= ve_rate;
		cd.scr.erase = ve;

		touch *= 2000;
		cd.scr.touch = touch;

		contact *= 1500;
		cd.scr.contact = contact;

		high *= 6000;
		cd.scr.height = high;

		//cd.update(shigune_AI::cs_BFS(cd, gcs));
		//cd.scr.closed = shigune_AI::cs_BFS(cd, r_field_AI);
		if (gcs.height_sum >= 64 && gcs.height_mxm >= 8) {
			cd.scr.closed = cs_BFS(r_field_AI) * 1000;
			Wall += cs_Wall(r_field_AI) * 800;
		}
		else {
			cd.scr.closed = cs_BFS(r_field_AI) * 1500;
			Wall += cs_Wall(r_field_AI) * 1500;
		}

		cd.scr.sum = shig::secure_add(cd.scr.sum, Wall);

		if (gcs.height_sum <= 80 || gcs.height_mxm <= 10) {
			if (cd.isSFT) {
				cd.scr.sum += -20;
			}
		}
		else
		{
			if (cd.isSFT) {
				cd.scr.sum += 0;
			}
		}

		if (gcs.height_sum <= 80 || gcs.height_mxm <= 10) {
			btbc *= 100;
		}
		else{
			btbc *= 20;
		}
		cd.scr.btbc = btbc;

		//cd.scr.sum += gcs.pre_score;
		cd.scr.sum = shig::secure_add(cd.scr.sum, gcs.pre_score);

		cd.scr.calc_sum();
		cd.score = shig::secure_add(cd.score, cd.scr.sum);

		/*if (loopc == -1) {
			cd.score = shig::secure_add(cd.score, cd.scr.sum);
		}
		else {
			cd.score = shig::secure_add(cd.score, cd.scr.sum);
		}*/

		return;
    }

    LL AiShigune::cs_BFS(const std::vector<std::vector<int>>& qf) {

        LL open_blc = 0;
        LL clos_blc = 0;
        LL mino_blc = 0;
        LL grbg_blc = 0;

        const size_t h_limit = 40;

        std::vector<std::vector<int>> b_field_AI(h_limit+1, (std::vector<int>(10, -1)));

        std::queue<std::pair<int, int>> xy;

        xy.push({ (int)h_limit, 0 });

        while (!xy.empty()) {
            pair<int, int> now = xy.front(); xy.pop();
            if (b_field_AI.at(now.first).at(now.second) != -1)continue;
            else b_field_AI.at(now.first).at(now.second) = 0;

            for (int i = 0; i < cc.size(); i++) {
                int y = now.first  + cc.at(i).first;
                int x = now.second + cc.at(i).second;

                if (y < 0 || y >= h_limit)continue;
                else if (x < 0 || x >= 10)continue;
                else if (qf.at(y).at(x) != 0)continue;
                else if (b_field_AI.at(y).at(x) != -1)continue;
                else xy.push({ y, x });

            }

        }

        for (int i = 0; i < h_limit; i++) {
            for (int j = 0; j < 10; j++) {

                if (qf.at(i).at(j) > 0)mino_blc++;
                else if (qf.at(i).at(j) == 8)grbg_blc++;
                else if (b_field_AI.at(i).at(j) == -1)clos_blc++;
                else if (b_field_AI.at(i).at(j) == 0)open_blc++;
            }
        }

        LL b_closure = 10000;
        if (clos_blc > 0)b_closure += -200 * clos_blc;
        b_closure += -1000 * clos_blc;
        b_closure += -100 * grbg_blc;
        b_closure += 0 * open_blc;

        b_closure *= 1;

        return b_closure;

    }

	LL AiShigune::cs_Wall(const std::vector<std::vector<int>>& qf)
	{
		LL Wall = 0;
		std::vector<WallProf> WallS(qf.size());

		for (size_t i = 0; i < qf.size(); i++) {
			WallS.at(i).Y = i;
			WallS.at(i).NumGap = 0;
			/*if (qf.at(i).front() == 8 or qf.at(i).back() == 8) {
				continue;
			}*/

			shig::Gap tg = { i, 0, 0, 0, 0 };

			for (size_t j = 0; j < qf.at(i).size(); j++) {

				if (qf.at(i).at(j) > 0) {
					if (tg.Size > 0) {
						tg.XR = (int)j - 1;
						tg.Center = (int)((tg.XL + (j - 1)) / 2);
						WallS.at(i).GapList.push_back(tg);
						WallS.at(i).NumGap += 1;
					}
					tg = { i, (int)j + 1, 0, 0, 0 };
				}
				else if(qf.at(i).at(j) == 0){
					tg.Size += 1;
				}
			}

			if (tg.Size == 10) {
				tg = {
					.Y = i,
					.XL = 0,
					.XR = 9,
					.Center = 4,
					.Size = 10,
				};
				WallS.at(i).GapList.push_back(tg);
				WallS.at(i).NumGap = 1;
				break;
			}
			else if (tg.Size > 0) {
				tg.XR = 9;
				tg.Center = (int)((9 + tg.XL) / 2);
				WallS.at(i).GapList.push_back(tg);
				WallS.at(i).NumGap += 1;
			}

		}


		for (size_t i = 1; i < WallS.size(); i++) {
			if (WallS.at(i).NumGap == 0 && WallS.at(i-1).NumGap == 1) {
				break;
			}

			// base score
			Wall += gn_score.at(WallS.at(i).NumGap);

			if (WallS.at(i - 1).NumGap == 1) {

				for (size_t j = 0; j < WallS.at(i).GapList.size(); j++)
				{
					if (WallS.at(i - 1).GapList.front().Center == WallS.at(i).GapList.at(j).Center) {
						Wall += WG_Center.at(WallS.at(i - 1).GapList.front().Size).at(WallS.at(i).GapList.at(j).Size);
					}
					else if (WallS.at(i - 1).GapList.front().Center == WallS.at(i).GapList.at(j).Center + 1) {
						Wall += WG_L1.at(WallS.at(i - 1).GapList.front().Size).at(WallS.at(i).GapList.at(j).Size);
					}
					else {
						Wall += -500;
					}
				}

			}
			else {
				Wall += -1000;
			}

		}

		return Wall;
	}



    bool AiShigune::height_calc(GameContainer& gch) {
        gch.height_sum = 0;
        gch.height_mxm = 0;
        shig_rep(j, 10) {
            int h = 44;
            while (h >= 0) {
                if (gch.field_AI.at(h).at(j) == 0) {
                    gch.height.at(j) = h;
                    h--;
                }
                else {
                    break;
                }
            }
            h++;
            gch.height_sum += h;
            gch.height_mxm = std::max(gch.height_mxm, h);
        }

        return true;
    }

    bool AiShigune::CheckMove(int to_x, int to_y, Tetri& s_check, GameContainer& ggc) {
        int cnt = 0, cntt = 4;
		auto&& [rot, size, H, W] = getTS(s_check);
        shig_rep(i, H) {
            shig_rep(j, W) {
                if (s_check.mino.at(rot).at(i).at(j) != 0) {
                    int sX = s_check.X + j + to_x;
                    if (sX < 0 || sX >= 10)break;
                    int sY = s_check.Y - i + to_y;
                    if (sY <= 0 || sY >= (ggc.field_AI.size() - 1))break;
                    if (ggc.field_AI.at(sY - 1LL).at(sX) == 0)cnt++;
                }
            }
        }
        return cnt == cntt;
    }

	int AiShigune::NextRotate(int n_rot, Rotate rt)
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

	bool AiShigune::CheckSRS_Clockwise(Tetri& s_now, GameContainer& ggc)
	{
		Tetri test = s_now;
		int to_X = 0, to_Y = 0, rot = test.rot;
		bool can = false;
		test.set_rot(NextRotate(rot, Rotate::Clockwise));
		if (test.id == 1) {
			int cnt = 0;
			for (auto&& tester : WallKick_clockI.at(rot)) {
				if (CheckMove(tester.first, tester.second, test, ggc)) {
					ggc.SRS_kind = cnt;
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
			for (auto&& tester : WallKick_clockW.at(rot)) {
				if (CheckMove(tester.first, tester.second, test, ggc)) {
					ggc.SRS_kind = cnt;
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
			ggc.TS_kind = TS_check(to_X, to_Y, test, ggc);
			s_now.set_rot(test.rot);
			s_now.addX(to_X);
			s_now.addY(to_Y);
		}

		return can;
	}

	bool AiShigune::CheckSRS_CounterClock(Tetri& s_now, GameContainer& ggc)
	{

		Tetri test = s_now;
		int to_X = 0, to_Y = 0, rot = test.rot;
		bool can = false;
		test.set_rot(NextRotate(rot, Rotate::CounterClockwise));
		if (test.id == 1) {
			int cnt = 0;
			for (auto&& tester : WallKick_counterI.at(rot)) {
				if (CheckMove(tester.first, tester.second, test, ggc)) {
					ggc.SRS_kind = cnt;
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
			for (auto&& tester : WallKick_counterW.at(rot)) {
				if (CheckMove(tester.first, tester.second, test, ggc)) {
					ggc.SRS_kind = cnt;
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
			ggc.TS_kind = TS_check(to_X, to_Y, test, ggc);
			s_now.set_rot(test.rot);
			s_now.addX(to_X);
			s_now.addY(to_Y);
		}

		return can;

	}

    int AiShigune::TS_check(int toX, int toY, Tetri& ts, GameContainer& ggc) {
        if (ts.id != 6) {
            ggc.TS_kind = 0;
            return 0;
        }

        int ts_cnt = 0, rot = ts.rot;
        std::vector<int> testX = { 0, 0, 2, 2 }, testY = { 0, 2, 0, 2 };
        pairI2 check = { 0, 0 };

        shig_rep(i, 4) {
            int sX = ts.X + testX.at(i) + toX;
            if (sX < 0 || sX >= 10) {
                ts_cnt++;
                continue;
            }
            int sY = ts.Y - testY.at(i) + toY - 1;
            if (sY < 0 || sY >= (ggc.field_AI.size() - 1)) {
                ts_cnt++;
                continue;
            }
            if (ggc.field_AI[sY].at(sX) != 0)ts_cnt++;
            else check = { testX.at(i), testY.at(i) };
        }

        if (ts_cnt == 3 && SRS_kind != 4) {
            if (rot == 0) {
                if (check.first == 0 && check.second == 0)ggc.TS_kind = 2;
                else if (check.first == 2 && check.second == 0)ggc.TS_kind = 2;
                else ggc.TS_kind = 1;
            }
            else if (rot == 1) {
                if (check.first == 2 && check.second == 0)ggc.TS_kind = 2;
                else if (check.first == 2 && check.second == 2)ggc.TS_kind = 2;
                else ggc.TS_kind = 1;
            }
            else if (rot == 2) {
                if (check.first == 2 && check.second == 2)ggc.TS_kind = 2;
                else if (check.first == 0 && check.second == 2)ggc.TS_kind = 2;
                else ggc.TS_kind = 1;
            }
            else if (rot == 3) {
                if (check.first == 0 && check.second == 2)ggc.TS_kind = 2;
                else if (check.first == 0 && check.second == 0)ggc.TS_kind = 2;
                else ggc.TS_kind = 1;
            }
        }
        else if (ts_cnt == 3 || ts_cnt == 4)ggc.TS_kind = 1;//normal
        else ggc.TS_kind = 0;

        return ggc.TS_kind;
    }

    std::set<int> AiShigune::CheckErase(Tetri& s_now, GameContainer &gce) {
		std::set<int> erase_itr;
        gce.p_field_AI = gce.field_AI;
        AiShigune::ApplyMino(gce.p_field_AI, s_now);
        int rot = s_now.rot;
        
        int H = (int)s_now.mino.at(rot).size();
        int pW = 10;

        shig_rep(i, H) {
            int sY = s_now.Y - i - 1;
            int cnt = 0;
            if (sY < 0 || sY >= fh - 1)continue;
            shig_rep(j, pW) {
                if (gce.p_field_AI[sY].at(j) != 0) cnt++;
                else break;
            }
            if (cnt == pW) erase_itr.insert(sY);
        }
        return erase_itr;
    }

    void AiShigune::ApplyMino(std::vector<std::vector<int>>& c_field, const Tetri& s_now) {
		//auto [rot, size, H, W] = getTS(s_now);
		for (int i = 0; i < s_now.mino.at(s_now.rot).size(); ++i) {
			for (int j = 0; j < s_now.mino.at(s_now.rot).at(i).size(); ++j) {
                if (s_now.mino[s_now.rot].at(i).at(j) != 0) {
                    int sX = s_now.X + j;
                    int sY = s_now.Y - i;
                    if (sX < 0 || sX >= 10)continue;
                    if (sY - 1 < 0 || sY - 1 > 45)continue;
                    c_field.at((size_t)sY - 1).at(sX) = s_now.id;
                }
            }
        }
    }

    bool AiShigune::MoveMino(Tetri& m_now, int s_action, GameContainer& ggc) {
        if (s_action == 1) {
            if (ggc.hold_AI == 0) {
                m_now.SetMino(ggc.q_next_AI.front());
            }
            else {
                m_now.SetMino(ggc.hold_AI);
            }
            return true;
        }
        else if (s_action == 2) {
            int sft = -1;
            if (CheckMove(0, sft, m_now, ggc)) {
                m_now.addY(sft);
                return true;
            }
            else return false;
        }
        else if (s_action == 3) {
            int sft = -1;
            while (CheckMove(0, sft, m_now, ggc)) {
                m_now.addY(sft);
            }
            return true;
        }
        else if (s_action == 4) {
            //return SRS_check(-1, m_now, ggc);
			return CheckSRS_CounterClock(m_now, ggc);
        }
        else if (s_action == 5) {
            //return SRS_check(1, m_now, ggc);
			return CheckSRS_Clockwise(m_now, ggc);
        }
        else if (s_action == 6) {
            int sft = -1;
            if (CheckMove(sft, 0, m_now, ggc)) {
                m_now.addX(sft);
                return true;
            }
            else return false;
        }
        else if (s_action == 7) {
            int sft = 1;
            if (CheckMove(sft, 0, m_now, ggc)) {
                m_now.addX(sft);
                return true;
            }
            else return false;
        }
        return true;
    }

    void AiShigune::PrintGhost(const Tetri& s_now) {
		for (int i = 0; i < s_now.mino.at(s_now.rot).size(); ++i) {
			for (int j = 0; j < s_now.mino.at(s_now.rot).at(i).size(); ++j) {
				if (s_now.mino[s_now.rot].at(i).at(j) != 0) {
					int sX = s_now.X + j;
					int sY = s_now.Y - i;
					if (sX < 0 || sX >= 10)continue;
					if (sY - 1 < 0 || sY - 1 > 45)continue;
					p_field_AI.at((size_t)sY - 1).at(sX) = -1 * s_now.id;
				}
			}
		}

        return;
    }

    bool AiShigune::load_ttrp() {
        GetTempNameList(ttrp_name_list);
        ttrp_size = (int)ttrp_name_list.size();
        //template_list.reserve(ttrp_size+1);
        vector<TetriPlate> vt(ttrp_size);
		for (size_t i = 0; i < ttrp_name_list.size(); ++i) {
            ReadTempData(ttrp_name_list.at(i), vt.at(i));
            ttrp_id_list.push_back(vt.at(i).tp_id);
        }

        ttrp_list = vt;

        return true;
    }

    bool AiShigune::ttrp_crr(Tetri& now_p, int& size_l) {
        bool chk_f = false;
        if (ttrp_able) {
			for (size_t i = 0; i < std::min((size_t)select_ttrp.mino_num, select_ttrp.list_mino.size()); ++i) {
                if (select_ttrp.list_mino.at(i) == now_p) {
                    if (now_p.id == 6) {
                        if ((select_ttrp.list_mino_s.at(i)) == size_l && TS_kind == 1) {
                            chk_f = true;
                        }
                        else if (select_ttrp.list_mino_s.at(i) == 0 && TS_kind == 0){
                            chk_f = true;
                        }
                        else {
                            chk_f = false;
                        }
                    }
                    else {
                        chk_f = true;
                    }

                    break;

                }
                else {
                    chk_f = false;
                }
            }

        }

        return chk_f;
    }

    bool AiShigune::ttrp_check(CmdPattern& slc, int& sle, std::vector<int>& mnL) {

        if (!ttrp_able)return true;

        if (slc.ttrp_f >= 0) {
			select_ttrp.mino_check.at(slc.ttrp_f) = true;
            int mchk_cnt = 0;
            shig_rep(i, select_ttrp.mino_num) {
                if (select_ttrp.mino_check.at(i) == true) {
                    mchk_cnt++;
                }
            }
            if (mchk_cnt >= select_ttrp.mino_num) {

                if (select_ttrp.list_size == 0) {
                    ttrp_able = false;
                }
                else {
                    shig_rep(i, select_ttrp.list_size) {
                        bool lpf = false;
                        shig_rep(j, ttrp_list.size()) {

                            if (ttrp_list.at(j).tp_id != select_ttrp.list_id.at(i))continue;

                            int tm_cnt = 0;
                            shig_rep(k, ttrp_list.at(j).terms_num) {
                                int tL = ttrp_list.at(j).terms[k].first;
                                int tR = ttrp_list.at(j).terms[k].second;
                                if (mnL[tL] < mnL[tR])tm_cnt++;
                            }

                            if (tm_cnt >= ttrp_list.at(j).terms_num) {
								select_ttrp = ttrp_list.at(j);
                                ttrp_able = true;
                                lpf = true;
                                break;
                            }
                            else {
                                lpf = false;
                                break;
                            }
                        }
                        if (lpf)break;
                    }
                }

            }
        }
        else {
            ttrp_able = false;
        }

        if (ttrp_able) {
            ttrp_ofsY += sle;
        }
        else {
            ttrp_ofsY = 0;
        }

        return true;
    }

    bool AiShigune::ttrp_check(CmdPattern& slc, int& sle, GameContainer& gct) {

		//std::vector<int> mnL = make_order_list(gct);

        if (gct.ttrp_able == false)return true;

        if (slc.ttrp_f >= 0) {
            gct.gc_ttrp.mino_check.at((size_t)slc.ttrp_f) = true;
            int mchk_cnt = 0;
            shig_rep(i, gct.gc_ttrp.mino_num) {
                if (gct.gc_ttrp.mino_check.at(i) == true) {
                    mchk_cnt++;
                }
            }
            if (mchk_cnt >= gct.gc_ttrp.mino_num) {
                gct.ttrp_able = false;
            }
        }
        else {
            gct.ttrp_able = false;
        }

        if (gct.ttrp_able) {
            gct.ttrp_ofsY += sle;
        }
        else {
            gct.ttrp_ofsY = 0;
        }

        return true;
    }

    bool AiShigune::next_crr_check() {

        std::set<int> all_chk;
        int lim_s = std::min(5, (int)next_AI.size());
		for (int i = 0; i < lim_s; ++i)all_chk.insert(next_AI.at(i));
        all_chk.insert(current_AI);
        all_chk.insert(hold_AI);
        if (all_chk.size() == 7)return true;
        else if (hold_AI == 0 && all_chk.size() == 6)return true;
        else return false;

        return false;

    }

    pair<int, string> AiShigune::get_sttrp_name() {
        std::string s("noting selected");
        if (ttrp_able)return make_pair(select_ttrp.tp_id, select_ttrp.temp_name);
        else return make_pair(-1, s);
    }

    bool AiShigune::set_gc(GameContainer &gc) const {

        std::vector<int> gi(12, 0);
        gi[0] = hold_AI;
        gi[1] = current_AI;
        gi[2] = pc_cnt;
        gi[3] = SRS_kind;
        gi[4] = TS_kind;
        gi[5] = btb;
        gi[6] = combo;
        gi[7] = height_sum;
        gi[8] = height_mxm;
        gi[9] = ttrp_size;
        gi[10] = ttrp_ofsY;
        gi[11] = 0;
        //if (!gc.set_gc_int(gi))return false;
        gc.set_gc_int(gi);

        vector<bool> gb(1);
        gb[0] = ttrp_able;
        //if (!gc.set_gc_bool(gb))return false;
        gc.set_gc_bool(gb);

        std::vector<std::vector<int>> gv(1, std::vector<int>(0, 0));
        gv[0] = this->height;
        //if (!gc.set_gc_VI(gv))return false;
        
        gc.set_gc_VI(gv);

        //if(!gc.set_gc_next(next_AI, q_next_AI))return false;
        gc.set_gc_next(next_AI, q_next_AI);

        vector<std::vector<std::vector<int>>> gf(3, std::vector<std::vector<int>>(0, std::vector<int>(0, 0)));
        gf[0] = field_AI;
        gf[1] = p_field_AI;
        //gf[2] = strategy_map;
        //if(!gc.set_gc_field(gf))return false;
        gc.set_gc_field(gf);


        //if(!gc.set_gc_ttrp(gc_ttrp))return false;
        gc.set_gc_ttrp(select_ttrp);

        //gc.p_field_AI = p_field_AI;
        //strategy_map = std::vector<std::vector<int>>(45, W_seed);
        //strategy_list = vector<strategy>(0);
        //cv = vector<cmd_pattern>(0);
        //cv.reserve(600);
        //ec = vector<cmd_pattern>(0);
        //ec.reserve(10);
        

        return true;
    }

    GameContainer AiShigune::update_gc(CmdPattern& ct, GameContainer gcp) {
        
        set<int> itr = AiShigune::CheckErase(ct.pat, gcp);
        int itr_s = (int)itr.size();

        std::vector<std::vector<int>> proxy(0); proxy.reserve(45);

        shig_rep(i, fh) {
            decltype(itr)::iterator it = itr.find(i);
            if (it == itr.end()) proxy.push_back(gcp.p_field_AI.at(i));
        }
        while (proxy.size() < fh) proxy.push_back(ev_empty);

        gcp.field_AI = proxy;

        if (ct.cmd_list.at(0) == 1) gcp.hold_AI = gcp.current_AI;
        if ((int)gcp.q_next_AI.size() <= 1) {
            shig_rep(i, NS_a.size()) {
                gcp.q_next_AI.push_back(NS_a.at(i));
            }
        }
        
        if (gcp.q_next_AI.size() != 0) {
            gcp.current_AI = gcp.q_next_AI.front();
            gcp.q_next_AI.pop_front();  gcp.next_AI.clear();
            while (!gcp.q_next_AI.empty()) {
                int q = gcp.q_next_AI.front();
                gcp.next_AI.push_back(q);
                gcp.q_next_AI.pop_front();
            }
			for (size_t i = 0; i < gcp.next_AI.size(); i++){
                gcp.q_next_AI.push_back(gcp.next_AI.at(i));
            }
        }
        
        AiShigune::height_calc(gcp);

        AiShigune::ttrp_check(ct, itr_s, gcp);

        if (gcp.combo == 0) {
            if (itr_s == 0) {

            }
            else {
                gcp.combo++;
            }
        }else if (gcp.combo > 0) {
            if (itr_s == 0) {
                gcp.combo = 0;
            }
            else {
                gcp.combo++;
            }
        }

        if (ct.scr.btbc > 0) {
            gcp.btb += 1;
        }
        else if (ct.scr.btbc <= 0) {
            gcp.btb = -1;
        }

        gcp.pre_score = shig::secure_add(gcp.pre_score, ct.score);
        //gcp.pre_score += ct.score;

        return gcp;
    }

    int AiShigune::ttrp_check_mino(Tetri& fd, GameContainer& gcf) {

        for (int i = 0; i < gcf.gc_ttrp.mino_num; i++) {
            if (gcf.gc_ttrp.list_mino.at(i) == fd)return i;
        }

        return -1;

    }

	void shig::AiShigune::loadTE(const TetriEngine& te){
		
		te.GetField(field_AI);
		current_AI = te.get_current().id;
		auto&& [_hold, _next] = te.get_mino_state();
		hold_AI = _hold;
		q_next_AI = _next;
		auto&& [_c, _b] = te.get_combo();
		combo = _c; btb = _b;

		return;
	}

	CmdPattern AiShigune::getCmd()
	{
		std::lock_guard<std::mutex> _lock{ AiMtx };
		return s_branch.at(0);
	}

	AiShigune::~AiShigune() {

	}

	// 非同期処理用
	int32 ExeThinking(AiShigune& As, const std::atomic<bool>& abort, std::atomic<bool>& think, std::deque<int>& CmdListS)
	{
		// 中断命令が出るまで
		while (!abort) {
			if (think) {
				As.thinking();
				As.makeAiSuggestion();
				for (auto&& cmd : As.getCmd().cmd_list) {
					CmdListS.push_back(cmd);
				}
				think = false;
			}
		}

		std::cerr << "非同期処理終了";

		return 334;
	}

}

namespace shig {

	bool GetTempNameList(std::vector<std::string>& name_list) {
		int n = 0;
		//FILE* fp = NULL;
		//fopen_s(&fp, "template\\tetriplate_list.txt", "r");
		//
		//if (fp != NULL) {
		//	fscanf_s(fp, "%d", &n);
		//	if (n < 0)n = 0;
		//	std::vector<std::string> list(n);
		//	shig_rep(i, n) {
		//		char tmpC[128];
		//		fscanf_s(fp, "%s", tmpC, 128);
		//		if (tmpC[0] == '\0' || tmpC[0] == '/') {
		//			i--;
		//			continue;
		//		}
		//		for (int j = 0; j < 128; j++) {
		//			if (tmpC.at(j) == '\0') {
		//				list.at(i) = std::string(tmpC, j);
		//				break;
		//			}
		//		}
		//		//list.at(i) = tmpC;
		//		name_list.push_back("template\\data\\" + list.at(i));
		//	}
		//	//name_list = list;
		//	fclose(fp);
		//}

		std::ifstream ifs("template\\tetriplate_list.txt");
		if (ifs.fail()) {
			std::cerr << "failed to open \"template\\tetriplate_list.txt\" ";
			exit(-4);
		}

		std::string readS;
		std::getline(ifs, readS);
		stringstream ss(readS);
		ss >> n;

		while (std::getline(ifs, readS)) {
			stringstream sl(readS);
			std::string tempS;
			sl >> tempS;
			if (tempS[0] == '\0' || tempS[0] == '/') {

			}
			else {
				name_list.push_back("template\\data\\" + tempS);
			}
		}

		return true;

	}

	bool ReadTempData(const std::string& name, shig::TetriPlate& ttrp) {

		std::ifstream ifs(name);
		if (ifs.fail()) {
			std::cerr << "failed to open -> " << name << "\n";
			exit(-5);
		}

		int l = 0, ls = 0, id = 0, tn = 0, bf = 0; //mino num : connect list num : ttrp id
		string rtdS = "";

		std::string readS;
		std::getline(ifs, readS);
		stringstream ss(readS);
		ss >> l >> ls >> id >> tn >> bf >> rtdS;
		ttrp.Setup(l, ls, id, tn, rtdS, bf);

		// mino座標読み込み
		if (l > 0) {
			vector<Tetri> tpl(0);
			std::vector<int> ts(0);
			for (int i = 0; i < l; i++) {
				int r = 0, x = 0, y = 0, d = 0, s = 0;
				std::getline(ifs, readS);
				stringstream mns(readS);
				mns >> r >> x >> y >> d >> s;
				Tetri tp(r, x, y, d);
				tpl.push_back(tp);
				ts.push_back(s);
			}
			ttrp.set_list(tpl, ts);
		}

		// 次に遷移できるテンプレid 
		if (ls > 0) {
			std::vector<int> vil(0);
			for (int i = 0; i < ls; i++) {
				int tid = 0;
				std::getline(ifs, readS);
				stringstream tpid(readS);
				tpid >> tid;
				vil.push_back(tid);
			}
			ttrp.set_id_list(vil);
		}

		// 選択条件
		if (tn > 0) {
			std::vector<pair<int, int>> vmp(0);
			for (int i = 0; i < tn; i++) {
				int ll = 0, rr = 0;
				std::getline(ifs, readS);
				stringstream terms(readS);
				terms >> ll >> rr;
				vmp.push_back(std::make_pair(ll, rr));
			}
			ttrp.set_terms(vmp);
		}
		
		return true;
	}

}

