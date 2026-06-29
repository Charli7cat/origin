CREATE TABLE IF NOT EXISTS publisher (
    id SERIAL PRIMARY KEY,
    name VARCHAR(255) NOT NULL
);

CREATE TABLE IF NOT EXISTS book (
    id SERIAL PRIMARY KEY,
    title VARCHAR(255) NOT NULL,
    price DECIMAL(10,2) NOT NULL,
    count INTEGER NOT NULL DEFAULT 0,
    id_publisher INTEGER REFERENCES publisher(id) ON DELETE CASCADE
);

CREATE TABLE IF NOT EXISTS shop (
    id SERIAL PRIMARY KEY,
    name VARCHAR(255) NOT NULL
);

CREATE TABLE IF NOT EXISTS stock (
    id SERIAL PRIMARY KEY,
    count INTEGER NOT NULL DEFAULT 0,
    id_book INTEGER REFERENCES book(id) ON DELETE CASCADE,
    id_shop INTEGER REFERENCES shop(id) ON DELETE CASCADE,
    UNIQUE(id_book, id_shop)
);

CREATE TABLE IF NOT EXISTS sale (
    id SERIAL PRIMARY KEY,
    date_sale TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    count INTEGER NOT NULL,
    id_stock INTEGER REFERENCES stock(id) ON DELETE CASCADE,
    id_shop INTEGER REFERENCES shop(id) ON DELETE CASCADE
);

-- Создание индексов для ускорения
CREATE INDEX idx_book_publisher ON book(id_publisher);
CREATE INDEX idx_stock_book ON stock(id_book);
CREATE INDEX idx_stock_shop ON stock(id_shop);
CREATE INDEX idx_sale_stock ON sale(id_stock);
CREATE INDEX idx_sale_shop ON sale(id_shop);