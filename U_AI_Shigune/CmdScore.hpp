
#pragma once
#include "shig_utility.h"

namespace shig {

	class CmdScore
	{

	public:
		long long sum = 0;
		long long height = 0;
		long long touch = 0;
		long long contact = 0;
		long long erase = 0;
		long long cmb = 0;
		long long btbc = 0;
		long long closed = 0;
		long long PC = 0;

	public:
		CmdScore();
		bool init();
		bool init_s();
		bool calc_sum();
		LL get_sum();
		~CmdScore();
		bool operator < (const CmdScore& ath) const;

	};


}
