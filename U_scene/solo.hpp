# pragma once
# include "common.hpp"

// ゲームシーン
class Solo : public App::Scene
{
public:

	Solo(const InitData& init);

	void update() override;

	void draw() const override;

private:// メンバ変数
	TetriEngine m_soloTE;
	shig::AiShigune m_soloAI;
	Texture m_bg;
	std::vector<Texture> m_MinoTex;
	KeyConf m_KeyConfS;
	uint64 sec_time;
	int delay_cnt;
	int DAS_flame;
	int Wait_flame;
	int passed_flame;
	bool reset_flag;
	shig::BoolSwitch suggest_flag;
	std::vector<int> act_flame;

private:// update関数
	void game_manage();
	void tetris_manage();
	void reset_manage();
	void actF_manage();
	
private:// draw関数 const
	void draw_field() const;
	void draw_s_field() const;
	void draw_tex()const;


};
