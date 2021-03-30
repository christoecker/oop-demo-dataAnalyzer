#include "dataSet.h"

// Messwerte aus csv-Datei importieren.
int dataSet::importFromCSV(const std::string& fileName, const char sep)
{
    /*-- Datei �ffnen und Inhalt (als stMPoints) in inpBfr einlesen --*/
    std::ifstream f(fileName);
    if (f.fail())           // Fehler beim �ffnen
        return 101;         // Fehlercode: Hier (noch) nicht ber�cksichtigt

    char buf[1024];
    std::vector<stMPoint> inpBfr;
    while (f.getline(buf, sizeof(buf))) {       // Einlesen zweilenweise
        char* token = std::strtok(buf, &sep);
        time_t dTime = std::stoi(token);        // Erfassen des Zeitstempels
        token = std::strtok(NULL, &sep);
        double dValue = std::stod(token);       // Erfassen des Messwerts
        inpBfr.push_back({ dTime, dValue });    // ...zum Vektor hinzuf�gen
    }
    f.close();             // Datei schliessen


    /*-- inpBfr und series vergleichen--*/
    inpBfr.shrink_to_fit();                 // Gr��e des Vektors anpassen
    // Elemente im vektor nach der Zeit aufsteigend sortieren
    std::sort(inpBfr.begin(), inpBfr.end(), compMPoint_byTime);
    /*
        ANMERKUNG: Es wird davon ausgegangen, dass in der Datei, aus der die 
        Messwerte eingelesen werden, dass jeder Messwert einen eindeutigen Zeitstempel 
        hat. --> Ggf. m�sste hier noch eine Fehlererkennung eingebaut werden.
    */

    // 1. Fall: Messreihe ist nicht leer. In diesem Fall muss erst gepr�ft werden, ob es
    // eine �berschneidung von Messwerten (bzgl. des Zeitstempels) aus der bestehen 
    // Messreihe und den neu eingelesenen Messwerten gibt.
    if (!series.empty()) {
        std::vector<stMPoint> intersect_series; // speichert m�gliche �berschneidungen
        
        // Ermittlung der �berschneidung
        std::set_intersection(inpBfr.begin(), inpBfr.end(), series.begin(), series.end(), std::back_inserter(intersect_series), compMPoint_byTime);
        
        /*-- Keine �berschneidung -> inpBfr kann an series einfach angef�gt werden. --*/
        if (intersect_series.empty()) {
            series.insert(series.begin(), inpBfr.begin(), inpBfr.end());       // Anf�gen ...
            std::sort(series.begin(), series.end(), compMPoint_byTime);        // und Sortieren
            series.shrink_to_fit();                                            // �berfl�ssigen Speicher freigeben...
        }
        /*-- Es gibt �berschneidungen: Abfrage, welche Werte behalten werden sollen. --*/
        else {
            // **** Benutzerschnittstelle in Konsole ****
            system("cls");
            std::cout << "Der Zeitstempel von ";
            switch (intersect_series.size()) {
            case 1: std::cout <<  "einem Messpunkt ";
                break;
            default:std::cout << intersect_series.size() << " Messpunkten ";
            }
            std::cout << "existiert schon in der Messreihe\n";
            
            char userInp = '0';
            while (1) {
                std::cout << "Sollen die alten Werte ueberschrieben werden? (j/n)\n";
                std::cin >> userInp;
                if (userInp == 'j' || userInp == 'J' || userInp == 'n' || userInp == 'N') break;
            }
            // **** ENDE: Benutzerschnittstelle in Konsole ****

            // Die alten Werte sollen �berschrieben werden.
            if (userInp == 'j' || userInp == 'J') {
                for (auto i = intersect_series.begin(); i != intersect_series.end(); i++) {
                    // Finde die Messwerte, bei denen die Zeitstempel in series und in intersect_series identisch sind
                    // Die Auswertung des Vergleichs findet hier �ber eine Lambda-Funktion statt
                    // Find_if liefert einen Pointer auf den gefundenen Messwert in series zur�ck
                    auto dblItem = std::find_if(series.begin(), series.end(), [i](stMPoint serItem) {return i->tStamp == serItem.tStamp; });
                    dblItem->value = i->value;                          // �berschreibe den alten Messwert
                    auto search = comments.find(dblItem->tStamp);       // Finde den alten Kommentar
                    if (search != comments.end()) {                     // Falls es einen alten Kommentar gibt...
                        search->second = "[[" + search->second + "]]";  // ...werden diesem doppelte [] hinzugef�gt
                    }
                }
            }

            // Ermittle nun die Differenz aus der bestehenden Messreihe und inpBfr. In diff stehen
            // dann alle aus der Datei eingelesenen Messwerte, deren Zeitstempel noch nicht in 
            // series vorkommen, die also wirklich neu sind!
            std::vector<stMPoint> diff;
            std::set_difference(inpBfr.begin(), inpBfr.end(), series.begin(), series.end(), std::inserter(diff, diff.begin()), compMPoint_byTime);

            series.insert(series.begin(), diff.begin(), diff.end());        // F�ge die neuen Messwerte an series an
            std::sort(series.begin(), series.end(), compMPoint_byTime);     // Sortiere den Vektor
            series.shrink_to_fit();                                         // �berfl�ssigen Speicher freigeben.
        }
    }
    // 2. Fall: Messreihe ist bislang leer. Die neu eingelesenen Messwerte k�nnen einfach an
    // den Vektor angef�gt werden.
    else {
        series.insert(series.begin(), inpBfr.begin(), inpBfr.end());        // F�ge die neuen Messwerte an series an
        std::sort(series.begin(), series.end(), compMPoint_byTime);         // Sortiere den Vektor
        series.shrink_to_fit();                                             // �berfl�ssigen Speicher freigeben.
    }

    // WICHTIG! Die analyzer haben �ber einen Pointer auf den Vektor der Messreihe Zugriff auf 
    // die Werte. Wenn an dem Vektor �nderungen vorgenommen werden, kann es sein, dass dieser
    // in einen anderen Speicherbereich kopiert wird. Diese �nderung muss den analyzern 
    // "mitgeteilt" werden, d.h. der Pointer auf den Vektor muss aktualisiert werden.
    for (auto& i : analyzer) {
        i->setOwnerSeries(&series);     // �bergebe neuen Pointer auf den Vektor series.
    }
    
    return 0;
}

// Hinzuf�gen eines Kommentars zu einem Messwert
int dataSet::addComment(const time_t dTime, const std::string c)
{
    // L�nge des Kommentars wird auf 1000 Zeichen begrenzt. Technisch w�ren sehr viel l�ngere 
    // Kommentare kein Problem.
    if (c.length() > 1001) return -1;
    
    // Durchlaufe die Messwerte in series und suche nach dem Zeitstempel dTime
    for (auto i = series.begin(); i != series.end(); i++) {
        if (i->tStamp == dTime) {   // Wenn der Zeitstempel existiert...
            // erg�nze in der Liste comments einen Kommentar.
            comments.insert(std::pair<time_t, std::string>(i->tStamp, c));
            return 1;   // -> Kommentar erfolgreich eingef�gt.
        }
    }

    // Der angegebene Zeitstempel existiert nicht. -> R�ckgabewert 0.
    return 0;
}

// Kommentar entfernen
bool dataSet::removeComment(const time_t t)
{
    // Druchlaufe die Liste comments, ...
    for (auto i = comments.begin(); i != comments.end(); i++) {
        // ... wenn darin ein Eintrag existiert, bei dem der erste Wert identisch ist mit 
        // dem �bergebenen Zeitstempel, dann ...
        if (i->first == t) {
            comments.erase(t);      // ... l�sche diesen.
            return true;            // -> L�schen erfolgreich
        }
    }
    return false;   // In den Liste existiert kein Eintrag mit dem angegebenen Zeitstempel
}

// Hinzuf�gen eines Analyzers
bool dataSet::addAnalyzer(analyzer_IF* a)
{
    std::string newType = a->getType();
    // Pr�fe ob der Typ des �bergebenen analyzers bereits zu der Messreihe hinzugef�gt wurde.
    for (auto& i : analyzer) {
        // Wenn dieser Typ schon existier, gib FALSE zur�ck.
        if (i->getType() == newType) return false;
    }

    // ... andernfalls �bergebe dem analyzer den Pointer auf das Objekt series und...
    a->setOwnerSeries(&(this->series));
    analyzer.push_back(a);                  // ... f�ge den analyzer hinzu.
    return true;                            // Hinzuf�gen war erfolgreich.
}

// Einstellungen der analyzer vornehmen
bool dataSet::analyzerSettings(std::string a)
{
    for (const auto& i : analyzer) {
        if (i->getType() == a) {    // Suche nach dem analyzer, dessen EInstellungen ge�ndert werden sollen.
            // Hier wird Polymorphismus ausgenutzt!
            return i->settings();   // TRUE, wenn die Einstellungen erfolgreich ge�ndert wurden.
        }
    }
    return false;   // Einstellungen konnten nicht gemacht werden.
}

// Testausgabe: Analyzer Liste
void dataSet::test_showAnalyzerList() {
    if (analyzer.size() == 0) {
        std::cout << "No analyzer attached yet!\n";
        return;
    }

    std::cout << "Attached analyzers: ";
    for (auto i : analyzer) {
        std::cout << i->getType() << "  ";
    }
    std::cout << '\n';
}

// Testausgabe der Analyseergebnisse
void dataSet::test_showAnalyzerResults() {
    if (analyzer.size() == 0) {
        std::cout << "No analyzer attached yet! -> No Results\n";
        return;
    }

    // Ausgabe der Ergebnisse der einzenen analyzer
    for (auto i : analyzer) {
        i->showResults();   // Hier wird Polymorphismus ausgenutzt!
    }
    std::cout << '\n';
}

