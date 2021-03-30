#include "analyzer_MovingAverage.h"
#include <deque>
#include <iomanip>

void analyzer_MovingAverage::setOwnerSeries(std::vector<stMPoint>* o)
{
	ownerSeries = o;
	update();
}

// Berechnet gleitenden Mittelwert
bool analyzer_MovingAverage::update()
{
	if (horizon == 0) return false;	// Falls Einstellung horizon auf 0 ist, kann 
									// keine Berechnung durchgef�hrt werden.

	mAverage.resize(ownerSeries->size());			// Passe Gr��e des Vektors mAverage an
	if (mAverage.size() <= minGap) return false;	// horizon ist zu gro� f�r die gegebene Messreihe

	std::deque<double> ringBfr(horizon, 0);			// Ringspeicher, um gleitenden Mittelwert zu berechnen
	double sum = 0;									// Speichert Summe der Werte im Ringspeicher
	for (int n = 0; n < ownerSeries->size(); n++) {
		mAverage[n].tStamp = (*ownerSeries)[n].tStamp;	// �bernehme Zeitstempel

		ringBfr.push_front((*ownerSeries)[n].value);	// F�ge neuen Messwert vorne an den Ringspeicher an
		sum = sum + ringBfr.front() - ringBfr.back();	// Berechne neue Summe (ohne letzten Wert)
		ringBfr.pop_back();								// Entferne letzten Wert

		if (n < horizon-1) mAverage[n].value = 0;		// F�r die ersten 'horizon' Werte: 0
		else mAverage[n].value = sum / horizon;			// sonst Mittelwert
	}
}

// Setze EInstellung horizon
bool analyzer_MovingAverage::setHorizon(int newHorizon)
{
	// Pr�fung, ob neuer Wert nicht zu klein und nicht zu gro� ist
	if (newHorizon > 2 && newHorizon < (ownerSeries->size() - minGap)) {
		horizon = newHorizon;
		return true;			// Einstellung erfolgreich
	}
	return false;				// Ung�ltiger Wert f�r die Einstellung
}

// Asugabe der Analyseergebnisse
void analyzer_MovingAverage::showResults()
{
	// Es gibt bislang keine Analyseergebnisse. Dies kann mehrere Gr�nde haben...
	if (mAverage.size() == 0) {
		std::cerr << "No analyzer results by " << type << "!\n";
		std::cerr << "This error can be caused by differend reasons:\n";
		std::cerr << "The data basis is empty.  -> Check wether the analyzer is attached to a data series which is not empty.\n";
		std::cerr << "The horizon is set to 0.  -> Increase the horizon using settings()\n.";
		return;
	}
	
	// Ausgabe der Werte
	std::cout << "Results of " << type << '\n';
	int cnt = 1;
	for (auto& i : mAverage) {
		std::cout << std::fixed << std::setprecision(3);
		std::cout << "(" << std::setw(3) << cnt++ << ") " << std::setw(10) << i.tStamp << ", " << std::setw(10) << i.value << '\n';
	}
}

// EInstellungen des analyzers vornehmen
bool analyzer_MovingAverage::settings()
{
	std::cout << "Settings of " << type << '\n';
	std::cout << "Please enter horizon: ";
	int local_Horizon;
	if (std::cin >> local_Horizon) {
		if (setHorizon(local_Horizon)) {	// Aufruf der privaten Funktion 
			update();						// Nach erfolgreicher �nderung der Einstellungen, m�ssen
											// Analysen aktualisiert werden.
			return true;
		}
	}

	return false;		// Einstellungen konnten nicht ge�ndert werden.
}
