#pragma once
#include <iostream>
#include <ctime>
#include <map>

// Mögliche Einheiten für Messwerte
enum class eUnit {empty, volt, amp, watt} ;
// Mögliche Präfixe für die Einheiten
enum class ePrefix {p=-12, n=-9, mu=-6, m=-3, none=0, k=3, M=6};

// Meswerte werden als Struktur von Zeitstempel und Wert gespeichert.
struct stMPoint {
	time_t tStamp;      // time_t ist ein int-Datentyp. Ggf. ist ein double besser geeignet...
	double value;
} ;

// Einheit für die Messreihe ist eine Struktur von Einheit und Präfix
typedef struct {
	eUnit unit;
	ePrefix prefix;
} stUnit;

// Ausgabeoperator für eUnit überladen
inline std::ostream& operator<<(std::ostream& os, eUnit const& u)
{
    switch (u) {
    case eUnit::amp:
        os << " amp";
        break;
    case eUnit::volt:
        os << " volt";
    case eUnit::watt:
        os << " watt";
    default:
        os << " ";
    }
    return os;
}

// Ausgabeoperator für Präfix überladen
inline std::ostream& operator<<(std::ostream& os, ePrefix const& p)
{
    if (p != ePrefix::none) {
        if (p == ePrefix::p || p == ePrefix::mu || p == ePrefix::n)
            os << "e" << static_cast<int>(p);
        else
            os << "e+" << static_cast<int>(p);
    }
    return os;
}

// Funktion wird benötigt, um die Messwerte in einem Vektor vergleichen zu 
// können, wobei der Vergleich der Strukturvariablen bzgl. des
// Zeitstempels erfolgen soll.
inline bool compMPoint_byTime(const stMPoint& p1, const stMPoint& p2) {
    return p1.tStamp < p2.tStamp;
}

inline bool myContains(std::map<time_t, std::string> m,  const time_t& t) {
    for (auto i = m.begin(); i != m.end(); i++) {
        if (i->first == t) return true;
    }
    return false;
}

