#ifndef BOOK_SHOP_H
#define BOOK_SHOP_H

#include <Wt/Dbo/Dbo.h>
#include <Wt/Dbo/backend/Postgres.h>
#include <string>
#include <vector>

namespace dbo = Wt::Dbo;

class Publisher;
class Book;
class Shop;
class Stock;
class Sale;

class Publisher {
public:
    int id;
    std::string name;

    dbo::collection<dbo::ptr<Book>> books;

    template<class Action>
    void persist(Action& a) {
        dbo::id(id, "id", dbo::AutoIncrement);
        dbo::field(a, name, "name");
        dbo::hasMany(a, books, dbo::ManyToOne, "publisher");
    }
};

class Book {
public:
    int id;
    std::string title;
    double price;
    int count;

    dbo::ptr<Publisher> publisher;

    dbo::collection<dbo::ptr<Stock>> stocks;

    template<class Action>
    void persist(Action& a) {
        dbo::id(id, "id", dbo::AutoIncrement);
        dbo::field(a, title, "title");
        dbo::field(a, price, "price");
        dbo::field(a, count, "count");

        dbo::belongsTo(a, publisher, "publisher", dbo::OnDeleteCascade);

        dbo::hasMany(a, stocks, dbo::ManyToOne, "book");
    }
};

class Shop {
public:
    int id;
    std::string name;

    dbo::collection<dbo::ptr<Stock>> stocks;

    dbo::collection<dbo::ptr<Sale>> sales;

    template<class Action>
    void persist(Action& a) {
        dbo::id(id, "id", dbo::AutoIncrement);
        dbo::field(a, name, "name");

        dbo::hasMany(a, stocks, dbo::ManyToOne, "shop");

        dbo::hasMany(a, sales, dbo::ManyToOne, "shop");
    }
};

class Stock {
public:
    int id;
    int count;

    dbo::ptr<Book> book;

    dbo::ptr<Shop> shop;

    dbo::collection<dbo::ptr<Sale>> sales;

    template<class Action>
    void persist(Action& a) {
        dbo::id(id, "id", dbo::AutoIncrement);
        dbo::field(a, count, "count");

        dbo::belongsTo(a, book, "book", dbo::OnDeleteCascade);

        dbo::belongsTo(a, shop, "shop", dbo::OnDeleteCascade);

        dbo::hasMany(a, sales, dbo::ManyToOne, "stock");
    }
};

class Sale {
public:
    int id;
    Wt::WDateTime dateSale;
    int count;

    dbo::ptr<Stock> stock;

    dbo::ptr<Shop> shop;

    template<class Action>
    void persist(Action& a) {
        dbo::id(id, "id", dbo::AutoIncrement);
        dbo::field(a, dateSale, "date_sale");
        dbo::field(a, count, "count");

        dbo::belongsTo(a, stock, "stock", dbo::OnDeleteCascade);

        dbo::belongsTo(a, shop, "shop", dbo::OnDeleteCascade);
    }
};

class DatabaseManager {
private:
    dbo::backend::Postgres connection;
    dbo::Session session;

public:
    DatabaseManager(const std::string& connectionString)
        : connection(connectionString) {
        session.setConnection(connection);
        mapClasses();
    }

    void mapClasses() {
        session.mapClass<Publisher>("publisher");
        session.mapClass<Book>("book");
        session.mapClass<Shop>("shop");
        session.mapClass<Stock>("stock");
        session.mapClass<Sale>("sale");
    }

    dbo::Session& getSession() {
        return session;
    }

    void createTables() {
        session.createTables();
    }

    void dropTables() {
        session.dropTables();
    }
};

#endif