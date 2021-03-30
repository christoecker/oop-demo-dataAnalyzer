#pragma once
#include "analyzer_IF.h"

/*
	Klasse analyzer_MovingAverage berechnet zu einer gegebenen Messreihe
	den gleitenden Mittelwert. Die Klasse muss die Schnittstelle
	analyzer_IF implementieren, um an ein dataSet-Objekt angefügt werden
	zu können.
*/
class analyzer_MovingAverage : public analyzer_IF
{
private:
	int horizon;			// Horizont, über den ein gleitender MW gebildet werden soll
	const int minGap = 10;	// min. Abstand zwischen horizon und ownerSeries->size()
	std::vector<stMPoint> mAverage;				// speichert Werte des gleitenden Mittelwerts
	const std::string type = "_MovingAverage";	// Analyzer Typ
	std::vector<stMPoint>* ownerSeries;			// Pointer auf Messreihe
	bool setHorizon(int);
public:
	analyzer_MovingAverage(int pHorizon = 0) : horizon(pHorizon) {
		ownerSeries = nullptr;
	};
	void setOwnerSeries(std::vector<stMPoint>* o);
	bool update();
	std::string getType() { return type; };
	void showResults();
	bool settings();
};

