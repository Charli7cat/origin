SELECT 
    title AS "Название трека",
    duration_seconds AS "Длительность (сек)",
    CONCAT(FLOOR(duration_seconds / 60), ':', LPAD(duration_seconds % 60, 2, '0')) AS "Длительность (мм:сс)"
FROM tracks
WHERE duration_seconds = (SELECT MAX(duration_seconds) FROM tracks);

SELECT 
    title AS "Название трека",
    CONCAT(FLOOR(duration_seconds / 60), ':', LPAD(duration_seconds % 60, 2, '0')) AS "Длительность (мм:сс)"
FROM tracks
WHERE duration_seconds >= 210
ORDER BY duration_seconds DESC;


SELECT 
    title AS "Название сборника",
    release_year AS "Год выпуска"
FROM collections
WHERE release_year BETWEEN 2018 AND 2020
ORDER BY release_year DESC;


SELECT 
    name AS "Имя исполнителя"
FROM artists
WHERE name NOT LIKE '% %'
ORDER BY name;

SELECT 
    title AS "Название трека",
    a.title AS "Альбом"
FROM tracks t
JOIN albums a ON t.album_id = a.id
WHERE LOWER(t.title) LIKE '%my%' 
   OR LOWER(t.title) LIKE '%мой%'
ORDER BY t.title;