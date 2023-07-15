#ifndef SPSCRB_ADAPTER
#define SPSCRB_ADAPTER

namespace spscrb {

template <typename T>
struct iterator {
    using value_type = typename T::value_type;
    using size_type = typename T::size_type;
    using reference = typename T::reference&;
    using const_reference = typename T::const_reference;
    using difference_type = typename T::difference_type;
    using pointer = typename T::pointer;
    using iterator_category = typename std::random_access_iterator_tag;

    T* rb;
    size_type idx;

    iterator() = delete;
    iterator(T* ringbuf, size_type index) : rb(ringbuf), idx(index) {};
    iterator(const iterator& it) : rb(it.rb), idx(it.idx) {};

    iterator& operator=(const iterator& it) noexcept {
        if (*this == it)
            return *this;
        rb = it.rb; // FIXME
        idx = it.idx;
        return *this;
    }

    reference operator*() const noexcept {
        return rb->at(idx);
    }

    pointer operator->() {
        return &(operator*());
    }

    iterator& operator++() noexcept {
        ++idx;
        return *this;
    }

    iterator& operator--() noexcept {
        --idx;
        return *this;
    }

    difference_type operator-(const iterator& it) const noexcept {
        return idx - it.idx;
    }

    iterator operator+(difference_type n) const noexcept {
        return iterator(*this) += n;
    }

    iterator operator-(difference_type n) const noexcept {
        return iterator(*this) -= n;
    }

    iterator& operator+=(difference_type n) noexcept {
        idx += n;
        return *this;
    }

    iterator& operator-=(difference_type n) noexcept {
        idx -= n;
        return *this;
    }
    
    difference_type operator<(const iterator& it) const noexcept {
        return idx < it.idx;
    }

    difference_type operator!=(const iterator& it) const noexcept {
        return idx != it.idx;
    }

    difference_type operator==(const iterator& it) const noexcept {
        return idx == it.idx;
    }
};

#define VALUE_TYPE char
#define N 4
#define SIZE_TYPE size_t
#define DIFFERENCE_TYPE ptrdiff_t;

struct spscrb_adapter {
    typedef VALUE_TYPE value_type;
    typedef SIZE_TYPE size_type;

    using reference = typename value_type&;
    using const_reference = typename const value_type&;
    using difference_type = typename DIFFERENCE_TYPE;
    using pointer = typename value_type*;

    value_type buf[N];
    size_type pushes;
    size_type pops;
    const size_type capacity;

    spscrb_adapter() noexcept : pushes(0), pops(0), capacity(N) {};

    void push_back(value_type v) noexcept {
        assert(size() < capacity);
        buf[pushes % capacity] = v;
        pushes++;
    }

    void pop_front() noexcept {
        assert(size() > 0);
        pops++;
    }

    value_type top() const noexcept {
        return buf[pops % capacity];
    }

    size_type size() const noexcept {
        return pushes - pops;
    }

    bool full() const noexcept {
        return size() == capacity;
    }

    bool empty() const noexcept {
        return size() == 0;
    }

    value_type& at(size_type offset) noexcept {
        assert(offset < size());
        assert(offset < capacity);
        return buf[(pops + offset) % capacity];
    }

    const value_type& at(size_type offset) const noexcept {
        return buf[(pops + offset) % capacity];
    }

    friend iterator<spscrb_adapter>;
    iterator<spscrb_adapter> begin() noexcept { return iterator(this, 0); }
    iterator<spscrb_adapter> end()   noexcept { return iterator(this, size()); }
};

}
#endif // SPSCRB_ADAPTER
