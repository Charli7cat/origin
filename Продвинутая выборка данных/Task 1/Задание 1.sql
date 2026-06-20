INSERT INTO artists (name) VALUES 
('The Weeknd'),
('Dua Lipa'),
('Ed Sheeran'),
('Billie Eilish');

INSERT INTO genres (name) VALUES 
('Pop'),
('R&B'),
('Alternative'),
('Dance');

INSERT INTO artist_genre (artist_id, genre_id) VALUES 
(1, 2), (1, 1),
(2, 1), (2, 4),
(3, 1),
(4, 3), (4, 1);

INSERT INTO albums (title, release_year) VALUES 
('After Hours', 2020),
('Future Nostalgia', 2020),
('÷ (Divide)', 2017),
('Happier Than Ever', 2021);

INSERT INTO artist_album (artist_id, album_id) VALUES 
(1, 1), -- The Weeknd - After Hours
(2, 2), -- Dua Lipa - Future Nostalgia
(3, 3), -- Ed Sheeran - ÷ (Divide)
(4, 4); -- Billie Eilish - Happier Than Ever

INSERT INTO tracks (album_id, title, duration_seconds) VALUES 
(1, 'Blinding Lights', 200),
(1, 'Save Your Tears', 215),
(2, 'Don''t Start Now', 183),
(2, 'Physical', 193),
(3, 'Shape of You', 233),
(4, 'Therefore I Am', 174),
(4, 'Happier Than Ever', 298);

INSERT INTO collections (title, release_year) VALUES 
('Pop Hits 2020', 2020),
('Dance Party', 2021),
('Best of 2020s', 2022),
('Global Superstars', 2021);

INSERT INTO collection_track (collection_id, track_id) VALUES 
(1, 1), (1, 3), (1, 4),
(2, 1), (2, 2), (2, 4),
(3, 1), (3, 2), (3, 3), (3, 4), (3, 6),
(4, 1), (4, 3), (4, 5), (4, 7);
