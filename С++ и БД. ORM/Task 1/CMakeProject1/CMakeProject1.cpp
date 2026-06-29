#include "BookShop.h"
#include <iostream>
#include <memory>

using namespace std;

void demonstrateORM() {
    try {
        std::string connString = "host=localhost port=5432 dbname=bookshop user=postgres password=postgres";
        DatabaseManager dbManager(connString);
        auto& session = dbManager.getSession();

        std::cout << "Создание таблиц..." << std::endl;
        dbManager.createTables();

        dbo::Transaction transaction(session);

        auto publisher = session.addNew<Publisher>();
        publisher->name = "Издательство АСТ";

        auto book = session.addNew<Book>();
        book->title = "Война и мир";
        book->price = 599.99;
        book->count = 100;
        book->publisher = publisher;

        auto shop = session.addNew<Shop>();
        shop->name = "Книжный мир";

        auto stock = session.addNew<Stock>();
        stock->count = 50;
        stock->book = book;
        stock->shop = shop;

        auto sale = session.addNew<Sale>();
        sale->dateSale = Wt::WDateTime::currentDateTime();
        sale->count = 2;
        sale->stock = stock;
        sale->shop = shop;

        transaction.commit();
        std::cout << "Данные успешно добавлены!" << std::endl;


        std::cout << "\n=== Книги и издательства ===" << std::endl;
        dbo::Transaction t1(session);
        auto books = session.find<Book>();
        for (auto& b : books) {
            std::cout << "Книга: " << b->title
                << ", Издатель: " << b->publisher->name
                << ", Цена: " << b->price
                << ", Кол-во: " << b->count << std::endl;
        }
        t1.commit();

        std::cout << "\n=== Книги в магазинах ===" << std::endl;
        dbo::Transaction t2(session);
        auto stocks = session.find<Stock>();
        for (auto& s : stocks) {
            std::cout << "Книга: " << s->book->title
                << ", Магазин: " << s->shop->name
                << ", В наличии: " << s->count << std::endl;
        }
        t2.commit();

        std::cout << "\n=== Продажи ===" << std::endl;
        dbo::Transaction t3(session);
        auto sales = session.find<Sale>();
        for (auto& s : sales) {
            std::cout << "Дата: " << s->dateSale.toString()
                << ", Книга: " << s->stock->book->title
                << ", Магазин: " << s->shop->name
                << ", Продано: " << s->count << std::endl;
        }
        t3.commit();

        std::cout << "\n=== Поиск книг с 'Война' ===" << std::endl;
        dbo::Transaction t4(session);
        auto foundBooks = session.find<Book>()
            .where("title LIKE ?").bind("%Война%");
        for (auto& b : foundBooks) {
            std::cout << "Найдена книга: " << b->title << std::endl;
        }
        t4.commit();

        std::cout << "\n=== Обновление цены книги ===" << std::endl;
        dbo::Transaction t5(session);
        auto bookToUpdate = session.find<Book>()
            .where("title = ?").bind("Война и мир")
            .limit(1);
        if (!bookToUpdate.empty()) {
            bookToUpdate.front()->price = 699.99;
            std::cout << "Цена обновлена!" << std::endl;
        }
        t5.commit();

        std::cout << "\n=== Удаление издательства ===" << std::endl;
        dbo::Transaction t6(session);
        auto publisherToDelete = session.find<Publisher>()
            .where("name = ?").bind("Издательство АСТ")
            .limit(1);
        if (!publisherToDelete.empty()) {
            publisherToDelete.front().remove();
            std::cout << "Издательство удалено (книги удалены каскадно)" << std::endl;
        }
        t6.commit();

    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
    }
}

int main() {
    demonstrateORM();
    return 0;
}