#include <Wt/Dbo/Dbo.h>
#include <Wt/Dbo/backend/Postgres.h>
#include <iostream>
#include <string>
#include <memory>

namespace dbo = Wt::Dbo;

class Publisher;
class Book;
class Store;
class Stock;

class Publisher {
public:
    std::string name;
    std::string country;
    int year_founded;

    dbo::collection<dbo::ptr<Book>> books;

    template<class Action>
    void persist(Action& a) {
        dbo::field(a, name, "name");
        dbo::field(a, country, "country");
        dbo::field(a, year_founded, "year_founded");
        dbo::hasMany(a, books, dbo::ManyToOne, "publisher");
    }
};

class Book {
public:
    std::string title;
    std::string isbn;
    int year;
    double price;

    dbo::ptr<Publisher> publisher;
    dbo::collection<dbo::ptr<Stock>> stocks;

    template<class Action>
    void persist(Action& a) {
        dbo::field(a, title, "title");
        dbo::field(a, isbn, "isbn");
        dbo::field(a, year, "year");
        dbo::field(a, price, "price");
        dbo::belongsTo(a, publisher, "publisher");
        dbo::hasMany(a, stocks, dbo::ManyToOne, "book");
    }
};

class Store {
public:
    std::string name;
    std::string address;
    std::string phone;

    dbo::collection<dbo::ptr<Stock>> stocks;

    template<class Action>
    void persist(Action& a) {
        dbo::field(a, name, "name");
        dbo::field(a, address, "address");
        dbo::field(a, phone, "phone");
        dbo::hasMany(a, stocks, dbo::ManyToOne, "store");
    }
};

class Stock {
public:
    int quantity;

    dbo::ptr<Book> book;
    dbo::ptr<Store> store;

    template<class Action>
    void persist(Action& a) {
        dbo::field(a, quantity, "quantity");
        dbo::belongsTo(a, book, "book");
        dbo::belongsTo(a, store, "store");
    }
};

int main() {
    try {
        std::unique_ptr<dbo::backend::Postgres> connection(new dbo::backend::Postgres(
            "host=localhost "
            "port=5432 "
            "dbname=bookstore_db "
            "user=postgres "
            "password=password"
        ));

        dbo::Session session;
        session.setConnection(std::move(connection));

        session.mapClass<Publisher>("publishers");
        session.mapClass<Book>("books");
        session.mapClass<Store>("stores");
        session.mapClass<Stock>("stocks");

        session.createTables();

        std::cout << "Таблицы успешно созданы!" << std::endl;

        {
            dbo::Transaction transaction(session);

            auto publisher1 = session.addNew<Publisher>();
            publisher1.modify()->name = "Penguin Random House";
            publisher1.modify()->country = "США";
            publisher1.modify()->year_founded = 1927;

            auto publisher2 = session.addNew<Publisher>();
            publisher2.modify()->name = "HarperCollins";
            publisher2.modify()->country = "Великобритания";
            publisher2.modify()->year_founded = 1989;

            auto publisher3 = session.addNew<Publisher>();
            publisher3.modify()->name = "Simon & Schuster";
            publisher3.modify()->country = "США";
            publisher3.modify()->year_founded = 1924;

            auto book1 = session.addNew<Book>();
            book1.modify()->title = "Великий Гэтсби";
            book1.modify()->isbn = "978-0-7432-7356-5";
            book1.modify()->year = 1925;
            book1.modify()->price = 15.99;
            book1.modify()->publisher = publisher1;

            auto book2 = session.addNew<Book>();
            book2.modify()->title = "1984";
            book2.modify()->isbn = "978-0-452-28423-4";
            book2.modify()->year = 1949;
            book2.modify()->price = 12.99;
            book2.modify()->publisher = publisher1;

            auto book3 = session.addNew<Book>();
            book3.modify()->title = "Убить пересмешника";
            book3.modify()->isbn = "978-0-06-112008-4";
            book3.modify()->year = 1960;
            book3.modify()->price = 14.99;
            book3.modify()->publisher = publisher2;

            auto book4 = session.addNew<Book>();
            book4.modify()->title = "Над пропастью во ржи";
            book4.modify()->isbn = "978-0-316-76948-0";
            book4.modify()->year = 1951;
            book4.modify()->price = 11.99;
            book4.modify()->publisher = publisher3;

            auto book5 = session.addNew<Book>();
            book5.modify()->title = "Скотный двор";
            book5.modify()->isbn = "978-0-452-28424-1";
            book5.modify()->year = 1945;
            book5.modify()->price = 9.99;
            book5.modify()->publisher = publisher1;

            auto store1 = session.addNew<Store>();
            store1.modify()->name = "Центральный книжный";
            store1.modify()->address = "ул. Ленина, 15, Москва";
            store1.modify()->phone = "+7-495-123-45-67";

            auto store2 = session.addNew<Store>();
            store2.modify()->name = "Книжный рай";
            store2.modify()->address = "пр. Невский, 45, Санкт-Петербург";
            store2.modify()->phone = "+7-812-234-56-78";

            auto store3 = session.addNew<Store>();
            store3.modify()->name = "Мир книг";
            store3.modify()->address = "ул. Гоголя, 22, Казань";
            store3.modify()->phone = "+7-843-345-67-89";

            auto store4 = session.addNew<Store>();
            store4.modify()->name = "Литературный салон";
            store4.modify()->address = "пр. Победы, 56, Новосибирск";
            store4.modify()->phone = "+7-383-456-78-90";

            auto stock1 = session.addNew<Stock>();
            stock1.modify()->book = book1;
            stock1.modify()->store = store1;
            stock1.modify()->quantity = 10;

            auto stock2 = session.addNew<Stock>();
            stock2.modify()->book = book1;
            stock2.modify()->store = store2;
            stock2.modify()->quantity = 5;

            auto stock3 = session.addNew<Stock>();
            stock3.modify()->book = book2;
            stock3.modify()->store = store1;
            stock3.modify()->quantity = 8;

            auto stock4 = session.addNew<Stock>();
            stock4.modify()->book = book2;
            stock4.modify()->store = store4;
            stock4.modify()->quantity = 3;

            auto stock5 = session.addNew<Stock>();
            stock5.modify()->book = book5;
            stock5.modify()->store = store2;
            stock5.modify()->quantity = 6;

            auto stock6 = session.addNew<Stock>();
            stock6.modify()->book = book5;
            stock6.modify()->store = store4;
            stock6.modify()->quantity = 4;

            auto stock7 = session.addNew<Stock>();
            stock7.modify()->book = book3;
            stock7.modify()->store = store1;
            stock7.modify()->quantity = 7;

            auto stock8 = session.addNew<Stock>();
            stock8.modify()->book = book3;
            stock8.modify()->store = store3;
            stock8.modify()->quantity = 9;

            auto stock9 = session.addNew<Stock>();
            stock9.modify()->book = book4;
            stock9.modify()->store = store2;
            stock9.modify()->quantity = 2;

            auto stock10 = session.addNew<Stock>();
            stock10.modify()->book = book4;
            stock10.modify()->store = store3;
            stock10.modify()->quantity = 5;

            auto stock11 = session.addNew<Stock>();
            stock11.modify()->book = book4;
            stock11.modify()->store = store4;
            stock11.modify()->quantity = 1;

            transaction.commit();
            std::cout << "Тестовые данные успешно добавлены!" << std::endl;
        }

        std::string ввод;
        std::cout << "\nВведите имя или ID издателя: ";
        std::getline(std::cin, ввод);

        {
            dbo::Transaction transaction(session);

            dbo::ptr<Publisher> издатель;

            bool являетсяЧислом = true;
            for (char c : ввод) {
                if (!std::isdigit(c)) {
                    являетсяЧислом = false;
                    break;
                }
            }

            if (являетсяЧислом) {
                int id = std::stoi(ввод);
                издатель = session.find<Publisher>().where("id = ?").bind(id);
            }
            else {
                издатель = session.find<Publisher>().where("name = ?").bind(ввод);
            }

            if (!издатель) {
                std::cout << "Издатель не найден!" << std::endl;
                return 0;
            }

            std::cout << "\n=== Информация об издателе ===" << std::endl;
            std::cout << "ID: " << издатель.id() << std::endl;
            std::cout << "Название: " << издатель->name << std::endl;
            std::cout << "Страна: " << издатель->country << std::endl;
            std::cout << "Год основания: " << издатель->year_founded << std::endl;

            std::cout << "\n=== Магазины, продающие книги этого издателя ===" << std::endl;

            dbo::collection<dbo::ptr<Book>> книги = издатель->books;

            if (книги.empty()) {
                std::cout << "У этого издателя нет книг." << std::endl;
            }
            else {
                for (const auto& книга : книги) {
                    std::cout << "\nКнига: " << книга->title << " (ISBN: " << книга->isbn << ")" << std::endl;

                    dbo::collection<dbo::ptr<Stock>> запасы = книга->stocks;

                    if (запасы.empty()) {
                        std::cout << "  Нет в наличии ни в одном магазине." << std::endl;
                    }
                    else {
                        for (const auto& запас : запасы) {
                            std::cout << "  - Магазин: " << запас->store->name
                                << " (Адрес: " << запас->store->address
                                << ", Телефон: " << запас->store->phone
                                << ", Количество: " << запас->quantity << ")" << std::endl;
                        }
                    }
                }

                std::cout << "\n=== Уникальные магазины (с использованием JOIN) ===" << std::endl;

                dbo::Query<dbo::ptr<Store>> запросМагазинов = session.query<dbo::ptr<Store>>(
                    "SELECT DISTINCT s FROM Store s "
                    "JOIN s.stocks st "
                    "JOIN st.book b "
                    "WHERE b.publisher = ?"
                ).bind(издатель);

                dbo::collection<dbo::ptr<Store>> магазины = запросМагазинов;

                if (магазины.empty()) {
                    std::cout << "Магазины не найдены." << std::endl;
                }
                else {
                    int счетчик = 1;
                    for (const auto& магазин : магазины) {
                        std::cout << счетчик++ << ". " << магазин->name
                            << " - " << магазин->address
                            << " (" << магазин->phone << ")" << std::endl;
                    }
                }
            }

            transaction.commit();
        }

    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}