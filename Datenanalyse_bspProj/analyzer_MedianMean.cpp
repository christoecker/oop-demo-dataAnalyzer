#include "analyzer_MedianMean.h"
#include <algorithm>

// Berechnet Median und Mittelwert
bool analyzer_MedianMean::update()
{
    double sum = 0;     // Speichert die Summe aller Messwerte
    int cnt = ownerSeries->size();
    if (cnt == 0)
        return false;   // Messreihe ist leer -> Keine Berechnung möglich.

    // valBfr ist ein vektor, in den nur die Messwerte (ohne Zeiten)
    // übernommen und sortiert werden, um den Median zu ermitteln.
    std::vector<double> valBfr(cnt);
    for (int n = 0; n < cnt; n++) {
        valBfr[n] = (*ownerSeries)[n].value;
        sum += valBfr[n];   // Berechne Summe aller Messwerte
    }
    mean = sum / cnt;       // Berechne Mittelwert

    std::sort(valBfr.begin(), valBfr.end());                    // Sortiere Messwerte aufsteigend.
    if (cnt % 2 == 1) median = valBfr[(cnt - 1) / 2];           // Median liegt genau in der Mitte, wenn Anzahl Messwerte ungerade...
    else median = (valBfr[cnt / 2] + valBfr[cnt / 2 - 1]) / 2;  // andernfalls arith. Mittel aus zwei Messwerten in der Mitte.

    return true;
}

// Ausgabe der Analyseergebnisse
void analyzer_MedianMean::showResults()
{
    std::cout << "Results of " << type << '\n';
    std::cout << "Mean: " << mean << "\nMedian: " << median << '\n';
}
