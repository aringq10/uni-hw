# Bin Packing problema

## Varianto sąlyga

Realizuoti „Bin Packing“ (daiktų talpinimo į dėžes) sprendiklį, kuris, duotai dėžės talpai `M` ir daiktų tūriams `v1 ... vN`, rastų būdus, kaip visus daiktus sutalpinti į minimalų dėžių skaičių.

Programa turi palaikyti kelis sprendimo režimus:
- pilną paiešką (optimalus sprendinys),
- pirmo atitikimo paiešką,
- euristinius metodus (`heuristic heuristic_number`).

Taip pat turi būti galimybė riboti vykdymo laiką (`timeout`).

---

## Bendroji kompetencija: bendravimas ir bendradarbiavimas

Darbo metu buvo laikomasi aiškios struktūros ir atsakomybės atskyrimo:

- API (antraštinis failas `bin-packing.h`) aiškiai apibrėžia struktūras (`Solver`, `Solution`) ir jų naudojimą.
- Implementacija (`bin-packing.c`) atskirta nuo vartotojo sąsajos (`main.c`).
- Naudojami aiškūs komentarai prie funkcijų ir struktūrų.
- Sprendimo režimai (`SearchMode`) ir jų pasirinkimas realizuoti taip, kad būtų paprasta pridėti naujus metodus.

---

## Įgyvendinimas

### Duomenų struktūros

- `Solver` – pagrindinė struktūra, sauganti:
  - įvestį (`n`, `capacity`, `volumes`)
  - konfigūraciją (`mode`, `heuristic`, `timeout`)
  - vykdymo būseną (`timedOut`, `solutionFound`, `error`)
  - rezultatus (`solutions`)

- `Solution` – vienas sprendinys:
  - `binCount` – dėžių skaičius
  - `whichBin[i]` – kuriai dėžei priskirtas i-tasis elementas

---

### Realizuoti algoritmai

#### 1. Pilna paieška (`FULL`)

Naudojamas rekursyvus backtracking algoritmas (`backtrack`):

- Bandoma kiekvieną elementą dėti į visas galimas dėžes.
- Jei reikia – sukuriama nauja dėžė.
- Taikomas šakų apkarpymas:
  - jei jau naudojama daugiau dėžių nei geriausiame sprendinyje – šaka nutraukiama
  - praleidžiamos simetriškos būsenos (`binVolumes[j] == binVolumes[j-1]`)
- Gali rasti visus optimalius sprendinius arba pirmą (priklausomai nuo režimo).

---

#### 2. First Fit (`heuristic != 1)

Algoritmas:

- Einama per elementus eilės tvarka
- Kiekvienas dedamas į pirmą dėžę, kur telpa
- Jei netelpa – sukuriama nauja dėžė

---

#### 3. First Fit Decreasing (`heuristic 1`)

Realizuota euristika:

- Elementai pirmiausia surikiuojami mažėjančia tvarka
- Tada taikomas `First Fit`

---

### Timeout mechanizmas

- Naudojama funkcija `isTimeout`
- Tikrinama kiekviename svarbiame algoritmų žingsnyje
- Jei viršytas laikas:
  - nutraukiama paieška
  - jei yra dalinių sprendinių – jie grąžinami

---

## Programos paleidimas

### Naudojimas

```
Usage: main.exe
   [-|failo_vardas]
   [-mode [fullSearch|firstMatchSearch|heuristic heuristic_number]]
   [-timeout miliseconds]
```

---

### Įvesties formatas

```
Data format:
   M            (Bin capacity)
   N            (Number of items)
   v1 v2 ... vN (Volumes of items)
```