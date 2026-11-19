#ifndef HIGHERFIRST_H
#define HIGHERFIRST_H


template <class T>
struct HigherFirst {

    bool operator()(const T& a, const T& b) const {
        return a > b;
    }
};

#endif
