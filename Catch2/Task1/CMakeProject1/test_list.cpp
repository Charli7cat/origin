#include <catch2/catch_test_macros.hpp>
#include <stdexcept>
#include "list.h"

TEST_CASE("List tests for Empty, Size and Clear", "[List]") {
    
    SECTION("Empty() returns true for newly created list") {
        List list;
        REQUIRE(list.Empty() == true);
        REQUIRE(list.Size() == 0);
    }
    
    SECTION("Size() returns correct number of elements after adding") {
        List list;
        
        REQUIRE(list.Size() == 0);
        
        list.PushFront(10);
        REQUIRE(list.Size() == 1);
        REQUIRE(list.Empty() == false);
        
        list.PushFront(20);
        REQUIRE(list.Size() == 2);
        
        list.PushFront(30);
        REQUIRE(list.Size() == 3);
        
        list.PushBack(40);
        REQUIRE(list.Size() == 4);
        
        list.PushBack(50);
        REQUIRE(list.Size() == 5);
    }
    
    SECTION("Size() returns 0 after Clear()") {
        List list;
        
        for(int i = 1; i <= 10; ++i) {
            list.PushBack(i);
        }
        
        REQUIRE(list.Size() == 10);
        REQUIRE(list.Empty() == false);
        
        list.Clear();
        
        REQUIRE(list.Size() == 0);
        REQUIRE(list.Empty() == true);
    }
}

TEST_CASE("Additional tests for Empty, Size and Clear", "[List][Extended]") {
    
    SECTION("Empty() after adding and removing all elements") {
        List list;
        
        list.PushBack(1);
        list.PushBack(2);
        list.PushBack(3);
        REQUIRE(list.Empty() == false);
        REQUIRE(list.Size() == 3);
        
        list.PopFront();
        list.PopFront();
        list.PopFront();
        
        REQUIRE(list.Empty() == true);
        REQUIRE(list.Size() == 0);
    }
    
    SECTION("Clear() on already empty list does nothing") {
        List list;
        
         REQUIRE_NOTHROW(list.Clear());
        REQUIRE(list.Size() == 0);
        REQUIRE(list.Empty() == true);
        
         list.Clear();
        REQUIRE(list.Size() == 0);
        REQUIRE(list.Empty() == true);
    }
    
    SECTION("Clear() then add new elements works correctly") {
        List list;
        
        list.PushBack(100);
        list.PushBack(200);
        list.Clear();
        
        REQUIRE(list.Size() == 0);
        REQUIRE(list.Empty() == true);
        
        list.PushBack(300);
        list.PushBack(400);
        
        REQUIRE(list.Size() == 2);
        REQUIRE(list.Empty() == false);
        
        REQUIRE(list.PopFront() == 300);
        REQUIRE(list.PopFront() == 400);
        REQUIRE(list.Empty() == true);
    }
    
    SECTION("Mixed operations with size tracking") {
        List list;
        
        REQUIRE(list.Size() == 0);
        REQUIRE(list.Empty() == true);
        
        list.PushFront(1);
        list.PushFront(2);
        list.PushFront(3);
        REQUIRE(list.Size() == 3);
        
        list.PushBack(4);
        list.PushBack(5);
        REQUIRE(list.Size() == 5);
        
        list.PopFront();
        REQUIRE(list.Size() == 4);
        
        list.PopBack();
        REQUIRE(list.Size() == 3);
        
        list.Clear();
        REQUIRE(list.Size() == 0);
        REQUIRE(list.Empty() == true);
    }
}