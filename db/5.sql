/* 
SELECT v.table_schema,
       v.table_name,
       COUNT(DISTINCT c.data_type) AS column_type_count
FROM information_schema.views AS v
JOIN information_schema.columns AS c
  ON c.table_schema = v.table_schema
     AND c.table_name = v.table_name
GROUP BY v.table_schema, v.table_name
ORDER BY v.table_schema, v.table_name;
 */
-- privileges
-- pk ar ik

/* 
SELECT DISTINCT table_schema, table_name FROM information_schema.key_column_usage
WHERE table_schema='stud' AND position_in_unique_constraint IS NOT NULL;

SELECT DISTINCT table_schema, table_name FROM information_schema.key_column_usage
WHERE table_schema='stud' AND position_in_unique_constraint IS NULL;

SELECT table_schema, table_name FROM information_schema.key_column_usage
WHERE table_schema='stud'
GROUP BY 1, 2
  HAVING COUNT(position_in_unique_constraint) = 0;
 */

/* 
SELECT t.table_schema,
       t.table_name,
       CASE WHEN  THEN 'yes' ELSE 'no' END AS has_pk,
       CASE WHEN  THEN 'yes' ELSE 'no' END AS has_fk
FROM information_schema.tables AS t
JOIN information_schema.key_column_usage AS k
  ON k.table_schema = t.table_schema AND k.table_name = t.table_name
GROUP BY 1, 2
 */

SELECT t.table_schema,
       t.table_name,
       CASE WHEN EXISTS(SELECT * FROM information_schema.key_column_usage
                        WHERE position_in_unique_constraint IS NULL
                          AND table_schema=t.table_schema AND table_name=t.table_name
       ) THEN 'yes' ELSE 'no' END AS has_pk,
       CASE WHEN EXISTS(SELECT * FROM information_schema.key_column_usage
                        WHERE position_in_unique_constraint IS NOT NULL
                          AND table_schema=t.table_schema AND table_name=t.table_name
       ) THEN 'yes' ELSE 'no' END AS has_fk
FROM information_schema.tables AS t
