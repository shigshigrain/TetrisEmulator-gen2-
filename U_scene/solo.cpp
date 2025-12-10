# include "U_scene/solo.hpp"

Solo::Solo(const InitData& init)
	: IScene{ init }
{
	TEp1 = std::make_unique<TetriEngine>(TetriEngine(1));
	TEp1->Init(0);
	AIp1 = std::make_unique<AiShigune>(1);
	AIp1->loadTE(*TEp1);
	AIp1->loadTTRP();

	m_bg = Texture{ U"tex\\background\\tetris_emulator_background02.bmp" };

	KeyConfp1 = make_unique<KeyConf>();
	KeyConfp1->SetDefault();

	for (auto&& mp : minotex_path) {
		m_MinoTex.emplace_back(Texture{ mp });
	}

	sec_time = Time::GetMillisec();
	delay_cnt = 0;
	DASFlame = 6;
	WaitFlame = 0;
	PassedFlame = 0;
	ResetFlag = false;
	suggest_flag = shig::BoolSwitch();//false
	ActFlame = vector<int>(8, 0);
	FieldS1 = std::vector<std::vector<int8_t>>(shig::fH, (std::vector<int8_t>(shig::fW, 0)));
	abortAIp1 = { false };
	thinkAIp1 = { false };
	CmdListAIp1 = std::deque<int>(0);
	// AI起動 
	asyncAIp1 = s3d::Async(shig::ExeThinking, ref(*AIp1), ref(abortAIp1), ref(thinkAIp1), ref(CmdListAIp1));

}

void Solo::update()
{

	if ((Time::GetMillisec() - sec_time) >= refrashRate60) {
		sec_time = Time::GetMillisec();
		PassedFlame++;
		KeyConfp1->SetDefault();

		if (WaitFlame > 0) {
			WaitFlame--;
		}
		else {
			TEp1->ResetFieldP();
			if (ResetFlag) reset_manage();
			

			// テトリス側操作入力
			tetris_manage();
			// DASフレームの更新
			actF_manage();
			// ゲーム側操作入力
			game_manage();
		}

	}

	//KeyConfp1.SetDefault(); // キー入力情報のセット

	if (KeyQ.pressed() or KeyEscape.pressed())
	{
		thinkAIp1 = false;
		abortAIp1 = true;
		// 非同期処理の終了を待機 
		if (asyncAIp1.isValid())asyncAIp1.wait();
		changeScene(State::Title);
	}


}

void Solo::draw() const
{

	m_bg.draw(0, 0);
	draw_field();
	draw_s_field();
	draw_tex();
	draw_state();

}

Solo::~Solo()
{
	thinkAIp1 = false;
	abortAIp1 = true;
	// 非同期処理の終了を待機 
	if (asyncAIp1.isValid())asyncAIp1.wait();
}

void Solo::game_manage(){

	if (IsKeyVP(*KeyConfp1, KeyVal::R)) {
		TEp1->CopyFiledP();
		AIp1->loadTE(*TEp1);
		WaitFlame = 40;
		ResetFlag = true;
	}

	if (IsKeyVP(*KeyConfp1, KeyVal::G)) {
		TEp1->CopyFiledP();
		TEp1->StackGarbage(-1);
	}

	if (IsKeyVP(*KeyConfp1, KeyVal::M)) {
		if (ActFlame.at(0) >= 0) {
			ActFlame.at(0) = -30;
			suggest_flag.sw();
		}
	}

	if (IsKeyVP(*KeyConfp1, KeyVal::I)) {
		/*if (suggest_flag)suggest_flag = false;
		else suggest_flag = true;*/
		if (suggest_flag.sw()) {
			TEp1->CopyFiledP();
			AIp1->loadTE(*TEp1);
			AIp1->thinking();
			AIp1->makeAiSuggestion();
		}
		
	}


}

void Solo::tetris_manage(){
	int g_check = 0;

	if (suggest_flag.get()) {
		// 非同期処理側で推奨手計算が終了している場合
		if (!thinkAIp1) {
			FieldS1 = AIp1->getSuggestionAi();
			if (!CmdListAIp1.empty()) {
				g_check = TEp1->Game(CmdListAIp1.front(), 0);
				WaitFlame = 0;
				if (CmdListAIp1.front() == 3) {
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
	else {
		if (KeyConfp1->GetKey(KeyVal::Left).pressed() && not KeyConfp1->GetKey(KeyVal::Right).pressed()) {
			if (ActFlame.at(6) == 0) {
				ActFlame.at(6) = -1 * DASFlame;
				g_check = TEp1->Game(6, 0);
			}
			else if (ActFlame.at(6) == -1) {
				ActFlame.at(6) = 1;
			}
			else if (ActFlame.at(6) > 0) {
				g_check = TEp1->Game(6, 0);
			}
			delay_cnt = 2;
		}

		if (not KeyConfp1->GetKey(KeyVal::Left).pressed() && KeyConfp1->GetKey(KeyVal::Right).pressed()) {
			if (ActFlame.at(7) == 0) {
				ActFlame.at(7) = -DASFlame;
				g_check = TEp1->Game(7, 0);
			}
			else if (ActFlame.at(7) == -1) {
				ActFlame.at(7) = 1;
			}
			else if (ActFlame.at(7) > 0) {
				g_check = TEp1->Game(7, 0);
			}
			delay_cnt = 2;
		}

		if (KeyConfp1->GetKey(KeyVal::Up).pressed() && not KeyConfp1->GetKey(KeyVal::Z).pressed()) {
			if (ActFlame.at(5) >= 0) {
				ActFlame.at(5) = -DASFlame;
				g_check = TEp1->Game(5, 0);
			}
			else {
				ActFlame.at(5) -= 1;
			}
			delay_cnt = 2;
		}

		if (not KeyConfp1->GetKey(KeyVal::Up).pressed() && KeyConfp1->GetKey(KeyVal::Z).pressed()) {
			if (ActFlame.at(4) >= 0) {
				ActFlame.at(4) = -DASFlame;
				g_check = TEp1->Game(4, 0);
			}
			else {
				ActFlame.at(4) -= 1;
			}
			delay_cnt = 2;
		}

		if (KeyConfp1->GetKey(KeyVal::C).pressed()) {
			if (ActFlame.at(1) >= 0) {
				ActFlame.at(1) = -2;
				g_check = TEp1->Game(1, 0);
			}
			else {
				ActFlame.at(1) += -1;
			}
			delay_cnt = 2;

		}

		if (KeyConfp1->GetKey(KeyVal::Down).pressed()) {
			if (ActFlame.at(2) >= 0) {
				ActFlame.at(2) = -1;
				g_check = TEp1->Game(2, 0);
			}
			else {
				ActFlame.at(2) = 0;
			}
			delay_cnt = 2;
		}

		if (KeyConfp1->GetKey(KeyVal::Space).pressed()) {
			if (ActFlame.at(3) >= 0) {
				ActFlame.at(3) = -2;
				g_check = TEp1->Game(3, 0);
				delay_cnt = TEp1->get_delayF();
			}
			else {
				ActFlame.at(3) += -1;
			}
			delay_cnt = 2;

			if (suggest_flag.get()) {
				AIp1->loadTE(*TEp1);
				AIp1->thinking();
				AIp1->makeAiSuggestion();
			}

		}
	}

	switch (g_check)
	{
	case 2:
		WaitFlame = TEp1->get_delayF();
		//WaitFlame = 0;
		delay_cnt = 0;
		break;
	case 1:
		TEp1->CopyFiledP();
		ResetFlag = true;
		WaitFlame = 30;
		break;
	case 0:
		TEp1->CopyFiledP();
		break;
	default:
		break;
	}

	/*if (g_check == 2) {
		WaitFlame = TEp1->get_delayF();
		delay_cnt = 0;
	}
	else if(g_check == 1) {
		TEp1->CopyFiledP();
		ResetFlag = true;
		WaitFlame = 72;
	}
	else if (g_check == 0) {
		TEp1->CopyFiledP();
	}*/

	return;
}


void Solo::actF_manage() {

	for (auto&& i : ActFlame) {
		i++;
		if (i > 0x11111110)i = 1;
	}

	if (not IsKeyVP(*KeyConfp1, KeyVal::Right) && not IsKeyVP(*KeyConfp1, KeyVal::Left)) {
		ActFlame.at(6) = 0;
		ActFlame.at(7) = 0;
	}

	if (IsKeyVP(*KeyConfp1, KeyVal::Right) && IsKeyVP(*KeyConfp1, KeyVal::Left)) {
		ActFlame.at(6) = 1;
		ActFlame.at(7) = 1;
	}

	return;
}


void Solo::reset_manage(){

	TEp1->SetField();
	TEp1->CopyFiledP();
	delay_cnt = 0;
	DASFlame = 6;
	WaitFlame = 0;
	ResetFlag = false;
	ActFlame = vector<int>(8, 0);
	FieldS1 = std::vector<std::vector<int8_t>>(shig::fH, (std::vector<int8_t>(10, 0)));
	thinkAIp1 = false;

	CmdListAIp1.clear();
	AIp1->loadTE(*TEp1);

	thinkAIp1 = true;

	return;
}

void Solo::draw_field() const{

	Rect{ 200, 50, 300, 630 }
		.draw(Palette::White)
		.drawFrame(0, 1, Palette::Black);

	for (int i = 0; i < 21; i++) {
		for (int j = 0; j < 10; j++) {
			Rect{ 201 + (j * 30), 51 + (i * 30), 29, 29 }
			.draw(minoC.at(TEp1->GetFieldBlock(20 - i, j, 0)));
		}

	}

	for (int i = 0; i < 11; i++) {
		Line{ 200 + i * 30, 50, 200 + i * 30, 681 }.draw(1, Palette::Black);
	}
	for (int i = 0; i < 22; i++) {
		Line{ 200, 50 + i * 30, 501, 50 + i * 30 }.draw(1, Palette::Black);
	}

	return;
}

void Solo::draw_s_field() const{

	

	for (int i = 0; i < 21; i++) {
		for (int j = 0; j < 10; j++) {
			if (FieldS1.at((size_t)20 - i).at(j) == 0)continue;
			Rect{ 201 + (j * 30), 51 + (i * 30), 29, 29 }
			.drawFrame(2, 0, minoC.at(FieldS1.at((size_t)20 - i).at(j)));
		}

	}

	return;
}

void Solo::draw_tex() const{

	auto&& [bhold, n_data] = TEp1->get_mino_state();

	if (bhold < 0 || bhold > 7)bhold = 0;

	m_MinoTex.at(bhold).draw(50, 80);

	int n_size = std::min(5, (int)n_data.size());
	for (int i = 0; i < n_size; i++) {
		int nq = n_data.at(i) + 8;
		m_MinoTex.at(nq).draw(525, 80 + i * 100);
	}

	return;
}

void Solo::draw_state() const
{

	std::deque<std::string> mino_his = TEp1->get_mino_his();

	int i = 1;
	for (auto&& ms : mino_his) {

		s3d::String deb_his = s3d::Unicode::Widen(std::to_string(i)) + U" : " + s3d::Unicode::Widen(ms);

		FontAsset(U"Debug")(deb_his).draw(s3d::Vec2{ 20, 360 + 20 * i }, Color(0, 0, 0));
		i++;

	}

	s3d::String stateTS = s3d::Unicode::Widen(TEp1->GetTSstring());

	FontAsset(U"Debug")(stateTS).draw(s3d::Vec2{ 20, 560 }, Color(0, 0, 0));

	return;

}
