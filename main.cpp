#include "hestabela.h"
#include "dinamickihes.h"

void meni() {
	
	cout << "1. Ubaci kljuceve i kreiraj tabelu" << endl
		<< "2. Pronadji kljuc" << endl
		<< "3. Obrisi kljuc" << endl
		<< "4. Broj umetnutih kljuceva" << endl
		<< "5. Velicina tabele" << endl
		<< "6.Ispis" << endl
		<< "7.Popunjenost tabele" << endl
		<< "8. Isprazni tabelu i zavrsi program" << endl
		<< "9. Dodaj predmet studentu" << endl
		<< "10. Obrisi predmet za nekog studenta" << endl;

}

void prvi_zadatak() {
	meni();
	HesT tabla;
	//cout << "Unesite opciju:";
	int opcija; //cin >> opcija;
	int kraj = 0;
	int key = 0;
	while (!kraj) {
		cout << "Unesite opciju:";
		cin >> opcija;
		switch (opcija) {
		case 1:
			tabla.ubaci_u_tabelu();
			cout << "Kjucevi  su ubaceni iz datoteke" << endl;
			break;
		case 2:

			cout << "Unesite kljuc za pretragu:"; cin >> key;
			tabla.findKey(key);
			break;
		case 3:

			cout << "Unesite kljuc za brisanje."; cin >> key;
			tabla.deleteKey(key);
			break;
		case 4:
			cout << "Broj umetnutih kljuceva je:" << tabla.dohvBrKluceva() << endl;
			break;
		case 5:
			cout << "Velicina tabele je:" << tabla.dohvVelTabele() << endl;
			break;
		case 6:
			tabla.print(cout);
			break;
		case 7:
			cout << "Popunjenost tabele je: " << tabla.fillRatio() << " %" << endl;
			break;
		case 8:
			kraj = 1;
			tabla.~HesT();
			exit(0);
			break;
		case 9:
			cout << "Unesite kljuc studenta i predmet:";
			char* predmet; predmet = new char[30];
			cin >> key >> predmet;
			tabla.dodajPredmet(key, predmet);
			break;
		case 10:
			cout << "Unesite kljuc studenta i predmet za brisanje:";
			char* p; p = new char[10];
			cin >> key >> p;
			tabla.obrisiPredmet(key, p);
			break;
		default:
			cout << "Nepravilan izbor,pokusaj ponovo." << endl;
			break;
		}

		cout << endl << endl;
		if (opcija == 8) break;
		meni();
	}
}

void drugi_zadatak() {
	
	/*
	tabla.ubaci_u_tabelu();
	tabla.print(cout);
	cout << "Unesite kljuc:";
	int key; cin >> key;
	tabla.findKey(key);
	tabla.deleteKey();
	cout << endl;
	tabla.print(cout);
	tabla.deleteKey();
	cout << endl;
	tabla.print(cout);
	tabla.fillRatio();

	*/

	meni();
	DHes tabla;
	//cout << "Unesite opciju:";
	int opcija; //cin >> opcija;
	int kraj = 0;
	int key = 0;
	while (!kraj) {
		cout << "Unesite opciju:";
		cin >> opcija;
		switch (opcija) {
		case 1:
			tabla.ubaci_u_tabelu();
			cout << "Kjucevi  su ubaceni iz datoteke" << endl;
			break;
		case 2:

			cout << "Unesite kljuc za pretragu:"; cin >> key;
			tabla.findKey(key);
			break;
		case 3:

			//cout << "Unesite kljuc za brisanje."; cin >> key;
			tabla.deleteKey();
			tabla.broj_kljuceva_u_tabeli -= 1;
			break;
		case 4:
			cout << "Broj umetnutih kljuceva je:" << tabla.dohvBrKluceva() << endl;
			break;
		case 5:
			cout << "Velicina tabele je:" << tabla.dohvVelTabele() << endl;
			break;
		case 6:
			tabla.print(cout);
			break;
		case 7:
			cout << "Popunjenost tabele je: " << tabla.fillRatio() << " %" << endl;
			break;
		case 8:
			kraj = 1;
			tabla.~DHes();
			exit(0);
			break;
		case 9:
			cout << "Unesite kljuc studenta i predmet:";
			char* predmet; predmet = new char[30];
			cin >> key >> predmet;
			tabla.dodajPredmet(key, predmet);
			break;
		case 10:
			cout << "Unesite kljuc studenta i predmet za brisanje:";
			char* p; p = new char[10];
			cin >> key >> p;
			tabla.obrisiPredmet(key, p);
			break;
		default:
			cout << "Nepravilan izbor,pokusaj ponovo." << endl;
			break;
		}
		
		cout << endl << endl;
		if (opcija == 8) break;
		meni();
	}
}

int main() {
	cout << "Koji zadatak radimo(1. ili 2.)?";
	int zadatak; cin >> zadatak;
	if (zadatak == 1) {
		prvi_zadatak();
	}
	else {
		drugi_zadatak();
	}
	
}