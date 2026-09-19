SELECT SUBSTRING(pavarde, 1, 1) FROM stud.skaitytojas;

SELECT DISTINCT s.vardas, s.pavarde, k.isbn, k.pavadinimas
  FROM stud.skaitytojas AS s
  JOIN stud.skaitymas AS s2
    ON (s.nr=s2.skaitytojas AND s2.grazinta IS NULL)
  JOIN stud.egzempliorius AS e
    ON s2.egzempliorius=e.nr
  JOIN stud.knyga AS k
    ON e.isbn=k.isbn

-- Vardai ir pavardės visų skaitytojų, kurie skaito bent vieną konkretaus autoriaus, nurodyto vardu ir pavarde, knygą.

SELECT DISTINCT skt.vardas, skt.pavarde
  FROM stud.skaitytojas AS skt
  JOIN stud.skaitymas AS skm
    ON (skt.nr=skm.skaitytojas AND skm.grazinta IS NULL)
  JOIN stud.egzempliorius AS e
    ON skm.egzempliorius=e.nr
  JOIN stud.knyga AS k
    ON e.isbn=k.isbn
  WHERE k.isbn
    IN (SELECT isbn FROM stud.autorius WHERE vardas='Jonas' AND pavarde='Petraitis');

SELECT isbn, gavimo_data
  FROM stud.egzempliorius
  WHERE gavimo_data > to_date('2020', 'YYYY');

SELECT pavadinimas, to_char(verte, '99999999D9999') AS verte
  FROM stud.knyga
  WHERE verte > to_number('20.2', '99D9');

SELECT pavadinimas, verte
  FROM stud.knyga
  WHERE NULLIF(verte, to_number('19.9', '99D9')) IS NOT NULL;

SELECT vardas, pavarde,
  COUNT(CASE WHEN skm.grazinta IS NULL
        THEN skm.nr END) AS "grazinti skaitymai",
  COUNT(CASE WHEN skm.grazinta IS NOT NULL
        THEN skm.nr END) AS "esami skaitymai"
  FROM stud.skaitytojas AS skt, stud.skaitymas AS skm
  WHERE skt.nr=skm.skaitytojas
  GROUP BY skt.nr;

SELECT vardas, pavarde, adresas
  FROM stud.skaitytojas
  WHERE vardas IN ('Jonas', 'Petras') AND (adresas LIKE '%Lenktoji%' OR adresas LIKE '%Mazoji%');

SELECT vardas, pavarde,
  COALESCE(adresas, 'BENAMIS') AS adresas
  FROM stud.skaitytojas;

SELECT A.vardas AS "A vardas", A.pavarde AS "A pavarde",
  B.vardas AS "B vardas", B.pavarde
  FROM stud.skaitytojas AS A, stud.skaitytojas AS B
  WHERE A.nr < B.nr
  ORDER BY A.nr;
