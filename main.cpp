#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

const int RozmiarBazy = 10; //Maksymalna liczba adres�w internetowych, kt�re mo�na przechowywa�
const int DlugoscSkrotu = 4; //Skr�t sk�ada si� z tylu znak�w
const int IloscPowtorzen = 3; //Podejmujemy tyle pr�b losowania skr�tu jesli wylosowany dotychczas skr�t jest ju� u�ywany przez inny adres
const char BrakOpcji = '\0'; //Znak symbolizuj�cy nie wybranie, �adnej opcji

string Baza[RozmiarBazy][2]; //Tablica dwuwymiarowa przechowuj�ca adresy internetowe i ich skr�ty. Baza[indesk][0] przechowuje adres, Baza[indesk][1] zawiera skr�t adresu
char ZnakiSkrotu[]{ 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
const int IloscZnakow = sizeof(ZnakiSkrotu) / sizeof(char); //Liczba znak�w przechowywanych w tablicy ZnakiSkrotu

int IloscAdresow = 0; //Zmienna przechowuj�ca aktualn� liczb� dodanych adres�w

//Funkcja zwraca tekst podany przez u�ytkownika
string WczytajTekst(string naglowek, string przedmiot)
{
	string tekst;
	cout << naglowek << " - " << "Podaj " << przedmiot << " (pusty tekst przerywa operacje). Nacisnij[ENTER] aby kontynuowac:\n";
	getline(cin, tekst);
	return tekst;
}

//Funkcja zwraca adres internetowy podany przez u�ytkownika
string WczytajAdres()
{
	return WczytajTekst("DODAWANIE", "adres internetowy");
}

//Zwraca adres internetowy podany przez u�ytkownika
string WczytajAdresDoSzukania()
{
	return WczytajTekst("WYSZUKIWANIE", "adres internetowy");
}

//Zwraca skr�t adresu podany przez u�ytkownika
string WczytajSkrotDoSzukania()
{
	return WczytajTekst("WYSZUKIWANIE", "skrot");
}

//Zwraca indeks podany przez u�ytkownika, lub -1 je�li podano pusty tekst
int WczytajIndeks()
{
	string tekst = WczytajTekst("WYSZUKIWANIE", "indeks");

	if (tekst.empty())
		return -1;
	else
		return atoi(tekst.c_str());
}

//Procedura wypisuje wiadomo�� i informacje o adresie z wybranego indeksu
void WypiszAdres(string wiadomosc, int indeks)
{
	cout << "\tNa pozycji: " << indeks << " " << wiadomosc << " adres: \"" << Baza[indeks][0] << "\" jego skrot wynosi: \"" << Baza[indeks][1] << "\"\n";
}

//Procedura wpisuj�ca adres i skr�t pod zadanym indeskem 
void WstawAdres(string adres, string skrot, int indeks)
{
	Baza[indeks][0] = adres;
	Baza[indeks][1] = skrot;
	IloscAdresow++; //Zwi�ksza aktualn� liczb� adres�w
}

//Funkcja zwracaj�ca skr�t dla adresu
string LosujSkrot(string adres)
{
	string skrot;

	for (int znak = 0; znak < DlugoscSkrotu; znak++) //P�tla wykonuj�ca si� dla ka�dego znaku w skr�cie
	{
		int indeks = rand() % IloscZnakow; //Generujemy losowy indeks z zakresu od 0 do IloscZankow - 1
		skrot = skrot + ZnakiSkrotu[indeks]; //Dodajemy wylosowany znak do skr�tu
	}

	return skrot;
}

//Funkcaj zwraca indeks do znalezionego adresu lub -1 je�li nic nie znalaz�a
int ZnajdzAdres(string adres)
{
	int adresyDoSprawdzenia = IloscAdresow;
	int indeks = 0;

	while (adresyDoSprawdzenia > 0) //P�tla wykonuj�ca si� dla wszystkich adres�w
	{
		if (!Baza[indeks][0].empty()) //Napotkali�my pierwszy adres do sprawdzenia
		{
			if (Baza[indeks][0] == adres) //Znale�li�my szukany adres, zwracamy do niego indeks
				return indeks;

			adresyDoSprawdzenia--; //Zmniejszamy pul� adres�w do sprawdzenia
		}

		indeks++; //B�dziemy sprawdza� nast�pny indeks
	}

	return -1; //Przeszukali�my ca�� baz� i nie znale�li�my identycznego adresu
}

//Funkcaj zwraca indeks do znalezionego adresu lub -1 je�li nic nie znalaz�a
int ZnajdzAdresPoSkrocie(string skrot)
{
	int skrotyDoSprawdzenia = IloscAdresow;
	int indeks = 0;

	while (skrotyDoSprawdzenia > 0) //P�tla wykonuj�ca si� dla wszystkich adres�w
	{
		if (!Baza[indeks][1].empty()) //Napotkali�my pierwszy skr�t do sprawdzenia
		{
			if (Baza[indeks][1] == skrot) //Znale�li�my szukany adres, zwracamy do niego indeks
				return indeks;

			skrotyDoSprawdzenia--; //Zmniejszamy pul� adres�w do sprawdzenia
		}

		indeks++; //B�dziemy sprawdza� nast�pny indeks
	}

	return -1; //Przeszukali�my ca�� baz� i nie znale�li�my identycznego adresu
}

//Funkcja dodaj�ca adres zwraca indeks dodanego adresu lub -1 je�li si� nie uda�o go doda� z braku miejsca lub -2 je�li istnieje inny adres o tym samym skr�cie
int DodajAdres(string adres)
{
	for (int proba = 0; proba < IloscPowtorzen; proba++)
	{
		string skrot = LosujSkrot(adres);
		int adresyDoSprawdzenia = IloscAdresow + 1; //Musimy wykona� o jedno sprawdzenie wi�cej w przypadku gdy dodajemy adres jako ostatni
		int indeks = 0;

		do
		{
			if (indeks == RozmiarBazy) //Przeszukali�my ca�� baz� musia�o zabrakn�� pustego miejsca
				return -1;
			else if (Baza[indeks][1] == skrot) //Natrafili�my na inny adres o takim samym skr�cie
				break; //Przerywamy i pr�bujemy wylosowa� inny skr�t
			else if (Baza[indeks][0].empty()) //Napotkali�my pierwszy pusty adres wstawiamy nasz adres wraz ze skr�tem i wychodzimy z funkcji
			{
				WstawAdres(adres, skrot, indeks);
				return indeks;
			}
			else
			{
				adresyDoSprawdzenia--;
				indeks++;
			}
		} while (adresyDoSprawdzenia > 0); //P�tla wykonuj�ca si� dla wszystkich adres�w + 1
	}

	return -2; //Nie mogli�my wylosowa� unikalnego skr�tu
}

//Procedura dodawania adres�w
void Dodaj()
{
	string adres = WczytajAdres(); //Pobiera adres od u�ytkownika

	while (!adres.empty())
	{
		int indeks = ZnajdzAdres(adres); //Szukaj adresu

		if (indeks >= 0) //Znaleziono adres
			WypiszAdres("znaleziono", indeks);
		else //W przeciwnym przypadku spr�buj doda� adres
		{
			cout << "\tNie znaleziono adresu\n";
			indeks = DodajAdres(adres);

			if (indeks >= 0) //Adres zosta� wstawiony pod indeksem
				WypiszAdres("dodano", indeks);
			else if (indeks == -1)
				cout << "\tBrak wolnego miejsca na dodanie adresu. Baza jest zapelniona!\n";
			else //Nasza funkcja losuj�ca skr�t zawiod�a inny adres o takim samym skr�cie ju� istnieje
				cout << "\tNie mozna stworzyc unikalnego skrotu dla adresu!\n";
		}

		adres = WczytajAdres();
	}
}

//Procedura wypisuj�ca dost�pne opcje dla menu g��wnego
void WypiszOpcjeMenuGlownego()
{
	cout << "\tWpisz \"d\", aby dodac adres(y)\n";
	cout << "\tWpisz \"u\", aby usunac adres(y)\n";
	cout << "\tWpisz \"s\", aby szukac adres(u|y)\n";
	cout << "\tWpisz \"o\", aby uzyskac liste opcji\n";
	cout << "\tPusta opcja konczy program\n";
}

//Funkcja zwraca opcj� wybran� przez u�ytkownika. Zwraca BrakOpcji je�li nic nie zosta�o wybrane.
char WczytajOpcje()
{
	string linia; cout << "Wybierz opcje (wpisz \"o\", aby uzyskac liste opcji):\n";
	getline(cin, linia); //Wczytaj lini�
	
	if (linia.empty()) //Wczytano pust� linijk�
		return BrakOpcji;
	else
		return linia[0];
}

//Procedura wypisuj�ca nierozpoznan� opcj�
void WypiszNieznanaOpcje(char opcja)
{
	cout << "\tNieznana opcja \"" << opcja << "\"\n";
	cout << "\tWpisz \"o\", aby uzyskac liste opcji\n";
}

//Procedura wypisuj�ca dost�pne opcje dla menu g��wnego
void WypiszOpcjeUsuwania()
{
	cout << "\tWpisz \"a\", aby usunac adres(y) przy pomocy samego adresu\n";
	cout << "\tWpisz \"o\", aby uzyskac liste opcji\n";
	cout << "\tPusta opcja wraca do menu glownego\n";
}

//Usuwa adres na danej pozycji
void UsunAdres(int indeks)
{
	Baza[indeks][0].clear(); //Czy�ci adres
	Baza[indeks][1].clear(); //Czy�ci skr�t
	IloscAdresow--; //Zmniejsza ilo�� adres�w
}

//Usuwa adresy wykorzystuj�c sam adres
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
		if (indeks >= RozmiarBazy)
			cout << "\tPodano niepoprawny indeks\n";
		else if (Baza[indeks][0].empty())
			cout << "\tPod podanym indeksem nic sie nie znajduje\n";
		else
			WypiszAdres("znaleziono", indeks);

		indeks = WczytajIndeks();
	}
}

void SzukajPoAdresie()
{
	string adres = WczytajAdresDoSzukania();

	while (!adres.empty())
	{
		int indeks = ZnajdzAdres(adres);

		if (indeks < 0)
			cout << "\tNie znaleziono adresu\n";
		else
			WypiszAdres("znaleziono", indeks);

		adres = WczytajAdresDoSzukania();
	}
}

void SzukajPoSkrocie()
{
	string skrot = WczytajSkrotDoSzukania();

	while (!skrot.empty())
	{
		if (skrot.length() != DlugoscSkrotu)
			cout << "\tPodano niepoprawny skrot, skrot musi skladac sie z " << DlugoscSkrotu << " znakow\n";
		else
		{
			int indeks = ZnajdzAdresPoSkrocie(skrot);

			if (indeks < 0)
				cout << "\tNie znaleziono adresu\n";
			else
				WypiszAdres("znaleziono", indeks);
		}

		skrot = WczytajSkrotDoSzukania();
	}
}


void WypiszOpcjeSzukania()
{
	cout << "\tWpisz \"i\", aby znalezc adres(y) za pomoca indeksu\n";
	cout << "\tWpisz \"a\", aby znalezc adres(y) za pomoca adresu\n";
	cout << "\tWpisz \"s\", aby znalezc adres(y) za pomoca skrotu\n";
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
		case 'a':
			SzukajPoAdresie();
			break;
		case 's':
			SzukajPoSkrocie();
			break;
		case 'o':
			WypiszOpcjeSzukania();
			break;
		case BrakOpcji:
			return; //Wracamy do menu g�ownego
		default:
			WypiszNieznanaOpcje(opcja);
			break;
		}
	} while (true); //Dop�ki u�ytkownik wybierze cokolwiek, pozostajemy na poziomie menu szukania
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
			return; //Wracamy do menu g�ownego
		default:
			WypiszNieznanaOpcje(opcja);
			break;
		}
	} while (true); //Dop�ki u�ytkownik wybierze cokolwiek, pozostajemy na poziomie menu usuwania
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
				return; //Ko�czymy program
			default:
				WypiszNieznanaOpcje(opcja);
				break;
		}
	} while (true); //Dop�ki u�ytkownik wybierze cokolwiek, kontunuujemy program
}

int main()
{
	srand((unsigned int)time(NULL)); //Inicjuje punk startowy generatora liczb pseudolosowych przy pomocy aktualnego czasu
	MenuGlowne(); //Przechodzi do menu g�ownego programu
	return 0;
}