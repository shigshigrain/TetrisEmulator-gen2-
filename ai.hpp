# pragma once
# include "common.hpp"

// タイトルシーン
class Ai : public App::Scene
{
public:

	Ai(const InitData& init);

	void update() override;

	void draw() const override;

private:// メンバ変数
	TetriEngine m_soloTE;
	shig::AiShigune m_soloAI;
	Texture m_bg;
	vector<Texture> m_MinoTex;
	KeyConf m_KeyConf1p;
	uint64 sec_time;
	int delay_cnt;
	int DAS_flame;
	int Wait_flame;
	int passed_flame;
	bool reset_flag;
	shig::BoolSwitch suggest_flag;
	std::vector<int> ActFlame;

private:// update関数
	void game_manage();
	void tetris_manage();
	void reset_manage();
	void actF_manage();

private:// draw関数 const
	void DrawField() const;
	void DrawGhost() const;
	void draw_tex()const;
	
};
