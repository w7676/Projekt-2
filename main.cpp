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

//Funkcja zwraca adres internetowy podany przez u�ytkownika
string WczytajAdres()
{
	string adres;
	cout << "Podaj adres internetowy (pusty adres przerywa operacje). Nacisnij [ENTER] aby kontynuowac:\n";
	getline(cin, adres);
	return adres;
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
	for (int indeks = 0; indeks < RozmiarBazy; indeks++) //P�tla wykonuj�ca si� dla wszystkich adres�w
	{
		if (Baza[indeks][0].empty()) //Napotkali�my pierwszy pusty adres wi�� nie ma co dalej szuka�
			return -1;

		if (Baza[indeks][0] == adres) //Znale�li�my szukany adres, zwracamy do niego indeks
			return indeks;
	}

	return -1; //Przeszukali�my ca�� baz� i nie znale�li�my identycznego adresu;
}

//Funkcja dodaj�ca adres zwraca indeks dodanego adresu lub -1 je�li si� nie uda�o go doda� z braku miejsca lub -2 je�li istnieje inny adres o tym samym skr�cie
int DodajAdres(string adres)
{
	for (int proba = 0; proba < IloscPowtorzen; proba++)
	{
		string skrot = LosujSkrot(adres);
		int indeks;

		for (indeks = 0; indeks < RozmiarBazy; indeks++) //P�tla wykonuj�ca si� dla wszystkich adres�w
		{
			if (Baza[indeks][1] == skrot) //Natrafili�my na inny adres o takim samym skr�cie
				break; //Przerywamy i pr�bujemy wylosowa� inny skr�t
			else if (Baza[indeks][0].empty()) //Napotkali�my pierwszy pusty adres wstawiamy nasz adres wraz ze skr�tem i wychodzimy z funkcji
			{
				WstawAdres(adres, skrot, indeks);
				return indeks;
			}
		}

		if (indeks == RozmiarBazy) //Pezeszukali�my ca�� baze musia�o zabrakn�� pustego miejsca
			return -1;
	}

	return -2; //Nie mogli�my wylosowa� unikalnego skr�tu
}

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
	cout << "\tWpisz \"p\", aby uzyskac pomoc\n";
	cout << "\tPusta opcja konczy program\n";
}

//Funkcja zwraca opcj� wybran� przez u�ytkownika. Zwraca BrakOpcji je�li nic nie zosta�o wybrane.
char WczytajOpcje()
{
	string linia; cout << "Wybierz opcje (wpisz \"p\", aby uzyskac pomoc):\n";
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
	cout << "\tWpisz \"p\", aby uzyskac pomoc\n";
}

//Procedura wypisuj�ca dost�pne opcje dla menu g��wnego
void WypiszOpcjeUsuwania()
{
	cout << "\tWpisz \"a\", aby usunac adres(y) przy pomocy samego adresu\n";
	cout << "\tWpisz \"p\", aby uzyskac pomoc\n";
	cout << "\tPusta opcja wraca do menu glownego\n";
}

void UsunAdres(int indeks)
{
	Baza[indeks][0].clear(); //Czy�ci adres
	Baza[indeks][1].clear(); //Czy�ci skr�t
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

void MenuUsuwania()
{
	char opcja;

	do
	{
		opcja = WczytajOpcje();

		switch (opcja)
		{
		case 'a':
			Usun();
			break;
		case 'p':
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
		opcja = WczytajOpcje();

		switch (opcja)
		{
			case 'd':
				Dodaj();
				break;
			case 'u':
				MenuUsuwania();
				break;
			case 'p':
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