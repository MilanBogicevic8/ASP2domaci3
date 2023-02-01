#ifndef _dinamickihes_h_
#define _dinamickihes_h_
#define _CRT_SECURE_NO_DEPRECATE
#include <iostream>
#include <string>
#include <cmath>
//#include "apstraktna.h"

using namespace std;


class DHes {
	friend class BSTinsertfin;
	int n;//velicina tabele
	int p = 12; //donjih 2^p bita

	FILE* file;
	
	
	struct Predmeti {
		string* predmet;
		Predmeti* next;
		Predmeti(const char* predmet, Predmeti* next = nullptr) :predmet(new string(predmet)), next(next) {}
	};

	struct Student {

		int id;
		string* ime;
		string* prezime;
		Predmeti* head;
		int stanje = 0;
		Student() = default;
		Student(int id, const char* ime, const char* prezime, Predmeti* head) :id(id), ime(new string(ime)), prezime(new string(prezime)), head(head) {}
		~Student() {
			Predmeti* stari = head;
			while (head) { stari = head; head = head->next; delete stari; }
			head = nullptr;
		}
		Student* next = nullptr;
		class BSTinsertfin* root = nullptr;
		int obr = 0;
	};

	struct TStudent {
		Student** student;
		int trenutan_broj = 0;
		int kolona = 0;
		
		TStudent() { student = new Student * [10]{ nullptr }; }
		~TStudent() { delete student; }
	};

	/*
	struct Niz {
		TStudent* niz = nullptr;
		Niz() :niz(new TStudent* [n]) {}
		~Niz() { delete niz; }
	};
	*/

	TStudent** niz;
	int* stanje = nullptr;
	int baket;

	class BSTinsertfin {
		friend DHes;

		struct Element
		{
			Student* s;
			int podatak;
			int list = 0;
			Element* levo;
			Element* desno;
		};

		enum { LEVO = -1, ZADOVOLJEN = 0, DESNO = 1 };

		static bool ProveriPoredak(int i, int j)
		{
			return i >= j;
		}


		static int ProveriKriterijumPretrage(int i, int j)
		{
			if (i == j)
				return ZADOVOLJEN;

			return (ProveriPoredak(i, j) == true) ? LEVO : DESNO;
		}


		static Element* PronadjiElement(Element* koren, int podatak, Element** prethodni)
		{
			if (prethodni != NULL)			
				*prethodni = NULL;			

			
			while (koren != NULL)
			{
				int rez = ProveriKriterijumPretrage(koren->podatak, podatak);
				if (rez == ZADOVOLJEN)
					return koren;

				if (prethodni != NULL)		
					*prethodni = koren;		

				if (rez == LEVO)
					koren = koren->levo;	
				else if (rez == DESNO)
					koren = koren->desno;	
			}

			return NULL;
		}


		static Element* DodajElementUStablo(int podatak, Element* stablo)
		{
			Element* prethodni = NULL;

			
			Element* tekuci = PronadjiElement(stablo, podatak, &prethodni);		
																				
			if (tekuci != NULL)
				return stablo;

			Element* novi = (Element*)malloc(sizeof(Element));
			if (novi == NULL)
				return stablo;

			novi->podatak = podatak;				
			novi->levo = NULL;						
			novi->desno = NULL;						

			if (stablo == NULL)						
				return novi;						

			if (ProveriPoredak(prethodni->podatak, novi->podatak) == true)
				prethodni->levo = novi;				
			else									
				prethodni->desno = novi;

			return stablo;
		}


		Element* UkloniElementIzStabla(Element* stablo, int podatak)
		{
			Element* prethodni = NULL;

			
			Element* tekuci = PronadjiElement(stablo, podatak, &prethodni);			
																					
																					
																					
			if (tekuci == NULL)
				return stablo;

			Element* sledeci = NULL;												
			if (tekuci->levo == NULL || tekuci->desno == NULL)                      
				sledeci = (tekuci->levo != NULL) ? tekuci->levo : tekuci->desno;    
			else																	
			{
				Element* preSledeci = tekuci;										
																					
				sledeci = tekuci->desno;
				while (sledeci->levo != NULL)
				{
					preSledeci = sledeci;
					sledeci = sledeci->levo;
				}

				sledeci->levo = tekuci->levo;           
				if (tekuci->desno != sledeci)           
				{
					preSledeci->levo = sledeci->desno;  
					sledeci->desno = tekuci->desno;     
				}
			}

			if (prethodni == NULL)						
				stablo = sledeci;						
			else if (prethodni->levo == tekuci)         
				prethodni->levo = sledeci;          	
			else if (prethodni->desno == tekuci)    	
				prethodni->desno = sledeci;         	

			free(tekuci);								

			return stablo;
		}


		bool PronadjiElementUStablu(const Element* stablo, int podatak)
		{

			return (PronadjiElement((Element*)stablo, podatak, NULL) != NULL);
		}



	};


public:
	int broj_kljuceva_u_tabeli = 0;
	DHes() {
		cout << "Unesite velicinu baketa:";
		int b; cin >> b;
		baket = b;
		//for (int i = 0; i < b; i++) baket *= 2;

		cout << "Unesite velicinu hes tabele:";
		cin >> n;

		cout << endl;

		niz = new TStudent * [n];

		stanje = new int[n];
		for (int i = 0; i < n; i++) {
			niz[i] = new TStudent();
			stanje[i] = 0;
		}

		file = fopen("student.txt", "r");
		char* ni = new char[300];
		fgets(ni, 299, file);
		delete[] ni;
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
	int duzina(Student* s) const {
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
	bool insertKey(int k, Student* s) {
		if (!findKey(k,1)) {
			int modular = 1;
			for (int i = 0; i < p; i++) modular *= 2;
			int j = (k % modular) % n;
			if (niz[j]->trenutan_broj == baket) {
				niz[j]->trenutan_broj = 0;
				niz[j]->kolona += 1;
			}

			/*
			if (stanje[j] == 0) {
				niz[j]->student = new Student * [10];
			}
			*/
			s->next = niz[j]->student[niz[j]->kolona];
			niz[j]->student[niz[j]->kolona] = s;
			/*
			while (tek && tek->next) {
				tek = tek->next;
			}
			*/
			//s->next = tek;
			//tek = s;
			stanje[j] = 1;
			niz[j]->trenutan_broj += 1;
			broj_kljuceva_u_tabeli += 1;
			//BSTinsertfin::DodajElementUStablo(k);
			//stanje[j] = 1;
			//tek->next = s;
			return true;
		}
		return false;
	}

	void print(ostream& os) const {

		for (int i = 0; i < n; i++) {
			os << i << "." << endl;
			int k = 0;
			if (niz[i]->kolona + niz[i]->trenutan_broj > 0) {
				for (int j = 0; j <= niz[i]->kolona; j++) {
					//if (!j) cout << "   ";
					
					if (duzina(niz[j]->student[niz[j]->kolona]) > 0) k++;
					os << "\t" << i << "." << k << "." << "baket";
					cout << endl;
					if (duzina(niz[i]->student[j]) == 0 && niz[i]->student[j] != nullptr) { cout << "\tDELETED" << endl; continue; }
					Student* tek = niz[i]->student[j];
					while (tek) {
						//os << i << "."<<j<<"." << *(tek->ime) << " " << *(tek->prezime) << "\t\n";
						//tek = tek->next;
						if (tek->obr == 0) {
							os << "\t" << tek->id << "\t" << *tek->ime << "\t" << *tek->prezime << "\t  ";
							os << "\t" << "Predmeti: ";
							Predmeti* tmp = tek->head;
							while (tmp) { os << " " << *tmp->predmet << "  "; tmp = tmp->next; }
							os << endl;
							//os << "    ";
						}
						tek = tek->next;
					}
				}
			}
			else if (stanje[i] == 0) { cout << "\tEMPTY" << endl; }
			else if (stanje[i] == -1) { cout << "\tDELETED" << endl; }
		}
	}







	Student* findKey(int key, int indikator=0) const {
		int modular = 1;
		for (int i = 0; i < p; i++) modular *= 2;

		int j = (key % modular) % n;

		for (int k = 0; k <= niz[j]->kolona; k++) {
			for (Student* tek = niz[j]->student[k]; tek; tek = tek->next) {
				if (tek->id == key && tek->obr==0 && indikator!=1) {
					cout << tek->id << "\t" << *tek->ime << "\t" << *tek->prezime << endl;
					return tek;
					break;
				}
			}
		}
		if (indikator!=1){
			cout << "Nema tog studenta." << endl;
		}
		return nullptr;
	}

	bool deleteKey() {
		/*int modular = 1;
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
			j = (add + k * k) % n;
			k++;
		}
		cout << "Nema tog studenta." << endl;
		return false;*/

		Student* s = nullptr;
		cout << "Unesite kljuc studenta koji se brise:";
		int key; cin >> key;
		s = findKey(key);
		if (!s) { //cout << "Nema tog studenta." << endl;  
			return false; }
		s->obr = 1;
		return true;
	}


	int dohvBrKluceva() const { return this->broj_kljuceva_u_tabeli; }
	int dohvVelTabele() const { return baket*(this->n); }
	double fillRatio() const { return (double)this->broj_kljuceva_u_tabeli / (baket*(this->n))*100; }

	friend ostream& operator<<(ostream& os, const DHes& hes) {
		hes.print(os);
		return os;

	}

	void dodajPredmet(int key, char* predmet) {
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
				if (*current->predmet == predmet) {
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


	~DHes() {
		fclose(file);
		delete[] niz;
		delete[] stanje;

	}




};

#endif