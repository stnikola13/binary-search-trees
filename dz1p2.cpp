#include <iostream>
#include <cmath>
using namespace std;

double *createArray (double min_val, double max_val, double error, int &noOfElems) {
	//noOfElems = (int)(ceil((max_val - min_val) / error)) + 1; 
	noOfElems = (int)(ceil((max_val - min_val) / (error*2))) + 1;				// Kreiramo niz mogucih brojeva koji ce pokriti sve realne brojeve za zadatu gresku
	double* arr = new double[noOfElems];										// error*2 jer svaka greska ide napred i nazad od broja (1,0.05 obuhvata 0.95 do 1.05).
	for (int i = 0; i < noOfElems; i++) {										// Podesavamo clanove niza.
		//arr[i] = min_val + i * error;
		arr[i] = min_val + i * error*2;
		if (arr[i] > max_val) arr[i] = max_val;
	}
	return arr;
}

int guessingGameV2(double* array, int noOfElems) {								// Druga verzija funkcije.
	int steps = 0;

	int fib[30];
	fib[0] = 0;
	fib[1] = 1;
	for (int i = 2; i < 30; i++) fib[i] = fib[i - 1] + fib[i - 2];				// Inicijalizacija clanova fibonacijevog niza u okviru niza.

	int fm2 = fib[0], fm1 = fib[1], fm = fib[2];								// Vrednosti tri uzastopna fibonacijeva broja.
	int ifm2 = 0, ifm1 = 1, ifm = 2;											// Indeksi ta tri uzastopna fibonacijeva broja u fibonacijevom nizu.

	while (fm < noOfElems - 1) {
		ifm2++; ifm1++; ifm++;
		fm2 = fib[ifm2]; fm1 = fib[ifm1]; fm = fib[ifm];						// Podesavanje da fm (najdesnji) bude na prvoj vrednosti izvan opsega niza.
	}
	if (ifm2 > 0) ifm2--; 
	if (ifm1 > 0) ifm1--; 
	if (ifm > 0) ifm--;															// Vracanje da sva tri budu u opsegu ali skroz desno.

	fm2 = fib[ifm2]; fm1 = fib[ifm1]; fm = fib[ifm];

	bool check = true;
	cout << "\nRacunar ce sada generisati sekvencu brojeva. Za svaki od prikazanih brojeva unesite da li je taj broj manji, veci ili u dozvoljnom opsegu vaseg broja. Unesite 'manji', 'veci' ili 'dobar': \n\n";
	while (check) {
		steps++;
		double cur = array[fm];
		//if (fm > noOfElems - 1) cur = array[noOfElems - 1];
		while (fm > noOfElems - 1) {											// Ako izadjemo iz opsega niza vracamo se sve dok opet sva tri fibonacija ne budu u nizu.
			cur = array[noOfElems - 1];
			fm -= fm2;
			int temp = fm1;
			fm1 = fm2;
			fm2 = temp - fm2;
		}
		cout << "Broj " << cur << ": ";											// Ispis broja i ucitavanje odgovora.
		string ans;
		cin >> ans;
		if (ans == "dobar") {
			break;
		}
		else if (ans == "manji") {												// Ako je broj manji, idemo desno u nizu pa pomeramo fibonacije za jedno mesto napred.
			if (fm1 == 1) return -1;
			fm += fm2;
			fm1 -= fm2;
			fm2 -= fm1;
		}
		else if (ans == "veci") {												// Ako je broj veci, idemo levo u nizu pa pomeramo fibonacije za jedno mesto nazad.
			if (fm2 == 0) {
				cout << "Broj " << array[0] << ": ";
				cin >> ans;
				if (ans == "dobar") {
					break;
				}
				return -1;
			}
			fm -= fm2;
			int temp = fm1;
			fm1 = fm2;
			fm2 = temp - fm2;
		}
		else {																	// Ako se ne unese nista od navedenog.
			steps--;
			cout << "Pogresan unos. Pokusajte ponovo." << endl;
		}
	}
	return steps;
}

int guessingGame(double *array, int noOfElems) { // Prva verzija funkcije
	int steps = 0;

	int fib[30];
	fib[0] = 0;
	fib[1] = 1;
	for (int i = 2; i < 30; i++) fib[i] = fib[i - 1] + fib[i - 2]; // inicijalizacija clanova fibonacijevog niza

	int fm2 = fib[0], fm1 = fib[1], fm = fib[2]; // vrednosti tri uzastopna fibonacijeva broja
	int ifm2 = 0, ifm1 = 1, ifm = 2; // indeksi tri uzastopna fibonacijeva broja u fibonaci nizu

	while (fm < noOfElems - 1) {
		ifm2++; ifm1++; ifm++;
		fm2 = fib[ifm2]; fm1 = fib[ifm1]; fm = fib[ifm]; // podesavanje da fm bude na prvoj vrednosti izvan opsega niza
	}

	double cur;
	int cur_ind = fm2;
	cout << "\nRacunar ce sada generisati sekvencu brojeva. Za svaki od prikazanih brojeva unesite da li je taj broj manji, veci ili u dozvoljnom opsegu vaseg broja. Unesite 'manji', 'veci' ili 'dobar': \n\n";
	while (true) {
		steps++;
		cur = array[cur_ind];
		cout << "Broj " << cur << ": ";
		string ans;
		cin >> ans;
		if (ans == "dobar") {
			return steps;
			break;
		}
		else if (ans == "manji") {
			// ako su prva dva elem fib niza ne mozemo da ih pomerimo u levo pa ne pomeramo fibonacije
			if (!(fm2 == 0 && fm1 == 1)) {
				ifm2--; ifm1--; ifm--;
				fm2 = fib[ifm2]; fm1 = fib[ifm1]; fm = fib[ifm];
			}
			if (fm2 == 0) cur_ind += 1;
			else cur_ind += fm2;
			
		}
		else if (ans == "veci") {
			// ako su prva dva ne pomeramo ih, ako su drugi i treci ne pomeramo ih, u suprotnom pomeramo za dva mesta
			if (!(fm2 == 0 && fm1 == 1 || fm2 == 1 && fm1 == 1)) {
				ifm2 -= 2; ifm1 -= 2; ifm -= 2;
				fm2 = fib[ifm2]; fm1 = fib[ifm1]; fm = fib[ifm];
			}
			if (fm2 == 0) cur_ind -= 1;
			else cur_ind -= fm2;
		}
		else {
			steps--;
			cout << "Pogresan unos. Pokusajte ponovo." << endl;
		}
	}
	return steps;
}

int main() {																	// Glavni program samo poziva funkcije za kreiranje niza i simulaciju igre.

	int n, steps = 0;
	bool check = true;
	double* arr = nullptr;

	while (check) {

		cout << "- - - - - - - Pogadjanje broja - - - - - - -" << endl;
		cout << "1. Zapocni igru" << endl << "2. Zavrsi igru" << endl;
		cout << "Unesite redni broj opcije: ";
		cin >> n;

		if (n <= 0 || n > 2) {
			cout << "Pogresan unos!" << endl;
			continue;
		}

		switch (n) {
		case 1:
			double min, max, err;
			int noOfElems;

			cout << "Molimo unesite donju i gornju granicu opsega, kao i dozvoljenu gresku odvojene razmakom: ";
			cin >> min >> max >> err;

			arr = createArray(min, max, err, noOfElems);
			//steps = guessingGame(arr, noOfElems);
			steps = guessingGameV2(arr, noOfElems);

			if (steps == -1) cout << "Neuspesna igra! Kompjuter je izgubio." << endl;
			else cout << "\nBroj koraka igre je " << steps << "!" << endl << endl;

			break;

		case 2:
			cout << "Hvala na igranju!" << endl;
			check = false;

			if (arr != nullptr) delete[] arr;
			break;
		}
	}
	return 0;
}