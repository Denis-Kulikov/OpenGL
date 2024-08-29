#include <iostream>

template<typename T>
class tree_node {
public:
    tree_node(T t) : value(t) {}
    tree_node() {}

    void set_value(const T v) { value = v; }
    T get_value()  const      { return value; }

    tree_node<T> const &left() const { return *left_; }
    tree_node<T> const &right() const { return *right_; }

    tree_node *left_ = nullptr;
    tree_node *right_ = nullptr;

protected:
    T value;
};
