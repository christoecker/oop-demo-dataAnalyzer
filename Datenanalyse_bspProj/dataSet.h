#pragma once
#include <vector>
#include <map>
#include <fstream>
#include <string>
#include <algorithm>
#include <iomanip>

#include "projBasics.h"
#include "analyzer_IF.h"
#include "analyzer_MedianMean.h"
#include "analyzer_MovingAverage.h"
class analyzer_IF;

/*
	Klasse Dataset ist eine zentrale Klasse in dem Programm,
	die die Messwerte in einem vector enthält. Darüber hinaus
	speichert dataSet auch Kommentare zu einzelnen Messwerten
	und Pointer auf die analyzer, die eine Analyse der enthaltenen
	Messwerte vornehmen.
*/
class dataSet
{
private:
	std::vector<stMPoint> series;				// speichert Messwerte
	stUnit unit;								// Einheit der Messwerte
	std::map<time_t, std::string> comments;		// Kommentare
	std::vector<analyzer_IF*> analyzer;			// Analyzer
public:
	dataSet(stUnit u) : unit(u) {};
	dataSet(stUnit u, std::vector<stMPoint> s) : unit(u), series(s) {};
	int valTotalCount() { return series.size(); };
	stUnit getUnit() { return unit; };
	int importFromCSV(const std::string&, const char);
	int addComment(const time_t, const std::string);
	bool removeComment(const time_t);

	bool addAnalyzer(analyzer_IF*);
	bool analyzerSettings(std::string);

	// --- Testausgaben der Messreihe für Entwicklungszwecke ---
	void test_showVec() {
		if (valTotalCount() == 0) {
			std::cout << "This data series is empty!\n";
			return;
		}

		int cnt = 1;
		for (const auto& i : series) {
			std::cout << std::fixed << std::setprecision(3);
			std::cout << "(" << std::setw(3) << cnt++ << ") " << i.tStamp << "; " << std::setw(6) << i.value;
			// Ausgabe der Kommentare (falls vorhanden)
			if (myContains(comments, i.tStamp))
				std::cout << " -> " << comments.at(i.tStamp) << '\n';
			else
				std::cout << '\n';
		}
	}

	void test_showAnalyzerList();		// Testausgabe der analyzer
	void test_showAnalyzerResults();	// Testausgabe der Analyseergebnisse
};

