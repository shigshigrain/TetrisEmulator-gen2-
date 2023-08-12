//# include <Siv3D.hpp> // OpenSiv3D v0.6.4
# pragma once

//#include "windows.h"

//# include "common.hpp"
# include "title.hpp"
# include "solo.hpp"


void Main()
{
	// 背景の色を設定 | Set background color
	Scene::SetBackground(ColorF{ 0.8, 0.9, 1.0 });

	// タイトル変更
	Window::SetTitle(U"TetrisEmulator2 -v2.1-");

	// サイズ変更
	
	Window::SetStyle(WindowStyle::Sizable);
	Window::Resize(1280, 720);
	Scene::SetResizeMode(ResizeMode::Keep);

	FontAsset::Register(U"TitleFont", FontMethod::MSDF, 50, U"example/font/RocknRoll/RocknRollOne-Regular.ttf");
	FontAsset(U"TitleFont").setBufferThickness(4);
	FontAsset::Register(U"Menu", FontMethod::MSDF, 40, Typeface::Medium);
	//FontAsset::Register(U"Ranking", 40, Typeface::Heavy);
	//FontAsset::Register(U"GameScore", 30, Typeface::Light);
	//AudioAsset::Register(U"Brick", GMInstrument::Woodblock, PianoKey::C5, 0.2s, 0.1s);

	App manager;
	manager.add<Title>(State::Title);
	manager.add<Solo>(State::Solo);
	//manager.add<Ranking>(State::Ranking);

	// 通常のフォントを作成 | Create a new font
	const Font font{ 60 };

	// 使用変数
	shig::BoolSwitch resize_flag(false);
	auto PreSceneRect = Window::GetState().virtualSize;
	int Resize_counter = -1; // 画面リサイズ処理の遅延用
	constexpr int Resize_delay = 0;
	auto DefWindowScale = Window::GetState().scaling;

	while (System::Update())
	{
		
		// マウスカーソルに追随する半透明な円を描く | Draw a red transparent circle that follows the mouse cursor
		Circle{ Cursor::PosF(), 5 }.draw(ColorF{1, 1, 1, 0.8 });

		auto SceneRect = Window::GetState().virtualSize;
		auto ScenePos = Window::GetPos();

		if (Window::GetState().sizeMove) {
			resize_flag.set(true);
		}
		else {
			if (resize_flag.get() && (SceneRect != PreSceneRect)) {
				Resize_counter = Resize_delay;
				resize_flag.set(false);
			}
		}

		if (Resize_counter == 0) {
			double Resized_x = ((double)SceneRect.x);
			double Resized_y = ((double)Resized_x * 9.0 / 16.0);
			Window::ResizeVirtual((int)Resized_x, (int)Resized_y, YesNo<Centering_tag>::No);
			Window::ResizeVirtual((int)Resized_x, (int)Resized_y, YesNo<Centering_tag>::No);
			Window::SetPos(ScenePos);
			resize_flag.set(false);
			Resize_counter = -1;
		}
		else {
			Resize_counter--;
		}


		if (not manager.update())
		{
			break;
		}

	}
}

//void Main()
//{
//	 背景の色を設定 | Set background color
//	Scene::SetBackground(ColorF{ 0.8, 0.9, 1.0 });
//
//	 タイトル変更
//	Window::SetTitle(U"TetrisEmulator-gen2-");
//
//	 サイズ変更
//	Window::SetStyle(WindowStyle::Sizable);
//	Window::Resize(1920, 1000);
//
//	FontAsset::Register(U"TitleFont", FontMethod::MSDF, 50, U"example/font/RocknRoll/RocknRollOne-Regular.ttf");
//	FontAsset(U"TitleFont").setBufferThickness(4);
//	FontAsset::Register(U"Menu", FontMethod::MSDF, 40, Typeface::Medium);
//	FontAsset::Register(U"Ranking", 40, Typeface::Heavy);
//	FontAsset::Register(U"GameScore", 30, Typeface::Light);
//	AudioAsset::Register(U"Brick", GMInstrument::Woodblock, PianoKey::C5, 0.2s, 0.1s);
//
//	App manager;
//	manager.add<Title>(State::Title);
//	manager.add<Game>(State::Game);
//	manager.add<Ranking>(State::Ranking);
//
//	 通常のフォントを作成 | Create a new font
//	const Font font{ 60 };
//
//	 絵文字用フォントを作成 | Create a new emoji font
//	const Font emojiFont{ 60, Typeface::ColorEmoji };
//
//	 `font` が絵文字用フォントも使えるようにする | Set emojiFont as a fallback
//	font.addFallback(emojiFont);
//
//	 画像ファイルからテクスチャを作成 | Create a texture from an image file
//	const Texture texture{ U"tex\\background\\tetris_emulator_background02.bmp" };
//
//	 絵文字からテクスチャを作成 | Create a texture from an emoji
//	const Texture emoji{ U"🐈"_emoji };
//
//	 絵文字を描画する座標 | Coordinates of the emoji
//	Vec2 emojiPos{ 300, 150 };
//
//	 テキストを画面にデバッグ出力 | Print a text
//	Print << U"Push [A] key";
//
//	while (System::Update())
//	{
//		 テクスチャを描く | Draw a texture
//		texture.draw(0, 0);
//
//		 テキストを画面の中心に描く | Put a text in the middle of the screen
//		font(U"Hello, Siv3D!").drawAt(Scene::Center(), Palette::Black);
//
//		 サイズをアニメーションさせて絵文字を描く | Draw a texture with animated size
//		emoji.resized(100 + Periodic::Sine0_1(1s) * 20).drawAt(emojiPos);
//
//		 マウスカーソルに追随する半透明な円を描く | Draw a red transparent circle that follows the mouse cursor
//		Circle{ Cursor::Pos(), 5 }.draw(ColorF{ 1, 1, 1, 0.5 });
//
//		 もし [A] キーが押されたら | When [A] key is down
//		if (KeyA.down())
//		{
//			 選択肢からランダムに選ばれたメッセージをデバッグ表示 | Print a randomly selected text
//			Print << Sample({ U"Hello!", U"こんにちは", U"你好", U"안녕하세요?" });
//		}
//
//		 もし [Button] が押されたら | When [Button] is pushed
//		if (SimpleGUI::Button(U"Button", Vec2{ 640, 40 }))
//		{
//			 画面内のランダムな場所に座標を移動
//			 Move the coordinates to a random position in the screen
//			emojiPos = RandomVec2(Scene::Rect());
//		}
//	}
//}
