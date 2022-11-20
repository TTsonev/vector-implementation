#ifndef VECTOR_H
#define VECTOR_H

#include <iostream>

template <typename T>
class Vector
{
public:
    class ConstIterator;
    class Iterator;
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = ptrdiff_t;
    using reference = value_type &;
    using const_reference = const value_type &;
    using pointer = value_type *;
    using const_pointer = const value_type *;
    using iterator = Vector::Iterator;
    using const_iterator = Vector::ConstIterator;

private:
    size_type sz;
    size_type max_sz;
    pointer values;

public:
    Vector(size_t n) : sz{0}, max_sz{n < 5 ? 5 : n}, values{new value_type[max_sz]} {}

    Vector() : Vector(5) {}

    Vector(const Vector &original) : Vector(original.sz)
    {
        for (size_t i{0}; i < original.sz; i++)
        {
            values[i] = original.values[i];
            sz++;
        }
    }

    Vector(std::initializer_list<value_type> list) : Vector(list.size())
    {
        size_t i{0};
        for (const auto &elem : list)
        {
            values[i] = elem;
            i++;
            sz++;
        }
    }

    ~Vector()
    {
        delete[] values;
    }


    //vector functuionality methods

    size_type size() const { return sz; }

    void clear() { sz = 0; }

    bool empty() const { return sz == 0; }

    void reserve(size_type n)
    {
        if (n <= max_sz)
            return;
        pointer nvals = new value_type[n];
        for (size_type i{0}; i < sz; i++)
        {
            nvals[i] = values[i];
        }
        delete[] values;
        values = nvals;
        max_sz = n;
    }

    void shrink_to_fit() { max_sz = sz; }

    void operator=(const Vector &rop)
    {
        delete[] values;
        sz = rop.sz;
        max_sz = rop.sz;
        values = new value_type[max_sz];
        for (size_t i{0}; i < sz; i++)
        {
            values[i] = rop.values[i];
        }
    }

    void push_back(value_type x)
    {
        if (sz == max_sz)
        {
            reserve(max_sz * 2);
        }
        values[sz] = x;
        sz++;
    }

    void pop_back()
    {
        if (empty() == true)
            throw std::out_of_range("pop_back() called on empty vector");
        sz -= 1;
        pointer nvals = new value_type[max_sz];
        for (size_t i{0}; i < sz; i++)
        {
            nvals[i] = values[i];
        }
        delete[] values;
        values = nvals;
    }

    reference operator[](size_t index)
    {
        if (index >= sz)
            throw std::out_of_range("index" + std::to_string(index) + "out of bounds");
        return values[index];
    }

    const_reference operator[](size_t index) const
    {
        if (index >= sz)
            throw std::out_of_range("index" + std::to_string(index) + "out of bounds");
        return values[index];
    }

    size_type capacity() const { return max_sz; }

    friend std::ostream &operator<<(std::ostream &o, const Vector<T> &vec)
    {
        if (vec.empty() == true)
        {
            o << "[]";
            return o;
        }

        o << "[";
        for (size_type i{0}; i < vec.sz - 1; i++)
        {
            o << vec.values[i] << ", ";
        }
        o << vec.values[vec.sz - 1] << "]";

        return o;
    }


    //iterator

    iterator begin()
    {
        return iterator{values, values + sz};
    }

    iterator end() { return iterator{values + sz}; }

    const_iterator begin() const
    {
        return const_iterator{values, values + sz};
    }

    const_iterator end() const { return const_iterator{values + sz}; }

    class Iterator
    {
    public:
        using value_type = Vector::value_type;
        using reference = Vector::reference;
        using pointer = Vector::pointer;
        using difference_type = Vector::difference_type;
        using iterator_category = std::forward_iterator_tag;

    private:
        pointer ptr;
        pointer e;

    public:
        Iterator() : ptr{nullptr}, e{nullptr} {}
        Iterator(pointer target) : ptr{target}, e{target} {}
        Iterator(pointer target, pointer e) : ptr{target}, e{e} {}

        reference operator*() const
        {
            if (ptr == e)
                throw std::out_of_range("iterator out of bounds");
            return *ptr;
        }

        pointer operator->() const
        {
            if (ptr == e)
                throw std::out_of_range("iterator out of bounds");
            return ptr;
        }

        iterator &operator++()
        {
            if (ptr == e)
                return *this;
            ptr++;
            return *this;
        }

        iterator operator++(int)
        {
            iterator kopie = *this;
            if (ptr == e)
                return kopie;
            ++*this;
            return kopie;
        }

        bool operator!=(const const_iterator &rop) const
        {
            return static_cast<const_iterator>(*this) != rop;
        }

        bool operator==(const const_iterator &rop) const
        {
            return static_cast<const_iterator>(*this) == rop;
        }

        Vector::difference_type operator-(const Vector::ConstIterator &rop)
        {
            return static_cast<const_iterator>(*this) - rop;
        }

        operator const_iterator() const
        {
            return const_iterator{ptr, e};
        };
    };


    //const_iterator

    class ConstIterator
    {
    public:
        using value_type = Vector::value_type;
        using reference = Vector::const_reference;
        using pointer = Vector::const_pointer;
        using difference_type = Vector::difference_type;
        using iterator_category = std::forward_iterator_tag;

    private:
        pointer ptr;
        pointer e;

    public:
        ConstIterator() : ptr{nullptr}, e{nullptr} {}
        ConstIterator(pointer target) : ptr{target}, e{target} {}
        ConstIterator(pointer target, pointer e) : ptr{target}, e{e} {}

        reference operator*() const
        {
            if (ptr == e)
                throw std::out_of_range("cons_titerator out of bounds");
            return *ptr;
        }
        pointer operator->() const
        {
            if (ptr == e)
                throw std::out_of_range("const_iterator out of bounds");
            return ptr;
        }

        ConstIterator &operator++()
        {
            if (ptr == e)
                return *this;
            ptr++;
            return *this;
        }

        ConstIterator operator++(int)
        {
            const_iterator kopie = *this;
            if (ptr == e)
                return kopie;
            ++*this;
            return kopie;
        }

        bool operator!=(const const_iterator &rop) const
        {
            return !(ptr == rop.ptr);
        }

        bool operator==(const const_iterator &rop) const
        {
            return ptr == rop.ptr;
        }

        Vector::difference_type operator-(const Vector::ConstIterator &rop)
        {
            return ptr - rop.ptr;
        }
    };


    //insert and erase
    
    iterator insert(const_iterator pos, const_reference val)
    {
        auto diff = pos - begin();

        if (diff < 0 || static_cast<size_type>(diff) > sz)
            throw std::out_of_range("iterator out of bounds");

        size_type current{static_cast<size_type>(diff)};

        if (sz >= max_sz)
            reserve(max_sz * 2);

        for (auto i{sz}; i-- > current;)
            values[i + 1] = values[i];

        values[current] = val;
        ++sz;
        return iterator{values + current, values + sz};
    }

    iterator erase(const_iterator pos)
    {
        auto diff = pos - begin();

        if (diff < 0 || static_cast<size_type>(diff) > sz)
            throw std::out_of_range("iterator out of bounds");

        size_type current{static_cast<size_type>(diff)};

        for (auto i{current}; i < sz - 1; ++i)
            values[i] = values[i + 1];

        --sz;
        return iterator{values + current, values + sz};
    }
};

#endif
