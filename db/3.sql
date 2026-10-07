-- 3. Kiekvienai datai, kada turi būti grąžintas bent vienas egzempliorius, visų
-- grąžintinų egzempliorių skaičius ir visų šių egzempliorių skaitytojų amžiaus
-- vidurkis.
/* 
SELECT
  skm.grazinti AS data,
  COUNT(egz.nr),
  AVG(EXTRACT(YEAR FROM AGE(skt.gimimo_data)))
FROM
  stud.skaitymas     AS skm,
  stud.egzempliorius AS egz,
  stud.skaitytojas   AS skt
WHERE
  skm.egzempliorius   = egz.nr
  AND skm.skaitytojas = skt.nr
  AND skm.grazinta    IS NULL
GROUP BY
  skm.grazinti
ORDER BY 1;
 */

-- skaitytojai, kurie velavo arba veluoja grazinti skaitini, ir velavimu skaicius
/* 
SELECT
  skt.nr,
  skt.pavarde,
  COUNT(*)
FROM
  stud.skaitymas     AS skm,
  stud.skaitytojas   AS skt
WHERE
  skm.skaitytojas = skt.nr
  AND ((skm.grazinta IS NOT NULL AND skm.grazinta > skm.grazinti)
    OR (skm.grazinta IS NULL AND CURRENT_DATE > skm.grazinti))
GROUP BY
  skt.nr
ORDER BY skt.nr;
 */

-- niekada neskaitytos knygos
/* 
SELECT
  kng.isbn,
  kng.pavadinimas
FROM
  stud.knyga AS kng
LEFT JOIN stud.egzempliorius AS egz
  ON kng.isbn = egz.isbn
LEFT JOIN stud.skaitymas     AS skm
  ON egz.nr   = skm.egzempliorius
GROUP BY kng.isbn
  HAVING COUNT(skm.nr) = 0
ORDER BY 1;
 */

-- knygu skaitymu kiekiai
/* 
SELECT
  kng.isbn,
  kng.pavadinimas,
  COUNT(skm.nr) AS "skaitymu skaicius"
FROM
  stud.knyga AS kng
LEFT JOIN stud.egzempliorius AS egz
  ON kng.isbn = egz.isbn
LEFT JOIN stud.skaitymas     AS skm
  ON egz.nr   = skm.egzempliorius
GROUP BY kng.isbn
ORDER BY 1;
 */


SELECT
  EXTRACT(YEAR FROM skm.paimta) AS Metai,
  EXTRACT(MONTH FROM skm.paimta) AS Menesis,
  COUNT(skm.nr) AS Skaicius
FROM
  stud.skaitymas AS skm
GROUP BY ROLLUP(Metai, Menesis)
ORDER BY 1, 2;


/* 
SELECT
  EXTRACT(YEAR FROM skt.gimimo_data) AS "Gimimo metai",
  kng.leidykla AS "Leidykla",
  COUNT(DISTINCT kng.isbn) AS "Skaitymu skaicius"
FROM
  stud.skaitymas AS skm
JOIN stud.skaitytojas AS skt
  ON skm.skaitytojas = skt.nr
JOIN stud.egzempliorius AS egz
  ON skm.egzempliorius = egz.nr
JOIN stud.knyga AS kng
  ON egz.isbn = kng.isbn
GROUP BY CUBE(1, 2);
 */
