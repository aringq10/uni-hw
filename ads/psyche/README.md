# Apsilankymas pas psichologą

## Naudoti ADT

| ADT | Šaltinis | Failai | Implementacija |
|---|---|---|---|
| Eilė | [`1jure2111.zip`](https://klevas.mif.vu.lt/~zagunas/ADS2026/StudWrk/1jure2111.zip) (1 namų darbas, 2 variantas) | `Deque.c`, `Deque.h` | Žiedinis dinaminis masyvas. Naudojamos `pushBack` / `popFront` operacijos (eilės FIFO režimas). |
| Ilgas sveikas skaičius | [`1juku2176.zip`](https://klevas.mif.vu.lt/~zagunas/ADS2026/StudWrk/1juku2176.zip) (1 namų darbas, 11 variantas) | `BigInt.c`, `BigInt.h` | Dinaminis masyvas, BASE = 100 (2 dešimtainiai skaitmenys vienam masyvo elementui). |


### Rasti defektai (3 vnt.)

1. **`Deque.popFront` / `popBack` grąžina 0 ir tuščios eilės atveju, ir
   tuomet, kai pati reikšmė yra 0** — neatskirsi nesėkmės nuo galiojančio
   nulio.
2. **`Deque.initDeque` netikrina `capacity <= 0`** ir nenuolydžiai
   `malloc(0)` kraunamas. Nepasireiškia, nes visuomet kviečiama su
   teigiama talpa (`QUEUE_CAP = 4096`).

## Variantas ir sąlyga

Studento ID: **arci2177**.
ADS 2026, 3 namų darbas. Užduoties sąlyga
([3uzduotis.htm](https://klevas.mif.vu.lt/~zagunas/ADS2026/Uzduotys/3uzduotis.htm)),
9 variantas:

> **(ADT: eilė, ilgas sveikas skaičius)**
>
> Psichologo konsultacijas teikia keli specialistai, dalis jų (n) formuoja
> terapines grupes iš k žmonių (terapijos trukmė GrT valandų), kiti (m) priima
> pacientus individualiai (Terapijos trukmė InT valandų). Kiekvienas naujai
> atėjęs pacientas (naujo paciento pasirodymo tikimybė p1, pakartotinai
> pacientai ateina po tam tikro laiko d) gali pasirinkti ar apsilankyti pas
> psichologą, teikiantį individualią paslaugą, ar pas grupines konsultacijas.
> Kol pas psichologą teikiantį grupinę konsultaciją susirenka grupė, į ją
> pretenduojantys pacientai turi laukti. Pakartotinai atėję pacientai pas savo
> psichologą gali rinktis ir mažesnėm grupėm, tačiau negali eiti kartu su
> naujokais. Yra pastebėta, kad individualiai priimami pacientai sveiksta
> greičiau, nei dirbantys grupėje (jiems reikia mažiau konsultacijų), tačiau
> pasitaiko išimčių — dėl socialinio bendravimo faktoriaus kai kurie pacientai
> (tikimybė p2) pasveiksta h procentų greičiau, nei konsultuojami
> individualiai. Individuali konsultacija kainuoja InK litų, grupinė GrK litų.
> Reikia įvertinti, kokį terapijos metodą turėtų pasirinkti pacientas
> optimizuodamas savo išteklius, bei kiek ir kokių specialistų turėtų samdyti
> sveikatos priežiūros įstaiga.

## Bendravimas ir bendradarbiavimas

API atskirta švariai, todėl skirtingi autoriai galėtų toliau dirbti
neperžengdami vienas kito ribų:

- `Deque.h`, `BigInt.h` — svetimi antraštiniai failai, neliečiami;
  simuliacijos kodas su jais bendrauja **tik per viešus prototipus**.
- `psyche.h` — simuliacijos pirminė sąsaja (`SimConfig`, `Simulation`,
  `simInit/Run/PrintStats/Free`). Visi vidiniai pagalbiniai metodai
  pažymėti `static` ir matomi tik `psyche.c`.
- `main.c` — vartotojo sąsaja (CLI, cfg). Nieko nežino apie
  konkrečių ADT vidų.

## Vykdymas

```sh
make                 # kompiliuoja main.exe (modulinis build: 4 .o failai)
./main.exe           # paleidžia su automatiškai surasta main.cfg
./main.exe -rnd 42   # pakeičia pseudo-atsitiktinių skaičių daviklį — atkartojamas rezultatas
./main.exe foo.cfg   # paduoda kitą konfigūracijos failą
```

Be argumentų programa randa savo .cfg failą per `argv[0]` keisdama
plėtinį į `.cfg` (todėl numatytasis cfg pavadintas `main.cfg`).
Pseudo-atsitiktinių skaičių daviklis visada spausdinamas pirmoje
eilutėje, kad rezultatą būtų galima atkartoti.

### Cfg formatas

Raktas-vertė poros po vieną eilutėje. Komentarai po `#`.

| Raktas | Reikšmė |
|---|---|
| `n` | grupinių specialistų skaičius |
| `k` | žmonių skaičius grupėje |
| `GrT` | grupinės sesijos trukmė (val.) |
| `m` | individualių specialistų skaičius |
| `InT` | individualios sesijos trukmė (val.) |
| `p1` | naujo paciento atvykimo tikimybė per valandą |
| `d` | pakartotinio vizito intervalas (val.) |
| `p2` | socialinio faktoriaus tikimybė |
| `h` | socialinio faktoriaus greitėjimas (%) |
| `SessIn` | baziniai indiv. seansai iki pasveikimo |
| `SessGr` | baziniai grup. seansai iki pasveikimo |
| `InK` | individualios konsultacijos kaina (lt) |
| `GrK` | grupinės konsultacijos kaina (lt) |
| `SimT` | simuliacijos trukmė (val.) |

## Pavyzdinis išvedimas

```
Pseudo-atsitiktinių skaičių daviklis: 42 (iš -rnd)
Cfg failas: ./main.cfg
Konfigūracija:
  ...
Vykdoma simuliacija...

--- REZULTATAI po 720 val. ---
Iš viso pacientų:        222
Pasveiko (indiv./grup.): 78 / 53
...
--- VIDUTINĖS IŠLAIDOS PASVEIKUSIEMS ---
Indiv. vid. išlaidos:    300 lt (n=78)
Grup. vid. išlaidos:     179 lt (n=53)
--- REKOMENDACIJOS ---
Pacientui: pigiau rinktis GRUPINĘ terapiją.
Įstaigai: personalas nepakankamai apkrautas — galima mažinti.
```
