#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#include "psyche.h"

/* --- Vidinės konstantos --- */

#define QUEUE_CAP 4096   /* eilės talpa kiekvienai studento Deque struktūrai */
#define PATIENT_INIT_CAP 256

/* --- Pagalbinės funkcijos --- */

/* Vienodai pasiskirstęs pseudo atsitiktinis skaičius [0, 1). */
static double randUnit(void) {
    return (double)rand() / ((double)RAND_MAX + 1.0);
}

/* Bernoulli bandymas su tikimybe p. */
static int bernoulli(double p) {
    return randUnit() < p ? 1 : 0;
}

/* Atsitiktinis sveikasis [0, n). */
static int randIdx(int n) {
    if (n <= 0) return 0;
    return (int)(randUnit() * n);
}

/* Apvalinimas į viršų sveikajame dalijime. */
static int divCeil(int a, int b) {
    if (b <= 0) return a;
    return (a + b - 1) / b;
}

/* Sukuria BigInt iš long. */
static BigInt *bigFromLong(long v) {
    char buf[64];
    snprintf(buf, sizeof(buf), "%ld", v);
    return bigint_from_str(buf);
}

/* Prideda b prie *a vietoje (a perskirstoma). */
static void bigAccum(BigInt **a, const BigInt *b) {
    BigInt *t = bigint_add(*a, b);
    bigint_free(*a);
    *a = t;
}

/* Padaugina BigInt iš mažo sveiko ir grąžina naują BigInt. */
static BigInt *bigMulSmall(const BigInt *a, long m) {
    BigInt *mb = bigFromLong(m);
    BigInt *res = bigint_mul(a, mb);
    bigint_free(mb);
    return res;
}

/* Padalija BigInt iš mažo sveiko (egzaktiškai) ir grąžina naują BigInt. */
static BigInt *bigDivSmall(const BigInt *a, long m) {
    BigInt *mb = bigFromLong(m);
    BigInt *res = bigint_div(a, mb);
    bigint_free(mb);
    return res;
}

/* Saugus dequeue: tikrina dydį, grąžina 1 ir užpildo value, jei pavyko. */
static int dequeFront(Deque *d, int *value) {
    if (d->size == 0) return 0;
    *value = popFront(d);
    return 1;
}

/* --- Specialisto konstrukcija --- */

static void specInit(Specialist *sp) {
    sp->idle = 1;
    sp->sessionEndsAt = -1;
    sp->inSessionCap = 0;
    sp->inSessionCount = 0;
    sp->inSessionIds = NULL;
    initDeque(&sp->waitingNew, QUEUE_CAP);
    initDeque(&sp->waitingRet, QUEUE_CAP);
    sp->revenue = bigFromLong(0);
    sp->sessionsRun = 0;
    sp->hoursBusy = 0;
}

static void specFree(Specialist *sp) {
    freeDeque(&sp->waitingNew);
    freeDeque(&sp->waitingRet);
    free(sp->inSessionIds);
    bigint_free(sp->revenue);
}

static void specEnsureCap(Specialist *sp, int needed) {
    if (needed <= sp->inSessionCap) return;
    int newcap = sp->inSessionCap == 0 ? 4 : sp->inSessionCap;
    while (newcap < needed) newcap *= 2;
    sp->inSessionIds = realloc(sp->inSessionIds, newcap * sizeof(int));
    sp->inSessionCap = newcap;
}

/* --- Paciento konstrukcija --- */

/* Apskaičiuoja seansų skaičių iki pasveikimo pagal modalumą ir soc. faktorių. */
static int sessionsToHeal(const SimConfig *cfg, Modality mod, int social) {
    if (mod == MOD_INDIVIDUAL) {
        return cfg->SessIn;
    }
    /* Grupinė konsultacija */
    if (social) {
        /* Soc. faktoriaus pacientas grupėje pasveiksta h% greičiau už indiv. bazę. */
        int s = divCeil(cfg->SessIn * (100 - cfg->h), 100);
        if (s < 1) s = 1;
        return s;
    }
    /* Įprastas grupinis pacientas — daugiau seansų nei individualus. */
    return cfg->SessGr;
}

/* Sukuria naują pacientą ir grąžina jo indeksą s->patients masyve. */
static int patientCreate(Simulation *s, Modality mod) {
    if (s->patientCount == s->patientCap) {
        int newcap = s->patientCap == 0 ? PATIENT_INIT_CAP : s->patientCap * 2;
        s->patients = realloc(s->patients, newcap * sizeof(Patient));
        s->patientCap = newcap;
    }
    int idx = s->patientCount++;
    Patient *p = &s->patients[idx];
    p->id = s->nextPatientId++;
    p->state = ST_WAITING;
    p->modality = mod;
    p->hasSocialFactor = bernoulli(s->cfg.p2);
    p->isReturning = 0;
    p->sessionsLeft = sessionsToHeal(&s->cfg, mod, p->hasSocialFactor);
    p->waitStartHour = s->currentHour;
    p->totalWaitHours = 0;
    p->sessionEndsAt = -1;
    p->restingUntil = -1;
    p->spent = bigFromLong(0);

    /* Priskiriamas atsitiktinis to modalumo specialistas. */
    if (mod == MOD_INDIVIDUAL && s->cfg.m > 0) {
        p->specialistIdx = randIdx(s->cfg.m);
        pushBack(&s->individualStaff[p->specialistIdx].waitingNew, idx);
    } else if (mod == MOD_GROUP && s->cfg.n > 0) {
        p->specialistIdx = randIdx(s->cfg.n);
        pushBack(&s->groupStaff[p->specialistIdx].waitingNew, idx);
    } else {
        /* Nėra tokio modalumo specialistų — pacientas iškart išeina. */
        p->state = ST_UNFINISHED;
        p->specialistIdx = -1;
    }
    return idx;
}

/* --- Simuliacijos žingsniai --- */

/* Naujas pacientas atvyksta su tikimybe p1 (vienas per valandą max.). */
static void spawnNewPatient(Simulation *s) {
    if (!bernoulli(s->cfg.p1)) return;
    /* Modalumas pasirenkamas atsitiktinai pagal turimą personalą. */
    int canGroup = s->cfg.n > 0;
    int canInd   = s->cfg.m > 0;
    if (!canGroup && !canInd) return;
    Modality mod;
    if (canGroup && canInd) {
        mod = bernoulli(0.5) ? MOD_GROUP : MOD_INDIVIDUAL;
    } else {
        mod = canGroup ? MOD_GROUP : MOD_INDIVIDUAL;
    }
    patientCreate(s, mod);
}

/* Pakartotiniai: poilsio pabaigoje grįžta į savo specialisto returner eilę. */
static void promoteReturning(Simulation *s) {
    for (int i = 0; i < s->patientCount; i++) {
        Patient *p = &s->patients[i];
        if (p->state != ST_RESTING) continue;
        if (s->currentHour < p->restingUntil) continue;
        p->state = ST_WAITING;
        p->isReturning = 1;
        p->waitStartHour = s->currentHour;
        Specialist *sp = (p->modality == MOD_GROUP)
            ? &s->groupStaff[p->specialistIdx]
            : &s->individualStaff[p->specialistIdx];
        pushBack(&sp->waitingRet, i);
    }
}

/* Užbaigia visas sesijas, kurios baigiasi šią valandą. */
static void finishSessions(Simulation *s) {
    for (int g = 0; g < s->cfg.n; g++) {
        Specialist *sp = &s->groupStaff[g];
        if (sp->idle || sp->sessionEndsAt != s->currentHour) continue;
        for (int j = 0; j < sp->inSessionCount; j++) {
            Patient *p = &s->patients[sp->inSessionIds[j]];
            p->sessionsLeft--;
            if (p->sessionsLeft <= 0) {
                p->state = ST_HEALED;
                s->healedGroup++;
            } else {
                p->state = ST_RESTING;
                p->restingUntil = s->currentHour + s->cfg.d;
            }
            p->sessionEndsAt = -1;
        }
        sp->inSessionCount = 0;
        sp->idle = 1;
        sp->sessionEndsAt = -1;
    }
    for (int i = 0; i < s->cfg.m; i++) {
        Specialist *sp = &s->individualStaff[i];
        if (sp->idle || sp->sessionEndsAt != s->currentHour) continue;
        for (int j = 0; j < sp->inSessionCount; j++) {
            Patient *p = &s->patients[sp->inSessionIds[j]];
            p->sessionsLeft--;
            if (p->sessionsLeft <= 0) {
                p->state = ST_HEALED;
                s->healedIndividual++;
            } else {
                p->state = ST_RESTING;
                p->restingUntil = s->currentHour + s->cfg.d;
            }
            p->sessionEndsAt = -1;
        }
        sp->inSessionCount = 0;
        sp->idle = 1;
        sp->sessionEndsAt = -1;
    }
}

/* Sėdina pacientus pas laisvus specialistus. */
static void startSessions(Simulation *s) {
    /* Grupiniai: arba k naujokų, arba grupė tik iš pakartotinių. */
    for (int g = 0; g < s->cfg.n; g++) {
        Specialist *sp = &s->groupStaff[g];
        if (!sp->idle) continue;

        int newCount = sp->waitingNew.size;
        int retCount = sp->waitingRet.size;
        int groupSize = 0;
        int useReturning = 0;

        if (newCount >= s->cfg.k) {
            groupSize = s->cfg.k;
        } else if (retCount > 0) {
            /* Pakartotinai gali eiti ir mažesne grupe, bet ne su naujokais. */
            groupSize = retCount;
            useReturning = 1;
        } else {
            continue; /* nepakanka pacientų */
        }

        specEnsureCap(sp, groupSize);
        for (int j = 0; j < groupSize; j++) {
            int patIdx;
            if (useReturning) {
                if (!dequeFront(&sp->waitingRet, &patIdx)) break;
            } else {
                if (!dequeFront(&sp->waitingNew, &patIdx)) break;
            }
            sp->inSessionIds[j] = patIdx;
            Patient *p = &s->patients[patIdx];
            p->state = ST_IN_SESSION;
            p->totalWaitHours += s->currentHour - p->waitStartHour;
            p->sessionEndsAt = s->currentHour + s->cfg.GrT;

            /* Pacientas moka. */
            BigInt *fee = bigFromLong(s->cfg.GrK);
            bigAccum(&p->spent, fee);
            bigAccum(&sp->revenue, fee);
            bigAccum(&s->totalRevenue, fee);
            bigAccum(&s->totalGroupRevenue, fee);
            bigAccum(&s->totalGroupSpent, fee);
            bigint_free(fee);

            s->sessionsGroup++;
            s->waitHoursGroup += s->currentHour - p->waitStartHour;
        }
        sp->inSessionCount = groupSize;
        sp->idle = 0;
        sp->sessionEndsAt = s->currentHour + s->cfg.GrT;
        sp->sessionsRun++;
    }

    /* Individualūs: po vieną, pirmenybė pakartotiniams. */
    for (int i = 0; i < s->cfg.m; i++) {
        Specialist *sp = &s->individualStaff[i];
        if (!sp->idle) continue;

        int patIdx;
        if (sp->waitingRet.size > 0) {
            if (!dequeFront(&sp->waitingRet, &patIdx)) continue;
        } else if (sp->waitingNew.size > 0) {
            if (!dequeFront(&sp->waitingNew, &patIdx)) continue;
        } else {
            continue;
        }

        specEnsureCap(sp, 1);
        sp->inSessionIds[0] = patIdx;
        sp->inSessionCount = 1;
        sp->idle = 0;
        sp->sessionEndsAt = s->currentHour + s->cfg.InT;
        sp->sessionsRun++;

        Patient *p = &s->patients[patIdx];
        p->state = ST_IN_SESSION;
        p->totalWaitHours += s->currentHour - p->waitStartHour;
        p->sessionEndsAt = s->currentHour + s->cfg.InT;

        BigInt *fee = bigFromLong(s->cfg.InK);
        bigAccum(&p->spent, fee);
        bigAccum(&sp->revenue, fee);
        bigAccum(&s->totalRevenue, fee);
        bigAccum(&s->totalIndividualRevenue, fee);
        bigAccum(&s->totalIndividualSpent, fee);
        bigint_free(fee);

        s->sessionsIndividual++;
        s->waitHoursIndividual += s->currentHour - p->waitStartHour;
    }
}

/* Didina užimtų valandų skaičių jei specialistas dirba. */
static void tickBusy(Simulation *s) {
    for (int g = 0; g < s->cfg.n; g++) {
        if (!s->groupStaff[g].idle) s->groupStaff[g].hoursBusy++;
    }
    for (int i = 0; i < s->cfg.m; i++) {
        if (!s->individualStaff[i].idle) s->individualStaff[i].hoursBusy++;
    }
}

/* --- Viešas API --- */

void simInit(Simulation *s, SimConfig cfg) {
    memset(s, 0, sizeof(*s));
    s->cfg = cfg;
    s->currentHour = 0;
    s->nextPatientId = 1;
    s->patients = NULL;
    s->patientCount = 0;
    s->patientCap = 0;

    s->groupStaff = (cfg.n > 0) ? calloc(cfg.n, sizeof(Specialist)) : NULL;
    s->individualStaff = (cfg.m > 0) ? calloc(cfg.m, sizeof(Specialist)) : NULL;
    for (int i = 0; i < cfg.n; i++) specInit(&s->groupStaff[i]);
    for (int i = 0; i < cfg.m; i++) specInit(&s->individualStaff[i]);

    s->totalRevenue = bigFromLong(0);
    s->totalIndividualRevenue = bigFromLong(0);
    s->totalGroupRevenue = bigFromLong(0);
    s->totalIndividualSpent = bigFromLong(0);
    s->totalGroupSpent = bigFromLong(0);
}

void simFree(Simulation *s) {
    for (int i = 0; i < s->patientCount; i++) {
        bigint_free(s->patients[i].spent);
    }
    free(s->patients);
    for (int i = 0; i < s->cfg.n; i++) specFree(&s->groupStaff[i]);
    for (int i = 0; i < s->cfg.m; i++) specFree(&s->individualStaff[i]);
    free(s->groupStaff);
    free(s->individualStaff);
    bigint_free(s->totalRevenue);
    bigint_free(s->totalIndividualRevenue);
    bigint_free(s->totalGroupRevenue);
    bigint_free(s->totalIndividualSpent);
    bigint_free(s->totalGroupSpent);
    memset(s, 0, sizeof(*s));
}

void simRun(Simulation *s) {
    for (s->currentHour = 0; s->currentHour < s->cfg.SimT; s->currentHour++) {
        finishSessions(s);
        promoteReturning(s);
        spawnNewPatient(s);
        startSessions(s);
        tickBusy(s);
    }
    /* Pažymime nepasveikusius. */
    for (int i = 0; i < s->patientCount; i++) {
        Patient *p = &s->patients[i];
        if (p->state == ST_HEALED) continue;
        if (p->modality == MOD_INDIVIDUAL) s->unfinishedIndividual++;
        else s->unfinishedGroup++;
        if (p->state != ST_UNFINISHED) p->state = ST_UNFINISHED;
    }
}

/* Statistika ir rekomendacijos */
void simPrintStats(const Simulation *s, FILE *out) {
    fprintf(out, "\n--- REZULTATAI po %d val. ---\n", s->cfg.SimT);
    fprintf(out, "Iš viso pacientų:        %d\n", s->patientCount);
    fprintf(out, "  Indiv. modalumas:      %d\n",
            s->healedIndividual + s->unfinishedIndividual);
    fprintf(out, "  Grupinis modalumas:    %d\n",
            s->healedGroup + s->unfinishedGroup);
    fprintf(out, "Pasveiko (indiv./grup.): %d / %d\n",
            s->healedIndividual, s->healedGroup);
    fprintf(out, "Nepabaigė (indiv./grup.):%d / %d\n",
            s->unfinishedIndividual, s->unfinishedGroup);
    fprintf(out, "Seansų pravesta (indiv./grup.): %ld / %ld\n",
            s->sessionsIndividual, s->sessionsGroup);

    char *totRev = bigint_to_str(s->totalRevenue);
    char *totInd = bigint_to_str(s->totalIndividualRevenue);
    char *totGr  = bigint_to_str(s->totalGroupRevenue);
    fprintf(out, "Visos pajamos:           %s lt\n", totRev);
    fprintf(out, "  iš individualių:       %s lt\n", totInd);
    fprintf(out, "  iš grupinių:           %s lt\n", totGr);
    free(totRev); free(totInd); free(totGr);

    /* Vidutinės išlaidos tik pasveikusiems pacientams. */
    BigInt *healedIndSpent = bigFromLong(0);
    BigInt *healedGrpSpent = bigFromLong(0);
    for (int i = 0; i < s->patientCount; i++) {
        const Patient *p = &s->patients[i];
        if (p->state != ST_HEALED) continue;
        if (p->modality == MOD_INDIVIDUAL) bigAccum(&healedIndSpent, p->spent);
        else                                bigAccum(&healedGrpSpent, p->spent);
    }
    fprintf(out, "\n--- VIDUTINĖS IŠLAIDOS PASVEIKUSIEMS ---\n");
    if (s->healedIndividual > 0) {
        BigInt *avg = bigDivSmall(healedIndSpent, s->healedIndividual);
        char *str = bigint_to_str(avg);
        fprintf(out, "Indiv. vid. išlaidos:    %s lt (n=%d)\n",
                str, s->healedIndividual);
        free(str); bigint_free(avg);
    } else {
        fprintf(out, "Indiv. vid. išlaidos:    n/a (niekas nepasveiko)\n");
    }
    if (s->healedGroup > 0) {
        BigInt *avg = bigDivSmall(healedGrpSpent, s->healedGroup);
        char *str = bigint_to_str(avg);
        fprintf(out, "Grup. vid. išlaidos:     %s lt (n=%d)\n",
                str, s->healedGroup);
        free(str); bigint_free(avg);
    } else {
        fprintf(out, "Grup. vid. išlaidos:     n/a (niekas nepasveiko)\n");
    }

    /* Vidutinis laukimas. */
    fprintf(out, "\n--- VIDUTINIS LAUKIMAS EILĖJE ---\n");
    if (s->sessionsIndividual > 0) {
        fprintf(out, "Indiv. vid. laukimas:    %.2f val./seansas\n",
                (double)s->waitHoursIndividual / (double)s->sessionsIndividual);
    }
    if (s->sessionsGroup > 0) {
        fprintf(out, "Grup. vid. laukimas:     %.2f val./seansas\n",
                (double)s->waitHoursGroup / (double)s->sessionsGroup);
    }

    /* Specialistų užimtumas. */
    fprintf(out, "\n--- SPECIALISTŲ UŽIMTUMAS ---\n");
    long busyGr = 0, busyIn = 0;
    for (int i = 0; i < s->cfg.n; i++) busyGr += s->groupStaff[i].hoursBusy;
    for (int i = 0; i < s->cfg.m; i++) busyIn += s->individualStaff[i].hoursBusy;
    long totalGrHours = (long)s->cfg.n * s->cfg.SimT;
    long totalInHours = (long)s->cfg.m * s->cfg.SimT;
    double utilGr = totalGrHours > 0 ? 100.0 * busyGr / totalGrHours : 0.0;
    double utilIn = totalInHours > 0 ? 100.0 * busyIn / totalInHours : 0.0;
    fprintf(out, "Grupinių specialistų:    %.1f%% (%ld iš %ld val.)\n",
            utilGr, busyGr, totalGrHours);
    fprintf(out, "Individualių sp.:        %.1f%% (%ld iš %ld val.)\n",
            utilIn, busyIn, totalInHours);

    /* Rekomendacijos pacientui ir įstaigai. */
    fprintf(out, "\n--- REKOMENDACIJOS ---\n");
    if (s->healedIndividual > 0 && s->healedGroup > 0) {
        BigInt *avgI = bigDivSmall(healedIndSpent, s->healedIndividual);
        BigInt *avgG = bigDivSmall(healedGrpSpent, s->healedGroup);
        int cmp = bigint_cmp(avgI, avgG);
        if (cmp < 0) {
            fprintf(out, "Pacientui: pigiau rinktis INDIVIDUALIĄ terapiją.\n");
        } else if (cmp > 0) {
            fprintf(out, "Pacientui: pigiau rinktis GRUPINĘ terapiją.\n");
        } else {
            fprintf(out, "Pacientui: abiejų metodų išlaidos lygios.\n");
        }
        bigint_free(avgI);
        bigint_free(avgG);
    } else {
        fprintf(out, "Pacientui: nepakanka duomenų rekomendacijai "
                     "(per mažai pasveikusių).\n");
    }
    fprintf(out, "Įstaigai: ");
    if (utilGr > 90.0 && utilIn < 60.0) {
        fprintf(out, "padidinti grupinių specialistų skaičių (n).\n");
    } else if (utilIn > 90.0 && utilGr < 60.0) {
        fprintf(out, "padidinti individualių specialistų skaičių (m).\n");
    } else if (utilGr < 30.0 && utilIn < 30.0) {
        fprintf(out, "personalas nepakankamai apkrautas — galima mažinti.\n");
    } else {
        fprintf(out, "personalo paskirstymas atrodo subalansuotas.\n");
    }

    bigint_free(healedIndSpent);
    bigint_free(healedGrpSpent);
}

void simPrintConfig(const SimConfig *cfg, FILE *out) {
    fprintf(out, "Konfigūracija:\n");
    fprintf(out, "  Grupinių specialistų (n):           %d\n", cfg->n);
    fprintf(out, "  Žmonių grupėje (k):                 %d\n", cfg->k);
    fprintf(out, "  Grupinės sesijos trukmė (GrT):      %d val.\n", cfg->GrT);
    fprintf(out, "  Individualių specialistų (m):       %d\n", cfg->m);
    fprintf(out, "  Individualios sesijos trukmė (InT): %d val.\n", cfg->InT);
    fprintf(out, "  Naujo paciento tikimybė (p1):       %.4f\n", cfg->p1);
    fprintf(out, "  Pakart. vizito intervalas (d):      %d val.\n", cfg->d);
    fprintf(out, "  Soc. faktoriaus tikimybė (p2):      %.4f\n", cfg->p2);
    fprintf(out, "  Soc. faktoriaus greitėjimas (h):    %d %%\n", cfg->h);
    fprintf(out, "  Seansų iki pasveikimo (SessIn):     %d\n", cfg->SessIn);
    fprintf(out, "  Seansų iki pasveikimo (SessGr):     %d\n", cfg->SessGr);
    fprintf(out, "  Individuali kaina (InK):            %ld lt\n", cfg->InK);
    fprintf(out, "  Grupinė kaina (GrK):                %ld lt\n", cfg->GrK);
    fprintf(out, "  Simuliacijos trukmė (SimT):         %d val.\n", cfg->SimT);
}

/* --- Cfg skaitymas --- */

void cfgDefaults(SimConfig *cfg) {
    cfg->n = 2;
    cfg->k = 4;
    cfg->GrT = 2;
    cfg->m = 3;
    cfg->InT = 1;
    cfg->p1 = 0.30;
    cfg->d = 48;
    cfg->p2 = 0.10;
    cfg->h = 30;
    cfg->SessIn = 5;
    cfg->SessGr = 8;
    cfg->InK = 60;
    cfg->GrK = 25;
    cfg->SimT = 720;
}

/* Nuskaito vieną raktas-vertė porą. Grąžina 0 jei viskas gerai. */
static int parseLine(char *line, SimConfig *cfg) {
    /* Iškerpame komentarą po '#'. */
    char *hash = strchr(line, '#');
    if (hash) *hash = '\0';

    /* Praleidžiame tarpus pradžioje. */
    while (*line && isspace((unsigned char)*line)) line++;
    if (*line == '\0') return 0;

    char key[64];
    char val[128];
    if (sscanf(line, "%63s %127s", key, val) != 2) return 0;

    if      (strcmp(key, "n")      == 0) cfg->n      = atoi(val);
    else if (strcmp(key, "k")      == 0) cfg->k      = atoi(val);
    else if (strcmp(key, "GrT")    == 0) cfg->GrT    = atoi(val);
    else if (strcmp(key, "m")      == 0) cfg->m      = atoi(val);
    else if (strcmp(key, "InT")    == 0) cfg->InT    = atoi(val);
    else if (strcmp(key, "p1")     == 0) cfg->p1     = atof(val);
    else if (strcmp(key, "d")      == 0) cfg->d      = atoi(val);
    else if (strcmp(key, "p2")     == 0) cfg->p2     = atof(val);
    else if (strcmp(key, "h")      == 0) cfg->h      = atoi(val);
    else if (strcmp(key, "SessIn") == 0) cfg->SessIn = atoi(val);
    else if (strcmp(key, "SessGr") == 0) cfg->SessGr = atoi(val);
    else if (strcmp(key, "InK")    == 0) cfg->InK    = atol(val);
    else if (strcmp(key, "GrK")    == 0) cfg->GrK    = atol(val);
    else if (strcmp(key, "SimT")   == 0) cfg->SimT   = atoi(val);
    else {
        fprintf(stderr, "Įspėjimas: nežinomas cfg raktas '%s'\n", key);
    }
    return 0;
}

int cfgLoad(const char *path, SimConfig *cfg) {
    FILE *f = fopen(path, "r");
    if (!f) {
        fprintf(stderr, "Klaida: nepavyko atidaryti cfg failo '%s'\n", path);
        return 1;
    }
    char line[512];
    while (fgets(line, sizeof(line), f)) {
        parseLine(line, cfg);
    }
    fclose(f);
    return 0;
}
