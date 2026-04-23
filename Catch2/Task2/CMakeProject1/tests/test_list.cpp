#include "list.h"
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

using namespace Catch::Matchers;

TEST_CASE("PushBack adds elements to the end of the list", "[PushBack]") {
    List list;

    REQUIRE(list.Empty());
    REQUIRE(list.Size() == 0);

    list.PushBack(10);
    REQUIRE_FALSE(list.Empty());
    REQUIRE(list.Size() == 1);

    list.PushBack(20);
    REQUIRE(list.Size() == 2);

    list.PushBack(30);
    REQUIRE(list.Size() == 3);
}

TEST_CASE("PushFront adds elements to the beginning of the list", "[PushFront]") {
    List list;

    list.PushFront(10);
    REQUIRE(list.Size() == 1);

    list.PushFront(20);
    REQUIRE(list.Size() == 2);

    list.PushFront(30);
    REQUIRE(list.Size() == 3);
}

TEST_CASE("PopBack throws exception when list is empty", "[PopBack]") {
    List list;

    REQUIRE(list.Empty());
    REQUIRE_THROWS_AS(list.PopBack(), std::runtime_error);
    REQUIRE_THROWS_WITH(list.PopBack(), ContainsSubstring("empty"));
}

TEST_CASE("PopFront throws exception when list is empty", "[PopFront]") {
    List list;

    REQUIRE(list.Empty());
    REQUIRE_THROWS_AS(list.PopFront(), std::runtime_error);
    REQUIRE_THROWS_WITH(list.PopFront(), ContainsSubstring("empty"));
}

TEST_CASE("PopBack removes and returns last element", "[PopBack]") {
    List list;

    list.PushBack(10);
    list.PushBack(20);
    list.PushBack(30);

    REQUIRE(list.Size() == 3);

    int value = list.PopBack();
    REQUIRE(value == 30);
    REQUIRE(list.Size() == 2);

    value = list.PopBack();
    REQUIRE(value == 20);
    REQUIRE(list.Size() == 1);

    value = list.PopBack();
    REQUIRE(value == 10);
    REQUIRE(list.Empty());
}

TEST_CASE("PopFront removes and returns first element", "[PopFront]") {
    List list;

    list.PushBack(10);
    list.PushBack(20);
    list.PushBack(30);

    REQUIRE(list.Size() == 3);

    int value = list.PopFront();
    REQUIRE(value == 10);
    REQUIRE(list.Size() == 2);

    value = list.PopFront();
    REQUIRE(value == 20);
    REQUIRE(list.Size() == 1);

    value = list.PopFront();
    REQUIRE(value == 30);
    REQUIRE(list.Empty());
}

TEST_CASE("Complex scenario mixing all operations", "[Integration]") {
    List list;

    SECTION("Mixed push operations") {
        list.PushBack(1);
        list.PushFront(0);
        list.PushBack(2);
        list.PushFront(-1);
        list.PushBack(3);

        REQUIRE(list.Size() == 5);

        REQUIRE(list.PopFront() == -1);
        REQUIRE(list.PopFront() == 0);
        REQUIRE(list.PopFront() == 1);
        REQUIRE(list.PopFront() == 2);
        REQUIRE(list.PopFront() == 3);
        REQUIRE(list.Empty());
    }

    SECTION("Alternating push and pop") {
        list.PushBack(100);
        list.PushFront(200);
        REQUIRE(list.PopBack() == 100);
        REQUIRE(list.PopFront() == 200);
        REQUIRE(list.Empty());

        list.PushBack(300);
        list.PushBack(400);
        list.PushFront(500);
        REQUIRE(list.PopFront() == 500);
        REQUIRE(list.PopBack() == 400);
        REQUIRE(list.PopBack() == 300);
        REQUIRE(list.Empty());
    }

    SECTION("Single element operations") {
        list.PushBack(42);
        REQUIRE_FALSE(list.Empty());
        REQUIRE(list.Size() == 1);

        REQUIRE(list.PopFront() == 42);
        REQUIRE(list.Empty());

        list.PushFront(99);
        REQUIRE(list.PopBack() == 99);
        REQUIRE(list.Empty());
    }

    SECTION("Clear after partial fill") {
        list.PushBack(1);
        list.PushBack(2);
        list.PushBack(3);
        list.PushBack(4);
        REQUIRE(list.Size() == 4);

        list.Clear();
        REQUIRE(list.Empty());
        REQUIRE(list.Size() == 0);

        list.PushBack(5);
        list.PushBack(6);
        REQUIRE(list.Size() == 2);
        REQUIRE(list.PopBack() == 6);
    }

    SECTION("Complex sequence with exceptions") {
        for (int i = 1; i <= 5; i++) {
            list.PushBack(i * 10);
        }

        REQUIRE(list.PopFront() == 10);
        REQUIRE(list.PopFront() == 20);

        list.PushFront(5);
        list.PushFront(0);

        REQUIRE(list.PopBack() == 50);
        REQUIRE(list.PopBack() == 40);

        REQUIRE(list.Size() == 3);
        REQUIRE(list.PopFront() == 0);
        REQUIRE(list.PopFront() == 5);
        REQUIRE(list.PopFront() == 30);
        REQUIRE(list.Empty());

        REQUIRE_THROWS(list.PopFront());
        REQUIRE_THROWS(list.PopBack());
    }
}

TEST_CASE("List can be reused after being emptied", "[Reuse]") {
    List list;

    list.PushBack(1);
    list.PushBack(2);
    list.PushBack(3);
    REQUIRE(list.PopFront() == 1);
    REQUIRE(list.PopBack() == 3);
    REQUIRE(list.Size() == 1);
    REQUIRE(list.PopFront() == 2);
    REQUIRE(list.Empty());

    list.PushFront(10);
    list.PushFront(20);
    list.PushBack(30);
    REQUIRE(list.Size() == 3);
    REQUIRE(list.PopBack() == 30);
    REQUIRE(list.PopFront() == 20);
    REQUIRE(list.PopFront() == 10);
    REQUIRE(list.Empty());
}