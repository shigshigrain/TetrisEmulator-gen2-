
# include "U_scene/common.hpp"

// タイトルシーン
class Title : public App::Scene
{
public:

	Title(const InitData& init);

	void update() override;

	void draw() const override;

private:

	s3d::Rect m_soloB{ s3d::Point(100, 210), 500, 80};
	s3d::Transition m_soloT{ 0.6s, 0.3s };

	s3d::Rect m_aiB{ s3d::Point(100, 310), 500, 80 };
	s3d::Transition m_aiT{ 0.6s, 0.3s };

	s3d::Rect m_setB{ s3d::Point(100, 510), 500, 80 };
	s3d::Transition m_setT{ 0.6s, 0.3s };

	s3d::Rect m_exitB{ s3d::Point(100, 610), 500, 80 };
	s3d::Transition m_exitT{ 0.6s, 0.3s };

	const s3d::Texture texture{ U"tex\\background\\tetris_emulator_background02.bmp" };

};
