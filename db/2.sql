-- dalyku pavadinimai ir ju temu pavadinimai
/*
SELECT
  A.pavadinimas AS pavadinimas,
  B.pavadinimas AS tema
FROM
  stud.dalykas AS A,
  stud.dalykas AS B
WHERE
  A.tema = B.id;
*/

-- SELF JOIN
/*
SELECT
  A.vardas,
  A.pavarde,
  B.vardas,
  B.pavarde
FROM
  stud.skaitytojas AS A,
  stud.skaitytojas AS B
WHERE
  A.adresas = B.adresas
  AND A.nr < B.nr;
*/

SELECT DISTINCT
  skt.nr,
  skt.vardas,
  skt.pavarde,
  skt.gimimo_data,
  egz.nr,
  egz.isbn
FROM
  stud.skaitytojas AS skt2,
  stud.skaitytojas AS skt
LEFT JOIN stud.skaitymas AS skm
  ON skt.nr = skm.skaitytojas
LEFT JOIN stud.egzempliorius AS egz
  ON skm.egzempliorius = egz.nr
WHERE
  EXTRACT(MONTH FROM skt.gimimo_data) = EXTRACT(MONTH FROM skt2.gimimo_data)
  AND skt.nr <> skt2.nr
ORDER BY 4, 1;

-- knygos be dalyku ir dalykai be knygu
/*
SELECT
  kng.pavadinimas AS knyga,
  dlk.pavadinimas AS dalykas
FROM
  stud.knyga AS kng
LEFT JOIN stud.dalykinimas AS dal
  ON kng.isbn = dal.knyga
FULL JOIN stud.dalykas AS dlk
  ON dal.dalykas = dlk.id
WHERE
  kng.pavadinimas    IS NULL
  OR dlk.pavadinimas IS NULL;
*/

-- Skaitytojai kurie netureje skaitymu
/*
SELECT
  skt.nr,
  skt.vardas,
  skt.pavarde
FROM
  stud.skaitytojas AS skt
LEFT JOIN stud.skaitymas AS skm
  ON skm.skaitytojas = skt.nr
WHERE
  skm.nr IS NULL;
*/

-- (2) Vardai ir pavardės visų skaitytojų, kurie skaito bent vieną konkretaus autoriaus, nurodyto vardu ir pavarde, knygą.
-- JOIN only
/*
SELECT DISTINCT skt.vardas, skt.pavarde
  FROM stud.skaitytojas AS skt
  JOIN stud.skaitymas AS skm
    ON (skt.nr=skm.skaitytojas AND skm.grazinta IS NULL)
  JOIN stud.egzempliorius AS e
    ON skm.egzempliorius=e.nr
  JOIN stud.knyga AS k
    ON e.isbn=k.isbn
  JOIN stud.autorius AS a
    ON (a.isbn=k.isbn AND a.vardas='Jonas' AND a.pavarde='Petraitis');
*/

-- (2) Vardai ir pavardės visų skaitytojų, kurie skaito bent vieną konkretaus autoriaus, nurodyto vardu ir pavarde, knygą.
-- WHERE only
/*
SELECT DISTINCT
  skt.vardas,
  skt.pavarde
FROM
  stud.skaitytojas   AS skt,
  stud.skaitymas     AS skm,
  stud.egzempliorius AS egz,
  stud.knyga         AS kng,
  stud.autorius      AS atr
WHERE
  skt.nr                 = skm.skaitytojas
  AND skm.grazinta       IS NULL
  AND skm.egzempliorius  = egz.nr
  AND egz.isbn           = kgn.isbn
  AND atr.isbn           = kgn.isbn
  AND LOWER(atr.vardas)  = 'jonas'
  AND LOWER(atr.pavarde) = 'petraitis';
*/
