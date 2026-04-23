#define CATCH_CONFIG_MAIN

#include "catch_amalgamated.hpp"

#include "list.h"

TEST_CASE("Тестирование метода Empty()", "[Empty]") {
    SECTION("Новый список пустой") {
        List lst;
        REQUIRE(lst.Empty() == true);
    }
    
    SECTION("После добавления элемента Empty возвращает false") {
        List lst;
        lst.PushFront(10);
        REQUIRE(lst.Empty() == false);
    }
}

TEST_CASE("Тестирование метода Size()", "[Size]") {
    SECTION("Размер нового списка = 0") {
        List lst;
        REQUIRE(lst.Size() == 0);
    }
    
    SECTION("Size увеличивается при добавлении элементов") {
        List lst;
        lst.PushFront(5);
        REQUIRE(lst.Size() == 1);
        lst.PushBack(10);
        REQUIRE(lst.Size() == 2);
    }
}

TEST_CASE("Тестирование метода Clear()", "[Clear]") {
    SECTION("Очистка списка с элементами") {
        List lst;
        lst.PushFront(1);
        lst.PushFront(2);
        lst.PushFront(3);
        
        REQUIRE(lst.Size() == 3);
        
        lst.Clear();
        
        REQUIRE(lst.Size() == 0);
        REQUIRE(lst.Empty() == true);
    }
}