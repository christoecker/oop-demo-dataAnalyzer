#pragma once
#include "analyzer_IF.h"

/*
	Klasse analyzer_MedianMean berechnet zu einer gegebenen Messreihe
	den Mittelwert und den Median. Die Klasse muss die Schnittstelle
	analyzer_IF implementieren, um an ein dataSet-Objekt angefügt werden
	zu können.
*/
class analyzer_MedianMean : public analyzer_IF
{
private:
	double median, mean;						// Speichern Analyseergebnisse
	const std::string type = "_MedianMean";		// Definiert den Typ
	// ANMERKUNG: Der Typ könnte mehrfach definiert werden. -> Verbesserungspotential
	std::vector<stMPoint>* ownerSeries;			// Pointer auf Messreihe
public:
	analyzer_MedianMean(){
		ownerSeries = nullptr;
		median = 0;
		mean = 0;
	};
	void setOwnerSeries(std::vector<stMPoint>* o) { ownerSeries = o; update(); }
	bool update();
	std::string getType() { return type; }
	void showResults();
	bool settings() {
		std::cout << type << " has no settings.\n";
		return true;
	}
};

