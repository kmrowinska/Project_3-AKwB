#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <sstream>
#include <iterator>
#include <cstdlib>
#include <algorithm>
using namespace std;

vector<vector<char>> seq;
vector<vector<int>> qual;
vector<vector<int>> indeksy;

void read_sequence() {
    fstream seqFile;
    string s_line;
    seqFile.open("C:\\Users\\kinga\\Desktop\\seq_test.txt", ios::in);

    if (seqFile.is_open()) {
        cout << "otwarto pomyslnie plik seq!" << endl;
        vector<char> temp;
        while (getline(seqFile, s_line)) {
            if (s_line[0] != '>') {
                temp.clear();
                for (char c: s_line) {
                    temp.push_back(c);
                }
                seq.push_back(temp);
            }
        }
    } else {
        cout << "wystapil blad przy zaladowaniu pliku seq" << endl;
    }
    seqFile.close();
}

void read_qual() {
    fstream credFile;
    string line;
    credFile.open("C:\\Users\\kinga\\Desktop\\cap_test.txt", ios::in);

    if (credFile.is_open()) {
        cout << "otwarto pomyslnie plik cred!" << endl;
        while (getline(credFile, line)) {
            if (line[0] != '>') {
                vector<int> row;
                stringstream ss(line);
                int num;
                while (ss >> num) {
                    row.push_back(num);
                }
                qual.push_back(row);
            }
        }
    } else {
        cout << "wystapil blad przy zaladowaniu pliku cred" << endl;
    }
    credFile.close();
}

void show_seq(){
    for (int i = 0; i < seq.size(); ++i) {
        cout << "\nSekwencja " << i << ": ";
        for (int j = 0; j < seq[i].size(); ++j) {
            cout << seq[i][j] << " ";
        }
        cout << endl;
    }
}

void show_qual(){
    for (int i = 0; i < qual.size(); ++i) {
        cout << "\nSekwencja " << i << ": ";
        for (int j = 0; j < qual[i].size(); ++j) {
            cout << qual[i][j] << " ";
        }
        cout << endl;
    }
}

void delete_after_treshold(int tsh) {
    cout << "\nJAKOSC PRZED: ";
    show_qual();
    cout << "\nSEK PRZED OBCIECIEM: ";
    show_seq();


    for (int i = 0; i < seq.size(); i++) {
        for (int j = 0; j < seq[i].size(); j++) {
            if (qual[i][j] < tsh) {
                seq[i][j] = 'Z';
                qual[i][j] = -1;
            }
        }
    }


    for (int i = 0; i < seq.size(); i++) {
        indeksy.emplace_back();
        for (int j = 0; j < seq[i].size(); j++) {
            if (seq[i][j] != 'Z') {
                indeksy[i].push_back(j + 1);
            }
        }
    }
    cout << "\nIndeksy po usunieciu";
    for (int i = 0; i < indeksy.size(); i++) {
        cout << "\nSekwencja " << i << ": ";
        for (int j = 0; j < indeksy[i].size(); j++) {
            cout << indeksy[i][j] << " ";
        }
        cout << endl;
    }

    /*cout << "\nJAKOSC PO ZMIANIE: ";
    show_qual();
    cout << "\nSEK PO ZMIANIE: ";
    show_seq();*/


    for (int i = 0; i < seq.size(); i++) {
        seq[i].erase(remove(seq[i].begin(), seq[i].end(), 'Z'), seq[i].end());
    }


    for (int i = 0; i < qual.size(); i++) {
        qual[i].erase(remove(qual[i].begin(), qual[i].end(), -1), qual[i].end());
    }

    cout << "\nJAKOSC PO OBCIECIU: ";
    show_qual();
    cout << "\nSEK PO OBCIECIU: ";
    show_seq();
}

class Wierzcholek{
    public:
    int position;
    int sequence;
    string substring; //podciag
};

bool check_sequence(vector<Wierzcholek> seq){
    vector<int> num_seq;
    for(auto item: seq){
        num_seq.push_back(item.sequence);
    }

    if(adjacent_find(num_seq.begin(), num_seq.end()) == num_seq.end()){
        return true;
    }
    return false;
}

int check_connections(vector<Wierzcholek> wierzcholki, int id, vector<pair<Wierzcholek, vector<Wierzcholek>>> graf){
    int punkty = 0;
    vector<Wierzcholek> kopia_w = wierzcholki;

    for(auto nastepnik: wierzcholki){
        if(nastepnik.sequence == 4){
            punkty += 1;
            return punkty;
        }
        else{
            kopia_w = {kopia_w.begin() + 1, kopia_w.end()};
            for(int i=id; i < graf.size(); i++){
                if(nastepnik.sequence == graf[i].first.sequence && nastepnik.position == graf[i].first.position){
                    for(int j=0; j < graf[i].second.size(); j++){
                        if(kopia_w[j].sequence == graf[i].second[j].sequence && kopia_w[j].position == graf[i].second[j].position){
                            punkty += 1;
                        }else{
                            punkty -= 1;
                        }
                    }
                }
            }
        }
    }
    return punkty;
}

void motive(vector<pair<Wierzcholek, vector<Wierzcholek>>> graf){
    pair<Wierzcholek, vector<Wierzcholek>> motyw;
    int punkty = 0;
    for(int i=0; i < graf.size(); i++){
            if(graf[i].second.size() == 4 && graf[i].first.sequence == 0){
                if(check_sequence(graf[i].second)){
                    punkty = check_connections(graf[i].second, i + 1, graf);
                    if(punkty == 7){
                        motyw = graf[i];
                        break;
                    }else{
                        punkty = 0;
                    }
                }
            }else if(graf[i].first.sequence != 0){
                cout<<"\nNie znaleziono motywu.";
                exit(0);
            }
        }

    cout << "\nMOTYW: " << motyw.first.substring << "\n";
    cout << "Znaleziony w sekwencji " << motyw.first.sequence << " na pozycji " << motyw.first.position << "\n";
    for(auto item:motyw.second){
        cout << "Znaleziony w sekwencji " << item.sequence << " na pozycji "  << item.position << "\n";

    }
}

void vertices(int lgth){
    string subseq;
    Wierzcholek obiekt;
    vector<Wierzcholek> obiekty;

    for(int i=0; i<seq.size(); i++){
        for(int j=0; j<seq[i].size() - lgth + 1; j++){
            subseq = "";
            for(int n = j; n<j+lgth; n++){
                subseq += seq[i][n];
            }
            obiekt.substring = subseq;
            obiekt.position = indeksy[i][j];
            obiekt.sequence = i;
            obiekty.push_back(obiekt);
        }
    }

    for(int i=0; i<obiekty.size(); i++){
        cout<<"\nPodciag: "<<obiekty[i].substring<<" Numer sekwencji: "<<obiekty[i].sequence<<" Pozycja: "<<obiekty[i].position<<"\n";
    }


    cout<<"Graf: \n";
    vector<pair<Wierzcholek, vector<Wierzcholek>>> graf;
    for(int i=0; i<obiekty.size()-1; i++){
        vector<Wierzcholek> polaczenia;
        for(int j = i + 1; j<obiekty.size(); j++){
            if(obiekty[i].substring == obiekty[j].substring){
                if(obiekty[i].sequence != obiekty[j].sequence){
                    if(obiekty[i].position > obiekty[j].position){
                        if(obiekty[i].position - obiekty[j].position < 10*obiekty[i].substring.size()){
                            polaczenia.push_back(obiekty[j]);
                        }
                    }else{
                        if(obiekty[j].position - obiekty[i].position < 10*obiekty[i].substring.size()){
                            polaczenia.push_back(obiekty[j]);
                        }
                    }
                }
            }
        }
        graf.push_back(make_pair(obiekty[i], polaczenia));
    }

    for(auto item: graf){
        cout << "\n[Wierzcholek:" << item.first.substring << ", Sekwencja:" << item.first.sequence << ", Pozycja:" << item.first.position << "]" << " ---> ";
        for(auto subitem: item.second){
            cout << "(Sekwencja:" << subitem.sequence << ", Pozycja:" << subitem.position << "),";
        }
        cout<<"\n";
    }

    motive(graf);
}

int main() {
    read_sequence();
    read_qual();
    int length;
    int treshold;

    cout << "Podaj dlugosc podciagu, liczbe miedzy 4 a 9: ";
    cin >> length;
    while (length < 4 || length > 9) {
        cout << "Nieprawidlowa dlugosc podciagu.\n"
                "Podaj poprawna wartosc: ";
        cin >> length;
    }

    cout << "Podaj prog wiarygodnosci: ";
    cin >> treshold;
    while (treshold < 0) {
        cout << "Nieprawidlowy prog.\n"
                "Podaj poprawna wartosc: ";
        cin >> treshold;
    }
    delete_after_treshold(treshold);
    vertices(length);

    return 0;
}
