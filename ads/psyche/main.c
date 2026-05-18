#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "psyche.h"

/*
 * Pakeičia argv[0] plėtinį į ".cfg". Pavyzdžiai:
 *   "main.exe"          -> "main.cfg"
 *   "./psyche"          -> "./psyche.cfg"
 *   "/path/to/main.exe" -> "/path/to/main.cfg"
 * Iš iškvietėjo paima naują buferį, kurį reikia free().
 */
static char *deriveCfgPath(const char *argv0) {
    size_t len = strlen(argv0);
    char *buf = malloc(len + 8);
    if (!buf) return NULL;
    strcpy(buf, argv0);

    /* Ieškome paskutinio '.' po paskutinio '/' ar '\\' */
    char *dot = strrchr(buf, '.');
    char *slash1 = strrchr(buf, '/');
    char *slash2 = strrchr(buf, '\\');
    char *slash = slash1 > slash2 ? slash1 : slash2;
    if (dot && (slash == NULL || dot > slash)) {
        strcpy(dot, ".cfg");
    } else {
        strcat(buf, ".cfg");
    }
    return buf;
}

static void usage(const char *prog) {
    fprintf(stderr,
        "Naudojimas: %s [cfg_failas] [-rnd <seed>]\n"
        "\n"
        "  cfg_failas   parametrų failo kelias. Jei nenurodyta, automatiškai\n"
        "               išvedamas iš argv[0] keičiant plėtinį į '.cfg'.\n"
        "  -rnd <seed>  pseudo-atsitiktinių skaičių generatoriaus daviklis.\n"
        "               Jei nenurodyta, naudojama time(NULL).\n"
        "\n"
        "Cfg formatas — raktas vertė poros eilutėse. Komentarai po '#'.\n"
        "Galimi raktai: n k GrT m InT p1 d p2 h SessIn SessGr InK GrK SimT\n",
        prog);
}

int main(int argc, char *argv[]) {
    const char *cfgPath = NULL;
    char *cfgPathOwned = NULL;
    unsigned int seed = 0;
    int seedFromArg = 0;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-rnd") == 0 && i + 1 < argc) {
            char *endptr;
            seed = (unsigned int)strtoul(argv[++i], &endptr, 10);
            if (endptr == argv[i] || *endptr != '\0') {
                fprintf(stderr, "Klaida: netaisyklingas pseudo-atsitiktinių skaičių daviklis '%s'\n", argv[i]);
                return 1;
            }
            seedFromArg = 1;
        } else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            usage(argv[0]);
            return 0;
        } else if (argv[i][0] == '-') {
            fprintf(stderr, "Klaida: nežinoma parinktis '%s'\n", argv[i]);
            usage(argv[0]);
            return 1;
        } else if (cfgPath == NULL) {
            cfgPath = argv[i];
        } else {
            fprintf(stderr, "Klaida: per daug pozicinių argumentų\n");
            usage(argv[0]);
            return 1;
        }
    }

    if (cfgPath == NULL) {
        cfgPathOwned = deriveCfgPath(argv[0]);
        cfgPath = cfgPathOwned;
    }

    if (!seedFromArg) {
        seed = (unsigned int)time(NULL);
    }
    srand(seed);
    printf("Pseudo-atsitiktinių skaičių daviklis: %u%s\n", seed, seedFromArg ? " (iš -rnd)" : " (iš time)");
    printf("Cfg failas: %s\n", cfgPath);

    SimConfig cfg;
    cfgDefaults(&cfg);
    if (cfgLoad(cfgPath, &cfg) != 0) {
        free(cfgPathOwned);
        return 1;
    }

    simPrintConfig(&cfg, stdout);

    Simulation s;
    simInit(&s, cfg);
    printf("\nVykdoma simuliacija...\n");
    simRun(&s);
    simPrintStats(&s, stdout);
    simFree(&s);

    free(cfgPathOwned);
    return 0;
}
