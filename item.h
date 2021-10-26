#pragma once

#include <iostream>

// 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

static int MAX_KEY = 1000;

using Key = int;
class Item {
public:
    Item() : k(MAX_KEY) {}
    explicit Item(Key k, float value) : k(k), data(value) {}

    Item(const Item& other) : k(other.k), data(other.data) {}
    Item& operator=(const Item& other) {
        k = other.k;
        data = other.data;
        return *this;
    }

    bool operator==(const Item& other) const {
        return (k == other.k && data == other.data);
    }

    Key key()const noexcept { return k; }
    bool isNull() noexcept { return k == MAX_KEY; }
    void rand() noexcept {
        k = 1000 * ::rand() / RAND_MAX;
        data = 1.0 * ::rand() / RAND_MAX;
    }

    void scan(std::istream& is = std::cin) {
        is >> k >> data;
        //return (is >> k >> data) != 0;
    }

    void show(std::ostream& os = std::cout) const {
        os << k << " " << data << '\n';
    }

private:
    Key k;
    float data;
};

std::ostream& operator<<(std::ostream& os, const Item& val) {
    val.show(os);
    return os;
}

