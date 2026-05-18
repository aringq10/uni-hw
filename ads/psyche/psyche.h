#ifndef PSYCHE_H
#define PSYCHE_H

#include <stdio.h>

#include "Deque.h"
#include "BigInt.h"

/* Simuliacijos konfigūracija — visi parametrai įvedami per .cfg failą. */
typedef struct {
    /* Specialistai */
    int n;        /* grupinių specialistų skaičius */
    int k;        /* žmonių skaičius grupėje */
    int GrT;      /* grupinės sesijos trukmė (val.) */
    int m;        /* individualių specialistų skaičius */
    int InT;      /* individualios sesijos trukmė (val.) */

    /* Pacientų srautas */
    double p1;    /* naujo paciento atvykimo tikimybė per valandą */
    int    d;     /* pakartotinio vizito intervalas (val.) */
    double p2;    /* socialinio faktoriaus tikimybė */
    int    h;     /* socialinio faktoriaus greitėjimas procentais */

    /* Gydymas */
    int SessIn;   /* baziniai individualūs seansai iki pasveikimo */
    int SessGr;   /* baziniai grupiniai seansai iki pasveikimo */

    /* Pinigai (litais) */
    long InK;     /* individualios konsultacijos kaina */
    long GrK;     /* grupinės konsultacijos kaina */

    /* Simuliacija */
    int SimT;     /* visas simuliacijos laikas (val.) */
} SimConfig;

typedef enum {
    MOD_INDIVIDUAL = 0,
    MOD_GROUP      = 1
} Modality;

typedef enum {
    ST_IN_SESSION = 0,  /* dabar konsultuojamas */
    ST_RESTING    = 1,  /* laukia kol praeis d val. nuo paskutinio vizito */
    ST_WAITING    = 2,  /* eilėje pas specialistą */
    ST_HEALED     = 3,  /* pasveikęs, simuliacijos atžvilgiu paliko sistemą */
    ST_UNFINISHED = 4   /* nepasveiko per SimT — pažymimas pabaigoje */
} PatientState;

typedef struct {
    int id;
    PatientState state;
    Modality modality;
    int sessionsLeft;       /* kiek dar konsultacijų reikia iki pasveikimo */
    int specialistIdx;      /* priskirto specialisto indeksas (grupiniame ar individualiame masyve) */
    int hasSocialFactor;    /* 1 — pacientas turi p2 efektą */
    int isReturning;        /* 0 — pirmas vizitas, 1 — pakartotinis */
    int waitStartHour;      /* kada įstojo į eilę */
    int totalWaitHours;     /* sukauptas laukimas */
    int sessionEndsAt;      /* val., kada baigsis dabartinė sesija (jei IN_SESSION) */
    int restingUntil;       /* val., kada baigsis poilsis (jei RESTING) */
    BigInt *spent;          /* iš viso išleista (litai, ilgas sk.) */
} Patient;

typedef struct {
    int idle;               /* 1 — laukia eilės, 0 — užimtas */
    int sessionEndsAt;      /* val., kada baigsis dabartinė sesija */
    int *inSessionIds;      /* dabar konsultuojamų pacientų ID masyvas */
    int  inSessionCount;
    int  inSessionCap;
    Deque waitingNew;       /* naujokai eilėje */
    Deque waitingRet;       /* pakartotiniai eilėje */
    BigInt *revenue;        /* iš viso uždirbo */
    int sessionsRun;        /* kiek sesijų pravedė */
    int hoursBusy;          /* užimtų valandų suma */
} Specialist;

typedef struct {
    SimConfig cfg;
    int currentHour;

    /* Pacientų rinkinys */
    Patient *patients;
    int patientCount;
    int patientCap;
    int nextPatientId;

    /* Specialistų masyvai */
    Specialist *groupStaff;       /* dydis cfg.n */
    Specialist *individualStaff;  /* dydis cfg.m */

    /* Statistika */
    int healedIndividual;
    int healedGroup;
    int unfinishedIndividual;
    int unfinishedGroup;
    long sessionsIndividual;
    long sessionsGroup;
    long waitHoursIndividual;
    long waitHoursGroup;

    BigInt *totalRevenue;
    BigInt *totalIndividualRevenue;
    BigInt *totalGroupRevenue;
    BigInt *totalIndividualSpent;
    BigInt *totalGroupSpent;
} Simulation;

/* --- KONSTRUKTORIAI / DESTRUKTORIAI --- */

void simInit(Simulation *s, SimConfig cfg);
void simFree(Simulation *s);

/* --- SIMULIACIJA --- */

/* Vykdo visą simuliaciją nuo 0 iki cfg.SimT valandų. */
void simRun(Simulation *s);

/* Spausdina rezultatus į out (statistika, rekomendacijos). */
void simPrintStats(const Simulation *s, FILE *out);

/* Spausdina cfg į out (žmogui skaitomu pavidalu). */
void simPrintConfig(const SimConfig *cfg, FILE *out);

/* --- CFG SKAITYMAS --- */

/*
 * Skaito raktažodžių poras (raktas vertė) iš path. Nuskaityti parametrai
 * surašomi į cfg. Grąžina 0 jei pavyko, kitu atveju !=0. Klaidos
 * pranešimai rašomi į stderr.
 */
int cfgLoad(const char *path, SimConfig *cfg);

/* Užpildo cfg numatytomis vertėmis (kad būtų galima paleisti be cfg). */
void cfgDefaults(SimConfig *cfg);

#endif
