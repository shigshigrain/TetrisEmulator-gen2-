#include "ai.hpp"

Ai::Ai(const InitData& init)
	: IScene{ init }
{
	m_1pTE = std::make_unique<TetriEngine>(1);
	m_1pTE->Init(1);
	m_2pTE = std::make_unique<TetriEngine>(2);
	m_2pTE->Init(2);
	m_2pAI = std::make_unique<AiShigune>(2);
	m_2pAI->loadTE(*m_2pTE);
	m_2pAI->load_ttrp();
	
	m_bg = Texture{ U"tex\\background\\tetris_emulator_background02.bmp" };

	//m_KeyConf1p = KeyConf();
	m_KeyConf1p.SetDefault();

	//m_MinoTex = Array<Texture>(0);
	for (auto&& mp : minotex_path) {
		m_MinoTex.emplace_back(Texture{mp});
	}

	sec_time = Time::GetMillisec();
	delay_cnt = 0;
	DASFlame1p = 6;
	WaitFlame1p = 0;
	WaitFlame2p = 0;
	passed_flame = 0;
	reset_flag = false;
	//suggest_flag = shig::BoolSwitch();//false
	FieldS = std::vector<std::vector<int>>(fh, (std::vector<int>(10, 0)));
	ActFlame = std::vector<int>(8, 0);
	abortAi = { false };
	thinkAi = { false };
	CmdList2pAi = std::deque<int>(0);

	// AI起動 
	asyncAi = s3d::Async(shig::ExeThinking, ref(*m_2pAI), ref(abortAi), ref(thinkAi), ref(CmdList2pAi));

	ResetManage();

}

void Ai::update()
{

	if (Time::GetMillisec() - sec_time >= refrashRate60) {
		sec_time = Time::GetMillisec();
		passed_flame++;
		m_KeyConf1p.SetDefault(); // キー入力情報のセット

		// 1p 人間側
		if (WaitFlame1p > 0) {
			WaitFlame1p--;
		}
		else {
			m_1pTE->ResetFieldP();
			
			if (reset_flag) ResetManage();

			// テトリス側操作入力
			TetrisManage1p();
			// DASフレームの更新
			actF_manage();
			// ゲーム側操作入力
			GameManage1p();
			
			
		}
		// 2p AI側 
		if (WaitFlame2p > 0) {
			WaitFlame2p--;
		}
		else {
			m_2pTE->ResetFieldP();
			if (reset_flag) ResetManage();
			// テトリス側操作入力
			TetrisManage2p();
			// ゲーム側操作入力
			GameManage2p();
			
		}

		//GameManage1p();
		//GameManage2p();

	}

	if (KeyQ.pressed() or KeyEscape.pressed())
	{
		thinkAi = false;
		abortAi = true;
		// 非同期処理の終了を待機 
		if (asyncAi.isValid())asyncAi.wait();
		changeScene(State::Title);
	}
}

void Ai::draw() const
{
	m_bg.draw(0, 0);
	DrawField();
	DrawGhost();
	DrawTex1p();
	DrawTex2p();
}

Ai::~Ai()
{
	thinkAi = false;
	abortAi = true;
	// 非同期処理の終了を待機 
	if (asyncAi.isValid())asyncAi.wait();
}

void Ai::GameManage1p() {

	if (IsKeyVP(m_KeyConf1p, KeyVal::R)) {
		m_1pTE->CopyFiledP();
		m_2pTE->CopyFiledP();
		WaitFlame1p = 40;
		WaitFlame2p = 40;
		reset_flag = true;
	}

	if (IsKeyVP(m_KeyConf1p, KeyVal::G)) {
		m_1pTE->CopyFiledP();
		//m_1pTE->edit_garbage_cmd(1);
		m_1pTE->StackGarbage(-1);

	}

}

void Ai::GameManage2p()
{
	if (IsKeyVP(m_KeyConf1p, KeyVal::M)) {
		if (ActFlame.at(0) >= 0) {
			ActFlame.at(0) = -15;
			suggest_flag.sw();
		}
	}

	if (IsKeyVP(m_KeyConf1p, KeyVal::O)) {
		m_2pTE->CopyFiledP();
		m_2pTE->StackGarbage(-1);
	}


}

void Ai::TetrisManage1p() {

	int g_check = 0;

	if (m_KeyConf1p.GetKey(KeyVal::Left).pressed() && not m_KeyConf1p.GetKey(KeyVal::Right).pressed()) {
		if (ActFlame.at(6) == 0) {
			ActFlame.at(6) = -1 * DASFlame1p;
			g_check = m_1pTE->Game(6, 0);
		}
		else if (ActFlame.at(6) == -1) {
			ActFlame.at(6) = 1;
		}
		else if (ActFlame.at(6) > 0) {
			g_check = m_1pTE->Game(6, 0);
		}
		delay_cnt = 2;
	}

	if (not m_KeyConf1p.GetKey(KeyVal::Left).pressed() && m_KeyConf1p.GetKey(KeyVal::Right).pressed()) {
		if (ActFlame.at(7) == 0) {
			ActFlame.at(7) = -DASFlame1p;
			g_check = m_1pTE->Game(7, 0);
		}
		else if (ActFlame.at(7) == -1) {
			ActFlame.at(7) = 1;
		}
		else if (ActFlame.at(7) > 0) {
			g_check = m_1pTE->Game(7, 0);
		}
		delay_cnt = 2;
	}

	if (m_KeyConf1p.GetKey(KeyVal::Up).pressed() && not m_KeyConf1p.GetKey(KeyVal::Z).pressed()) {
		if (ActFlame.at(5) >= 0) {
			ActFlame.at(5) = -DASFlame1p;
			g_check = m_1pTE->Game(5, 0);
		}
		else {
			ActFlame.at(5) -= 1;
		}
		delay_cnt = 2;
	}

	if (not m_KeyConf1p.GetKey(KeyVal::Up).pressed() && m_KeyConf1p.GetKey(KeyVal::Z).pressed()) {
		if (ActFlame.at(4) >= 0) {
			ActFlame.at(4) = -DASFlame1p;
			g_check = m_1pTE->Game(4, 0);
		}
		else {
			ActFlame.at(4) -= 1;
		}
		delay_cnt = 2;
	}

	if (m_KeyConf1p.GetKey(KeyVal::C).pressed()) {
		if (ActFlame.at(1) >= 0) {
			ActFlame.at(1) = -2;
			g_check = m_1pTE->Game(1, 0);
		}
		else {
			ActFlame.at(1) += -1;
		}
		delay_cnt = 2;

	}

	if (m_KeyConf1p.GetKey(KeyVal::Down).pressed()) {
		if (ActFlame.at(2) >= 0) {
			ActFlame.at(2) = -1;
			g_check = m_1pTE->Game(2, 0);
		}
		else {
			ActFlame.at(2) = 0;
		}
		delay_cnt = 2;
	}

	if (m_KeyConf1p.GetKey(KeyVal::Space).pressed()) {
		if (ActFlame.at(3) >= 0) {
			ActFlame.at(3) = -2;
			g_check = m_1pTE->Game(3, 0);
			m_2pTE->StackGarbage(m_1pTE->getGarbage());
			//delay_cnt = m_1pTE->get_delayF();
		}
		else {
			ActFlame.at(3) += -1;
		}
		//delay_cnt = 2;

	}

	if (g_check == 2) {
		WaitFlame1p = m_1pTE->get_delayF();
		delay_cnt = 0;
	}
	else if (g_check == 1) {
		m_1pTE->CopyFiledP();
		reset_flag = true;
		WaitFlame1p = 72;
	}
	else if (g_check == 0) {
		m_1pTE->CopyFiledP();
	}

	return;
}

void Ai::TetrisManage2p()
{
	int g_check = 0;

	if (suggest_flag.get()) {
		// 非同期処理側で推奨手計算が終了している場合
		if (!thinkAi) {
			FieldS = m_2pAI->getSuggestionAi();
			if (!CmdList2pAi.empty()) {
				g_check = m_2pTE->Game(CmdList2pAi.front(), 0);
				WaitFlame2p = 0;
				if (CmdList2pAi.front() == 3) {
					//m_1pTE->StackGarbage((int)(m_2pTE->getGarbage() / 2));
					m_1pTE->StackGarbage(m_2pTE->getGarbage());
					m_2pTE->getGarbage();
				}

				CmdList2pAi.pop_front();
				// 操作をし終わったタイミングで先に思考開始
				if (CmdList2pAi.empty()) {
					m_2pAI->loadTE(*m_2pTE);
					thinkAi = true;
				}
			}
			else {
				thinkAi = true;
			}
		}
		else if (thinkAi) {
			// することがない 
		}
	}

	switch (g_check)
	{
	case 2:
		WaitFlame2p = m_2pTE->get_delayF();
		delay_cnt = 0;
		break;
	case 1:
		m_2pTE->CopyFiledP();
		reset_flag = true;
		WaitFlame2p = 30;
		break;
	case 0:
		m_2pTE->CopyFiledP();
		break;
	default:
		break;
	}


	return;

}

void Ai::actF_manage() {

	for (auto&& af : ActFlame) {
		if (af <= 0x11111110)af++;
	}

	if (not IsKeyVP(m_KeyConf1p, KeyVal::Right) && not IsKeyVP(m_KeyConf1p, KeyVal::Left)) {
		ActFlame.at(6) = 0;
		ActFlame.at(7) = 0;
	}

	if (IsKeyVP(m_KeyConf1p, KeyVal::Right) && IsKeyVP(m_KeyConf1p, KeyVal::Left)) {
		ActFlame.at(6) = 1;
		ActFlame.at(7) = 1;
	}

	return;
}

void Ai::ResetManage() {

	m_1pTE->SetField();
	m_1pTE->CopyFiledP();

	m_2pTE->SetField();
	m_2pTE->CopyFiledP();

	delay_cnt = 0;
	DASFlame1p = 6;
	WaitFlame1p = 30;
	WaitFlame2p = 30;
	reset_flag = false;
	ActFlame = std::vector<int>(8, 0);
	FieldS = std::vector<std::vector<int>>(fh, (std::vector<int>(10, 0)));
	thinkAi = false;

	CmdList2pAi.clear();
	m_2pAI->loadTE(*m_2pTE);

	thinkAi = true;

	return;
}

void Ai::DrawField() const {

	// 左フィールド用 
	Rect{ 150, 50, 300, 630 }
		.draw(Palette::White)
		.drawFrame(0, 1, Palette::Black);
	// 右フィールド用 
	Rect{ 790, 50, 300, 630 }
		.draw(Palette::White)
		.drawFrame(0, 1, Palette::Black);

	// 左フィールド用 
	for (int i = 0; i < 21; i++) {
		for (int j = 0; j < 10; j++) {
			Rect{ 151 + (j * 30), 51 + (i * 30), 29, 29 }
			.draw(minoC.at((size_t)m_1pTE->get_field_state(20 - i, j, 0)));
		}
	}
	// 右フィールド用 
	for (int i = 0; i < 21; i++) {
		for (int j = 0; j < 10; j++) {
			Rect{ 791 + (j * 30), 51 + (i * 30), 29, 29 }
			.draw(minoC.at((size_t)m_2pTE->get_field_state(20 - i, j, 0)));
		}
	}

	// 左フィールド用 
	for (size_t i = 0; i <= 10; i++) {
		Line{ 150 + i * 30, 50, 150 + i * 30, 681 }.draw(1, Palette::Black);
	}
	for (size_t i = 0; i <= 21; i++) {
		Line{ 150, 50 + i * 30, 451, 50 + i * 30 }.draw(1, Palette::Black);
	}
	// 右フィールド用 
	for (size_t i = 0; i <= 10; i++) {
		Line{ 790 + i * 30, 50, 790 + i * 30, 681 }.draw(1, Palette::Black);
	}
	for (size_t i = 0; i <= 21; i++) {
		Line{ 790, 50 + i * 30, 1091, 50 + i * 30 }.draw(1, Palette::Black);
	}

	return;
}

void Ai::DrawGhost() const {

	for (size_t i = 0; i < 21; i++) {
		for (size_t j = 0; j < 10; j++) {
			int fs = FieldS.at((size_t)20 - i).at(j);
			if (fs == 0)continue;
			Rect{ 791 + (j * 30), 51 + (i * 30), 29, 29 }
			.drawFrame(2, 0, minoC.at((size_t)fs));
		}

	}

	return;
}

void Ai::DrawTex1p() const {

	auto&& [bhold, n_data] = m_1pTE->get_mino_state();

	if (bhold < 0 || bhold > 7)bhold = 0;

	m_MinoTex.at(bhold).draw(20, 80);

	size_t n_size = std::min(5Ui64, n_data.size());
	for (size_t i = 0; i < n_size; i++) {
		size_t nq = (size_t)n_data.at(i) + 8;
		m_MinoTex.at(nq).draw(490, 80 + (double)i * 100);
	}

	return;
}

void Ai::DrawTex2p() const {

	auto&& [bhold, n_data] = m_2pTE->get_mino_state();

	if (bhold < 0 || bhold > 7)bhold = 0;

	m_MinoTex.at(bhold).draw(660, 80);

	size_t n_size = std::min(5Ui64, n_data.size());
	for (size_t i = 0; i < n_size; i++) {
		size_t nq = (size_t)n_data.at(i) + 8;
		m_MinoTex.at(nq).draw(1130, 80 + (double)i * 100);
	}

	return;
}
