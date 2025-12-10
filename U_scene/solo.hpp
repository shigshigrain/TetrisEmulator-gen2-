# pragma once
# include "U_scene/common.hpp"

// ゲームシーン
class Solo : public App::Scene
{
public:

	Solo(const InitData& init);

	void update() override;

	void draw() const override;

	~Solo();

private:// メンバ変数
	std::unique_ptr<TetriEngine> TEp1;
	std::unique_ptr<AiShigune> AIp1;
	Texture m_bg;
	Array<Texture> m_MinoTex;
	std::unique_ptr<KeyConf> KeyConfp1;
	uint64 sec_time;
	int delay_cnt;
	int DASFlame;
	int WaitFlame;
	int PassedFlame;
	bool ResetFlag;
	shig::BoolSwitch suggest_flag;
	std::vector<int> ActFlame;
	std::vector<std::vector<int8_t>> FieldS1;
	std::atomic<bool> abortAIp1;
	std::atomic<bool> thinkAIp1;
	std::deque<int> CmdListAIp1;
	s3d::AsyncTask<bool> asyncAIp1;

private:// update関数
	void game_manage();
	void tetris_manage();
	void actF_manage();
	void reset_manage();
	
private:// draw関数 const
	void draw_field() const;
	void draw_s_field() const;
	void draw_tex() const;
	void draw_state() const;

};
