#include "ai.hpp"


Ai::Ai(const InitData& init)
	: IScene{ init }
{

	m_soloTE = TetriEngine(2);
	m_soloAI.loadTE(m_soloTE);
	m_soloAI.load_ttrp();
	m_soloAI.thinking();

	m_bg = Texture{ U"tex\\background\\tetris_emulator_background02.bmp" };

	m_KeyConfS = KeyConf();

	m_MinoTex = vector<Texture>(minotex_path_size);
	for (int i = 0; i < minotex_path_size; i++) {
		m_MinoTex.at(i) = Texture{ minotex_path.at(i) };
	}

	sec_time = Time::GetMillisec();
	delay_cnt = 0;
	DAS_flame = 5;
	Wait_flame = 0;
	passed_flame = 0;
	reset_flag = false;
	suggest_flag = shig::BoolSwitch();//false
	act_flame = vector<int>(8, 0);

}

void Ai::update()
{

	if (Time::GetMillisec() - sec_time >= refrashRate60) {
		sec_time = Time::GetMillisec();
		passed_flame++;

		if (Wait_flame > 0) {
			Wait_flame += -1;
		}
		else {

			/*if (delay_cnt > 0) {
				delay_cnt += -1;

			}
			else {


			}*/

			m_soloTE.reset_pfield();

			if (reset_flag) {
				reset_manage();
			}

			m_KeyConfS.set_defalut();

			// テトリス側操作入力
			tetris_manage();
			// DASフレームの更新
			actF_manage();
			// ゲーム側操作入力
			game_manage();

		}



	}

	//m_KeyConfS.set_defalut(); // キー入力情報のセット

	if (KeyQ.pressed())
	{
		changeScene(State::Title);
	}

	//m_soloTE.copy_pfield();// 描写用セットアップ

}

void Ai::draw() const
{

	m_bg.draw(0, 0);
	draw_field();
	draw_s_field();
	draw_tex();

}

void Ai::game_manage() {

	if (IsKeyVP(m_KeyConfS, KeyVal::R)) {
		m_soloTE.copy_pfield();
		Wait_flame = 30;
		reset_flag = true;
	}

	if (IsKeyVP(m_KeyConfS, KeyVal::G)) {
		m_soloTE.copy_pfield();
		m_soloTE.edit_garbage_cmd(1);
	}

	if (IsKeyVP(m_KeyConfS, KeyVal::I)) {
		/*if (suggest_flag)suggest_flag = false;
		else suggest_flag = true;*/
		if (suggest_flag.sw()) {
			m_soloAI.thinking();
			m_soloAI.make_AI_suggestion();
		}

	}


}

void Ai::tetris_manage() {
	int g_check = 0;

	if (m_KeyConfS.get_key(KeyVal::Left).pressed() && not m_KeyConfS.get_key(KeyVal::Right).pressed()) {
		if (act_flame[6] == 0) {
			act_flame[6] = -1 * DAS_flame;
			g_check = m_soloTE.game(6, 0);
		}
		else if (act_flame[6] == -1) {
			act_flame[6] = 1;
		}
		else if (act_flame[6] > 0) {
			g_check = m_soloTE.game(6, 0);
		}
		delay_cnt = 2;
	}

	if (not m_KeyConfS.get_key(KeyVal::Left).pressed() && m_KeyConfS.get_key(KeyVal::Right).pressed()) {
		if (act_flame[7] == 0) {
			act_flame[7] = -DAS_flame;
			g_check = m_soloTE.game(7, 0);
		}
		else if (act_flame[7] == -1) {
			act_flame[7] = 1;
		}
		else if (act_flame[7] > 0) {
			g_check = m_soloTE.game(7, 0);
		}
		delay_cnt = 2;
	}

	if (m_KeyConfS.get_key(KeyVal::Up).pressed() && not m_KeyConfS.get_key(KeyVal::Z).pressed()) {
		if (act_flame[5] >= 0) {
			act_flame[5] = -DAS_flame;
			g_check = m_soloTE.game(5, 0);
		}
		else {
			act_flame[5] -= 1;
		}
		delay_cnt = 2;
	}

	if (not m_KeyConfS.get_key(KeyVal::Up).pressed() && m_KeyConfS.get_key(KeyVal::Z).pressed()) {
		if (act_flame[4] >= 0) {
			act_flame[4] = -DAS_flame;
			g_check = m_soloTE.game(4, 0);
		}
		else {
			act_flame[4] -= 1;
		}
		delay_cnt = 2;
	}

	if (m_KeyConfS.get_key(KeyVal::C).pressed()) {
		if (act_flame[1] >= 0) {
			act_flame[1] = -2;
			g_check = m_soloTE.game(1, 0);
		}
		else {
			act_flame[1] += -1;
		}
		delay_cnt = 2;

	}

	if (m_KeyConfS.get_key(KeyVal::Down).pressed()) {
		if (act_flame[2] >= 0) {
			act_flame[2] = -1;
			g_check = m_soloTE.game(2, 0);
		}
		else {
			act_flame[2] = 0;
		}
		delay_cnt = 2;
	}

	if (m_KeyConfS.get_key(KeyVal::Space).pressed()) {
		if (act_flame[3] >= 0) {
			act_flame[3] = -2;
			g_check = m_soloTE.game(3, 0);
			delay_cnt = m_soloTE.get_delayF();
		}
		else {
			act_flame[3] += -1;
		}
		delay_cnt = 2;

		if (suggest_flag.get()) {
			m_soloAI.loadTE(m_soloTE);
			m_soloAI.thinking();
			m_soloAI.make_AI_suggestion();
			//Print << U"thinking";
		}

	}

	if (g_check == 2) {
		Wait_flame = m_soloTE.get_delayF();
		delay_cnt = 0;
	}
	else if (g_check == 1) {
		m_soloTE.copy_pfield();
		reset_flag = true;
		Wait_flame = 72;
	}
	else if (g_check == 0) {
		m_soloTE.copy_pfield();
	}




	return;
}

void Ai::reset_manage() {

	m_soloTE.set_field();
	delay_cnt = 0;
	DAS_flame = 7;
	Wait_flame = 0;
	reset_flag = false;
	act_flame = vector<int>(8, 0);
	m_soloAI.loadTE(m_soloTE);
	m_soloAI.thinking();
	//m_soloAI.make_AI_suggestion();

	return;
}

void Ai::draw_field() const {

	Rect{ 200, 50, 300, 630 }
		.draw(Palette::White)
		.drawFrame(0, 1, Palette::Black);

	for (int i = 0; i < 11; i++) {
		Line{ 200 + i * 30, 50, 200 + i * 30, 681 }.draw(1, Palette::Black);
	}
	for (int i = 0; i < 22; i++) {
		Line{ 200, 50 + i * 30, 501, 50 + i * 30 }.draw(1, Palette::Black);
	}

	for (int i = 0; i < 21; i++) {
		for (int j = 0; j < 10; j++) {
			Rect{ 201 + (j * 30), 51 + (i * 30), 29, 29 }
			.draw(minoC.at(m_soloTE.get_field_state(20 - i, j, 0)));
		}

	}

	return;
}

void Ai::draw_s_field() const {

	vector<vector<int>> SF = m_soloAI.get_AI_suggestion();

	for (int i = 0; i < 21; i++) {
		for (int j = 0; j < 10; j++) {
			if (SF.at(20 - i).at(j) == 0)continue;
			Rect{ 201 + (j * 30), 51 + (i * 30), 29, 29 }
			.drawFrame(2, 0, minoC.at(SF.at(20 - i).at(j)));
		}

	}

	return;
}

void Ai::actF_manage() {

	for (auto& i : act_flame) {
		i++;
		if (i > 0x10000000)i = 1;
	}

	if (not IsKeyVP(m_KeyConfS, KeyVal::Right) && not IsKeyVP(m_KeyConfS, KeyVal::Left)) {
		act_flame[6] = 0;
		act_flame[7] = 0;
	}

	if (IsKeyVP(m_KeyConfS, KeyVal::Right) && IsKeyVP(m_KeyConfS, KeyVal::Left)) {
		act_flame[6] = 1;
		act_flame[7] = 1;
	}

	return;
}

void Ai::draw_tex() const {

	pair<int, deque<int>> data = m_soloTE.get_mino_state();
	int bhold = data.first; if (bhold < 0 || bhold > 7)bhold = 0;

	m_MinoTex.at(bhold).draw(50, 80);

	int n_size = min(5, (int)data.second.size());
	for (int i = 0; i < n_size; i++) {
		int nq = data.second.at(i) + 8;
		m_MinoTex.at(nq).draw(525, 80 + i * 100);
	}

	return;
}
