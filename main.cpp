#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <fstream>

using namespace std;

const int RozmiarBazy = 10; //Maksymalna liczba adres�w internetowych, kt�re mo�na przechowywa�
const int DlugoscSkrotu = 4; //Skr�t sk�ada si� z tylu znak�w
const int IloscPowtorzen = 3; //Podejmujemy tyle pr�b losowania skr�tu jesli wylosowany dotychczas skr�t jest ju� u�ywany przez inny adres
const char BrakOpcji = '\0'; //Znak symbolizuj�cy nie wybranie, �adnej opcji
const char Separator = ','; //Znak u�ywany do rozdzielenia adresu od skr�tu w pliku. Musi to by� znak nie wykorzystywany w adresie i skr�cie.

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
string WczytajAdresDoDodania()
{
	return WczytajTekst("DODAWANIE", "adres internetowy");
}

//Zwraca adres internetowy podany przez u�ytkownika
string WczytajAdresDoSzukania()
{
	return WczytajTekst("WYSZUKIWANIE", "adres internetowy");
}

//Funkcja zwraca adres internetowy podany przez u�ytkownika
string WczytajAdresDoUsuwania()
{
    return WczytajTekst("USUWANIE", "adres internetowy");
}

//Zwraca skr�t adresu podany przez u�ytkownika
string WczytajSkrotDoUsuwania()
{
    return WczytajTekst("USUWANIE", "skrot");
}

//Zwraca skr�t adresu podany przez u�ytkownika
string WczytajSkrotDoSzukania()
{
	return WczytajTekst("WYSZUKIWANIE", "skrot");
}

//Zwraca indeks podany przez u�ytkownika, lub -1 je�li podano pusty tekst
int WczytajIndeksDoSzukania()
{
	string tekst = WczytajTekst("WYSZUKIWANIE", "indeks");

	if (tekst.empty())
		return -1;
	else
		return atoi(tekst.c_str());
}

//Zwraca indeks podany przez u�ytkownika, lub -1 je�li podano pusty tekst
int WczytajIndeksDoUsuwania()
{
    string tekst = WczytajTekst("USUWANIE", "indeks");

    if (tekst.empty())
        return -1;
    else
        return atoi(tekst.c_str());
}

//Funkcja zwraca nazw� pliku podan� przez u�ytkownika
string WczytajNazwe()
{
    string nazwa;
    cout << "Podaj nazwe pliku (pusty nazwa przerywa operacje). Nacisnij[ENTER] aby kontynuowac:\n";
    getline(cin, nazwa);
    return nazwa;
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

//Funkcaj zwraca indeks do znalezionego adresu lub -1 je�li nic nie znalaz�a. Parametr indeksTekstu s�u�y do wyboru sprawdzania po samym adresie lub po jego skr�cie
int ZnajdzAdres(string tekst, int indeksTekstu)
{
	int adresyDoSprawdzenia = IloscAdresow;
	int indeks = 0;

	while (adresyDoSprawdzenia > 0) //P�tla wykonuj�ca si� dla wszystkich adres�w
	{
		if (!Baza[indeks][indeksTekstu].empty()) //Napotkali�my pierwszy tekst do sprawdzenia
		{
			if (Baza[indeks][indeksTekstu] == tekst) //Znale�li�my szukany adres, zwracamy do niego indeks
				return indeks;

			adresyDoSprawdzenia--; //Zmniejszamy pul� adres�w do sprawdzenia
		}

		indeks++; //B�dziemy sprawdza� nast�pny indeks
	}

	return -1; //Przeszukali�my ca�� baz� i nie znale�li�my identycznego adresu
}

//Funkcaj zwraca indeks do znalezionego adresu lub -1 je�li nic nie znalaz�a
int ZnajdzAdresPoAdresie(string adres)
{
	return ZnajdzAdres(adres, 0);
}

//Funkcaj zwraca indeks do znalezionego adresu lub -1 je�li nic nie znalaz�a
int ZnajdzAdresPoSkrocie(string skrot)
{
	return ZnajdzAdres(skrot, 1);
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
	string adres = WczytajAdresDoDodania(); //Pobiera adres od u�ytkownika

	while (!adres.empty())
	{
		int indeks = ZnajdzAdresPoAdresie(adres); //Szukaj adresu

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

		adres = WczytajAdresDoDodania();
	}
}

//Procedura wypisuj�ca dost�pne opcje dla menu g��wnego
void WypiszOpcjeMenuGlownego()
{
	cout << "\tWpisz \"d\", aby dodac adres(y)\n";
	cout << "\tWpisz \"u\", aby usunac adres(y)\n";
	cout << "\tWpisz \"s\", aby szukac adres(u|y)\n";
	cout << "\tWpisz \"w\", aby wypisac wszystkie adresy\n";
	cout << "\tWpisz \"e\", aby eksportowac adresy do pliku\n";
    cout << "\tWpisz \"i\", aby importowac adresy z pliku\n";
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
	cout << "\tWpisz \"i\", aby usunac adres(y) przy pomocy indeksu\n";
	cout << "\tWpisz \"a\", aby usunac adres(y) przy pomocy samego adresu\n";
	cout << "\tWpisz \"s\", aby usunac adres(y) przy pomocy skrotu\n";
	cout << "\tWpisz \"c\", aby usunac wszystkie adresy\n";
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
void UsunPoAdresie()
{
	string adres = WczytajAdresDoUsuwania();

	while (!adres.empty())
	{
		int indeks = ZnajdzAdresPoAdresie(adres);

		if (indeks == -1)
			cout << "\tNie znaleziono adresu do usuniecia\n";
		else
		{
			UsunAdres(indeks);
			cout << "\tUsunieto adres\n";
		}

		adres = WczytajAdresDoUsuwania();
	}
}

//Usuwa adresy wykorzystuj�c indeks
void UsunPoIndeksie()
{
    int indeks = WczytajIndeksDoUsuwania();

    while (indeks >= 0)
    {
        if (indeks >= RozmiarBazy)
            cout << "\tPodano niepoprawny indeks\n";
        else if (Baza[indeks][0].empty())
            cout << "\tPod podanym indeksem nic sie nie znajduje\n";
        else
        {
            UsunAdres(indeks);
            cout << "\tUsunieto adres\n";
        }

        indeks = WczytajIndeksDoUsuwania();
    }
}

//Usuwa adresy wykorzystuj�c skrot
void UsunPoSkrocie()
{
    string skrot = WczytajSkrotDoUsuwania();

    while (!skrot.empty())
    {
        if (skrot.length() != DlugoscSkrotu)
            cout << "\tPodano niepoprawny skrot, skrot musi skladac sie z " << DlugoscSkrotu << " znakow\n";
        else
        {
            int indeks = ZnajdzAdresPoSkrocie(skrot);

            if (indeks == -1)
                cout << "\tNie znaleziono adresu do usuniecia\n";
            else
            {
                UsunAdres(indeks);
                cout << "\tUsunieto adres\n";
            }
        }

        skrot = WczytajSkrotDoUsuwania();
    }
}

//Czy�ci wszystkie adresy
void UsunWszystko()
{
    int adresyDoUsuniecia = IloscAdresow;
    int liczbaAdresow = IloscAdresow;
    int indeks = 0;

    while (adresyDoUsuniecia > 0) //P�tla wykonuj�ca si� dla wszystkich adres�w
    {
        if (!Baza[indeks][0].empty()) //Napotkali�my pierwszy adres do usuni�cia
        {
            UsunAdres(indeks);
            adresyDoUsuniecia--; //Zmniejszamy pul� adres�w do usuni�cia
        }

        indeks++; //B�dziemy sprawdza� nast�pny indeks
    }

    cout << "\tUsunieto " << liczbaAdresow << " adres(y|ow)\n";
}

//Wypisuje wszystkie adresy
void WypiszAdresy()
{
    int adresyDoWypisania = IloscAdresow;
    int indeks = 0;

    while (adresyDoWypisania > 0) //P�tla wykonuj�ca si� dla wszystkich adres�w
    {
        if (!Baza[indeks][0].empty()) //Napotkali�my pierwszy adres do wypisania
        {
            WypiszAdres("znajduje sie", indeks);
            adresyDoWypisania--; //Zmniejszamy pul� adres�w
        }

        indeks++; //B�dziemy sprawdza� nast�pny indeks
    }
}

//Znajduje adresy pos�uguj�c si� indeksem
void SzukajPoIndeksie()
{
	int indeks = WczytajIndeksDoSzukania();

	while (indeks >= 0)
	{
		if (indeks >= RozmiarBazy)
			cout << "\tPodano niepoprawny indeks\n";
		else if (Baza[indeks][0].empty())
			cout << "\tPod podanym indeksem nic sie nie znajduje\n";
		else
			WypiszAdres("znaleziono", indeks);

		indeks = WczytajIndeksDoSzukania();
	}
}

//Znajduje adresy pos�uguj�c si� samym adresem
void SzukajPoAdresie()
{
	string adres = WczytajAdresDoSzukania();

	while (!adres.empty())
	{
		int indeks = ZnajdzAdresPoAdresie(adres);

		if (indeks < 0)
			cout << "\tNie znaleziono adresu\n";
		else
			WypiszAdres("znaleziono", indeks);

		adres = WczytajAdresDoSzukania();
	}
}

//Znajduje adresy pos�uguj�c si� skr�tem adresu
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

//Zapisuje adresy do pliku
void EksportujAdresy()
{
    string nazwa = WczytajNazwe();

    if (!nazwa.empty())
    {
        ofstream plik(nazwa.c_str()); //otwiera plik do zapisu
        int adresyDoZapisania = IloscAdresow;
        int indeks = 0;

        while (adresyDoZapisania > 0) //P�tla wykonuj�ca si� dla wszystkich adres�w
        {
            if (!Baza[indeks][0].empty()) //Napotkali�my pierwszy adres do zapisania
            {
                plik << Baza[indeks][0] << Separator << Baza[indeks][1] << "\n";
                adresyDoZapisania--; //Zmniejszamy pul� adres�w
            }

            indeks++; //B�dziemy sprawdza� nast�pny indeks
        }

        cout << "\tZapisano " << IloscAdresow << "\n";
    }
}

//Zwraca indeks pierwszego pustego miejsca lub -1 jesli wszystko jest zaj�te
int ZnajdzWolneMiejsce()
{
    for (int indeks = 0; indeks < RozmiarBazy; indeks++)
    {
        if (Baza[indeks][0].empty()) //Wystarczy, �e sprawdzimy czy sam adres jest pusty
            return indeks;
    }

    return -1; //Wszystko jest u�ywane
}

//Wczytuje adresy wraz ze skr�tami z pliku
void ImportujAdresy()
{
    string nazwa = WczytajNazwe();

    if (!nazwa.empty())
    {
        ifstream plik(nazwa.c_str()); //otwiera plik do odczytu
        int wczytaneAdresy = 0;
        int dodaneAdresy = 0;

        while (!plik.eof()) //P�tla wykonuje si� dop�ki nie przeczytamy ca�ego pliku
        {
            string linia;
            getline(plik, linia);

            if (!linia.empty()) //Je�li uda�o si� cokolwiek odczyta�
            {
                string::size_type indeksSeparatora = linia.find(Separator); //Znajdujemy indeks znaku separatora

                if (indeksSeparatora != string::npos && indeksSeparatora < linia.size()) //Uda�o si� znale�� separator i nie jest on ostatnim znakiem
                {
                    string adres = linia.substr(0, indeksSeparatora); //Wyci�gamy adres
                    string skrot = linia.substr(indeksSeparatora + 1); //Wyci�gamy skr�t
                    wczytaneAdresy++;

                    int indeks = ZnajdzAdresPoAdresie(adres);

                    if (indeks == -1) //Nie znaleziono takiego adresu
                    {
                        int indeks = ZnajdzAdresPoSkrocie(skrot);

                        if (indeks == -1) //Nie znaleziono takiego skr�tu
                        {
                            indeks = ZnajdzWolneMiejsce();

                            if (indeks != -1)
                            {
                                WstawAdres(adres, skrot, indeks);
                                dodaneAdresy++;
                            }
                        }
                    }
                }
            }
        }

        cout << "\tDodano " << dodaneAdresy << " z " << wczytaneAdresy << "\n";
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
	    case 'i':
            UsunPoIndeksie();
            break;
		case 'a':
			UsunPoAdresie();
			break;
		case 's':
            UsunPoSkrocie();
            break;
        case 'c':
            UsunWszystko();
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
			case 'w':
                WypiszAdresy();
                break;
            case 'e':
                EksportujAdresy();
                break;
            case 'i':
                ImportujAdresy();
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
