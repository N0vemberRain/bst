#pragma once

#pragma once

#include <iostream>
#include <stack>
#include <algorithm>
#include <iterator>
#include "item.h"

template <typename Node, typename Item>
class BSTIterator {
public:
    using link = Node*;

    explicit BSTIterator(link head, link end) {
        if (head == nullptr) {
            return;
        }

        data_.push(end);
        auto current_node = head;
        while (current_node != nullptr) {
            data_.push(current_node);
            current_node = current_node->left;
        }
    }
    explicit BSTIterator(link head) {
        if (head == nullptr) {
            return;
        }

        auto current_node = head;
        while (current_node != nullptr) {
            data_.push(current_node);
            current_node = current_node->left;
        }
    }
    const link& operator*() const { return data_.top(); }

    BSTIterator& operator++() {
        //data_ = data_->right;
        auto current_node = data_.top()->right;
        data_.pop();
        while (current_node != nullptr) {
            data_.push(current_node);
            current_node = current_node->left;
        }
        return *this;
    }
    bool operator!=(const BSTIterator& other) const {
        return this->data_.top() != other.data_.top();
    }
    bool operator==(const BSTIterator& other) const {
        return !(this->data_.top() != other.data_.top());
    }

    bool empty() const {
        return data_.empty();
    }
private:

    //link data_;
    std::stack<link> data_;
};

namespace std {
    template<typename Node, typename Item>
    struct iterator_traits<BSTIterator<Node, Item>> {
        using iterator_category = std::forward_iterator_tag;
        using value_type = Node*;
    };
}

template <typename Item, typename Key>
class BST {
private:
    struct Node {
        Item data;
        Node* left;
        Node* right;
        int N;

        Node(Item v) : data(v), left(nullptr), right(nullptr), N(0) {}
    };

    using link = Node*;

    link head_;
    Item nullitem_;
    link nullnode_ = new Node(nullitem_);

    Item searchR(link h, Key k) {
        if (h == nullptr) return nullitem_;
        Key t = h->data.key();
        if (k == t) return h->data;

        if (k < t)
            return searchR(h->left, k);
        else
            return searchR(h->right, k);
    }

    void insertR(link& h, Item d) {
        if (h == nullptr) {
            h = new Node(d);
            return;
        }

        if (d.key() < h->data.key()) {
            insertR(h->left, d);
            //h->N++;
        }
        else {
            insertR(h->right, d);
            //h->N++;
        }
    }

    void showR(const link& h, std::ostream& os) {
        if (h == nullptr)
            return;
        showR(h->left, os);
        h->data.show(os);
        showR(h->right, os);
    }

    void rotLeft(link& h) {
        link x = h->right;
        h->right = x->left;
        x->left = h;
        h = x;
    }

    void rotRight(link& h) {
        link x = h->left;
        h->left = x->right;
        x->right = h;
        h = x;
    }

    void insertT(link& h, Item v) {
        if (h == nullptr) {
            h = new Node(v);
            return;
        }

        if (v.key() < h->data.key()) {
            insertT(h->left, v);
            rotRight(h);
        }
        else {
            insertT(h->right, v);
            rotLeft(h);
        }

        h->N = count(h);
        if (h->left)
            h->left->N = count(h->left);
        if (h->right)
            h->right->N = count(h->right);
    }

    size_t count(const link& h) const {
        if (h == nullptr) return 0;
        return count(h->left) + count(h->right) + 1;
    }

    size_t updateCount(link& h) {
        if (h == nullptr) return 0;
        if (h->left == nullptr && h->right == nullptr)
            return 1;
        h->N += updateCount(h->left) + updateCount(h->right) + 1;
        return h->N;
    }

    Item selectR(const link& h, int k) const {
        if (h == nullptr)
            return nullitem_;
        int t = (h->left == nullptr) ? 0 : h->left->N;
        if (t > k) {
            return selectR(h->left, k);
        }
        if (t < k) {
            return selectR(h->right, k - t - 1);
        }
        return h->data;
    }

    void partR(link& h, int k) {
        // if (h == nullptr) return;
        int t = (h->left == nullptr) ? 0 : h->left->N;
        if (t > k) {
            partR(h->left, k);
            rotRight(h);
        }
        if (t < k) {
            partR(h->right, k - t - 1);
            rotLeft(h);
        }

        h->N = count(h);
        if (h->left)
            h->left->N = count(h->left);
        if (h->right)
            h->right->N = count(h->right);
    }

    link joinR(link a, link b) {
        if (b == nullptr) return a;
        if (a == nullptr) return b;
        insertT(b, a->data);
        b->left = joinR(a->left, b->left);
        b->right = joinR(a->right, b->right);
        delete a;
        return b;
    }

    link joinLR(link a, link b) {
        if (b == nullptr) return a;
        partR(b, 0);
        b->left = a;
        return b;
    }

    void removeR(link& h, Key k) {
        if (h == nullptr) return;
        auto t = h->data.key();
        if (k < t) {
            removeR(h->left, k);
        }
        if (k > t) {
            removeR(h->right, k);
        }
        if (k == t) {
            link old_head = h;
            h = joinLR(h->left, h->right);
            delete old_head;
        }

        h->N = count(h);
        if (h->left)
            h->left->N = count(h->left);
        if (h->right)
            h->right->N = count(h->right);
    }

    size_t height(const link& h) const {
        if (h == nullptr) return -1;
        size_t u{ height(h->left) };
        size_t v{ height(h->right) };
        if (u > v)
            return u + 1;
        else
            return v + 1;
    }

    void printNode(const Item data, size_t height) const {
        for (int i = 0; i < height; i++) std::cout << "  ";
        std::cout << data << std::endl;
    }

    void printEmptyNode(char c, size_t height) const {
        for (int i = 0; i < height; i++) std::cout << "  ";
        std::cout << c << std::endl;
    }

    void showFormR(const link& h, size_t height) const {
        if (h == nullptr) {
            printEmptyNode('*', height);
            return;
        }

        showFormR(h->right, height + 1);
        printNode(h->data, height);
        showFormR(h->left, height + 1);
    }
public:
    BST(int max_n) : head_(nullptr) {}

    Item search(Key k) { return searchR(head_, k); }
    void insert(Item d) { insertT(head_, d); }
    void show(std::ostream& os) { showR(head_, os); }
    void showForm() const { showFormR(head_, height(head_)); }
    Item select(int k) { return selectR(head_, k); }
    void remove(Item v) { removeR(head_, v.key()); }
    void join(BST<Item, Key>& st) { head_ = joinR(head_, st.head_); }

    void part_test(int k) { partR(head_, k); }

    using iterator = BSTIterator<Node, Item>;
    using const_iterator = BSTIterator<const Node, const Item>;

    iterator begin() { return BSTIterator<Node, Item>(head_, nullnode_); }
    iterator end() { return BSTIterator<Node, Item>(nullnode_); }
    const_iterator begin() const;
    const_iterator end() const;
};

void BSTTest() {
    int N, max_N = 3;
    BST<Item, int> st(max_N);
    Item tmp;
    int i = 0;

    auto i1 = Item(10, 4.32);
    auto i2 = Item(15, 5.75);
    auto i3 = Item(7, 10.32);
    auto i4 = Item(30, 12.4);
    auto i5 = Item(3, 12.4);
    auto i6 = Item(9, 12.4);


    st.insert(i1);
    st.insert(i2);
    st.insert(i3);
    st.insert(i4);
    st.insert(i5);
    st.insert(i6);

    //const auto v = st.select(i);
    std::cout << "finded item ";
    st.showForm();

    std::cout << "for loop:\n";

    for (auto& it : st) {
        std::cout << it->data;
    }
 
    std::copy(std::begin(st), std::end(st), std::ostream_iterator<Item>{std::cout, ", "});

    //auto [min_it, max_it](std::minmax_element(std::begin(st), std::end(st));

    st.select(0).show();
    st.select(1).show();
    st.select(2).show();
    st.select(3).show();
    st.select(4).show();
    st.select(5).show();

    //st.part_test(3);
    st.remove(i4);
    st.showForm();
    std::cout << "\n";
    st.select(0).show();
    st.select(1).show();
    st.select(2).show();
    st.select(3).show();
    st.select(4).show();

    std::cout << "all items: ";
    st.show(std::cout);
}

