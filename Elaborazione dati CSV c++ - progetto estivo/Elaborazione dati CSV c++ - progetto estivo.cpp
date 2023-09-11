#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <conio.h>

using namespace std;

void Spaziatura(const string& file, const string& appoggio, int RecordLength) {
    // Dichiarazioni variabili
    string line, line2, sep = ";";
    int n = 0;

    // Apertura file
    ifstream reader(appoggio);
    ofstream writer(file);

    // Ciclo per leggere tutte le righe fino alla fine del file e aggiungere la spaziatura necessaria per rendere tutto uguale + campo univoco
    while (getline(reader, line)) {
        if (n == 0) {
            line2 = line + sep + "CampoUnivoco" + sep;
            line2.resize(RecordLength - 4, ' ');
            line2 += "##\n";
            writer << line2;
        }
        else {
            line2 = line + sep + to_string(n) + sep;
            line2.resize(RecordLength - 4, ' ');
            line2 += "##\n";
            writer << line2;
        }
        n++;
    }

    reader.close();
    writer.close();
}

void AggiuntaRecords(const string& file, int RecordLength) {
    // Dichiarazioni variabili
    string sep = ";";
    int x = 0;
    vector<char> br(RecordLength); //initialize br with RecordLength elements
    vector<char> brAppoggio;
    string line = "";

    // Inizializza il generatore di numeri casuali con il tempo corrente come seme
    srand(static_cast<unsigned>(time(nullptr)));

    // Apertura del file in modalità binaria
    fstream reader(file, ios::in | ios::out | ios::binary);

    // Ciclo while per verificare quando si arriva alla fine del file
    while (!reader.eof()) { //use readsome to check for buffer size
        // Posizionamento all'inizio della riga indicata dal contatore
        reader.seekg(RecordLength * x, ios_base::beg);

        // Lettura di tutta la riga in formato binario
        reader.read(br.data(), RecordLength);

        // Conversione in stringa
        line = string(br.data(), br.size());

        // Trova la posizione dell'ultimo ";"
        int index = line.find_last_of(";");
        if (index == string::npos)
            break;
        else {
            // Lettura di tutta la riga fino al ";" e conversione in stringa
            brAppoggio = vector<char>(line.begin(), line.begin() + index);
            line = string(brAppoggio.begin(), brAppoggio.end());

            // Riposizionamento all'inizio della riga attuale
            reader.seekp(RecordLength * x);

            // Controllo per capire se siamo sulla prima riga o no
            // Sovrascrittura della riga con la stringa originale + Tipo record / numero casuale e campo per cancellazione logica
            if (x == 0) {
                string nuovaLinea = line + ";miovalore;cancellazione logica;";
                nuovaLinea.resize(RecordLength - 4, ' ');
                reader.write(nuovaLinea.c_str(), nuovaLinea.length());
            }
            else {
                int numeroCasuale = rand() % 12 + 10;
                string nuovaLinea = line + sep + to_string(numeroCasuale) + ";true;";
                nuovaLinea.resize(RecordLength - 4, ' ');
                reader.write(nuovaLinea.c_str(), nuovaLinea.length());
            }
            x++;
        }
    }
    reader.close();
}

int NumeroCampi(const string& file, int RecordLength) {
    // Dichiarazione delle variabili
    string line = "";
    int campi = 1;
    vector<char> br(RecordLength);
    vector<char> brAppoggio;

    // Apertura del file in modalità binaria
    ifstream reader(file, ios::binary);

    // Posizionamento sulla prima riga del file
    reader.read(br.data(), RecordLength);
    line = string(br.data(), RecordLength);

    // Posizione dell'ultimo carattere
    size_t index = line.find_last_of(";");

    // Lettura di tutta la riga fino al ";" e conversione in stringa
    brAppoggio = vector<char>(line.begin(), line.begin() + index);
    line = string(brAppoggio.begin(), brAppoggio.end());

    // Conteggio degli split per capire il numero di campi presenti
    size_t pos = 0;
    while ((pos = line.find(";", pos)) != string::npos) {
        campi++;
        pos += 1; // Avanza al prossimo carattere dopo il ";"
    }

    return campi;
}

int LunghezzaMassima(const string& file, int RecordLength) {
    // Dichiarazione delle variabili
    string line = "";
    int max = 0, index = 0;
    ifstream reader(file);

    
    while (getline(reader, line)) {
        index = line.find_last_of(';');
        if (index != std::string::npos) {
            if (max < index) {
                max = index;
            }
        }
    }
    
    reader.close();
    return max;
}
    
int NumRighe(const string& file) {
    // Dichiarazione del contatore
    int n = 0;
    string line;

    // Apertura del file in modalità lettura
    ifstream reader(file);
    
    // Ciclo per leggere le righe fino alla fine del file con incremento del contatore per tenerne il conto
    while (getline(reader, line)) {
        n++;
    }

    reader.close();
    return n;
}

void RecordCoda(const string& file, int RecordLength, const string& line) {
    // Dichiarazione delle variabili
    vector<char> br(RecordLength);
    string nuovoRecord;
    srand(static_cast<unsigned>(time(nullptr))); // Inizializzazione del generatore di numeri casuali

    // Calcola il numero di righe nel file
    int n = NumRighe(file);

    // Apertura del file in modalità binaria in append
    ofstream writer(file, ios::binary | ios::app);

    // Creazione del nuovo record
    nuovoRecord = line + to_string(n) + ";" + to_string(rand() % 12 + 10) + ";true;";
    nuovoRecord.resize(RecordLength - 4, ' ');
    nuovoRecord += "##";

    // Scrittura del nuovo record nel file
    writer.write(nuovoRecord.c_str(), nuovoRecord.length());

    writer.close();
}

vector<vector<string>> TreCampi(const string& file, int RecordLength, int split1, int split2, int split3, int& righe) {
    // Dichiarazione delle variabili
     vector<vector<string>> matrice(righe, vector<string>(5));
    string line, split;
    int contatore = 0;
    char sep = ';';
    ifstream reader(file);

    while(getline(reader, line)){
        split = line.substr(split1, line.find(";"));
        matrice[contatore][0] = split;
        split = line.substr(split2, line.find(";"));
        matrice[contatore][1] = split;
        split = line.substr(split3, line.find(";"));
        matrice[contatore][2] = split;
        contatore++;
    }
    reader.close();
    return matrice;
}

string Ricerca(const string& file, int RecordLength, int riga) {
    
    string line;
    vector<char> br(RecordLength);

    ifstream reader(file, ios::binary);

    reader.seekg(RecordLength * riga, ios::beg);

    reader.read(br.data(), RecordLength);
    reader.close();

    line.assign(br.data(), br.size());

    int index = line.find_last_of(";");

    if (index != string::npos) {
        line = line.substr(0, index);
    }

    reader.close();
    return line;
}

vector<int> PosizioneCampo(const string& str, int campo) {
    vector<int> contenitore(15);
    int IndexAppoggio = str.find(";");

    for (int i = 0; i <= campo; i++) {
        if (IndexAppoggio == string::npos) {
            break;
        }
        contenitore[i] = IndexAppoggio;
        IndexAppoggio = str.find(";", IndexAppoggio + 1);
    }

    return contenitore;
}

void Modifica(const string& file, int RecordLength, int riga, int campo, const string& input) {
    string line, line1, line2, sep = ";";
    int index;
    vector<int> contenitore(15);

    // Apertura del file in modalità lettura/scrittura
    fstream fileStream(file, ios::in | ios::out | ios::binary);

    // Posizionamento sulla riga dove si trova il campo univoco
    fileStream.seekg(RecordLength * riga, ios::beg);

    // Lettura di tutta la riga
    getline(fileStream, line);

    // Posizione dell'ultimo carattere ;
    index = line.find_last_of(";");
    contenitore = PosizioneCampo(line, campo);
    if (campo > 0) {
        // Lettura della prima parte fino al campo desiderato
        line1 = line.substr(0, contenitore[campo-1]);

        // Lettura della parte successiva rispetto al campo desiderato
        line2 = line.substr(contenitore[campo], index);

        // Posizionamento all'inizio della riga da sovrascrivere
        fileStream.seekp(RecordLength * riga, ios::beg);

        // Sovrascrittura della riga con le parti estratte e l'input
        fileStream << line1 << sep << input << line2;
    }
    else {
        // Lettura della parte successiva rispetto al campo desiderato
        line2 = line.substr(contenitore[campo], index);

        // Posizionamento all'inizio della riga da sovrascrivere
        fileStream.seekp(RecordLength * riga, ios::beg);

        // Sovrascrittura della riga con l'input e la parte estratta
        fileStream << input << sep << line2;
    }

    // Chiudi il file
    fileStream.close();
}

void Cancellazione(const string& file, int RecordLength, int riga) {
    string line, line1, line2, sep = ";";
    vector<int> contenitore(15);
    vector<char> br(RecordLength);

    // Apertura del file
    fstream fileStream(file, ios::in | ios::out | ios::binary);

    // Posizionamento sulla riga dove si trova il campo univoco
    fileStream.seekg(RecordLength * riga, ios::beg);

    // Lettura di tutta la riga
    getline(fileStream, line);

    // Posizione dell'ultimo carattere ;
    int index = line.find_last_of(";");

    // Richiamo alla funzione per la posizione dei caratteri ;
    contenitore = PosizioneCampo(line, 8);

    // Lettura della prima parte di bytes fino al campo desiderato e conversione in stringa
    line1 = line.substr(0, contenitore[8 - 1]);

    // Lettura della parte successiva di bytes rispetto al campo desiderato e conversione in stringa
    line2 = line.substr(contenitore[8], index - contenitore[8]);

    // Torniamo all'inizio della riga
    fileStream.seekp(RecordLength * riga, ios::beg);

    // Sovrascrittura della riga precedente con "false"
    line1 += sep + "false" + line2;
    line1.resize(RecordLength - 4, ' ');
    fileStream.write(line1.c_str(), line1.size());

    // Chiudiamo il file
    fileStream.close();
}

int main() {
    string file = "corsi.csv", appoggio = "originale.csv", sep = ";", line = "", input;
    int RecordLength = 250, scelta = 0, controllo = 0, n, val, val1, val2, val3, campi = 0, righe = 0, campoUnivoco = 0;
    bool booleano = false;

    Spaziatura(file, appoggio, RecordLength);

    do {
        system("CLS");
        cout << "1 - Aggiunta di un campo chiamato \"miovalore\" ed un campo per marcare la cancellazione logica" << endl;
        cout << "2 - Conteggio del numero dei campi che compongono il record" << endl;
        cout << "3 - Calcolo della lunghezza massima dei record presenti" << endl;
        cout << "5 - Aggiunta di un record in coda" << endl;
        cout << "6 - Visualizzazione dei dati mostrando tre campi significativi a scelta" << endl;
        cout << "7 - Ricerca di un record per campo chiave a scelta" << endl;
        cout << "8 - Modifica di un record" << endl;
        cout << "9 - Cancellazione logica di un record" << endl;
        cout << "0 - Uscita dal programma" << endl;

        // Scelta delle opzioni
        cout << "Inserisci la scelta: ";
        cin >> scelta;

        switch (scelta) {
        case 1:
            if (controllo == 0){
                AggiuntaRecords(file, RecordLength);
                cout << "\nI campi \"miovalore\" e \"cancellazione logica\" sono stati aggiunti con successo!" << endl;
                controllo++;
            }
            else
                cout << "\nQuesta funzione e' gia' stata utilizzata" << endl;
            
            cout << "Premere un pulsante per continuare...";
            _getch();
            break;
        case 2:
            n = NumeroCampi(file, RecordLength);
            cout << "\nIl numero di campi e': " + to_string(n) << endl;
            
            cout << "Premere un pulsante per continuare...";
            _getch();
            break;
        case 3:
            n = LunghezzaMassima(file, RecordLength);
            cout << "\nLa lunghezza massima dei record presenti e': " + to_string(n) << endl;

            cout << "Premere un pulsante per continuare...";
            _getch();
            break;
        case 5:
            if (controllo == 1) {
                // Input dei dati dall'utente
                string comune;
                cout << "\nInserire il Comune: ";
                cin >> comune;
                line = comune;

                string provincia;
                cout << "Inserire La Provincia: ";
                cin >> provincia;
                line += sep + provincia;

                string denominazioneSito;
                cout << "Inserire la Denominazione Sito: ";
                cin >> denominazioneSito;
                line += sep + denominazioneSito;

                string indirizzo;
                cout << "Inserire l'indirizzo: ";
                cin >> indirizzo;
                line += sep + indirizzo;

                string numeroCivico;
                cout << "Inserire il Numero Civico: ";
                cin >> numeroCivico;
                line += sep + numeroCivico;

                string idAnagrafe;
                cout << "Inserire l'ID anagrafe: ";
                cin >> idAnagrafe;
                line += sep + idAnagrafe + sep;

                // Aggiunta del record in coda al file
                RecordCoda(file, RecordLength, line);
                cout << "\nRecord in coda aggiunto correttamente!" << endl;
            }
            else
                cout << "\nPrima di utilizzare questa funzione è necessario utilizzare l'Aggiunta in coda (1)";

            cout << "Premere un pulsante per continuare...";
            _getch();
            break;
        case 6:
            campi = NumeroCampi(file, RecordLength);
            righe = NumRighe(file);

            cout << "\nInserire il Primo campo: ";
            cin >> val1;
            cout << "\nInserire il Secondo campo: ";
            cin >> val2;
            cout << "\nInserire il Terzo campo: ";
            cin >> val3;

            if (val1 > campi || val2 > campi || val3 > campi) {
                cout << "Il numero massimo di campi è: " << campi << "\nInserire un numero minore o uguale a: " << campi << endl;
            }
            else {
                vector<vector<string>> matrice = TreCampi(file, RecordLength, val1, val2, val3, righe);

                for (int i = 0; i < righe; i++) {
                    cout << "Primo campo: " << matrice[i][0] << "    Secondo campo: " << matrice[i][1] << "    Terzo campo: " << matrice[i][2] << endl;
                }
            }

            cout << "Premere un pulsante per continuare...";
            _getch();
            break;
        case 7:
            campi = NumeroCampi(file, RecordLength);
            righe = NumRighe(file);

            do {
                cout << "\nInserire la riga che si vuole cercare: ";
                booleano = static_cast<bool>(cin >> campoUnivoco);

                if (!booleano) {
                    cout << "Nella riga bisogna inserire SOLO input di tipo Int" << endl;
                    booleano = false;
                }
                else if (campoUnivoco < 0 || campoUnivoco > righe) {
                    cout << "Inserire un numero compreso tra 0 e " << righe << endl;
                    booleano = false;
                }
                else {
                    line = Ricerca(file, RecordLength, campoUnivoco);
                    cout << line << endl;
                }
            } while (!booleano);

            cout << "Premere un pulsante per continuare...";
            _getch();
            break;
        case 8:
            campi = NumeroCampi(file, RecordLength);
            do {
                cout << "\nInserire la riga che si vuole selezionare: ";
                booleano = static_cast<bool>(std::cin >> campoUnivoco);

                if (!booleano) {
                    cout << "Nella riga bisogna inserire SOLO input di tipo Int" << endl;
                    booleano = false;
                }
                else {
                    cout << "\nInserire il campo che si vuole modificare: ";
                    booleano = static_cast<bool>(cin >> val);

                    if (!booleano) {
                        cout << "Nella riga bisogna inserire SOLO input di tipo Int" << endl;
                        booleano = false;
                    }
                    else {
                        cout << "\nInserire il nuovo input: ";
                        cin >> input;

                        if (val == 6) {
                            cout << "Il campo numero 6 è il campo univoco, pertanto non è possibile modificarlo" << endl;
                            booleano = false;
                        }
                        else {
                            line = Ricerca(file, RecordLength, campoUnivoco);
                            cout << line << endl;
                            Modifica(file, RecordLength, campoUnivoco, val, input);
                            line = Ricerca(file, RecordLength, campoUnivoco);
                            cout << line << endl;

                            cout << "Modifica effettuata con successo." << endl;
                        }
                    }
                }
            } while (!booleano);

            cout << "Premere un pulsante per continuare...";
            _getch();
            break;
        case 9:
            righe = NumRighe(file);

            if (controllo == 1) {
                do {
                    cout << "\nInserire la riga che si vuole cercare: ";
                    booleano = static_cast<bool>(cin >> campoUnivoco);

                    if (!booleano) {
                        cout << "Nella riga bisogna inserire SOLO input di tipo Int" << endl;
                        booleano = false;
                    }
                    else if (campoUnivoco > righe) {
                        cout << "Inserire un numero compreso tra 0 e " << righe << endl;
                        booleano = false;
                    }
                    else {
                        line = Ricerca(file, RecordLength, campoUnivoco);
                        cout << "Riga vecchia: " << line << endl;
                        Cancellazione(file, RecordLength, campoUnivoco);
                        line = Ricerca(file, RecordLength, campoUnivoco);
                        cout << "Riga nuova: " << line << endl;
                    }
                } while (!booleano);
            }
            else {
                cout << "Prima di utilizzare questa funzione è necessario utilizzare l'Aggiunta in coda (1)" << endl;
            }
            cout << "Premere un pulsante per continuare...";
            _getch();
            break;

        default:
            std::cout << "Scelta non valida. Riprova." << std::endl;
            break;
        }
    } while (scelta != 0);
}