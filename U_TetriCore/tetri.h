#pragma once

#include "shig_utility.h"

namespace shig {

	enum TetriMino :int16
	{
		I,
		J,
		L,
		O,
		S,
		T,
		Z,
	};

	const vector<vector<vector<int>>> minoI = {

				{
					{ 0, 0, 0, 0 },{ 1, 1, 1, 1 },{ 0, 0, 0, 0 },{ 0, 0, 0, 0 }
				},
				{
					{ 0, 0, 1, 0 },{ 0, 0, 1, 0 },{ 0, 0, 1, 0 },{ 0, 0, 1, 0 }
				},
				{
					{ 0, 0, 0, 0 },{ 0, 0, 0, 0 },{ 1, 1, 1, 1 },{ 0, 0, 0, 0 }
				},
				{
					{ 0, 1, 0, 0 },{ 0, 1, 0, 0 },{ 0, 1, 0, 0 },{ 0, 1, 0, 0 }
				}

	};

	const vector<vector<vector<int>>> minoJ = {

				{
					{ 2, 0, 0 },{ 2, 2, 2 },{ 0, 0, 0 }
				},
				{
					{ 0, 2, 2 },{ 0, 2, 0 },{ 0, 2, 0 }
				},
				{
					{ 0, 0, 0 },{ 2, 2, 2 },{ 0, 0, 2 }
				},
				{
					{ 0, 2, 0 },{ 0, 2, 0 },{ 2, 2, 0 }
				}

	};

	const vector<vector<vector<int>>> minoL = {

				{
					{ 0, 0, 3 },{ 3, 3, 3 },{ 0, 0, 0 }
				},
				{
					{ 0, 3, 0 },{ 0, 3, 0 },{ 0, 3, 3 }
				},
				{
					{ 0, 0, 0 },{ 3, 3, 3 },{ 3, 0, 0 }
				},
				{
					{ 3, 3, 0 },{ 0, 3, 0 },{ 0, 3, 0 }
				}

	};

	const vector<vector<vector<int>>> minoO = {

				{
					{ 0, 4, 4, 0 },{ 0, 4, 4, 0 },{ 0, 0, 0, 0 }
				},
				{
					{ 0, 4, 4, 0 },{ 0, 4, 4, 0 },{ 0, 0, 0, 0 }
				},
				{
					{ 0, 4, 4, 0 },{ 0, 4, 4, 0 },{ 0, 0, 0, 0 }
				},
				{
					{ 0, 4, 4, 0 },{ 0, 4, 4, 0 },{ 0, 0, 0, 0 }
				}

	};

	const vector<vector<vector<int>>> minoS = {

				{
					{ 0, 5, 5 },{ 5, 5, 0 },{ 0, 0, 0 }
				},
				{
					{ 0, 5, 0 },{ 0, 5, 5 },{ 0, 0, 5 }
				},
				{
					{ 0, 0, 0 },{ 0, 5, 5 },{ 5, 5, 0 }
				},
				{
					{ 5, 0, 0 },{ 5, 5, 0 },{ 0, 5, 0 }
				}

	};

	const vector<vector<vector<int>>> minoT = {

				{
					{ 0, 6, 0 },{ 6, 6, 6 },{ 0, 0, 0 }
				},
				{
					{ 0, 6, 0 },{ 0, 6, 6 },{ 0, 6, 0 }
				},
				{
					{ 0, 0, 0 },{ 6, 6, 6 },{ 0, 6, 0 }
				},
				{
					{ 0, 6, 0 },{ 6, 6, 0 },{ 0, 6, 0 }
				}

	};

	const vector<vector<vector<int>>> minoZ = {

				{
					{ 7, 7, 0 },{ 0, 7, 7 },{ 0, 0, 0 }
				},
				{
					{ 0, 0, 7 },{ 0, 7, 7 },{ 0, 7, 0 }
				},
				{
					{ 0, 0, 0 },{ 7, 7, 0 },{ 0, 7, 7 }
				},
				{
					{ 0, 7, 0 },{ 7, 7, 0 },{ 7, 0, 0 }
				}

	};


	class Tetri
	{
	public:
		int rot;
		int X;
		int Y;
		int id;
		int px_size;

		Tetri();
		Tetri(int id);
		Tetri(int rot, int X, int Y, int id);

		vector<vector<vector<int>>> mino;

		void set(int a, int b, int c, int d);
		void addX(int add);
		void addY(int add);
		void set_rot(int to);
		void set_id(int d);
		string get_mino_str();
		void set_mino(int i);
		bool operator < (const Tetri& ath) const;
		bool operator == (const Tetri& ath) const;
	};

	tuple<int, int, int, int> getTS(Tetri& t);

}
