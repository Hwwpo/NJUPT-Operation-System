//
// Created by 潘鹏宇 on 2023/12/26.
//

#include "process.h"

process::process(const char &name) : name(name) {
    this->name = name;
}

bool process::operator<(const process &rhs) const {
    return ArrivalTime < rhs.ArrivalTime;
}

bool process::operator>(const process &rhs) const {
    return rhs < *this;
}

bool process::operator<=(const process &rhs) const {
    return !(rhs < *this);
}

bool process::operator>=(const process &rhs) const {
    return !(*this < rhs);
}
