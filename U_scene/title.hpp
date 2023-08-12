# pragma once
# include "common.hpp"

// タイトルシーン
class Title : public App::Scene
{
public:

	Title(const InitData& init);

	void update() override;

	void draw() const override;

private:

	Rect m_soloB{ Point(100, 210), 500, 80};
	Transition m_soloT{ 0.6s, 0.3s };

	Rect m_aiB{ Point(100, 310), 500, 80 };
	Transition m_aiT{ 0.6s, 0.3s };

	Rect m_setB{ Point(100, 510), 500, 80 };
	Transition m_setT{ 0.6s, 0.3s };

	Rect m_exitB{ Point(100, 610), 500, 80 };
	Transition m_exitT{ 0.6s, 0.3s };

	const Texture texture{ U"tex\\background\\tetris_emulator_background02.bmp" };

};
