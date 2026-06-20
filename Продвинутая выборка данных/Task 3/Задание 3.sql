SELECT 
    g.name AS "Жанр",
    COUNT(ag.artist_id) AS "Количество исполнителей"
FROM genres g
LEFT JOIN artist_genre ag ON g.id = ag.genre_id
GROUP BY g.id, g.name
ORDER BY COUNT(ag.artist_id) DESC;


SELECT 
    COUNT(t.id) AS "Количество треков"
FROM tracks t
JOIN albums a ON t.album_id = a.id
WHERE a.release_year BETWEEN 2019 AND 2020;


SELECT 
    a.title AS "Альбом",
    a.release_year AS "Год выпуска",
    COUNT(t.id) AS "Количество треков",
    ROUND(AVG(t.duration_seconds), 2) AS "Средняя длительность (сек)",
    CONCAT(
        FLOOR(AVG(t.duration_seconds) / 60), 
        ':', 
        LPAD(ROUND(AVG(t.duration_seconds) % 60)::TEXT, 2, '0')
    ) AS "Средняя длительность (мм:сс)"
FROM albums a
LEFT JOIN tracks t ON a.id = t.album_id
GROUP BY a.id, a.title, a.release_year
HAVING COUNT(t.id) > 0
ORDER BY AVG(t.duration_seconds) DESC;


SELECT DISTINCT 
    ar.name AS "Исполнитель"
FROM artists ar
LEFT JOIN artist_album aa ON ar.id = aa.artist_id
LEFT JOIN albums a ON aa.album_id = a.id AND a.release_year = 2020
WHERE a.id IS NULL
ORDER BY ar.name;


SELECT DISTINCT
    c.title AS "Название сборника",
    c.release_year AS "Год выпуска"
FROM collections c
JOIN collection_track ct ON c.id = ct.collection_id
JOIN tracks t ON ct.track_id = t.id
JOIN albums a ON t.album_id = a.id
JOIN artist_album aa ON a.id = aa.album_id
WHERE aa.artist_id = 1  -- ID исполнителя
ORDER BY c.release_year DESC, c.title;