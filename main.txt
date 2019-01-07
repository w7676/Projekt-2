#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

const int RozmiarBazy = 10; //Maksymalna liczba adresów internetowych, które mo¿na przechowywaæ
const int DlugoscSkrotu = 4; //Skrót sk³ada siê z tylu znaków
const int IloscPowtorzen = 3; //Podejmujemy tyle prób losowania skrótu jesli wylosowany dotychczas skrót jest ju¿ u¿ywany przez inny adres
const char BrakOpcji = '\0'; //Znak symbolizuj¹cy nie wybranie, ¿adnej opcji

string Baza[RozmiarBazy][2]; //Tablica dwuwymiarowa przechowuj¹ca adresy internetowe i ich skróty. Baza[indesk][0] przechowuje adres, Baza[indesk][1] zawiera skrót adresu
char ZnakiSkrotu[]{ 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
const int IloscZnakow = sizeof(ZnakiSkrotu) / sizeof(char); //Liczba znaków przechowywanych w tablicy ZnakiSkrotu

int IloscAdresow = 0; //Zmienna przechowuj¹ca aktualn¹ liczbê dodanych adresów

//Funkcja zwraca tekst podany przez u¿ytkownika
string WczytajTekst(string naglowek, string przedmiot)
{
	string tekst;
	cout << naglowek << " - " << "Podaj " << przedmiot << " (pusty tekst przerywa operacje). Nacisnij[ENTER] aby kontynuowac:\n";
	getline(cin, tekst);
	return tekst;
}

//Funkcja zwraca adres internetowy podany przez u¿ytkownika
string WczytajAdres()
{
	return WczytajTekst("DODAWANIE", "adres internetowy");
}

//Zwraca indeks podany przez u¿ytkownika, lub -1 jeœli podano pusty tekst
int WczytajIndeks()
{
	string tekst = WczytajTekst("WYSZUKIWANIE", "indeks");

	if (tekst.empty())
		return -1;
	else
		return atoi(tekst.c_str());
}

//Procedura wypisuje wiadomoœæ i informacje o adresie z wybranego indeksu
void WypiszAdres(string wiadomosc, int indeks)
{
	cout << "\tNa pozycji: " << indeks << " " << wiadomosc << " adres: \"" << Baza[indeks][0] << "\" jego skrot wynosi: \"" << Baza[indeks][1] << "\"\n";
}

//Procedura wpisuj¹ca adres i skrót pod zadanym indeskem 
void WstawAdres(string adres, string skrot, int indeks)
{
	Baza[indeks][0] = adres;
	Baza[indeks][1] = skrot;
	IloscAdresow++; //Zwiêksza aktualn¹ liczbê adresów
}

//Funkcja zwracaj¹ca skrót dla adresu
string LosujSkrot(string adres)
{
	string skrot;

	for (int znak = 0; znak < DlugoscSkrotu; znak++) //Pêtla wykonuj¹ca siê dla ka¿dego znaku w skrócie
	{
		int indeks = rand() % IloscZnakow; //Generujemy losowy indeks z zakresu od 0 do IloscZankow - 1
		skrot = skrot + ZnakiSkrotu[indeks]; //Dodajemy wylosowany znak do skrótu
	}

	return skrot;
}

//Funkcaj zwraca indeks do znalezionego adresu lub -1 jeœli nic nie znalaz³a
int ZnajdzAdres(string adres)
{
	int adresyDoSprawdzenia = IloscAdresow;
	int indeks = 0;

	while (adresyDoSprawdzenia > 0) //Pêtla wykonuj¹ca siê dla wszystkich adresów
	{
		if (!Baza[indeks][0].empty()) //Napotkaliœmy pierwszy adres do sprawdzenia
		{
			if (Baza[indeks][0] == adres) //ZnaleŸliœmy szukany adres, zwracamy do niego indeks
				return indeks;

			adresyDoSprawdzenia--; //Zmniejszamy pulê adresów do sprawdzenia
		}

		indeks++; //Bêdziemy sprawdzaæ nastêpny indeks
	}

	return -1; //Przeszukaliœmy ca³¹ bazê i nie znaleŸliœmy identycznego adresu
}

//Funkcja dodaj¹ca adres zwraca indeks dodanego adresu lub -1 jeœli siê nie uda³o go dodaæ z braku miejsca lub -2 jeœli istnieje inny adres o tym samym skrócie
int DodajAdres(string adres)
{
	for (int proba = 0; proba < IloscPowtorzen; proba++)
	{
		string skrot = LosujSkrot(adres);
		int adresyDoSprawdzenia = IloscAdresow + 1; //Musimy wykonaæ o jedno sprawdzenie wiêcej w przypadku gdy dodajemy adres jako ostatni
		int indeks = 0;

		do
		{
			if (indeks == RozmiarBazy) //Przeszukaliœmy ca³¹ bazê musia³o zabrakn¹æ pustego miejsca
				return -1;
			else if (Baza[indeks][1] == skrot) //Natrafiliœmy na inny adres o takim samym skrócie
				break; //Przerywamy i próbujemy wylosowaæ inny skrót
			else if (Baza[indeks][0].empty()) //Napotkaliœmy pierwszy pusty adres wstawiamy nasz adres wraz ze skrótem i wychodzimy z funkcji
			{
				WstawAdres(adres, skrot, indeks);
				return indeks;
			}
			else
			{
				adresyDoSprawdzenia--;
				indeks++;
			}
		} while (adresyDoSprawdzenia > 0); //Pêtla wykonuj¹ca siê dla wszystkich adresów + 1
	}

	return -2; //Nie mogliœmy wylosowaæ unikalnego skrótu
}

//Procedura dodawania adresów
void Dodaj()
{
	string adres = WczytajAdres(); //Pobiera adres od u¿ytkownika

	while (!adres.empty())
	{
		int indeks = ZnajdzAdres(adres); //Szukaj adresu

		if (indeks >= 0) //Znaleziono adres
			WypiszAdres("znaleziono", indeks);
		else //W przeciwnym przypadku spróbuj dodaæ adres
		{
			cout << "\tNie znaleziono adresu\n";
			indeks = DodajAdres(adres);

			if (indeks >= 0) //Adres zosta³ wstawiony pod indeksem
				WypiszAdres("dodano", indeks);
			else if (indeks == -1)
				cout << "\tBrak wolnego miejsca na dodanie adresu. Baza jest zapelniona!\n";
			else //Nasza funkcja losuj¹ca skrót zawiod³a inny adres o takim samym skrócie ju¿ istnieje
				cout << "\tNie mozna stworzyc unikalnego skrotu dla adresu!\n";
		}

		adres = WczytajAdres();
	}
}

//Procedura wypisuj¹ca dostêpne opcje dla menu g³ównego
void WypiszOpcjeMenuGlownego()
{
	cout << "\tWpisz \"d\", aby dodac adres(y)\n";
	cout << "\tWpisz \"u\", aby usunac adres(y)\n";
	cout << "\tWpisz \"s\", aby szukac adres(u|y)\n";
	cout << "\tWpisz \"o\", aby uzyskac liste opcji\n";
	cout << "\tPusta opcja konczy program\n";
}

//Funkcja zwraca opcjê wybran¹ przez u¿ytkownika. Zwraca BrakOpcji jeœli nic nie zosta³o wybrane.
char WczytajOpcje()
{
	string linia; cout << "Wybierz opcje (wpisz \"o\", aby uzyskac liste opcji):\n";
	getline(cin, linia); //Wczytaj liniê
	
	if (linia.empty()) //Wczytano pust¹ linijkê
		return BrakOpcji;
	else
		return linia[0];
}

//Procedura wypisuj¹ca nierozpoznan¹ opcjê
void WypiszNieznanaOpcje(char opcja)
{
	cout << "\tNieznana opcja \"" << opcja << "\"\n";
	cout << "\tWpisz \"o\", aby uzyskac liste opcji\n";
}

//Procedura wypisuj¹ca dostêpne opcje dla menu g³ównego
void WypiszOpcjeUsuwania()
{
	cout << "\tWpisz \"a\", aby usunac adres(y) przy pomocy samego adresu\n";
	cout << "\tWpisz \"o\", aby uzyskac liste opcji\n";
	cout << "\tPusta opcja wraca do menu glownego\n";
}

//Usuwa adres na danej pozycji
void UsunAdres(int indeks)
{
	Baza[indeks][0].clear(); //Czyœci adres
	Baza[indeks][1].clear(); //Czyœci skrót
	IloscAdresow--; //Zmniejsza iloœæ adresów
}

//Usuwa adresy wykorzystuj¹c sam adres
void Usun()
{
	string adres = WczytajAdres();

	while (!adres.empty())
	{
		int indeks = ZnajdzAdres(adres);

		if (indeks == -1)
			cout << "\tNie znaleziono adresu do usuniecia\n";
		else
		{
			UsunAdres(indeks);
			cout << "\tUsunieto adres\n";
		}

		adres = WczytajAdres();
	}
}

void SzukajPoIndeksie()
{
	int indeks = WczytajIndeks();

	while (indeks >= 0)
	{
		if (Baza[indeks][0].empty())
			cout << "\tPod podanym indeksem nic sie nie znajduje\n";
		else
			WypiszAdres("znaleziono", indeks);

		indeks = WczytajIndeks();
	}
}


void WypiszOpcjeSzukania()
{
	cout << "\tWpisz \"i\", aby znalezc adres(y) za pomoca indeksu\n";
	cout << "\tWpisz \"o\", aby uzyskac liste opcji\n";
	cout << "\tPusta opcja wraca do menu glownego\n";
}

void MenuSzukania()
{
	char opcja;

	do
	{
		cout << "MENU SZUKANIA - ";
		opcja = WczytajOpcje();

		switch (opcja)
		{
		case 'i':
			SzukajPoIndeksie();
			break;
		case 'o':
			WypiszOpcjeSzukania();
			break;
		case BrakOpcji:
			return; //Wracamy do menu g³ownego
		default:
			WypiszNieznanaOpcje(opcja);
			break;
		}
	} while (true); //Dopóki u¿ytkownik wybierze cokolwiek, pozostajemy na poziomie menu szukania
}

void MenuUsuwania()
{
	char opcja;

	do
	{
		cout << "MENU USUWANIA - ";
		opcja = WczytajOpcje();

		switch (opcja)
		{
		case 'a':
			Usun();
			break;
		case 'o':
			WypiszOpcjeUsuwania();
			break;
		case BrakOpcji:
			return; //Wracamy do menu g³ownego
		default:
			WypiszNieznanaOpcje(opcja);
			break;
		}
	} while (true); //Dopóki u¿ytkownik wybierze cokolwiek, pozostajemy na poziomie menu usuwania
}

void MenuGlowne()
{
	char opcja;

	do
	{
		cout << "MENU GLOWNE - ";
		opcja = WczytajOpcje();

		switch (opcja)
		{
			case 'd':
				Dodaj();
				break;
			case 'u':
				MenuUsuwania();
				break;
			case 's':
				MenuSzukania();
				break;
			case 'o':
				WypiszOpcjeMenuGlownego();
				break;
			case BrakOpcji:
				return; //Koñczymy program
			default:
				WypiszNieznanaOpcje(opcja);
				break;
		}
	} while (true); //Dopóki u¿ytkownik wybierze cokolwiek, kontunuujemy program
}

int main()
{
	srand((unsigned int)time(NULL)); //Inicjuje punk startowy generatora liczb pseudolosowych przy pomocy aktualnego czasu
	MenuGlowne(); //Przechodzi do menu g³ownego programu
	return 0;
}