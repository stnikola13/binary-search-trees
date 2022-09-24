#include <iostream>
#include <fstream>
#include <string>
#include <stack>
#include <queue>
using namespace std;

bool doTheySharePrefix(string word1, string word2) {									// Prva verzija funckije za pronalazenje prefiksa (neki vid estimacije duzine prefiksa).
	bool check = false;
	if (word1.length() < 5 || word2.length() < 5) {
		if (word1[0] == word2[0]) check = true;
	}
	else if (word1.length() < 10 || word2.length() < 10) {
		if (word1[0] == word2[0] && word1[1] == word2[1]) check = true;
	}
	else {
		if (word1[0] == word2[0] && word1[1] == word2[1] && word1[2] == word2[2]) check = true;
	}
	return check;
}

bool doTheySharePrefixGivenLength(string word1, string word2, int len) {				// Druga verzija funkcije za pronalazenje prefiksa (uz argument duzine prefiksa).
	if (word1.length() < len || word2.length() < len) return false;
	for (int i = 0; i < len; i++) {
		if (word1[i] != word2[i]) return false;
	}
	return true;
}

typedef struct Node {																	// Struktura cvora stabla, level se koristi samo zbog lepseg ispisa.
	string keyword;
	string transl;
	struct Node* left = nullptr;
	struct Node* right = nullptr;
	struct Node* parent = nullptr;
	int level = 0;
} Node;

class Tree {																			// Klasa stabla, jedini atribut je pokazivac na koren.
private:
	Node* root = nullptr;
public:
	bool isActive() {																	// Metoda za proveravanje da li je stablo inicijalizovano.
		if (this->root == nullptr) return false;
		else return true;
	}
	void create_tree(string *keys, string *translations, int n) {						// Metoda za formiranje stabla na osnovu prosledjenih nizova kljuceva i vrednosti, gde 
		for (int i = 0; i < n; i++) {													// je n broj parova reci.
			this->insert(keys[i], translations[i]);
		}
	}
	void search(string key) const {														// Metoda za pronalazenje kljuca u stablu. 
		queue<Node*> queue;															    // Red se koristi da se u njega stave svi prevodi na koje se naidje.
		Node* p = this->root;
		while (p != nullptr) {
			if (key <= p->keyword) {													// Svakako ako se naidje na kljuc skrece se levo jer se kljucevi skladiste kao prethodnici.
				if (key == p->keyword) queue.push(p);
				p = p->left;
			}
			else p = p->right;
		}
		if (queue.empty()) cout << "Nije pronadjen kljuc!" << endl;						// Praznjenje reda i ispis kljuceva.
		else {
			cout << "Prevod/prevodi za kljuc " << key << " su: ";
			while (!queue.empty()) {
				Node* q = queue.front();
				queue.pop();
				cout << q->transl;
				if (!queue.empty()) cout << ", ";
				else cout << "." << endl;
			}
		}
		return;
	}
	void remove(string key) {															// Metoda za brisanje zadatog kljuca ako postoji u stablu.
		bool check = true;																// Brisanje se ponavlja dokle god se ne izvrsi neuspesna pretraga na kljuc, tome sluzi promenljiva check.
		int hasDeleted = 0;
		while (check) {
			Node* p = this->root;														// Pretraga na kljuc.
			while (p != nullptr) {
				if (key == p->keyword) break;
				else if (key < p->keyword) p = p->left;
				else p = p->right;
			}
			if (p == nullptr) check = false;											// Ako kljuc ne postoji tu se zaustavlja.
			else {
				if (p->left == nullptr && p->right == nullptr) {						// Slucaj ukoliko brisani cvor nema decu.
					if (p == root) {													// Ako je koren cvor koji treba da se obrise, samo se resetuje pokazivac.
						this->root = nullptr;
						delete p;
						hasDeleted++;
					}
					else {
						Node* q = p->parent;											// Ako nije koren, samo se resetuje pokazivac roditelja na sina brisanog cvora.
						if (q->left == p) q->left = nullptr;
						else if (q->right == p) q->right = nullptr;
						if (p == this->root) this->root = nullptr;
						delete p;
						hasDeleted++;
					}
				}
				else if (p->left == nullptr || p->right == nullptr) {					// Slucaj ukoliko brisani cvor ima samo jedno dete.
					if (p == this->root) {												// Ako je koren cvor koji treba da se obrise, samo se premesta pokazivac na koren na sina.
						if (p->left != nullptr) this->root = p->left;
						else if (p->right != nullptr) this->root = p->right;
						delete p;
						hasDeleted++;
					}
					else {																// Ako nije koren, deli se na slucajeve kada brisani cvor ima levog i kad ima desnog sina.
						Node* q = p->parent;
						Node* child = nullptr;
						if (p->left != nullptr) {										// Ukoliko ima samo levog sina.
							if (q->left == p) {
								child = p->left;
								q->left = child;
								child->parent = q;
							}
							else if (q->right == p) {
								child = p->left;
								q->right = child;
								child->parent = q;
							}
						}
						else if (p->right != nullptr) {									// Ukoliko ima samo desnog sina.
							if (q->left == p) {
								child = p->right;
								q->left = child;
								child->parent = q;
							}
							else if (q->right == p) {
								child = p->right;
								q->right = child;
								child->parent = q;
							}
						}
						if (p == this->root) this->root = child;
						delete p;
						hasDeleted++;
					}
				}
				else {																	// Slucaj ukoliko brisani cvor ima oba sina.
					bool isRoot = (p == this->root) ? 1 : 0;
					Node* succ = p->right;												// Prvo trazimo sledbenika kojim ce se zameniti brisani cvor.
					while (succ->left != nullptr) succ = succ->left;					// Sledbenik se trazi u desnom podstablu (mora ga imati jer ima oba sina).
					if (p->right == succ) {												// Slucaj kada je sledbenik brisanog cvora njegov sin.
						if (!isRoot) {
							Node* parent = p->parent;
							if (parent->left == p) parent->left = succ;
							else if (parent->right == p) parent->right = succ;
							succ->parent = parent;
						}
						if (p->left != nullptr) {										// Levo podstablo brisanog cvora se prebacuje na levo podstablo sledbenika.
							succ->left = p->left;
							p->left->parent = succ;
						}
					}
					else {																// Slucaj kada mu sledbenik nije sin.
						Node* parent = p->parent;
						Node* succsparent = succ->parent;
						if (!isRoot) {
							if (parent->left == p) parent->left = succ;
							else if (parent->right == p) parent->right = succ;
							succ->parent = parent;
						}
						if (p->left != nullptr) {
							succ->left = p->left;
							p->left->parent = succ;
						}	
						if (succ->right != nullptr) {
							succsparent->left = succ->right;
							succ->right->parent = succsparent;
						}
						else {
							succsparent->left = nullptr;
						}
						succ->right = p->right;
						p->right->parent = succ;
					}
					if (this->root == p) root = succ;
					delete p;
					hasDeleted++;
				}
			}
		}
		if (hasDeleted) cout << "Obrisano je " << hasDeleted << " cvor/cvorova sa kljucem " << key << "." << endl;
		else cout << "Cvor sa kljucem " << key << " ne postoji!" << endl;
	}
	void insert(string key, string word) {												// Metoda za umetanje kljuca u stablo.
		Node* node = new Node;
		node->keyword = key;
		node->transl = word;
		if (this->root == nullptr) this->root = node;
		else {
			Node* p = this->root, * q = nullptr;										// Pretraga gde je potrebno umetnuti kljuc.
			while (p != nullptr) {
				q = p;
				if (p->keyword == key) {
					p = p->left;
					break;
				}
				else if (key < p->keyword) p = p->left;
				else p = p->right;
			}
			if (key < q->keyword) {
				q->left = node;
				node->parent = q;
			}
			else if (key > q->keyword) {
				q->right = node;
				node->parent = q;
			}
			else {
				while (p != nullptr) {													// Ako kljuc postoji, trazi se mesto njegovog prethodnika - ulazi se u levo podstablo i ide desno.
					q = p;
					p = p->right;
				}
				if (key <= q->keyword) q->left = node;
				else q->right = node;
				node->parent = q;
			}
		}
	}
	void print() const {
		cout << "------------   Ispis stabla   ------------" << endl << endl;
		/*queue<Node*> queue;															// Prva verzija ispisa.
		if (root == nullptr) return;
		queue.push(this->root);
		while (!queue.empty()) {
			Node* p = queue.front();
			queue.pop();
			if (p->left == nullptr && p->right == nullptr) cout << p->keyword << ": " << "X" << " - " << "X" << endl;
			else if (p->left == nullptr) cout << p->keyword << ": " << "X" << " - " << p->right->keyword << endl;
			else if (p->right == nullptr) cout << p->keyword << ": " << p->left->keyword << " - " << "X" << endl;
			else cout << p->keyword << ": " << p->left->keyword << " - " << p->right->keyword << endl;
			if (p->left != nullptr) queue.push(p->left);
			if (p->right != nullptr) queue.push(p->right);
		}*/
		queue<Node*> queue;
		if (root == nullptr) return;
		root->level = 0;
		queue.push(this->root);															// Inicijalizacija nivoa svih cvorova stabla.
		while (!queue.empty()) {
			Node* p = queue.front();
			queue.pop();
			if (p->left != nullptr) {
				p->left->level = p->level + 1;
				queue.push(p->left);
			}
			if (p->right != nullptr) {
				p->right->level = p->level + 1;
				queue.push(p->right);
			}
		}
		stack<Node*>stack;
		stack.push(this->root);															// Svi cvorovi se stavljaju na stek i vrsi se ispis.
		while (!stack.empty()) {
			Node* p = stack.top();
			stack.pop();
			string blanks = "";
			for (int i = 0; i < p->level; i++) blanks += "- - - - ";
			if (p != this->root && p->parent->left == p) blanks += "L. ";
			if (p != this->root && p->parent->right == p) blanks += "R. ";
			cout << p->level << " " << blanks << p->keyword << " : " << p->transl << endl;
			if (p->right != nullptr) stack.push(p->right);
			if (p->left != nullptr) stack.push(p->left);
		}
	}
	void delete_tree() {																// Metoda za unistavanje svih cvorova stabla.
		if (this->root == nullptr) return;
		stack<Node*> stack;
		stack.push(this->root);
		while (!stack.empty()) {
			Node* p = stack.top();
			stack.pop();
			if (p->left != nullptr) stack.push(p->left);
			if (p->right != nullptr) stack.push(p->right);
			delete p;
		}
		this->root = nullptr;
	}
	string shortest_common_word(string word) const {									// Metoda za pronalazenje najkrace reci sa istim prefiskom, prva verzija.
		int curlen = 0;
		string shortest = "";
		Node* p = this->root;
		while (p != nullptr) {
			if (doTheySharePrefix(p->keyword, word) && (curlen == 0 || p->keyword.length() <= curlen)) {
					curlen = p->keyword.length();
					shortest = p->keyword;
			}
			int lenL = 0, lenR = 0;
			if (p->left != nullptr)  lenL = (p->left->keyword).length();
			if (p->right != nullptr)  lenR = (p->right->keyword).length();
			if (p->left == nullptr && p->right == nullptr) p = nullptr;
			else if (lenL > 0 && lenL == 0) p = p->left;
			else if (lenL == 0 && lenL > 0) p = p->right;
			else if (p->left != nullptr && doTheySharePrefix(p->left->keyword, word)) {
				if (p->right != nullptr && doTheySharePrefix(p->right->keyword, word)) {
					if (p->left->keyword.length() < p->right->keyword.length()) p = p->left;
					else p = p->right;
				}
				else p = p->left;
			}
			else if (word < p->keyword) p = p->left;
			else p = p->right;
		}
		return shortest;
	}
	string shortestWordWithPrefixLenGiven(string word, int len) const {					// Metoda za pronalazenje najkrace reci sa istim prefiskom, druga verzija.
		int curlen = 0;
		string shortest = "";
		Node* p = this->root;
		while (p != nullptr) {
			if (doTheySharePrefixGivenLength(p->keyword, word, len) && (curlen == 0 || p->keyword.length() <= curlen)) {
				curlen = p->keyword.length();
				shortest = p->keyword;
			}
			if (p->left != nullptr && p->right != nullptr &&							// Ako zadata rec ima prefiks sa levim sinom trenutnog cvora, a ne sa desnim, skrece se levo.
				doTheySharePrefixGivenLength(p->left->keyword, word, len) && !doTheySharePrefixGivenLength(p->right->keyword, word, len)
				|| p->left != nullptr && p->right == nullptr) p = p->left;
			else if (p->left != nullptr && p->right != nullptr &&						// Ako zadata rec ima prefiks sa desnim sinom trenutnog cvora, a ne sa levim, skrece se desno.
				!doTheySharePrefixGivenLength(p->left->keyword, word, len) && doTheySharePrefixGivenLength(p->right->keyword, word, len)
				|| p->left == nullptr && p->right != nullptr) p = p->right;
			else if (word <= p->keyword) p = p->left;									// Ako zadata rec ima prefiks sa oba ili nema ni sa jednim skrece se klasicno.
			else p = p->right;
		}
		return shortest;
	}
};

int loadFromSTD(string& key, string& translation) { // Funkcija za unos jednog para reci sa standardnog ulaza.
	cout << "Unesite rec i prevod odvojene dvotackom: ";
	string words;
	cin >> words;
	if (words == "0") return 0;
	else {
		int p = 0;
		for (p = 0; p < words.length(); p++) {
			if (words[p] == ':') break;
		}
		if (p == words.length()) return 0;
		key = words.substr(0, p);
		translation = words.substr(p + 1, words.length());
		return 1;
	}
}

int loadFromFile(string fileName, string* keys, string* translations, int& noOfPairs) { // Funkcija za citanje liste parova reci iz datoteke.
	ifstream file(fileName);
	if (!file.is_open()) {
		cout << "Greska u radu sa datotekom!" << endl;
		return 0;
	}
	string words;
	int count = 0;
	while (getline(file, words)) {
		int p = 0;
		for (p = 0; p < words.length(); p++) {
			if (words[p] == ':') break;
		}
		keys[count] = words.substr(0, p);
		translations[count] = words.substr(p + 1, words.length());
		count++;
	}
	noOfPairs = count;
	file.close();
	return 1;
}

int main() {																			// Glavna funkcija koja poziva prethodne funkcije i metode klase stabla za rad nad stablom.

	Tree tree;
	string keys[100], translations[100], fileName = "", key = "", translation = "", word = "";
	int i = 0, n = 0, noOfPairs = 0, j = 0;
	bool cycle = true;

	while (cycle) {
		cout << endl << "- - - - - - - - - - Meni - - - - - - - - - -" << endl << "1. Formiranje stabla" << endl <<
			"2. Pretraga recnika" << endl << "3. Unos reci u recnik" << endl << "4. Brisanje reci iz recnika" << endl <<
			"5. Izgled stabla" << endl << "6. Pronalazenje najkrace reci sa zajednickim prefiksom" << endl << "7. Brisanje stabla" << endl <<
			"8. Kraj programa" << endl << endl << "Unesite redni broj zeljene opcije: ";
		cin >> n;
		if (n <= 0 || n > 8) {
			cout << "Pogresno uneta opcija. Molimo pokusajte ponovo!" << endl;
			continue;
		}

		switch (n) {
		case 1:
			if (tree.isActive()) {
				cout << "Stablo je vec prethodno kreirano! Izaberite drugu opciju." << endl;
				continue;
			}
			int m;
			cout << "1. Standardni ulaz" << endl << "2. Datoteka" << endl << "Unesite opciju izvora podataka: ";
			cin >> m;
			if (m <= 0 || m > 2) {
				cout << "Pogresno uneta opcija. Molimo pokusajte ponovo!" << endl;
				continue;
			}
			switch (m) {
			case 1:
				cout << "Unosite reci u obliku rec:prevod. Za prekid unosa, unesite 0." << endl;
				for (i = 0; i < 100; i++) {
					j = loadFromSTD(keys[i], translations[i]);
					if (!j) break;
				}
				noOfPairs = i;
				cout << "Unos uspesan." << endl;
				break;
			case 2:
				cout << "Unesite ime datoteke: ";
				cin >> fileName;
				j = loadFromFile(fileName, keys, translations, noOfPairs);
				if (j) cout << "Unos uspesan." << endl;
				break;
			}
			for (int k = 0; k < noOfPairs; k++) tree.insert(keys[k], translations[k]);
			break;

		case 2:
			if (!tree.isActive()) {
				cout << "Stablo nije incijalizovano!" << endl;
				continue;
			}
			else {
				cout << "Unesite kljuc za pretragu: ";
				cin >> key;
				if (key.length() < 1) {
					cout << "Neispravno uneta rec!" << endl;
					continue;
				}
				else tree.search(key);
			}
			break;

		case 3:
			if (!tree.isActive()) {
				cout << "Stablo nije incijalizovano!" << endl;
				continue;
			}
			j = loadFromSTD(key, translation);
			if (j == 0 || key.length() < 1 || translation.length() < 1) {
				cout << "Neispravan unos reci." << endl;
				continue;
			}
			tree.insert(key, translation);
			cout << "Kljuc uspesno umetnut u stablo!" << endl;
			break;

		case 4:
			if (!tree.isActive()) {
				cout << "Stablo nije incijalizovano!" << endl;
				continue;
			}
			cout << "Unesite kljuc koji zelite da obrisete iz stabla: ";
			cin >> key;
			if (key.length() < 1) {
				cout << "Neispravan unos kljuca." << endl;
				continue;
			}
			tree.remove(key);
			break;

		case 5:
			if (!tree.isActive()) {
				cout << "Stablo nije incijalizovano!" << endl;
				continue;
			}
			tree.print();
			break;

		case 6:
			if (!tree.isActive()) {
				cout << "Stablo nije incijalizovano!" << endl;
				continue;
			}
			cout << "Unesite rec za koju zelite da se pronadje rec sa istim prefiksom: ";
			cin >> key;
			if (key.length() < 1) {
				cout << "Neispravan unos kljuca." << endl;
				continue;
			}
			cout << "Unesite minimalan broj karaktera koji prefiks mora da sadrzi: ";
			cin >> j;
			if (j <= 0) {
				cout << "Neispravan unos broja karaktera." << endl;
				continue;
			}
			word = tree.shortestWordWithPrefixLenGiven(key, j);
			if (word.length() == 0) cout << "U recniku ne postoji rec sa istim prefiksom." << endl;
			else cout << "Najkraca rec sa istim prefiksom kao zadata rec je " << word << "." << endl;
			break;

		case 7:
			if (tree.isActive()) {
				tree.delete_tree();
				cout << "Stablo uspesno obrisano." << endl;
			}
			else cout << "Stablo je vec obrisano." << endl;
			break;

		case 8:
			cout << "Zdravo!" << endl;
			cycle = false;
			break;
		}
	}
	return 0;
}