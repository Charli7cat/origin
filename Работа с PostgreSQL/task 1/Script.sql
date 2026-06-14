CREATE TABLE genres (
    id SERIAL PRIMARY KEY,
    name VARCHAR(100) NOT NULL UNIQUE
);

CREATE TABLE artists (
    id SERIAL PRIMARY KEY,
    name VARCHAR(200) NOT NULL UNIQUE
);

CREATE TABLE artist_genres (
    artist_id INTEGER NOT NULL,
    genre_id INTEGER NOT NULL,
    PRIMARY KEY (artist_id, genre_id),
    FOREIGN KEY (artist_id) REFERENCES artists(id) ON DELETE CASCADE,
    FOREIGN KEY (genre_id) REFERENCES genres(id) ON DELETE CASCADE
);

CREATE TABLE albums (
    id SERIAL PRIMARY KEY,
    title VARCHAR(200) NOT NULL,
    release_year INTEGER CHECK (release_year >= 1900 AND release_year <= 2100)
);

CREATE TABLE album_artists (
    album_id INTEGER NOT NULL,
    artist_id INTEGER NOT NULL,
    PRIMARY KEY (album_id, artist_id),
    FOREIGN KEY (album_id) REFERENCES albums(id) ON DELETE CASCADE,
    FOREIGN KEY (artist_id) REFERENCES artists(id) ON DELETE CASCADE
);

CREATE TABLE tracks (
    id SERIAL PRIMARY KEY,
    title VARCHAR(200) NOT NULL,
    duration INTEGER NOT NULL CHECK (duration > 0),
    album_id INTEGER NOT NULL,
    FOREIGN KEY (album_id) REFERENCES albums(id) ON DELETE CASCADE
);

CREATE TABLE collections (
    id SERIAL PRIMARY KEY,
    title VARCHAR(200) NOT NULL,
    release_year INTEGER CHECK (release_year >= 1900 AND release_year <= 2100)
);

CREATE TABLE collection_tracks (
    collection_id INTEGER NOT NULL,
    track_id INTEGER NOT NULL,
    PRIMARY KEY (collection_id, track_id),
    FOREIGN KEY (collection_id) REFERENCES collections(id) ON DELETE CASCADE,
    FOREIGN KEY (track_id) REFERENCES tracks(id) ON DELETE CASCADE
);

CREATE INDEX idx_tracks_album_id ON tracks(album_id);
CREATE INDEX idx_album_artists_album_id ON album_artists(album_id);
CREATE INDEX idx_album_artists_artist_id ON album_artists(artist_id);
CREATE INDEX idx_artist_genres_artist_id ON artist_genres(artist_id);
CREATE INDEX idx_artist_genres_genre_id ON artist_genres(genre_id);
CREATE INDEX idx_collection_tracks_collection_id ON collection_tracks(collection_id);
CREATE INDEX idx_collection_tracks_track_id ON collection_tracks(track_id);

SELECT table_name 
FROM information_schema.tables 
WHERE table_schema = 'public' 
ORDER BY table_name;

