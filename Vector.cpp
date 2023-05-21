//
// Created by user on 17.05.2023.
//

#include <stdexcept>
#include "Vector.h"

Vector::Vector(const Value *rawArray, const size_t size, float coef) {
    this->_multiplicativeCoef = coef;
    this->_size = size;
    this->_capacity = size * coef;
    this->_data = new Value[this->_capacity];
    for (size_t i = 0; i < size; ++i) {
        this->_data[i] = rawArray[i];
    }
}

Vector::Vector(const Vector &other) {
    this->_multiplicativeCoef = other._multiplicativeCoef;
    this->_size = other._size;
    this->_capacity = other._size * other._multiplicativeCoef;
    this->_data = new Value[other._capacity];
    for (size_t i = 0; i < other._size; ++i) {
        this->_data[i] = other._data[i];
    }
}

Vector& Vector::operator=(const Vector &other) {
    if (this == &other) {
        return *this;
    }
    this->_multiplicativeCoef = other._multiplicativeCoef;
    this->_size = other._size;
    this->_capacity = other._size;
    delete[] this->_data;
    this->_data = new Value[other._size];
    for (size_t i = 0; i < this->_size; ++i) {
        this->_data[i] = other._data[i];
    }
    return *this;
}

Vector::Vector(Vector &&other) noexcept {
    this->_multiplicativeCoef = other._multiplicativeCoef;
    this->_size = other._size;
    other._size = 0;
    this->_capacity = other._capacity;
    other._capacity = 0;
    this->_data = other._data;
    other._data = nullptr;
}

Vector& Vector::operator=(Vector&& other) noexcept {
    if (this == &other) {
        return *this;
    }
    this->_multiplicativeCoef = other._multiplicativeCoef;
    this->_size = other._size;
    other._size = 0;
    this->_capacity = other._capacity;
    other._capacity = 0;
    delete[] this->_data;
    this->_data = other._data;
    other._data = nullptr;
    return *this;
}

Vector::~Vector() {
    delete[] this->_data;
}

void Vector::reserve(size_t capacity) {
    if(capacity == 0){
        return;
    }
    if (capacity > this->_capacity) {
        auto* newData = new Value[capacity];
        for (size_t i = 0; i < this->_size; ++i) {
            newData[i] = this->_data[i];
        }
        delete[] this->_data;
        this->_data = newData;
        this->_capacity = capacity;
    }
}

void Vector::pushBack(const Value &value) {
    if (this->_size == 0) {
        this->reserve(this->_multiplicativeCoef);
    } else if(this->_size == this->_capacity) {
        this->reserve(this->_capacity * this->_multiplicativeCoef);
    }
    this->_data[this->_size] = value;
    ++this->_size;
}

void Vector::pushFront(const Value &value) {
    if (this->_capacity == 0) {
        this->reserve(this->_multiplicativeCoef);
    } else if(this->_size == this->_capacity) {
        this->reserve(this->_capacity * this->_multiplicativeCoef);
    }
    for (size_t i = this->_size; i > 0; --i) {
        this->_data[i] = this->_data[i - 1];
    }
    this->_data[0] = value;
    this->_size++;
}

void Vector::popBack() {
    if(this->_size == 0){
        throw std::out_of_range("Vector is empty");
    }
    if (this->_size > 0) {
        this->_size--;
    }
}

void Vector::popFront() {
    if(this->_size == 0){
        throw std::out_of_range("Vector is empty");
    }
    if (this->_size > 0) {
        for (size_t i = 0; i < this->_size - 1; ++i) {
            this->_data[i] = this->_data[i + 1];
        }
        this->_size--;
    }
    if(this->_size < this->_capacity / this->_multiplicativeCoef){
        this->shrinkToFit();
    }
}

void Vector::insert(const Value &value, size_t pos) {
    if (pos > this->_size) {
        throw std::out_of_range("Position is out of range");
    }
    if (this->_size == this->_capacity) {
        this->reserve(this->_capacity * this->_multiplicativeCoef);
    }
    for (size_t i = this->_size; i > pos; --i) {
        this->_data[i] = this->_data[i - 1];
    }
    this->_data[pos] = value;
    this->_size++;
}

void Vector::insert(const Vector &vector, size_t pos) {
    insert(vector._data, vector._size, pos);
}

void Vector::insert(const Value *values, size_t size, size_t pos) {
    if(values == nullptr){
        throw std::invalid_argument("Values is nullptr");
    }
    if (pos > this->_size) {
        throw std::out_of_range("Position is out of range");
    }
    if (this->_size + size > this->_capacity) {
        this->reserve((this->_size + size) * this->_multiplicativeCoef);
    }
    for (size_t i = this->_size + size - 1; i > pos + size - 1; --i) {
        this->_data[i] = this->_data[i - size];
    }
    for (size_t i = pos; i < pos + size; ++i) {
        this->_data[i] = values[i - pos];
    }
    this->_size += size;
}

void Vector::erase(size_t pos, size_t count){
    if (pos > this->_size) {
        return;
    }
    if (pos + count > this->_size) {
        count = this->_size - pos;
    }
    for (size_t i = pos; i < this->_size - count; ++i) {
        this->_data[i] = this->_data[i + count];
    }
    this->_size -= count;
}

void Vector::eraseBetween(size_t beginPos, size_t endPos) {
    if(beginPos > endPos) {
        return;
    }
    erase(beginPos, endPos - beginPos);
}

size_t Vector::size() const {
    return this->_size;
}

size_t Vector::capacity() const {
    return this->_capacity;
}

double Vector::loadFactor() const {
    return (double) this->_size / this->_capacity;
}

Value& Vector::operator[](size_t idx) {
    return this->_data[idx];
}

const Value& Vector::operator[](size_t idx) const {
    return this->_data[idx];
}

long long Vector::find(const Value &value) const {
    for (size_t i = 0; i < this->_size; ++i) {
        if (this->_data[i] == value) {
            return i;
        }
    }
    return -1;
}

void Vector::shrinkToFit() {
    if (this->_size < this->_capacity) {
        auto* newData = new Value[this->_size];
        for (size_t i = 0; i < this->_size; ++i) {
            newData[i] = this->_data[i];
        }
        delete[] this->_data;
        this->_data = newData;
        this->_capacity = this->_size;
    }
}

Vector::Iterator::Iterator(Value *ptr) {
    this->_ptr = ptr;
}

Value& Vector::Iterator::operator*(){
    return *this->_ptr;
}

const Value& Vector::Iterator::operator*() const {
    return *this->_ptr;
}

Value* Vector::Iterator::operator->() {
    return this->_ptr;
}

const Value* Vector::Iterator::operator->() const {
    return this->_ptr;
}

Vector::Iterator Vector::Iterator::operator++(){
    this->_ptr++;
    return *this;
}

Vector::Iterator Vector::Iterator::operator++(int){
    auto tmp = *this;
    this->_ptr++;
    return tmp;
}

bool Vector::Iterator::operator==(const Iterator &other) const {
    return this->_ptr == other._ptr;
}

bool Vector::Iterator::operator!=(const Iterator &other) const {
    return this->_ptr != other._ptr;
}

Vector::Iterator Vector::begin() {
    return Iterator(this->_data);
}

Vector::Iterator Vector::end() {
    return Iterator(this->_data + this->_size);
}