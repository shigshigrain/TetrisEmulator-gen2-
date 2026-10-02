
# include "U_scene/common.hpp"
# include "Aishigune/AiShigune.hpp"
# include "U_KeyConf/KeyConf.hpp"

// ゲームシーン
class Solo : public App::Scene
{
public:

	Solo(const InitData& init);

	void update() override;

	void draw() const override;

	~Solo();

private:
	/// @brief テトリス本体
	std::unique_ptr<shig::TetriEngine> TEp1;
	/// @brief AI本体
	std::unique_ptr<shig::AiShigune> AIp1;
	/// @brief 背景テクスチャ
	s3d::Texture m_bg;
	/// @brief ミノテクスチャ
	s3d::Array<s3d::Texture> m_MinoTex;
	/// @brief キーコンフィグ
	std::unique_ptr<KeyConf> KeyConfp1;
	/// @brief ゲーム進行管理用変数
	uint64 sec_time;
	uint64 sync_rate;
	int delay_cnt;
	int DASFlame;
	int WaitFlame;
	float PassedFlame;
	/// @brief ゲームリセットフラグ
	bool f_reset;
	/// @brief AI思考/操作フラグ
	bool f_bot;
	/// @brief AI思考/表示フラグ
	bool f_suggest;
	/// @brief 横ためフレーム管理用変数
	std::vector<int> ActFlame;
	/// @brief AI推奨手の表示フィールド
	std::vector<std::vector<int8_t>> FieldS1;
	std::atomic<bool> abortAIp1;
	std::atomic<bool> thinkAIp1;
	std::deque<shig::TetriAction> CmdListAIp1;
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
