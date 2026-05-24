#include "graph.h"

#include <stdio.h>
#include <stdlib.h>

static void usage(const char *prog) {
    fprintf(stderr,
            "Naudojimas:\n"
            "  %s <failas> <kas> <kam>\n\n"
            "Tikrina, ar <kas> gali tiketis, kad <kam> paskolins pinigu.\n"
            "Failo formatas: kiekvienoje eiluteje pora vardu,\n"
            "  pirmas vardas - padarei paslauga antrajam, pvz.:\n"
            "    Jonas Maryte\n"
            "    Maryte Petras\n",
            prog);
}

static int loadGraph(Graph *g, const char *filename) {
    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        perror("Nepavyko atidaryti failo");
        return 0;
    }

    char from[256];
    char to[256];
    int  line = 0;
    while (fscanf(f, "%255s %255s", from, to) == 2) {
        line++;
        if (!graphAddFavor(g, from, to)) {
            fprintf(stderr, "Klaida skaitant %d eilute.\n", line);
            fclose(f);
            return 0;
        }
    }

    fclose(f);
    return 1;
}

int main(int argc, char **argv) {
    if (argc != 4) {
        usage(argv[0]);
        return EXIT_FAILURE;
    }

    const char *filename = argv[1];
    const char *asker    = argv[2];
    const char *target   = argv[3];

    Graph *g = graphCreate();
    if (g == NULL) {
        fprintf(stderr, "Nepavyko sukurti grafo.\n");
        return EXIT_FAILURE;
    }

    if (!loadGraph(g, filename)) {
        graphDestroy(g);
        return EXIT_FAILURE;
    }

    if (graphCanExpectLoan(g, asker, target)) {
        printf("TAIP: %s gali tiketis, kad %s paskolins pinigu.\n",
               asker, target);
    } else {
        printf("NE: %s negali tiketis, kad %s paskolins pinigu.\n",
               asker, target);
    }

    graphDestroy(g);
    return EXIT_SUCCESS;
}
