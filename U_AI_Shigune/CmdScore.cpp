
#include "CmdScore.hpp"

namespace shig {

	CmdScore::CmdScore() {
		init_s();
	}


	bool CmdScore::init() {
		height = 0;
		touch = 0;
		contact = 0;
		erase = 0;
		cmb = 0;
		btbc = 0;
		closed = 0;
		PC = 0;
		return true;
	}

	bool CmdScore::init_s() {
		sum = 0;
		height = 0;
		touch = 0;
		contact = 0;
		erase = 0;
		cmb = 0;
		btbc = 0;
		closed = 0;
		PC = 0;
		return true;
	}


	bool CmdScore::calc_sum() {

		//this->sum = 0;
		this->sum = shig::secure_add(sum, height);
		this->sum = shig::secure_add(sum, touch);
		this->sum = shig::secure_add(sum, contact);
		this->sum = shig::secure_add(sum, erase);
		this->sum = shig::secure_add(sum, cmb);
		this->sum = shig::secure_add(sum, btbc);
		this->sum = shig::secure_add(sum, closed);
		this->sum = shig::secure_add(sum, PC);

		return true;
	}

	long long CmdScore::get_sum() {
		calc_sum();
		return this->sum;
	}

	bool CmdScore::operator < (const CmdScore& ath) const {

		if (sum != ath.sum)return sum < ath.sum;
		else {
			if (PC != ath.PC)return PC < ath.PC;
			else {
				if (height != ath.height)return height < ath.height;
				else {
					if (touch != ath.touch)return touch < ath.touch;
					else {
						if (contact != ath.contact)return contact < ath.contact;
						else {
							if (erase != ath.erase)return erase < ath.erase;
							else {
								if (closed != ath.closed)return closed < ath.closed;
								else return btbc < ath.btbc;

							}

						}

					}
				}
			}
		}

	}

	CmdScore::~CmdScore() {

	}

}
