#ifndef _apstraktna_h_
#define _apstraktna_h_


class Apstraktna {

public:
	int getAddress(int key, int adresa, int attempt, int size) {
		return adresa + attempt * attempt;
	}

protected:
	virtual ~Apstraktna() = 0;
};

Apstraktna::~Apstraktna() {}


#endif