#ifndef _hestabela_h_
#define _hestabela_h_

#define _CRT_SECURE_NO_DEPRECATE
#include <iostream>
#include <string>
#include <cmath>
#include "apstraktna.h"

using namespace std;

class HesT: public Apstraktna {
	int n;//velicina tabele
	int p=15; //donjih p bita
	FILE* file;
	int broj_kljuceva_u_tabeli = 0;
	int vel_baket = 0;
	struct Predmeti {
		string* predmet;
		Predmeti* next;
		Predmeti(const char* predmet, Predmeti* next = nullptr) :predmet(new string (predmet)), next(next) {}
	};

	struct Student {
		int id;
		string* ime;
		string* prezime;
		Predmeti* head;
		int stanje = 0;
		Student() = default;
		Student(int id, const char* ime, const char* prezime, Predmeti* head) :id(id), ime(new string (ime)), prezime(new string (prezime)), head(head) {}
		~Student() {
			Predmeti* stari = head;
			while (head) {stari = head;head = head->next;delete stari;}
			head = nullptr;
		}
		int obr = 0;
		Student* next=nullptr;
	};

	Student** tabela = { nullptr };
	int* stanje = nullptr;

public:

	HesT() {
		cout << "Unesite velicinu hes tabele:";
		cin >> n;
		//cout << endl;
		//cout << "Unesite koliko bitova broja se posmatra:";
		//cin >> p;
		cout << endl;
		cout << "Unesite velicinu baketa:";
		cin >> vel_baket;
		tabela = new Student * [n];
		stanje = new int[n];
		for (int i = 0; i < n; i++) {
			tabela[i] = nullptr;
			stanje[i] = 0;
		}
		file = fopen("student.txt", "r");
		char * niz= new char[300];
		fgets(niz, 299, file);
		delete [] niz;
	}

	Student* formiraj_cvor() {

		int id;
		char ime[256];
		char prezime[256];
		char niz[256];
		Predmeti* prvi = nullptr, * posl = nullptr;
		if (fscanf(file, "%d %s %s %[^\n]\n", &id, ime, prezime, niz) == 4) {
			
			char* token = strtok(niz, " ");
			while (token != NULL) {

				posl = (prvi ? posl->next : prvi) = new Predmeti(token);
				//cout << token << endl;
				token = strtok(NULL, " ");
			}

			Student* s = new Student(id, ime, prezime, prvi);
			
			s->stanje = 1;
			return s;
		}
		else {
			return nullptr;
		}
	}

	void ubaci_u_tabelu() {
		Student* s;
		while (s = formiraj_cvor()) {
			insertKey(s->id, s);
		}
}


	bool insertKey(int k, Student* s) {
		int modular = 1;
		for (int i = 0; i < p; i++) modular *= 2;
		int j = (k % modular) % n;
		//static int iii;
		if (duzina(tabela[j])<vel_baket){
			s->next=tabela[j];
			tabela[j] = s;
			stanje[j] = 1;
			broj_kljuceva_u_tabeli += 1;
			//cout << 'm' << endl;
			return true;
		}
		else {
			//iii++;
			int i = 0;
			for (int i = 0; i < n; i++) {
				int m = getAddress(k, j, i, n);
				m = m % n;
				if (duzina(tabela[m]) < vel_baket) {
					tabela[m] = s;
					stanje[m] = 1;
					broj_kljuceva_u_tabeli += 1;
					return true;
					break;
				}

				if (stanje[m] == 1 && tabela[m]->id == k) {
					return false;
					break;
				}
			}
		}

		return false;
	}

	int duzina(Student* s) const{
		int i = 0;
		if (s) {
			for (Student* tek = s; tek; tek = tek->next) {
				if (!tek->obr) {
					i++;
				}
			}
		}
		return i;
	}
	void print(ostream& os) const {

		for (int i = 0; i < n; i++) {
			os << i << ". "; //<< duzina(tabela[i]) << "---";
			
			if (stanje[i]==1) {
				for (Student* tek = tabela[i]; tek; tek = tek->next) {
					if (!tek->obr) {
						os << tek->id << "\t" << *tek->ime << " " << *tek->prezime << "\t";
						os << "\t" << "Predmeti: ";
						Predmeti* tmp = tek->head;
						while (tmp) { os << *tmp->predmet << "  "; tmp = tmp->next; }
						os << endl;
						os << "    ";
					}
				}
				os << endl;
			}
			else if (stanje[i] == 0) {
				os << "EMPTY" << endl;
			}
			else if (stanje[i] == -1) {
				os << "DELETED" << endl;
			}
			//cout << (tabela[i]?()) << endl;
		}
	}

Student* findKey(int key) const {
		int modular = 1;
		for (int i = 0; i < p; i++) modular *= 2;

		int j = (key % modular) % n;
		int add = j;

		int k = 0;
		while (stanje[j] != 0) {
			for (Student* tek = tabela[j]; tek; tek = tek->next) {
				if (stanje[j] == 1 && tek->id == key) {
					cout << tek->id << "\t" << *tek->ime << "\t" << *tek->prezime << endl;
					return tek;
					break;
				}
			}
				j = (add + k * k) % n;
				k++;
			
		}
		cout << "Nema tog studenta." << endl;
		return nullptr;
	}


bool deleteKey(int key) {
	int modular = 1;
	for (int i = 0; i < p; i++) modular *= 2;

	int j = (key % modular) % n;
	int add = j;

	int k = 0;
	while (stanje[j] != 0) {
		for (Student* tek = tabela[j]; tek; tek = tek->next) {
			if (stanje[j] == 1 && tek->id == key) {
				cout << "Obrisan student:" << tek->id << "\t" << *tek->ime << "\t" << *tek->prezime << endl;
				if (duzina(tabela[j]) - 1 == 0) {
					stanje[j] = -1;
				}
				tek->obr = 1;
				broj_kljuceva_u_tabeli -= 1;
				return true;
				break;
			}
		}
		j = (add + k * k)%n;
		k++;
	}
	cout << "Nema tog studenta." << endl;
	return false;
}

int dohvBrKluceva() const { return this->broj_kljuceva_u_tabeli; }
int dohvVelTabele() const { return vel_baket*this->n; }
double fillRatio() const { return (double)this->broj_kljuceva_u_tabeli / (vel_baket*(this->n))*100; }

friend ostream& operator<<(ostream& os, const HesT& hes) {
	hes.print(os);
	return os;

}

void dodajPredmet(int key,char* predmet) {
	Student* s = findKey(key);
	if (s) {
		Predmeti* p = new Predmeti(predmet);
		p->next = s->head;
		s->head = p;
		cout << "Predmet je dodat";
	}
	cout << endl;
}

void obrisiPredmet(int key, string predmet) {
	Student* s = findKey(key);
	if (s) {
		for (Predmeti* current = s->head, *previous = nullptr; current != nullptr;) {
			if (*current->predmet==predmet) {
				Predmeti* old = current;
				current = current->next;
				delete old;
				if (previous == nullptr) {
					s->head = current;
				}
				else {
					previous->next = current;
				}
				cout << "Uspesno brisanje.";
				break;
			}
			else {
				previous = current;
				current = current->next;
			}
		}
	}
	cout << endl;
}
   ~HesT() {
		fclose(file);
		delete [] tabela;
		delete[] stanje;
	}
};



#endif
