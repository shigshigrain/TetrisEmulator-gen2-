# pragma once
# include <Siv3D.hpp>

// シーンの名前
enum class State
{
	Title,
	Solo,
	Ai,
	Setting,
};

// 共有するデータ
struct GameData
{

};

using App = s3d::SceneManager<State, GameData>;

static constexpr uint64 refrashRateU15 = 1000000 / 15;
static constexpr uint64 refrashRateU30 = 1000000 / 30;
static constexpr uint64 refrashRateU60 = 1000000 / 60;
static constexpr uint64 refrashRateU120 = 1000000 / 120;
static constexpr uint64 refrashRateU180 = 1000000 / 180;
static constexpr uint64 refrashRateU240 = 1000000 / 240;
static constexpr uint64 refrashRateU300 = 1000000 / 300;

static const s3d::Array<s3d::Color> minoC{
	s3d::Color(255, 255, 255), // px=0
	s3d::Color(0, 191, 255), // px=1
	s3d::Color(66, 66, 255), // px=2
	s3d::Color(255, 165, 0), // px=3
	s3d::Color(255, 255, 0), // px=4
	s3d::Color(0, 208, 0), // px=5
	s3d::Color(156, 73, 231), // px=6
	s3d::Color(255, 80, 49), // px=7
	s3d::Color(119, 136, 153), // px=8
	s3d::Color(128, 223, 255), // px=-1
	s3d::Color(104, 104, 255), // px=-2
	s3d::Color(255, 214, 136), // px=-3
	s3d::Color(255, 255, 155), // px=-4
	s3d::Color(140, 208, 140), // px=-5
	s3d::Color(208, 169, 243), // px=-6
	s3d::Color(255, 139, 119), // px=-7
	s3d::Color(0, 128, 128) // px=-8
};

constexpr int minotex_path_size = 16;

const s3d::Array<s3d::StringView> minotex_path = {
	U"tex\\mino\\flameS_D.bmp",
	U"tex\\mino\\I-mino_D.bmp",
	U"tex\\mino\\J-mino_D.bmp",
	U"tex\\mino\\L-mino_D.bmp",
	U"tex\\mino\\O-mino_D.bmp",
	U"tex\\mino\\S-mino_D.bmp",
	U"tex\\mino\\T-mino_D.bmp",
	U"tex\\mino\\Z-mino_D.bmp",
	U"tex\\mino\\flameS_D.bmp",
	U"tex\\mino\\I-mino_D.bmp",
	U"tex\\mino\\J-mino_D.bmp",
	U"tex\\mino\\L-mino_D.bmp",
	U"tex\\mino\\O-mino_D.bmp",
	U"tex\\mino\\S-mino_D.bmp",
	U"tex\\mino\\T-mino_D.bmp",
	U"tex\\mino\\Z-mino_D.bmp",
};
