#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

const int RozmiarBazy = 10; //Maksymalna liczba adresów internetowych, które mo¿na przechowywaæ
const int MaksymalnaDlugoscAdresu = 255; //Dopuszczalna liczba znaków z których mo¿e siê sk³adaæ adres internetowy, d³u¿sze adresy bêd¹ ucinane
const int DlugoscSkrotu = 4; //Skrót sk³ada siê z tylu znaków
const int IloscPowtorzen = 3; //Podejmujemy tyle prób losowania skrótu jesli wylosowany dotychczas skrót jest ju¿ u¿ywany przez inny adres

string Baza[RozmiarBazy][2]; //Tablica dwuwymiarowa przechowuj¹ca adresy internetowe i ich skróty. Baza[indesk][0] przechowuje adres, Baza[indesk][1] zawiera skrót adresu
char ZnakiSkrotu[]{ 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
const int IloscZnakow = sizeof(ZnakiSkrotu) / sizeof(char); //Liczba znaków przechowywanych w tablicy ZnakiSkrotu

//Funkcja zwraca adres internetowy podany przez u¿ytkownika
string WczytajAdres()
{
	cout << "Podaj adres internetowy (pusty adres konczy program). Nacisnij [ENTER] aby kontynuowac:\n";
	char znaki[MaksymalnaDlugoscAdresu + 1]; //Bufor na adres jest wiêkszy o 1 ze wzglêdu na miejsce na znak '\0' koñcz¹cy ³añcuch znaków
	cin.getline(znaki, MaksymalnaDlugoscAdresu); //Wczytanie znaków adresu do bufora
	string adres(znaki);

	return adres;
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
	for (int indeks = 0; indeks < RozmiarBazy; indeks++) // //Pêtla wykonuj¹ca siê dla wszystkich adresów
	{
		if (Baza[indeks][0].empty()) //Napotkaliœmy pierwszy pusty adres wiêæ nie ma co dalej szukaæ
			return -1;

		if (Baza[indeks][0] == adres) //ZnaleŸliœmy szukany adres, zwracamy do niego indeks
			return indeks;
	}

	return -1; //Przeszukaliœmy ca³¹ bazê i nie znaleŸliœmy identycznego adresu;
}

//Funkcja dodaj¹ca adres zwraca indeks dodanego adresu lub -1 jeœli siê nie uda³o go dodaæ z braku miejsca lub -2 jeœli istnieje inny adres o tym samym skrócie
int DodajAdres(string adres)
{
	for (int proba = 0; proba < IloscPowtorzen; proba++)
	{
		string skrot = LosujSkrot(adres);
		int indeks;

		for (indeks = 0; indeks < RozmiarBazy; indeks++) //Pêtla wykonuj¹ca siê dla wszystkich adresów
		{
			if (Baza[indeks][1] == skrot) //Natrafiliœmy na inny adres o takim samym skrócie
				break; //Przerywamy i próbujemy wylosowaæ inny skrót
			else if (Baza[indeks][0].empty()) //Napotkaliœmy pierwszy pusty adres wstawiamy nasz adres wraz ze skrótem i wychodzimy z funkcji
			{
				WstawAdres(adres, skrot, indeks);
				return indeks;
			}
		}

		if (indeks == RozmiarBazy) //Pezeszukaliœmy ca³¹ baze musia³o zabrakn¹æ pustego miejsca
			return -1;
	}

	return -2; //Nie mogliœmy wylosowaæ unikalnego skrótu
}

int main()
{
	srand((unsigned int)time(NULL)); //Inicjuje punk startowy generatora liczb pseudolosowych przy pomocy aktualnego czasu
	string adres = WczytajAdres(); //Pobiera adres od u¿ytkownika

	while (!adres.empty()) //Jeœli adres nie jest pusty
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
	};

	return 0;
}