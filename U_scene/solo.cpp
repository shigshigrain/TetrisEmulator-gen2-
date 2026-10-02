# include "U_scene/solo.hpp"

Solo::Solo(const InitData& init)
	: IScene{ init }
{
	TEp1 = std::make_unique<shig::TetriEngine>(1);
	TEp1->Init(0);
	AIp1 = std::make_unique<shig::AiShigune>(1);
	AIp1->loadTE(*TEp1);

	m_bg = s3d::Texture{ U"tex\\background\\background04D.png" };

	KeyConfp1 = make_unique<KeyConf>();
	KeyConfp1->SetDefault();

	for (auto&& mp : minotex_path)
	{
		m_MinoTex.emplace_back(mp);
	}

	sec_time = Time::GetMicrosec();
	sync_rate = refrashRateU60;
	delay_cnt = 0;
	DASFlame = 11;
	WaitFlame = 0;
	PassedFlame = 0.0f;
	f_reset = false;
	f_bot = false;
	f_suggest = false;
	ActFlame = std::vector<int>(8, 0);
	FieldS1 = std::vector<std::vector<int8_t>>(shig::fH, (std::vector<int8_t>(shig::fW, 0)));
	abortAIp1 = { false };
	thinkAIp1 = { false };
	CmdListAIp1.clear();
	// AI起動 
	asyncAIp1 = s3d::Async(shig::ExeThinking, ref(*AIp1), ref(abortAIp1), ref(thinkAIp1), ref(CmdListAIp1));

}

void Solo::update()
{
	if (f_bot)
	{
		// AIプレイ時のフレームレートを300FPSに設定
		sync_rate = refrashRateU300;
	}
	else
	{
		//人間プレイ時のフレームレートを60FPSに設定
		sync_rate = refrashRateU60;
	}


	if ((s3d::Time::GetMicrosec() - sec_time) >= sync_rate)
	{
		sec_time = s3d::Time::GetMicrosec();
		
		KeyConfp1->SetDefault();

		if (f_bot)
		{
			// AIプレイ時のフレームレートは300FPS（60fps換算で0.2F）
			PassedFlame += 0.2f;
		}
		else
		{
			//人間プレイ時のフレームレートは60FPS
			PassedFlame += 1.0f;
		}

		if (WaitFlame > 0)
		{
			WaitFlame--;
		}
		else
		{
			TEp1->ResetFieldP();

			if (f_reset)
			{
				reset_manage();
			}

			// テトリス側操作入力
			tetris_manage();
			// DASフレームの更新
			//actF_manage();
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
	if (asyncAIp1.isValid())
	{
		asyncAIp1.wait();
	}
}

void Solo::game_manage()
{
	if (IsKeyDown(*KeyConfp1, KeyVal::R))
	{
		TEp1->CopyFiledP();
		AIp1->loadTE(*TEp1);
		WaitFlame = 40;
		f_reset = true;
	}

	if (IsKeyDown(*KeyConfp1, KeyVal::G))
	{
		ActFlame.at(0) = 1;
		TEp1->CopyFiledP();
		TEp1->StackGarbage(-1);
	}

	if (IsKeyDown(*KeyConfp1, KeyVal::M))
	{
		if (f_bot)
		{
			f_bot = false;
		}
		else
		{
			// bot起動時は推奨手表示も有効化
			f_suggest = true;
			f_bot = true;
			// AI起動
			TEp1->CopyFiledP();
			AIp1->loadTE(*TEp1);
			thinkAIp1 = true;
		}
	}

	if (IsKeyDown(*KeyConfp1, KeyVal::I))
	{
		if (f_suggest)
		{
			f_suggest = false;
		}
		else
		{
			f_suggest = true;
			// AI起動
			TEp1->CopyFiledP();
			AIp1->loadTE(*TEp1);
		}
	}
}

void Solo::tetris_manage()
{
	int g_check = 0;

	if (f_bot)
	{
		// 非同期処理側で推奨手計算が終了している場合
		if (!thinkAIp1)
		{
			FieldS1 = AIp1->getSuggestionAi();
			if (!CmdListAIp1.empty())
			{
				g_check = TEp1->Game(CmdListAIp1.front(), 0);
				WaitFlame = 0;
				if (CmdListAIp1.front() == shig::TetriAction::Hard)
				{
					TEp1->GetGarbage();
				}
				CmdListAIp1.pop_front();
				// 操作をし終わったタイミングで先に思考開始
				if (CmdListAIp1.empty())
				{
					AIp1->loadTE(*TEp1);
					thinkAIp1 = true;
				}
			}
			else
			{
				thinkAIp1 = true;
			}
		}
		else if (thinkAIp1)
		{
			// することがない 
		}
	}
	else
	{
		if (f_suggest)
		{
			// 非同期処理側で推奨手計算が終了している場合
			if (not thinkAIp1)
			{
				FieldS1 = AIp1->getSuggestionAi();
				thinkAIp1 = true;
			}
			else if (thinkAIp1)
			{
				// することがない 
			}
		}

		// プレイヤー操作入力

		// 左右入力
		if (KeyConfp1->GetKey(KeyVal::Left).pressed() and KeyConfp1->GetKey(KeyVal::Right).pressed())
		{
			ActFlame.at(6) = 0;
			ActFlame.at(7) = 0;
		}
		else
		{
			if (KeyConfp1->GetKey(KeyVal::Left).pressed())
			{
				if (ActFlame.at(6) == 0)
				{
					ActFlame.at(6) = 1;
					g_check = TEp1->Game(6, 0);
				}
				else if (ActFlame.at(6) > DASFlame)
				{
					g_check = TEp1->Game(6, 0);
				}
				else
				{
					ActFlame.at(6)++;
				}

				ActFlame.at(7) = 0;
				delay_cnt = 2;
			}
			else if (KeyConfp1->GetKey(KeyVal::Right).pressed())
			{
				if (ActFlame.at(7) == 0)
				{
					ActFlame.at(7) = 1;
					g_check = TEp1->Game(7, 0);
				}
				else if (ActFlame.at(7) > DASFlame)
				{
					g_check = TEp1->Game(7, 0);
				}
				else
				{
					ActFlame.at(7)++;
				}

				ActFlame.at(6) = 0;
				delay_cnt = 2;
			}
			else
			{
				ActFlame.at(6) = 0;
				ActFlame.at(7) = 0;
			}
		}

		if (KeyConfp1->GetKey(KeyVal::Up).pressed() and KeyConfp1->GetKey(KeyVal::Z).pressed())
		{
			ActFlame.at(5) = 0;
			ActFlame.at(4) = 0;
		}
		else
		{
			if (KeyConfp1->GetKey(KeyVal::Up).pressed())
			{
				if (ActFlame.at(5) == 0)
				{
					ActFlame.at(5) = 1;
					g_check = TEp1->Game(5, 0);
				}
				else if (ActFlame.at(5) > 2)
				{
					ActFlame.at(5) = 2;
				}
				else
				{
					ActFlame.at(5)++;
				}
				ActFlame.at(4) = 0;
				delay_cnt = 2;
			}
			else if (KeyConfp1->GetKey(KeyVal::Z).pressed())
			{
				if (ActFlame.at(4) == 0)
				{
					ActFlame.at(4) = 1;
					g_check = TEp1->Game(4, 0);
				}
				else if (ActFlame.at(4) > 2)
				{
					ActFlame.at(4) = 2;
				}
				else
				{
					ActFlame.at(4)++;
				}

				ActFlame.at(5) = 0;
				delay_cnt = 2;
			}
			else
			{
				ActFlame.at(4) = 0;
				ActFlame.at(5) = 0;
			}
		}

		if (KeyConfp1->GetKey(KeyVal::C).pressed())
		{
			if (ActFlame.at(1) == 0)
			{
				ActFlame.at(1) = 1;
				g_check = TEp1->Game(1, 0);
			}
			else if (ActFlame.at(1) > 2)
			{
				ActFlame.at(1) = 2;
			}
			else
			{
				ActFlame.at(1)++;
			}
			delay_cnt = 2;
		}
		else
		{
			ActFlame.at(1) = 0;
		}

		if (KeyConfp1->GetKey(KeyVal::Down).pressed())
		{
			if (ActFlame.at(2) == 0)
			{
				ActFlame.at(2) = 1;
				g_check = TEp1->Game(2, 0);
			}
			else if (ActFlame.at(2) > 2)
			{
				g_check = TEp1->Game(2, 0);
				//ActFlame.at(2) = 2;
			}
			else
			{
				ActFlame.at(2)++;
			}
			delay_cnt = 2;
		}
		else
		{
			ActFlame.at(2) = 0;
		}

		if (KeyConfp1->GetKey(KeyVal::Space).pressed())
		{
			if (ActFlame.at(3) == 0)
			{
				ActFlame.at(3) = 1;
				g_check = TEp1->Game(3, 0);
				delay_cnt = TEp1->get_delayF();
			}
			else if (ActFlame.at(3) > 2)
			{
				//g_check = TEp1->Game(3, 0);
				ActFlame.at(3) = 2;
			}
			else
			{
				ActFlame.at(3)++;
			}
			delay_cnt = 2;

			if (f_suggest)
			{
				AIp1->loadTE(*TEp1);
				AIp1->thinking();
				AIp1->makeAiSuggestion();
				FieldS1 = AIp1->getSuggestionAi();
			}
		}
		else
		{
			ActFlame.at(3) = 0;
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
		f_reset = true;
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
		f_reset = true;
		WaitFlame = 72;
	}
	else if (g_check == 0) {
		TEp1->CopyFiledP();
	}*/

	return;
}

void Solo::actF_manage()
{

	for (auto&& act : ActFlame)
	{
		act++;
		if (act > 256)
		{
			act = 256;
		}
	}

	if (ActFlame.at(6) > DASFlame)
	{
		ActFlame.at(6) = DASFlame;
	}

	if (ActFlame.at(7) > DASFlame)
	{
		ActFlame.at(7) = DASFlame;
	}

	return;
}

void Solo::reset_manage()
{
	TEp1->SetField();
	TEp1->CopyFiledP();
	delay_cnt = 0;
	DASFlame = 6;
	WaitFlame = 0;
	f_reset = false;

	ActFlame = vector<int>(8, 0);
	FieldS1 = std::vector<std::vector<int8_t>>(shig::fH, (std::vector<int8_t>(10, 0)));
	thinkAIp1 = false;

	CmdListAIp1.clear();

	if(f_suggest)
	{
		AIp1->loadTE(*TEp1);
		AIp1->thinking();
		AIp1->makeAiSuggestion();
	}

	thinkAIp1 = true;

	return;
}

void Solo::draw_field() const
{

	Rect{ 200, 50, 300, 630 }
		.draw(Color(60, 60, 60, 255))
		.drawFrame(0, 1, Palette::White);

	for (int i = 0; i < 21; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			const auto _mino = TEp1->GetFieldBlock(20 - i, j, 0);
			if (_mino == 0i8) {
				continue;
			}
			Rect{ 201 + (j * 30), 51 + (i * 30), 29, 29 }
			.draw(minoC.at(_mino));
		}
	}

	for (int i = 0; i < 11; i++)
	{
		Line{ 200 + i * 30, 50, 200 + i * 30, 681 }.draw(1, Palette::Ghostwhite);
	}
	for (int i = 0; i < 22; i++)
	{
		Line{ 200, 50 + i * 30, 501, 50 + i * 30 }.draw(1, Palette::Ghostwhite);
	}

	return;
}

void Solo::draw_s_field() const
{
	if (not f_suggest)
	{
		return;
	}

	for (int i = 0; i < 21; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			const auto _mino = FieldS1.at((size_t)20 - i).at(j);
			if (_mino == 0i8)
			{
				continue;
			}
			Rect{ 201 + (j * 30), 51 + (i * 30), 29, 29 }
				.drawFrame(2, 0, minoC.at(_mino));
		}
	}

	return;
}

void Solo::draw_tex() const
{

	const auto& [_current, _hold, _n1, _n2, _n3, _n4, _n5] = TEp1->get_mino_state();

	m_MinoTex.at(static_cast<size_t>(_hold)).draw(60.0, 80.0);

	/*int n_size = std::min(5, (int)n_data.size());
	for (int i = 0; i < n_size; i++) {
		int nq = n_data.at(i);
		m_MinoTex.at(nq).draw(525, 80 + i * 100);
	}*/

	double y_pos = 80.0;
	m_MinoTex.at(static_cast<size_t>(_n1)).draw(525.0, y_pos);
	y_pos += 100.0;
	m_MinoTex.at(static_cast<size_t>(_n2)).draw(525.0, y_pos);
	y_pos += 100.0;
	m_MinoTex.at(static_cast<size_t>(_n3)).draw(525.0, y_pos);
	y_pos += 100.0;
	m_MinoTex.at(static_cast<size_t>(_n4)).draw(525.0, y_pos);
	y_pos += 100.0;
	m_MinoTex.at(static_cast<size_t>(_n5)).draw(525.0, y_pos);

	return;
}

void Solo::draw_state() const
{

	std::deque<std::string> mino_his = TEp1->get_mino_his();

	int i = 1;
	for (auto&& ms : mino_his)
	{
		s3d::String deb_his = s3d::Unicode::Widen(std::to_string(i)) + U" : " + s3d::Unicode::Widen(ms);

		FontAsset(U"Debug")(deb_his).draw(s3d::Vec2{ 20, 360 + 20 * i }, Color(0, 0, 0));
		i++;
	}

	s3d::String stateTS = s3d::Unicode::Widen(TEp1->GetTSstring());
	FontAsset(U"Debug")(stateTS).draw(s3d::Vec2{ 20, 560 }, Color(0, 0, 0));

	const auto&& [combo, btb] = TEp1->GetComboBtb();

	s3d::String stCombo = s3d::Unicode::Widen(std::string("Combo : ") + std::to_string(combo));
	FontAsset(U"Debug")(stCombo).draw(s3d::Vec2{ 20, 580 }, Color(0, 0, 0));
	s3d::String stBTB = s3d::Unicode::Widen(std::string("B-2-B : ") + std::to_string(btb));
	FontAsset(U"Debug")(stBTB).draw(s3d::Vec2{ 20, 600 }, Color(0, 0, 0));

	return;
}

