/*
WITH vel(nr) AS (
    SELECT DISTINCT
        skaitytojas
    FROM
        stud.skaitymas AS skm
    WHERE
        skm.grazinta IS NULL
        OR skm.grazinta < CURRENT_DATE
)
SELECT
    vardas,
    pavarde
FROM
    stud.skaitytojas AS skt,
    vel
WHERE
    skt.nr = vel.nr
ORDER BY 1, 2;
*/

/* 
WITH RECURSIVE dal(id, pavadinimas, lygmuo) AS (
    SELECT id, pavadinimas, 1
    FROM stud.dalykas
    WHERE tema IS NULL
    UNION
    SELECT A.id, A.pavadinimas, B.lygmuo + 1
    FROM stud.dalykas AS A, dal AS B
    WHERE A.tema = B.id AND A.id <> B.id
)
SELECT id, pavadinimas, lygmuo FROM dal
ORDER BY 3;
 */

/* 
WITH skm(n) AS (
    SELECT COUNT(*)
    FROM stud.skaitymas
    GROUP BY skaitytojas
)
SELECT AVG(n) FROM skm;
 */

/* 
WITH vidurkiai AS (
    SELECT COUNT(*) FROM stud.egzempliorius
    GROUP BY gavimo_data
),
temp AS (
    SELECT AVG(count) AS vidurkis FROM vidurkiai
)
SELECT
    EXTRACT(YEAR FROM gavimo_data) AS metai,
    EXTRACT(MONTH FROM gavimo_data) AS menesis,
    COUNT(*) AS kiekis
FROM
    stud.egzempliorius,
    temp
GROUP BY metai, menesis, vidurkis
HAVING COUNT(*) < vidurkis
ORDER BY 1, 2;
 */

/* 
WITH egzemplioriai AS (
    SELECT kng.metai, kng.isbn, COUNT(egz.nr) AS kiekis
    FROM stud.knyga AS kng
    LEFT JOIN stud.egzempliorius AS egz
        ON egz.isbn = kng.isbn
    GROUP BY kng.isbn
)
SELECT * FROM egzemplioriai AS e1
WHERE e1.kiekis <= ALL (SELECT kiekis FROM egzemplioriai WHERE metai = e1.metai)
ORDER BY 1;
 */

/* 
WITH egzemplioriai AS (
    SELECT kng.metai, kng.isbn, COUNT(egz.nr) AS kiekis
    FROM stud.knyga AS kng
    LEFT JOIN stud.egzempliorius AS egz
        ON egz.isbn = kng.isbn
    GROUP BY kng.isbn
), minimumai AS (
    SELECT metai, MIN(kiekis) AS minimumas
    FROM egzemplioriai
    GROUP BY metai
)
SELECT e1.metai, e1.isbn, e1.kiekis FROM egzemplioriai AS e1, minimumai AS min
WHERE e1.metai = min.metai AND e1.kiekis = min.minimumas
ORDER BY 1;
 */

/* 
WITH Skaitytos(isbn, pavadinimas) AS (
    SELECT DISTINCT
        kng.isbn,
        kng.pavadinimas
    FROM
        stud.skaitymas AS skm
    JOIN stud.egzempliorius AS egz
        ON skm.egzempliorius = egz.nr
    JOIN stud.knyga AS kng
        ON egz.isbn = kng.isbn
)
SELECT isbn, pavadinimas FROM stud.knyga
EXCEPT
SELECT isbn, pavadinimas FROM Skaitytos
ORDER BY 1;
 */
