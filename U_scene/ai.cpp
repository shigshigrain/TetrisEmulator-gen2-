#include "ai.hpp"

Ai::Ai(const InitData& init)
	: IScene{ init }
{
	TEp1 = std::make_unique<TetriEngine>(1);
	TEp1->Init(1);
	AIp1 = std::make_unique<AiShigune>(1);
	AIp1->loadTE(*TEp1);
	AIp1->loadTTRP();
	//
	TEp2 = std::make_unique<TetriEngine>(2);
	TEp2->Init(2);
	AIp2 = std::make_unique<AiShigune>(2);
	AIp2->loadTE(*TEp2);
	AIp2->loadTTRP();
	
	m_bg = Texture{ U"tex\\background\\tetris_emulator_background02.bmp" };

	KeyConfp1 = make_unique<KeyConf>();
	KeyConfp1->SetDefault();

	for (auto&& mp : minotex_path) {
		m_MinoTex.emplace_back(Texture{mp});
	}

	sec_time = Time::GetMillisec();
	delay_cnt = 0;
	DASFlame1p = 6;
	WaitFlame1p = 0;
	WaitFlame2p = 0;
	PassedFlame = 0;
	ResetFlag = false;
	FieldS1 = std::vector<std::vector<int8_t>>(shig::fH, (std::vector<int8_t>(10, 0)));
	FieldS2 = std::vector<std::vector<int8_t>>(shig::fH, (std::vector<int8_t>(10, 0)));
	ActFlame = std::vector<int>(8, 0);
	abortAIp1 = { false };
	thinkAIp1 = { false };
	abortAIp2 = { false };
	thinkAIp2 = { false };
	CmdListAIp1 = std::deque<int>(0);
	CmdListAIp2 = std::deque<int>(0);

	// AI起動 
	asyncAIp1 = s3d::Async(shig::ExeThinking, ref(*AIp1), ref(abortAIp1), ref(thinkAIp1), ref(CmdListAIp1));
	asyncAIp2 = s3d::Async(shig::ExeThinking, ref(*AIp2), ref(abortAIp2), ref(thinkAIp2), ref(CmdListAIp2));

	ResetManage();

}

void Ai::update()
{

	if ((Time::GetMillisec() - sec_time) >= refrashRate60) {
		sec_time = Time::GetMillisec();
		PassedFlame++;
		KeyConfp1->SetDefault(); // キー入力情報のセット

		// 1p Ai
		if (WaitFlame1p > 0) {
			WaitFlame1p--;
		}
		else {
			TEp1->ResetFieldP();
			
			if (ResetFlag) ResetManage();

			// テトリス側操作入力
			TetrisManage1p();
			// DASフレームの更新
			InputFlameManage();
			// ゲーム側操作入力
			GameManage1p();
			
		}
		// 2p AI側 
		if (WaitFlame2p > 0) {
			WaitFlame2p--;
		}
		else {
			TEp2->ResetFieldP();
			if (ResetFlag) ResetManage();
			// テトリス側操作入力
			TetrisManage2p();
			// ゲーム側操作入力
			GameManage2p();
			
		}

	}

	if (KeyQ.pressed() or KeyEscape.pressed())
	{
		thinkAIp1 = false;
		abortAIp1 = true;
		thinkAIp2 = false;
		abortAIp2 = true;
		// 非同期処理の終了を待機 
		if (asyncAIp1.isValid())asyncAIp1.wait();
		if (asyncAIp2.isValid())asyncAIp2.wait();
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
	DrawState();
}

Ai::~Ai()
{
	thinkAIp1 = false;
	abortAIp1 = true;
	thinkAIp2 = false;
	abortAIp2 = true;
	// 非同期処理の終了を待機 
	if (asyncAIp1.isValid())asyncAIp1.wait();
	if (asyncAIp2.isValid())asyncAIp2.wait();
}

void Ai::GameManage1p() {

	if (IsKeyVP(*KeyConfp1, KeyVal::R)) {
		TEp1->CopyFiledP();
		TEp2->CopyFiledP();
		WaitFlame1p = 40;
		WaitFlame2p = 40;
		ResetFlag = true;
	}

	if (IsKeyVP(*KeyConfp1, KeyVal::G)) {
		TEp1->CopyFiledP();
		TEp1->StackGarbage(-1);

	}

}

void Ai::GameManage2p()
{
	if (IsKeyVP(*KeyConfp1, KeyVal::M)) {
		if (ActFlame.at(0) >= 0) {
			ActFlame.at(0) = -30;
			suggest_flag.sw();
		}
	}

	if (IsKeyVP(*KeyConfp1, KeyVal::O)) {
		TEp2->CopyFiledP();
		TEp2->StackGarbage(-1);
	}

}

void Ai::TetrisManage1p() {

	int g_check = 0;

	if (suggest_flag.get()) {
		// 非同期処理側で推奨手計算が終了している場合
		if (!thinkAIp1) {
			FieldS1 = AIp1->getSuggestionAi();
			if (!CmdListAIp1.empty()) {
				g_check = TEp1->Game(CmdListAIp1.front(), 0);
				WaitFlame1p = 0;
				if (CmdListAIp1.front() == 3) {
					//TEp1->StackGarbage((int)(TEp2->getGarbage() / 2));
					TEp2->StackGarbage(TEp1->GetGarbage());
					TEp1->GetGarbage();
				}
				CmdListAIp1.pop_front();
				// 操作をし終わったタイミングで先に思考開始
				if (CmdListAIp1.empty()) {
					AIp1->loadTE(*TEp1);
					thinkAIp1 = true;
				}
			}
			else {
				thinkAIp1 = true;
			}
		}
		else if (thinkAIp1) {
			// することがない 
		}
	}

	switch (g_check)
	{
	case 2:
		WaitFlame1p = TEp1->get_delayF();
		//WaitFlame1p = 0;
		delay_cnt = 0;
		break;
	case 1:
		TEp1->CopyFiledP();
		ResetFlag = true;
		WaitFlame1p = 30;
		break;
	case 0:
		TEp1->CopyFiledP();
		break;
	default:
		break;
	}

	//if (KeyConfp1->GetKey(KeyVal::Left).pressed() && not KeyConfp1->GetKey(KeyVal::Right).pressed()) {
	//	if (ActFlame.at(6) == 0) {
	//		ActFlame.at(6) = -1 * DASFlame1p;
	//		g_check = TEp1->Game(6, 0);
	//	}
	//	else if (ActFlame.at(6) == -1) {
	//		ActFlame.at(6) = 1;
	//	}
	//	else if (ActFlame.at(6) > 0) {
	//		g_check = TEp1->Game(6, 0);
	//	}
	//	delay_cnt = 2;
	//}
	//if (not KeyConfp1->GetKey(KeyVal::Left).pressed() && KeyConfp1->GetKey(KeyVal::Right).pressed()) {
	//	if (ActFlame.at(7) == 0) {
	//		ActFlame.at(7) = -DASFlame1p;
	//		g_check = TEp1->Game(7, 0);
	//	}
	//	else if (ActFlame.at(7) == -1) {
	//		ActFlame.at(7) = 1;
	//	}
	//	else if (ActFlame.at(7) > 0) {
	//		g_check = TEp1->Game(7, 0);
	//	}
	//	delay_cnt = 2;
	//}
	//if (KeyConfp1->GetKey(KeyVal::Up).pressed() && not KeyConfp1->GetKey(KeyVal::Z).pressed()) {
	//	if (ActFlame.at(5) >= 0) {
	//		ActFlame.at(5) = -DASFlame1p;
	//		g_check = TEp1->Game(5, 0);
	//	}
	//	else {
	//		ActFlame.at(5) -= 1;
	//	}
	//	delay_cnt = 2;
	//}
	//if (not KeyConfp1->GetKey(KeyVal::Up).pressed() && KeyConfp1->GetKey(KeyVal::Z).pressed()) {
	//	if (ActFlame.at(4) >= 0) {
	//		ActFlame.at(4) = -DASFlame1p;
	//		g_check = TEp1->Game(4, 0);
	//	}
	//	else {
	//		ActFlame.at(4) -= 1;
	//	}
	//	delay_cnt = 2;
	//}
	//if (KeyConfp1->GetKey(KeyVal::C).pressed()) {
	//	if (ActFlame.at(1) >= 0) {
	//		ActFlame.at(1) = -2;
	//		g_check = TEp1->Game(1, 0);
	//	}
	//	else {
	//		ActFlame.at(1) += -1;
	//	}
	//	delay_cnt = 2;
	//}
	//if (KeyConfp1->GetKey(KeyVal::Down).pressed()) {
	//	if (ActFlame.at(2) >= 0) {
	//		ActFlame.at(2) = -1;
	//		g_check = TEp1->Game(2, 0);
	//	}
	//	else {
	//		ActFlame.at(2) = 0;
	//	}
	//	delay_cnt = 2;
	//}
	//if (KeyConfp1->GetKey(KeyVal::Space).pressed()) {
	//	if (ActFlame.at(3) >= 0) {
	//		ActFlame.at(3) = -2;
	//		g_check = TEp1->Game(3, 0);
	//		TEp2->StackGarbage(TEp1->getGarbage());
	//		//delay_cnt = TEp1->get_delayF();
	//	}
	//	else {
	//		ActFlame.at(3) += -1;
	//	}
	//	//delay_cnt = 2;
	//}
	//if (g_check == 2) {
	//	WaitFlame1p = TEp1->get_delayF();
	//	delay_cnt = 0;
	//}
	//else if (g_check == 1) {
	//	TEp1->CopyFiledP();
	//	ResetFlag = true;
	//	WaitFlame1p = 30;
	//}
	//else if (g_check == 0) {
	//	TEp1->CopyFiledP();
	//}

	return;
}

void Ai::TetrisManage2p()
{
	int g_check = 0;

	if (suggest_flag.get()) {
		// 非同期処理側で推奨手計算が終了している場合
		if (!thinkAIp2) {
			FieldS2 = AIp2->getSuggestionAi();
			if (!CmdListAIp2.empty()) {
				g_check = TEp2->Game(CmdListAIp2.front(), 0);
				WaitFlame2p = 0;
				if (CmdListAIp2.front() == 3) {
					//TEp1->StackGarbage((int)(TEp2->getGarbage() / 2));
					TEp1->StackGarbage(TEp2->GetGarbage());
					TEp2->GetGarbage();
				}

				CmdListAIp2.pop_front();
				// 操作をし終わったタイミングで先に思考開始
				if (CmdListAIp2.empty()) {
					AIp2->loadTE(*TEp2);
					thinkAIp2 = true;
				}
			}
			else {
				thinkAIp2 = true;
			}
		}
		else if (thinkAIp2) {
			// することがない 
		}
	}

	switch (g_check)
	{
	case 2:
		WaitFlame2p = TEp2->get_delayF();
		//WaitFlame2p = 0;
		delay_cnt = 0;
		break;
	case 1:
		TEp2->CopyFiledP();
		ResetFlag = true;
		WaitFlame2p = 30;
		break;
	case 0:
		TEp2->CopyFiledP();
		break;
	default:
		break;
	}

	return;

}

void Ai::InputFlameManage() {

	for (auto&& af : ActFlame) {
		if (af <= 0x11111110)af++;
	}

	if (not IsKeyVP(*KeyConfp1, KeyVal::Right) and not IsKeyVP(*KeyConfp1, KeyVal::Left)) {
		ActFlame.at(6) = 0;
		ActFlame.at(7) = 0;
	}

	if (IsKeyVP(*KeyConfp1, KeyVal::Right) and IsKeyVP(*KeyConfp1, KeyVal::Left)) {
		ActFlame.at(6) = 1;
		ActFlame.at(7) = 1;
	}

	return;
}

void Ai::ResetManage() {

	TEp1->SetField();
	TEp1->CopyFiledP();

	TEp2->SetField();
	TEp2->CopyFiledP();

	delay_cnt = 0;
	DASFlame1p = 6;
	WaitFlame1p = 30;
	WaitFlame2p = 30;
	ResetFlag = false;
	ActFlame = std::vector<int>(8, 0);
	FieldS1 = std::vector<std::vector<int8_t>>(shig::fH, (std::vector<int8_t>(10, 0)));
	thinkAIp1 = false;
	FieldS2 = std::vector<std::vector<int8_t>>(shig::fH, (std::vector<int8_t>(10, 0)));
	thinkAIp2 = false;

	CmdListAIp1.clear();
	AIp1->loadTE(*TEp1);
	CmdListAIp2.clear();
	AIp2->loadTE(*TEp2);

	thinkAIp1 = true;
	thinkAIp2 = true;

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
			.draw(minoC.at((size_t)TEp1->GetFieldBlock(20 - i, j, 0)));
		}
	}
	// 右フィールド用 
	for (int i = 0; i < 21; i++) {
		for (int j = 0; j < 10; j++) {
			Rect{ 791 + (j * 30), 51 + (i * 30), 29, 29 }
			.draw(minoC.at((size_t)TEp2->GetFieldBlock(20 - i, j, 0)));
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
			int fs = FieldS1.at((size_t)20 - i).at(j);
			if (fs == 0)continue;
			Rect{ 151 + (j * 30), 51 + (i * 30), 29, 29 }
			.drawFrame(2, 0, minoC.at((size_t)fs));
		}

	}

	for (size_t i = 0; i < 21; i++) {
		for (size_t j = 0; j < 10; j++) {
			int fs = FieldS2.at((size_t)20 - i).at(j);
			if (fs == 0)continue;
			Rect{ 791 + (j * 30), 51 + (i * 30), 29, 29 }
			.drawFrame(2, 0, minoC.at((size_t)fs));
		}

	}

	return;
}

void Ai::DrawTex1p() const {

	auto&& [bhold, n_data] = TEp1->get_mino_state();

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

	auto&& [bhold, n_data] = TEp2->get_mino_state();

	if (bhold < 0 || bhold > 7)bhold = 0;

	m_MinoTex.at(bhold).draw(660, 80);

	size_t n_size = std::min(5Ui64, n_data.size());
	for (size_t i = 0; i < n_size; i++) {
		size_t nq = (size_t)n_data.at(i) + 8;
		m_MinoTex.at(nq).draw(1130, 80 + (double)i * 100);
	}

	return;
}

void Ai::DrawState() const
{

	s3d::String stateTS1 = s3d::Unicode::Widen(TEp1->GetTSstring());
	s3d::String stateTS2 = s3d::Unicode::Widen(TEp2->GetTSstring());

	FontAsset(U"Debug")(stateTS1).draw(s3d::Vec2{  20, 560 }, Color(0, 0, 0));
	FontAsset(U"Debug")(stateTS2).draw(s3d::Vec2{ 660, 560 }, Color(0, 0, 0));

	return;

}
