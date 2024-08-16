#include <iostream>
#include <string>
#include <regex>
#include <fstream>
#include <thread>
#include <mutex>
using namespace std;

const char* crt = "\n-------------------------------------------\n";
enum Pojas { BIJELI = 1, ZUTI, NARANDZASTI, ZELENI, PLAVI, SMEDJI, CRNI };

string ispisiPojas(Pojas pojas) {
	switch (pojas)
	{
	case BIJELI:
		return "Bijeli";
	case ZUTI:
		return "Zuti";
	case NARANDZASTI:
		return "Narandzasti";
	case ZELENI:
		return "Zeleni";
	case PLAVI:
		return "Plavi";
	case SMEDJI:
		return "Smedji";
	case CRNI:
		return "Crni";
	default:
		return "Greska";
	}
}

const int brojTehnika = 6;
const char* NIJE_VALIDNA = "<VRIJEDNOST_NIJE_VALIDNA>";
char* GetNizKaraktera(const char* sadrzaj) {
	if (sadrzaj == nullptr)return nullptr;
	int vel = strlen(sadrzaj) + 1;
	char* temp = new char[vel];
	strcpy_s(temp, vel, sadrzaj);
	return temp;
}


bool ValidirajLozinku(string lozinka) {
	return regex_search(lozinka, regex(".{7,}"))
		&& regex_search(lozinka, regex("[A-Z]{1,}"))
		&& regex_search(lozinka, regex("[a-z]{1,}"))
		&& regex_search(lozinka, regex("[0-9]{1,}"))
		&& regex_search(lozinka, regex("[@*-]{1,}"));
}

template<class T1, class T2, int max = 10>
class Kolekcija {
	T1* _elementi1[max] = { nullptr };
	T2* _elementi2[max] = { nullptr };
	int* _trenutno;
public:
	Kolekcija() {
		_trenutno = new int(0);
	}
	Kolekcija(const Kolekcija& obj) {
		_trenutno = new int(*obj._trenutno);

		for (int i = 0; i < *obj._trenutno; i++) {
			_elementi1[i] = new T1(*obj._elementi1[i]);
			_elementi2[i] = new T2(*obj._elementi2[i]);
		}
	}
	Kolekcija& operator=(const Kolekcija& obj) {
		if (this != &obj) {
			for (size_t i = 0; i < *_trenutno; i++) {
				delete _elementi1[i]; _elementi1[i] = nullptr;
				delete _elementi2[i]; _elementi2[i] = nullptr;
			}

			delete _trenutno; _trenutno = nullptr;

			_trenutno = new int(*obj._trenutno);

			for (int i = 0; i < *obj._trenutno; i++) {
				_elementi1[i] = new T1(*obj._elementi1[i]);
				_elementi2[i] = new T2(*obj._elementi2[i]);
			}
		}
		return *this;
	}
	~Kolekcija() {
		for (size_t i = 0; i < *_trenutno; i++) {
			delete _elementi1[i]; _elementi1[i] = nullptr;
			delete _elementi2[i]; _elementi2[i] = nullptr;
		}
		delete _trenutno; _trenutno = nullptr;
	}
	T1& getElement1(int lokacija)const { return *_elementi1[lokacija]; }
	T2& getElement2(int lokacija)const { return *_elementi2[lokacija]; }
	int getTrenutno()const { return *_trenutno; }
	friend ostream& operator<< (ostream& COUT, const Kolekcija& obj) {
		for (size_t i = 0; i < *obj._trenutno; i++)
			COUT << obj.getElement1(i) << " " << obj.getElement2(i) << endl;
		return COUT;
	}

	void AddElement(T1 el1, T2 el2) {
		if (*_trenutno == max)
			throw exception("Niz je popunjen.");

		_elementi1[*_trenutno] = new T1(el1);
		_elementi2[*_trenutno] = new T2(el2);

		(*_trenutno)++;
	}

	void RemoveAt(int lokacija) {
		if (lokacija<0 || lokacija>*_trenutno) return;

		for (int i = lokacija; i < *_trenutno - 1; i++) {
			_elementi1[i] = _elementi1[i + 1];
			_elementi2[i] = _elementi2[i + 1];
		}

		(*_trenutno)--;
	}

	void AddElement(T1 el1, T2 el2, int lokacija) {
		if (lokacija<0 || lokacija>*_trenutno || *_trenutno == max) return;

		for (int i = *_trenutno; i > lokacija; i--) {
			_elementi1[i] = _elementi1[i - 1];
			_elementi2[i] = _elementi2[i - 1];
		}

		_elementi1[lokacija] = new T1(el1);
		_elementi2[lokacija] = new T2(el2);

		(*_trenutno)++;
	}

	T1& operator[](T2 el1) {
		for (int i = 0; i < *_trenutno; i++) {
			if (*_elementi2[i] == el1)
				return *_elementi1[i];
		}
	}
};
class Datum {
	int* _dan, * _mjesec, * _godina;
public:
	Datum(int dan = 1, int mjesec = 1, int godina = 2000) {
		_dan = new int(dan);
		_mjesec = new int(mjesec);
		_godina = new int(godina);
	}
	Datum(const Datum& obj) {
		_dan = new int(*obj._dan);
		_mjesec = new int(*obj._mjesec);
		_godina = new int(*obj._godina);
	}
	Datum& operator=(const Datum& obj) {
		if (this != &obj) {
			delete _dan; _dan = nullptr;
			delete _mjesec; _mjesec = nullptr;
			delete _godina; _godina = nullptr;

			_dan = new int(*obj._dan);
			_mjesec = new int(*obj._mjesec);
			_godina = new int(*obj._godina);
		}
		return *this;
	}
	~Datum() {
		delete _dan; _dan = nullptr;
		delete _mjesec; _mjesec = nullptr;
		delete _godina; _godina = nullptr;
	}
	friend ostream& operator<< (ostream& COUT, const Datum& obj) {
		COUT << *obj._dan << "." << *obj._mjesec << "." << *obj._godina;
		return COUT;
	}

	int toDays()const {
		return *_godina * 365 + *_mjesec * 30 + *_dan;
	}


};
class Tehnika {
	char* _naziv;
	//int se odnosi na ocjenu u opsegu od 1 � 5, a Datum na datum kada je ocijenjena odredjena tehnika
	Kolekcija<int, Datum*, brojTehnika> _ocjene;
public:
	Tehnika(const char* naziv) {
		_naziv = GetNizKaraktera(naziv);
	}
	Tehnika(const Tehnika& obj) {
		_naziv = GetNizKaraktera(obj._naziv);
		_ocjene = obj._ocjene;
	}
	Tehnika& operator=(const Tehnika& obj) {
		if (this != &obj) {
			delete[] _naziv; _naziv = nullptr;

			_naziv = GetNizKaraktera(obj._naziv);
			_ocjene = obj._ocjene;
		}
		return *this;
	}
	~Tehnika() {
		delete[] _naziv; _naziv = nullptr;
	}
	char* GetNaziv() { return _naziv; }
	Kolekcija<int, Datum*, brojTehnika>& GetOcjene() { return _ocjene; }


	bool AddOcjena(int ocjena, Datum datum) {
		for (int i = 0; i < _ocjene.getTrenutno(); i++) {
			if (_ocjene.getElement2(i)->toDays() == datum.toDays())
				return false;

			if (datum.toDays() - _ocjene.getElement2(i)->toDays() < 3)
				return false;
		}

		_ocjene.AddElement(ocjena, new Datum(datum));
		return true;
	}

	friend ostream& operator<< (ostream& COUT, const Tehnika& obj) {
		COUT << "Naziv: " << obj._naziv << endl;
		COUT << "Ocjene: " << endl;
		for (int i = 0; i < obj._ocjene.getTrenutno(); i++) {
			COUT << obj._ocjene.getElement1(i) << " " << *obj._ocjene.getElement2(i) << endl;
		}
		COUT << "Prosjek tehnike: " << obj.ProsjekTehnike() << endl;

		return COUT;
	}

	float ProsjekTehnike()const {
		float suma = 0.0f;

		if (_ocjene.getTrenutno() == 0) return 0;

		for (int i = 0; i < _ocjene.getTrenutno(); i++) {
			suma += _ocjene.getElement1(i);
		}

		return suma / _ocjene.getTrenutno();
	}

	bool operator==(const Tehnika& obj) {
		return strcmp(this->GetNaziv(), obj._naziv) == 0;
	}
};
class Polaganje {
	Pojas _pojas;
	vector<Tehnika*> _polozeneTehnike;
public:
	Polaganje(Pojas pojas = BIJELI) {
		_pojas = pojas;
	}
	Polaganje(const Polaganje& obj) {
		_pojas = obj._pojas;

		for (int i = 0; i < obj._polozeneTehnike.size(); i++) {
			_polozeneTehnike.push_back(new Tehnika(*obj._polozeneTehnike[i]));
		}
	}
	Polaganje& operator=(const Polaganje& obj) {
		if (this != &obj) {
			for (size_t i = 0; i < _polozeneTehnike.size(); i++) {
				delete _polozeneTehnike[i];
				_polozeneTehnike[i] = nullptr;
			}
			_polozeneTehnike.clear();

			_pojas = obj._pojas;

			for (int i = 0; i < obj._polozeneTehnike.size(); i++) {
				_polozeneTehnike.push_back(new Tehnika(*obj._polozeneTehnike[i]));
			}
		}
		return *this;
	}
	~Polaganje() {
		for (size_t i = 0; i < _polozeneTehnike.size(); i++) {
			delete _polozeneTehnike[i];
			_polozeneTehnike[i] = nullptr;
		}
	}
	vector<Tehnika*>& GetTehnike() { return _polozeneTehnike; }
	Pojas GetPojas() { return _pojas; }
	friend ostream& operator<< (ostream& COUT, const Polaganje& obj) {
		COUT << obj._pojas << endl;
		for (size_t i = 0; i < obj._polozeneTehnike.size(); i++)
			COUT << *obj._polozeneTehnike[i];
		return COUT;
	}

	float ProsjekPojasa() {
		float suma = 0.0f;

		if (_polozeneTehnike.size() == 0) return 0;

		for (int i = 0; i < _polozeneTehnike.size(); i++) {
			suma += _polozeneTehnike[i]->ProsjekTehnike();
		}

		return suma / _polozeneTehnike.size();
	}
};
class Korisnik {
	char* _imePrezime;
	string _emailAdresa;
	string _lozinka;
public:
	Korisnik(const char* imePrezime, string emailAdresa, string lozinka)
	{
		_imePrezime = GetNizKaraktera(imePrezime);
		_emailAdresa = emailAdresa;
		_lozinka = ValidirajLozinku(lozinka) ? lozinka : NIJE_VALIDNA;
	}
	Korisnik(const Korisnik& obj) {
		_imePrezime = GetNizKaraktera(obj._imePrezime);
		_emailAdresa = obj._emailAdresa;
		_lozinka = obj._lozinka;
	}
	Korisnik& operator=(const Korisnik& obj) {
		if (this != &obj) {
			delete[] _imePrezime; _imePrezime = nullptr;

			_imePrezime = GetNizKaraktera(obj._imePrezime);
			_emailAdresa = obj._emailAdresa;
			_lozinka = obj._lozinka;
		}
		return *this;
	}
	virtual ~Korisnik() { delete[] _imePrezime; _imePrezime = nullptr; }
	string GetEmail() { return _emailAdresa; }
	string GetLozinka() { return _lozinka; }
	char* GetImePrezime() { return _imePrezime; }
};

mutex m;
class KaratePolaznik : public Korisnik {
	vector<Polaganje*> _polozeniPojasevi;
public:
	KaratePolaznik(const char* imePrezime, string emailAdresa, string
		lozinka)
		: Korisnik(imePrezime, emailAdresa, lozinka) {
	}
	KaratePolaznik(const KaratePolaznik& obj)
		: Korisnik(obj) {
		for (int i = 0; i < _polozeniPojasevi.size(); i++) {
			_polozeniPojasevi[i] = new Polaganje(*obj._polozeniPojasevi[i]);
		}
	}
	KaratePolaznik& operator=(const KaratePolaznik& obj) {
		if (this != &obj) {

			for (size_t i = 0; i < _polozeniPojasevi.size(); i++) {
				delete _polozeniPojasevi[i];
				_polozeniPojasevi[i] = nullptr;
			}

			(Korisnik&)(*this) = obj;
			for (int i = 0; i < _polozeniPojasevi.size(); i++) {
				_polozeniPojasevi[i] = new Polaganje(*obj._polozeniPojasevi[i]);
			}
		}
		return *this;

	}
	virtual ~KaratePolaznik() {
		cout << crt << "DESTRUKTOR -> KaratePolaznik" << crt;
		for (size_t i = 0; i < _polozeniPojasevi.size(); i++) {
			delete _polozeniPojasevi[i];
			_polozeniPojasevi[i] = nullptr;
		}
	}
	friend ostream& operator<< (ostream& COUT, KaratePolaznik& obj) {
		COUT << obj.GetImePrezime() << " " << obj.GetEmail() << " " <<
			obj.GetLozinka() << endl;
		for (size_t i = 0; i < obj._polozeniPojasevi.size(); i++)
			COUT << *obj._polozeniPojasevi[i];
		COUT << "Prosjek polaznika: "<<obj.ProsjekPolaznika() << endl;

		return COUT;
	}
	vector<Polaganje*>& GetPolozeniPojasevi() { return _polozeniPojasevi; }


	/*nakon evidentiranja tehnike na bilo kojem pojasu kandidatu se salje
   email sa porukom:
	FROM:info@karate.ba
	TO: emailKorisnika
	Postovani ime i prezime, evidentirana vam je thenika X za Y pojas.
   Dosadasnji uspjeh (prosjek ocjena)
	na pojasu Y iznosi F, a ukupni uspjeh (prosjek ocjena) na svim pojasevima
   iznosi Z.
   Fakultet informacijskih tehnologija
   Pismeni dio ispita
   Pozdrav.
	KARATE Team.
	slanje email poruka implemenitrati koristeci zasebne thread-ove.
	*/
	void SaljiMail(Tehnika tehnika, Pojas pojas, float prosjek) {
		m.lock();

		cout << "FROM:info@karate.ba" << endl;
		cout << "TO :"<< GetEmail() << endl;
		cout << "Postovani " << GetImePrezime() << endl;
		cout << ", evidentirana vam je tehnika " << tehnika.GetNaziv() << " za " << ispisiPojas(pojas) << " pojas." << endl;
		cout << " Dosadasnji uspjeh na pojasu " << ispisiPojas(pojas) << " iznosi " << prosjek << ", a ukupni uspjeh(prosjek ocjena) na svim pojasevima iznosi " << ProsjekPolaznika() << endl;
		cout << "Fakultet informacijskih tehnologija" << endl;
		cout << "Pismeni dio ispita\nPozdrav\nKARATE Team." << endl;
		
		m.unlock();
	}

	bool AddTehniku(Pojas pojas, Tehnika tehnika) {
		for (int i = 0; i < _polozeniPojasevi.size(); i++) {
			if (pojas == _polozeniPojasevi[i]->GetPojas()) {
				for (int j = 0; j < _polozeniPojasevi[i]->GetTehnike().size(); j++) {
					if (*_polozeniPojasevi[i]->GetTehnike()[j] == tehnika)
						return false;
				}
			}

			if (pojas - 1 == _polozeniPojasevi[i]->GetPojas()) {
				if (_polozeniPojasevi[i]->GetTehnike().size() < 3
					|| _polozeniPojasevi[i]->ProsjekPojasa() < 3.5)
					return false;
			}

			_polozeniPojasevi[i]->GetTehnike().push_back(new Tehnika(tehnika));

			thread slanjeMaila(&KaratePolaznik::SaljiMail, this, tehnika, pojas, _polozeniPojasevi[i]->ProsjekPojasa());
			slanjeMaila.join();

			return true;
		}

		Polaganje polaganje(Pojas::ZUTI);
		polaganje.GetTehnike().push_back(new Tehnika(tehnika));
		_polozeniPojasevi.push_back(new Polaganje(polaganje));


		int zadnji = _polozeniPojasevi.size() - 1;
		thread slanjeMaila(&KaratePolaznik::SaljiMail, this, tehnika, pojas, _polozeniPojasevi[zadnji]->ProsjekPojasa());
		slanjeMaila.join();

		return true;
	}

	float ProsjekPolaznika()const {
		float suma = 0.0f;

		if (_polozeniPojasevi.size() == 0) return 0;

		for (int i = 0; i < _polozeniPojasevi.size(); i++) {
			suma += _polozeniPojasevi[i]->ProsjekPojasa();
		}

		return suma / _polozeniPojasevi.size();
	}
};
const char* GetOdgovorNaPrvoPitanje() {
	cout << "Pitanje -> Pojasnite ulogu operatora const_cast?\n";
	return "Odgovor -> OVDJE UNESITE VAS ODGOVOR";
}
const char* GetOdgovorNaDrugoPitanje() {
	cout << "Pitanje -> Ukratko opisite redoslijed kreiranja objekta bazne klase u slucaju visestrukog nasljedjivanja(prilikom instanciranja objekta najizvedenije klase), te koja su moguca rjesenja najznacajnijih problema ubtom kontekstu ? \n";
	return "Odgovor -> OVDJE UNESITE VAS ODGOVOR";
}
void main() {
	//cout << PORUKA;
	cin.get();
	cout << GetOdgovorNaPrvoPitanje() << endl;
	cin.get();
	cout << GetOdgovorNaDrugoPitanje() << endl;
	cin.get();
	Datum datum19062020(19, 6, 2020),
		datum20062020(20, 6, 2020),
		datum30062020(30, 6, 2020),
		datum05072020(5, 7, 2020);
	int kolekcijaTestSize = 10;
	Kolekcija<int, int> kolekcija1;
	for (int i = 0; i < kolekcijaTestSize; i++)
		kolekcija1.AddElement(i, i);
	cout << kolekcija1 << endl;
	try {
		/*metoda AddElement baca izuzetak u slucaju da se pokusa
		dodati vise od maksimalnog broja elemenata*/
		kolekcija1.AddElement(11, 11);
	}
	catch (exception& err) {
		cout << crt << "Greska -> " << err.what() << crt;
	}

	cout << "Dodavanje" << endl;
	cout << kolekcija1 << crt;
	kolekcija1.RemoveAt(2);

	/*uklanja par (T1 i T2) iz kolekcije koji se nalazi na lokaciji sa
   proslijedjenim indeksom.
	nakon uklanjanja vrijednosti onemoguciti pojavu praznog prostora unutar
   kolekcije tj.
	pomjeriti sve elemente koji se nalaze nakon proslijedjene lokacije za
   jedno mjesto unazad
	npr. ako unutar kolekcije postoje elementi
	0 0
	1 1
	2 2
	3 3
	nakon uklanjanja vrijednosti na lokaciji 1, sadrzaj kolekcije ce biti
   sljedeci
	0 0
	2 2
	3 3
	*/
	cout << "Uklanjanje" << endl;
	cout << kolekcija1 << crt;
	kolekcija1.AddElement(9, 9, 2);
	/*funkciji AddElement se, kao treci parametar, moze proslijediti i
   lokacija na koju se dodaju
	nove vrijednosti pri cemu treba zadrzati postojece vrijednosti pomjerene
   za jedno mjesto unaprijed
	u odnosu na definisanu lokaciju npr. ako unutar kolekcije postoje
   elementi
	0 0
	1 1
	2 2
	3 3
	nakon dodavanja vrijednosti 9 i 9 na lokaciju 1, sadrzaj kolekcije ce
   biti sljedeci
	0 0
	9 9
	1 1
	2 2
   3 3
	*/
	cout << "Dodaj na lokaciju" << endl;
	cout << kolekcija1 << crt;

	cout << "Dodjela" << endl;
	Kolekcija<int, int> kolekcija2 = kolekcija1;
	cout << kolekcija1 << crt;
	//na osnovu vrijednosti T2 mijenja vrijednost T1.
	kolekcija1[9] = 2;
	/* npr.ako unutar kolekcije postoje elementi:
	0 0
	9 9
	1 1
	2 2
	3 3
	nakon promjene vrijednosti sadrzaj kolekcije ce biti sljedeci
	0 0
	2 9
	1 1
	2 2
	3 3
	*/
	cout << "Uglaste" << endl;
	cout << kolekcija1 << crt;

	Tehnika choku_zuki("choku_zuki"),
		gyaku_zuki("gyaku_zuki"),
		kizami_zuki("kizami_zuki"),
		oi_zuki("oi_zuki");



	/*svaka tehnika moze imati vise ocjena tj. moze se polagati u vise
   navrata.
	- razmak izmedju polaganja dvije tehnike mora biti najmanje 3 dana
	- nije dozvoljeno dodati ocjenu sa ranijim datumom u odnosu na vec
   evidentirane (bez obzira sto je stariji od 3 dana)
	*/



	if (choku_zuki.AddOcjena(1, datum19062020))
		cout << "Ocjena evidentirana!" << endl;
	if (!choku_zuki.AddOcjena(5, datum20062020))
		cout << "Ocjena NIJE evidentirana!" << endl;
	if (choku_zuki.AddOcjena(5, datum30062020))
		cout << "Ocjena evidentirana!" << endl;



	// ispisuje: naziv tehnike, ocjene (zajedno sa datumom) i prosjecnu ocjenu za tu tehniku
		// ukoliko tehnika nema niti jednu ocjenu prosjecna treba biti 0



	cout << choku_zuki << endl;

	if (ValidirajLozinku("john4Do*e"))
		cout << "OK" << crt;
	if (!ValidirajLozinku("john4Doe"))
		cout << "Specijalni znak?" << crt;
	if (!ValidirajLozinku("jo*4Da"))
		cout << "7 znakova?" << crt;
	if (!ValidirajLozinku("4jo-hnoe"))
		cout << "Veliko slovo?" << crt;
	if (ValidirajLozinku("@john2Doe"))
		cout << "OK" << crt;



	/*
	za autentifikaciju svaki korisnik mora posjedovati lozinku koja sadrzi:
	- najmanje 7 znakova
	- velika i mala slova
	- najmanje jedan broj
   - najmanje jedan specijalni znak
	za provjeru validnosti lozinke koristiti globalnu funkciju
   ValidirajLozinku, a unutar nje regex metode.
	validacija lozinke se vrsi unutar konstruktora klase Korisnik, a u
   slucaju da nije validna
	postaviti je na podrazumijevanu vrijednost: <VRIJEDNOST_NIJE_VALIDNA>
	*/



	Korisnik* jasmin = new KaratePolaznik("Jasmin Azemovic",
		"jasmin@karate.ba", "j@sm1N*");
	Korisnik* adel = new KaratePolaznik("Adel Handzic", "adel@edu.karate.ba",
		"4Ade1*H");
	Korisnik* lozinkaNijeValidna = new KaratePolaznik("John Doe",
		"john.doe@google.com", "johndoe");



	/*
	svi kandidati podrazumijevano imaju BIJELI pojas (za njega se ne dodaju
   tehnike)
	sve tehnike na nivou jednog pojasa (ZUTI, ZELENI ... ) se evidentiraju
   unutar istog objekta tipa Polaganje,
	tom prilikom onemoguciti:
	- dodavanje istih (moraju biti identicne vrijednosti svih atributa)
   tehnika na nivou jednog pojasa,
	- dodavanje tehnika za visi pojas ako prethodni pojas nema evidentirane
   najmanje 3 tehnike ili nema prosjecnu ocjenu svih tehnika vecu od 3.5
	(onemoguciti dodavanje tehnike za NARANDZASTI ako ne postoji najmanje 3
   tehnike za ZUTI pojas i njihov prosjek je veci od 3.5)
	funkcija vraca true ili false u zavisnosti od (ne)uspjesnost izvrsenja
	*/
	//doraditi klase da nacin da omoguce izvrsenje naredne linije koda



	KaratePolaznik* jasminPolaznik = dynamic_cast<KaratePolaznik*>(jasmin);

	if (jasminPolaznik != nullptr) {
		if (jasminPolaznik->AddTehniku(ZUTI, gyaku_zuki))
			cout << "Tehnika uspjesno dodan!" << crt;
		//ne treba dodati kizami_zuki jer ne postoje evidentirane 3 tehnike za ZUTI pojas
		if (!jasminPolaznik->AddTehniku(NARANDZASTI, kizami_zuki))
			cout << "Tehnika NIJE uspjesno dodana!" << crt;
		if (jasminPolaznik->AddTehniku(ZUTI, kizami_zuki))
			cout << "Tehnika uspjesno dodan!" << crt;
		if (jasminPolaznik->AddTehniku(ZUTI, oi_zuki))
			cout << "Tehnika uspjesno dodan!" << crt;
		if (jasminPolaznik->AddTehniku(ZUTI, choku_zuki))
			cout << "Tehnika uspjesno dodan!" << crt;
		//ne treba dodati choku_zuki jer je vec dodana za zuti pojas
		if (!jasminPolaznik->AddTehniku(ZUTI, choku_zuki))
			cout << "Tehnika NIJE uspjesno dodana!" << crt;
		//ispisuje sve dostupne podatke o karate polazniku
		cout << *jasminPolaznik << crt;
	}



	/*nakon evidentiranja tehnike na bilo kojem pojasu kandidatu se salje
   email sa porukom:
	FROM:info@karate.ba
	TO: emailKorisnika
	Postovani ime i prezime, evidentirana vam je thenika X za Y pojas.
   Dosadasnji uspjeh (prosjek ocjena)
	na pojasu Y iznosi F, a ukupni uspjeh (prosjek ocjena) na svim pojasevima
   iznosi Z.
   Fakultet informacijskih tehnologija
   Pismeni dio ispita
   Pozdrav.
	KARATE Team.
	slanje email poruka implemenitrati koristeci zasebne thread-ove.
	*/
	//osigurati da se u narednim linijama poziva i destruktor klase KaratePolaznik



	delete jasmin;
	delete adel;
	delete lozinkaNijeValidna;



	cin.get();
	system("pause>0");
}