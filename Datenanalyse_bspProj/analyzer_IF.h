#pragma once
#include <string>
#include <vector>
#include "projBasics.h"

/*
	analyzer_IF ist eine Schnittstelle zwischen den Klassen dataSet und
	analyzer_X. Die Klasse ist abstrakt und fordert von jeder Klasse, die diese
	Schnittstelle implementiert, dass verschiedene Funktionen implementiert werde.
*/
class analyzer_IF
{
protected:
	
public:
	virtual bool update() = 0;									// Update der Analyseergebnisse
	virtual void setOwnerSeries(std::vector<stMPoint>*) = 0;	// Pointer auf Messreihe setzen 
	virtual std::string getType() = 0;							// Rückgabe des analyzer-Typs
	virtual void showResults() = 0;								// Ausgabe der Analyseergebnisse
	virtual bool settings() = 0;								// Eeinstellungen des analyzers
};

