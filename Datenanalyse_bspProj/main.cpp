#include <cstdlib>
#include <iostream>
#include <ctime>
#include <algorithm>

#include "projBasics.h"
#include "dataSet.h"

int main() {
	// Test 1: Dataset erzeigen, Messwerte aus csv importieren und Messreihe ausgeben
	dataSet s = dataSet({ eUnit::volt, ePrefix::none });
	s.importFromCSV("Mappe1.csv", ';');
	std::cout << "1. Ausgabe der Messreihe nach Importieren der Messwerte:\n";
	s.test_showVec();
	system("pause");

	// Test 2: Einem Messwert wird ein Kommentar hinzugefügt
	system("cls");
	s.addComment(44270, "Ein Testkommentar...");
	std::cout << "2. Ausgabe der Messreihe nach Hinzufuegen eines Kommentars:\n";
	s.test_showVec();
	system("pause");

	// Test 3: Zwei analyzer hinzufügen und Angabe der analyzer
	system("cls");
	s.addAnalyzer(new analyzer_MedianMean());
	s.addAnalyzer(new analyzer_MovingAverage());
	std::cout << "3. Ausgabe der hinzugefuegten analyzer:\n";
	s.test_showAnalyzerList();
	system("pause");

	// Test 4: Einstellungen der analyzer vornehmen
	system("cls");
	std::cout << "4. Einstellungen der analyzer:\n";
	s.analyzerSettings("_MedianMean");
	s.analyzerSettings("_MovingAverage");
	system("pause");

	// Test 5: Ausgabe der Analyseergebnisse
	system("cls");
	std::cout << "5. Ausgabe der Ergebnisse der analyzer:\n";
	s.test_showAnalyzerResults();
	system("pause");

	// Test 6: Messreihe um weitere Werte ergänzen. Dabei treten Messwerte
	// auf, deren Zeitstempel bereits in s enthalten sind
	system("cls");
	s.importFromCSV("Mappe2.csv", ';');
	std::cout << "6. Ausgabe der um einige Werte ergaenzten Messreihe:\n";
	s.test_showVec();
	system("pause");

	// Test 7: Ergebnisse der analyzer werden automatisch aktualisiert,
	// nachdem neue Messwerte hinzugefügt wurden. 
	system("cls");
	std::cout << "7. Ausgabe der aktualisierten Ergebnisse der analyzer:\n";
	s.test_showAnalyzerResults();
	
	return EXIT_SUCCESS;
}