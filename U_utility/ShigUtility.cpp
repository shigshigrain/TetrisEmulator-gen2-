
#include "ShigUtility.h"



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
		if (BoolSwitch::TF) {
			set(false);
			return false;
		}
		else {
			set(true);
			return true;
		}
	};

	bool BoolSwitch::set(bool s) {
		return BoolSwitch::TF = s;
	};

	bool BoolSwitch::get() const {
		return BoolSwitch::TF;
	};











	long long secure_add(long long a, long long b){

		constexpr long long mxm = numeric_limits<long long>::max();
		constexpr long long mum = numeric_limits<long long>::min();

		if (b == 0)return a;
		else if (b > 0) {
			if (a < mxm - b) {
				return (a + b);
			}
			else {
				return (mxm - 1);
			}
		}
		else {
			if (mum - b < a) {
				return (a + b);
			}
			else {
				return (mum + 1);
			}
		}

	}









}
