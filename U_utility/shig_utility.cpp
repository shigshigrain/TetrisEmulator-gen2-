﻿
#include "shig_utility.h"



namespace shig {

	BoolSwitch::BoolSwitch() {
		BoolSwitch::TF = false;
	};

	BoolSwitch::BoolSwitch(bool s) {
		BoolSwitch::TF = s;
	};

	BoolSwitch::~BoolSwitch() {

	};

	bool BoolSwitch::sw() {
		if (BoolSwitch::TF)return BoolSwitch::TF = false;
		else return BoolSwitch::TF = true;
	};

	bool BoolSwitch::set(bool s) {
		return BoolSwitch::TF = s;
	};

	bool BoolSwitch::get() {
		return BoolSwitch::TF;
	};











	long long secure_add(long long a, long long b){

		constexpr long long mxm = numeric_limits<long long>::max();
		constexpr long long mum = numeric_limits<long long>::min();

		if (b == 0)return a;
		else if (b > 0) {
			if (a < mxm - b)return a + b;
			else return mxm - 1;
		}
		else {
			if (mum - b < a)return a + b;
			else return mum + 1;
		}

		return long long(0);
	}









}
