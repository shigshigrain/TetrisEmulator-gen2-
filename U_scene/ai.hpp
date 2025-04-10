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
	std::unique_ptr<TetriEngine> TEp1;
	std::unique_ptr<AiShigune> AIp1;
	std::unique_ptr<TetriEngine> TEp2;
	std::unique_ptr<AiShigune> AIp2;
	Texture m_bg;
	Array<Texture> m_MinoTex;
	std::unique_ptr<KeyConf> KeyConfp1;
	uint64 sec_time;
	int delay_cnt;
	int DASFlame1p;
	int WaitFlame1p;
	int WaitFlame2p;
	int PassedFlame;
	bool ResetFlag;
	shig::BoolSwitch suggest_flag;
	std::vector<vector<int8_t>> FieldS1;
	std::vector<vector<int8_t>> FieldS2;
	std::vector<int> ActFlame;
	std::atomic<bool> abortAIp1;
	std::atomic<bool> thinkAIp1;
	std::atomic<bool> abortAIp2;
	std::atomic<bool> thinkAIp2;
	std::deque<int> CmdListAIp1;
	std::deque<int> CmdListAIp2;
	AsyncTask<bool> asyncAIp1;
	AsyncTask<bool> asyncAIp2;

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
	void DrawState() const;

};
