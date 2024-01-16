#pragma once

#include "stdafx.h"
// shig_utility

using namespace std;
//using namespace shig;
typedef long long LL;
typedef pair<int, int> pairI2;
typedef pair<LL, LL> pairLL2;
typedef vector<LL> VLL;
typedef vector<std::vector<std::vector<int>>> VVVI;

#define shig_for(i, a, b) for(int i = (a); i < (b); ++i)
#define shig_rep(i, n) shig_for(i, 0, n)
#define shig_forB(bit,a,b) for(int bit = (a); bit < (1<<(b)); ++bit)
#define shig_repB(bit,n) shig_forB(bit,0,n)
#define CINI(a) int (a); cin >> (a)
#define CINLL(a) LL (a); cin >> (a)
#define CINI2(a, b) int a, b; cin >> (a) >> (b)
#define CINLL2(a, b) LL a, b; cin >> (a) >> (b)
#define tmp int temp = 0
#define str string s

namespace shig{

	//utility-class
	class BoolSwitch {
	private:
		bool TF;
	public:
		BoolSwitch();
		BoolSwitch(bool s);
		~BoolSwitch();
		bool sw();
		bool set(bool s);
		bool get() const;

	};

	//non-menber-function
	long long secure_add(long long a, long long b);

	const std::vector<int> W_seed = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

}
