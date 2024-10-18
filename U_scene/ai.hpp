# pragma once
# include "common.hpp"
// ゲームシーン
class Ai : public App::Scene
{
public:

	Ai(const InitData& init);

	void update() override;

	void draw() const override;

	~Ai();

private:// メンバ変数
	std::unique_ptr<TetriEngine> m_1pTE;
	std::unique_ptr<AiShigune> m_1pAI;
	std::unique_ptr<TetriEngine> m_2pTE;
	std::unique_ptr<AiShigune> m_2pAI;
	Texture m_bg;
	Array<Texture> m_MinoTex;
	std::unique_ptr<KeyConf> m_KeyConf1p;
	uint64 sec_time;
	int delay_cnt;
	int DASFlame1p;
	int WaitFlame1p;
	int WaitFlame2p;
	int passed_flame;
	bool reset_flag;
	shig::BoolSwitch suggest_flag;
	std::vector<vector<int8_t>> FieldS1;
	std::vector<vector<int8_t>> FieldS2;
	std::vector<int> ActFlame;
	std::atomic<bool> abortAi1;
	std::atomic<bool> thinkAi1;
	std::atomic<bool> abortAi2;
	std::atomic<bool> thinkAi2;
	std::deque<int> CmdList1pAi;
	std::deque<int> CmdList2pAi;
	AsyncTask<bool> asyncAi1;
	AsyncTask<bool> asyncAi2;

private:// update関数
	void GameManage1p();
	void GameManage2p();
	void TetrisManage1p();
	void TetrisManage2p();
	void ResetManage();
	void InputFlameManage();

private:// draw関数 const
	void DrawField() const;
	void DrawGhost() const;
	void DrawTex1p()const;
	void DrawTex2p() const;

};
