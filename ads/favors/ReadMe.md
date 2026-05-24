# 4-asis laboratorinis darbas — Grafas (variantas 6)

## Naudoti ADT

| ADT | Šaltinis | Failai | Realizacija |
|---|---|---|---|
| Dekas                                   | [`1lalu2135.zip`](https://klevas.mif.vu.lt/~zagunas/ADS2026/StudWrk/1lalu2135.zip) (1 namų darbas, 9 variantas) | `Deque.c`, `Deque.h` | Dvigubai susietas mazgų sąrašas (`Node{prev,next,data}`). |
| Orientuotas grafas, kaimynystės sąrašai | Savo realizacija | `graph.c`, `graph.h` | Vardų masyvas + `Deque` masyvas — kiekvienai viršūnei atskiras dekas su kaimynų indeksais. |

### Rasti defektai svetimame ADT (3 vnt.)

1. `peekFront/peekRear/popFront/popRear` grąžina `-1` ir tuomet, kai
   dekas tuščias (`Deque.c:75-87, 135-151`). Iškvietėjas neatskirs
   nesėkmės nuo `-1` reikšmės Deke.
2. `pushFront/pushRear`** — jei `malloc` grąžina `NULL`, funkcija
   tiesiog `return;` ir dekas nesikeičia, bet kvietėjas apie
   tai nesužino (`Deque.c:95-96, 116-117`).
3. **Joks viešas metodas netikrina `d == NULL`** — perduodant `NULL`
   gauname segfault'ą (visos `Deque.c` funkcijos).

## Varianto sąlyga

> **6.** Duota informacija, kas kam yra padaręs paslaugą (gali būti
> situacijos, kad du žmonės yra padarę po paslaugą vienas kitam). Jei
> A yra padaręs paslaugą B, tai gali tikėtis, kad tiek B, tiek visi,
> kuriems B padarė paslaugą, paskolins jam pinigų. Patikrinti, ar,
> pavyzdžiui, Jonas gali tikėtis, kad Marytė paskolins jam pinigų.
> (grafo realizacija paremta kaimynystės sąrašais; naudoti paieškos į
> gylį metodą)

## Bendravimas ir bendradarbiavimas

API sluoksniai švariai atskirti — skirtingi autoriai gali toliau
dirbti neperžengdami vienas kito ribų:

- `Deque.h` — svetimas antraštinis failas, neliečiamas; grafas su juo
  bendrauja **tik per viešus prototipus** (`Create`, `pushRear`,
  `Done`) ir tipą `Node`.
- `graph.h` — viešoji grafo sąsaja (`Graph`, `graphCreate/Destroy`,
  `graphAddFavor`, `graphCanExpectLoan`).`.
- `main.c` — vartotojo sąsaja (CLI). Nieko nežino nei apie Deque,
  nei apie Graph vidų.

## Projekto struktūra

- `Deque.c`, `Deque.h` — perimtas dekas (kaimynų sąrašai).
- `graph.c`, `graph.h` — grafo ADT ir DFS.
- `main.c` — komandinės eilutės sąsaja.
- `Makefile` — atskiri transliacijos tikslai `Deque.o`, `graph.o`, `programa.exe`
- `Go.cmd` — programos transliacija ir 4 testai.

## Paleidimas

```sh
make
./programa.exe <failas> <kas> <kam>
```

Failo formatas — po porą vardų eilutėje, pirmas padarė paslaugą antrajam:

```
Jonas Maryte
Maryte Petras
```

### Pavyzdys

```sh
$ ./programa.exe data.txt Jonas Petras
TAIP: Jonas gali tiketis, kad Petras paskolins pinigu.
```

### Testai (`Go.cmd`)

| # | Scenarijus | Tikimasi |
|---|---|---|
| 1 | Tiesioginė paslauga `Jonas → Maryte`; klausimas Jonas → Maryte | TAIP |
| 2 | Grandinė `Jonas → Maryte → Petras`; klausimas Jonas → Petras | TAIP |
| 3 | Du atskiri komponentai; klausimas Jonas → Ona | NE |
| 4 | Abipuses paslaugos `Jonas ↔ Maryte`, `Maryte → Petras`; klausimas Petras → Jonas | NE |
