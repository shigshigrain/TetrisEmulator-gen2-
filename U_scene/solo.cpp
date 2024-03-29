﻿# include "solo.hpp"

Solo::Solo(const InitData& init)
	: IScene{ init }
{
	m_soloTE = std::make_unique<TetriEngine>(TetriEngine());
	m_soloTE->Init(0);
	m_soloAI = std::make_unique<AiShigune>(1);
	m_soloAI->loadTE(*m_soloTE);
	m_soloAI->loadTTRP();
	//m_soloAI->thinking();

	m_bg = Texture{ U"tex\\background\\tetris_emulator_background02.bmp" };

	//m_KeyConfS = KeyConf();
	m_KeyConfS.SetDefault();

	//m_MinoTex = std::vector<Texture>(0);
	for (auto&& mp : minotex_path) {
		m_MinoTex.emplace_back(Texture{ mp });
	}

	sec_time = Time::GetMillisec();
	delay_cnt = 0;
	DAS_flame = 6;
	Wait_flame = 0;
	passed_flame = 0;
	reset_flag = false;
	suggest_flag = shig::BoolSwitch();//false
	act_flame = vector<int>(8, 0);

}

void Solo::update()
{

	if (Time::GetMillisec() - sec_time >= refrashRate60) {
		sec_time = Time::GetMillisec();
		passed_flame++;

		if (Wait_flame > 0) {
			Wait_flame += -1;
		}
		else {
			m_soloTE->ResetFieldP();
			if (reset_flag) reset_manage();
			m_KeyConfS.SetDefault();

			// テトリス側操作入力
			tetris_manage();
			// DASフレームの更新
			actF_manage();
			// ゲーム側操作入力
			game_manage();
		}

	}

	//m_KeyConf1p.SetDefault(); // キー入力情報のセット

	if (KeyQ.pressed())
	{
		changeScene(State::Title);
	}

	//m_2pTE.CopyFiledP();// 描写用セットアップ

}

void Solo::draw() const
{

	m_bg.draw(0, 0);
	draw_field();
	draw_s_field();
	draw_tex();

}

void Solo::game_manage(){

	if (IsKeyVP(m_KeyConfS, KeyVal::R)) {
		m_soloTE->CopyFiledP();
		m_soloAI->loadTE(*m_soloTE);
		Wait_flame = 40;
		reset_flag = true;
	}

	if (IsKeyVP(m_KeyConfS, KeyVal::G)) {
		m_soloTE->CopyFiledP();
		m_soloTE->StackGarbage(-1);
	}

	if (IsKeyVP(m_KeyConfS, KeyVal::I)) {
		/*if (suggest_flag)suggest_flag = false;
		else suggest_flag = true;*/
		if (suggest_flag.sw()) {
			m_soloTE->CopyFiledP();
			m_soloAI->loadTE(*m_soloTE);
			m_soloAI->thinking();
			m_soloAI->makeAiSuggestion();
		}
		
	}


}

void Solo::tetris_manage(){
	int g_check = 0;

	if (m_KeyConfS.GetKey(KeyVal::Left).pressed() && not m_KeyConfS.GetKey(KeyVal::Right).pressed()) {
		if (act_flame.at(6) == 0) {
			act_flame.at(6) = -1 * DAS_flame;
			g_check = m_soloTE->Game(6, 0);
		}
		else if (act_flame.at(6) == -1) {
			act_flame.at(6) = 1;
		}
		else if (act_flame.at(6) > 0) {
			g_check = m_soloTE->Game(6, 0);
		}
		delay_cnt = 2;
	}

	if (not m_KeyConfS.GetKey(KeyVal::Left).pressed() && m_KeyConfS.GetKey(KeyVal::Right).pressed()) {
		if (act_flame.at(7) == 0) {
			act_flame.at(7) = -DAS_flame;
			g_check = m_soloTE->Game(7, 0);
		}
		else if (act_flame.at(7) == -1) {
			act_flame.at(7) = 1;
		}
		else if (act_flame.at(7) > 0) {
			g_check = m_soloTE->Game(7, 0);
		}
		delay_cnt = 2;
	}

	if (m_KeyConfS.GetKey(KeyVal::Up).pressed() && not m_KeyConfS.GetKey(KeyVal::Z).pressed()) {
		if (act_flame.at(5) >= 0) {
			act_flame.at(5) = -DAS_flame;
			g_check = m_soloTE->Game(5, 0);
		}
		else{
			act_flame.at(5) -= 1;
		}
		delay_cnt = 2;
	}

	if (not m_KeyConfS.GetKey(KeyVal::Up).pressed() && m_KeyConfS.GetKey(KeyVal::Z).pressed()) {
		if (act_flame.at(4) >= 0) {
			act_flame.at(4) = -DAS_flame;
			g_check = m_soloTE->Game(4, 0);
		}
		else {
			act_flame.at(4) -= 1;
		}
		delay_cnt = 2;
	}

	if (m_KeyConfS.GetKey(KeyVal::C).pressed()) {
		if (act_flame.at(1) >= 0) {
			act_flame.at(1) = -2;
			g_check = m_soloTE->Game(1, 0);
		}
		else {
			act_flame.at(1) += -1;
		}
		delay_cnt = 2;

	}

	if (m_KeyConfS.GetKey(KeyVal::Down).pressed()) {
		if (act_flame.at(2) >= 0) {
			act_flame.at(2) = -1;
			g_check = m_soloTE->Game(2, 0);
 		}
		else {
			act_flame.at(2) = 0;
		}
		delay_cnt = 2;
	}

	if (m_KeyConfS.GetKey(KeyVal::Space).pressed()) {
		if (act_flame.at(3) >= 0) {
			act_flame.at(3) = -2;
			g_check = m_soloTE->Game(3, 0);
			delay_cnt = m_soloTE->get_delayF();
		}
		else {
			act_flame.at(3) += -1;
		}
		delay_cnt = 2;
		
		if (suggest_flag.get()) {
			m_soloAI->loadTE(*m_soloTE);
			m_soloAI->thinking();
			m_soloAI->makeAiSuggestion();
			//Print << U"thinking";
		}

	}

	if (g_check == 2) {
		Wait_flame = m_soloTE->get_delayF();
		delay_cnt = 0;
	}
	else if(g_check == 1) {
		m_soloTE->CopyFiledP();
		reset_flag = true;
		Wait_flame = 72;
	}
	else if (g_check == 0) {
		m_soloTE->CopyFiledP();
	}




	return;
}


void Solo::actF_manage() {

	for (auto&& i : act_flame) {
		i++;
		if (i > 0x11111110)i = 1;
	}

	if (not IsKeyVP(m_KeyConfS, KeyVal::Right) && not IsKeyVP(m_KeyConfS, KeyVal::Left)) {
		act_flame.at(6) = 0;
		act_flame.at(7) = 0;
	}

	if (IsKeyVP(m_KeyConfS, KeyVal::Right) && IsKeyVP(m_KeyConfS, KeyVal::Left)) {
		act_flame.at(6) = 1;
		act_flame.at(7) = 1;
	}

	return;
}


void Solo::reset_manage(){

	m_soloTE->SetField();
	m_soloTE->CopyFiledP();
	delay_cnt = 0;
	DAS_flame = 6;
	Wait_flame = 0;
	reset_flag = false;
	act_flame = vector<int>(8, 0);
	m_soloAI->loadTE(*m_soloTE);
	if (suggest_flag.get()) {
		m_soloAI->thinking();
		m_soloAI->makeAiSuggestion();
	}
	//suggest_flag.Setup(false);

	return;
}

void Solo::draw_field() const{

	Rect{ 200, 50, 300, 630 }
		.draw(Palette::White)
		.drawFrame(0, 1, Palette::Black);

	for (int i = 0; i < 21; i++) {
		for (int j = 0; j < 10; j++) {
			Rect{ 201 + (j * 30), 51 + (i * 30), 29, 29 }
			.draw(minoC.at(m_soloTE->get_field_state(20 - i, j, 0)));
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

	vector<vector<int>> SF = m_soloAI->getSuggestionAi();

	for (int i = 0; i < 21; i++) {
		for (int j = 0; j < 10; j++) {
			if (SF.at((size_t)20 - i).at(j) == 0)continue;
			Rect{ 201 + (j * 30), 51 + (i * 30), 29, 29 }
			.drawFrame(2, 0, minoC.at(SF.at((size_t)20 - i).at(j)));
		}

	}

	return;
}

void Solo::draw_tex() const{

	auto&& [bhold, n_data] = m_soloTE->get_mino_state();

	if (bhold < 0 || bhold > 7)bhold = 0;

	m_MinoTex.at(bhold).draw(50, 80);

	int n_size = std::min(5, (int)n_data.size());
	for (int i = 0; i < n_size; i++) {
		int nq = n_data.at(i) + 8;
		m_MinoTex.at(nq).draw(525, 80 + i * 100);
	}

	return;
}
