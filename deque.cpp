#include <iostream>

template <typename T>
class _deque;

template <typename T>
class _dinamic_vector {
public:
    T *m_data;
    T *m_end;
    T *m_capacity;
    T *m_begin;
    
public:

    _dinamic_vector();
    _dinamic_vector(int size);
    _dinamic_vector(const _dinamic_vector& other);
    _dinamic_vector(_dinamic_vector&& other);
    ~_dinamic_vector();
    _dinamic_vector<T>& operator=(const _dinamic_vector<T>& other);

    void _push_back(const T& value);
    void _pop_back();
    void _push_front(const T& value);
    void _pop_front();
    size_t _size() const;
    size_t _capacity() const;
    bool _empty() const;
    T& operator[](int index);
    T& _at(int index);
    void _reserve(int new_capacity);
    void _shrink_to_fit();
    void _resize(int new_size);
    void _clear();
    T& _front();
    T& _back();

    class _iterator {
        T* _it;
    public:
        _iterator(T* it) : _it(it) {}
        _iterator& operator++() {
            ++_it;
            if(_it >= m_capacity) {
                _it = m_data;
            }
            return *this;
        }
        bool operator!=(const _iterator& other) { return _it != other._it; }
        T& operator*() { return *_it; }
        friend std::ostream& operator<<(std::ostream& os, const _iterator& it) {
            os << it._it;
            return os;
        }
    };

    class _reverse_iterator {
        T* _it;
    public:
        _reverse_iterator(T* it) : _it(it) {}
        _reverse_iterator& operator++() {
            --_it;
            if(_it < m_data) {
                _it = m_capacity - 1;
            }
            return *this;
        }
        bool operator!=(const _reverse_iterator& other) { return _it != other._it; }
        T& operator*() { return *_it; }
        friend std::ostream& operator<<(std::ostream& os, const _reverse_iterator& it) {
            os << it._it;
            return os;
        }
    };

    _iterator _begin();
    _iterator _end();
    _reverse_iterator _rbegin();
    _reverse_iterator _rend();

};

template <typename T>
_dinamic_vector<T>::_dinamic_vector() {
    m_data = new T[0];
    m_begin = m_data;
    m_end = m_data;
    m_capacity = m_end;
}

template <typename T>
_dinamic_vector<T>::_dinamic_vector(int size) {
    if(size <= 2) {
        m_data = new T[size]; 
    } else {
        int i = 2;
        while(i <= size) {
            if(i % 2 == 0 && i * 2 >= size) {
                m_data = new T[i * 2];
                m_capacity = m_data + i * 2;
                break;
            }
            i = i * 2;
        }
    }
    m_begin = m_data;
    m_end = m_data + size;
}

template <typename T>
_dinamic_vector<T>::_dinamic_vector(const _dinamic_vector& other) {
    size_t size = other.m_end - other.m_data;
    if(size <= 2) {
        m_data = new T[size]; 
    } else {
        int i = 2;
        while(i <= size) {
            if(i % 2 == 0 && i * 2 >= size) {
                m_data = new T[i * 2];
                m_capacity = m_data + i * 2;
                break;
            }
            i = i * 2;
        }
        std::copy(other.m_data, other.m_end, m_data);
        std::cout << "Copy constructor called\n";
    }
    m_begin = m_data;
    m_end = m_data + size;
}

template <typename T>
_dinamic_vector<T>::_dinamic_vector(_dinamic_vector&& other) {
    m_data = other.m_data;
    m_end = other.m_end;
    m_capacity = other.m_capacity;
    m_begin = m_data;

    other.m_data = nullptr;
    other.m_end = nullptr;
    other.m_capacity = nullptr;
    other.m_begin = nullptr;

    std::cout << "Move constructor called\n";
}

template <typename T>
_dinamic_vector<T>::~_dinamic_vector() {
    delete[] m_data;
}

template <typename T>
_dinamic_vector<T>& _dinamic_vector<T>::operator=(const _dinamic_vector<T>& other) {
    if (this == &other) return *this;  // Handle self-assignment

    // Clean up the current data
    // Copy data from 'other'
    m_capacity = other.m_capacity;
    m_begin = other.m_data;
    m_end = other.m_end;
    //m_data = new T[_size()];

    // Copy elements from the other vector
    // for (size_t i = 0; i < size; ++i) {
    //     m_data[i] = other.m_data[i];
    // }

    return *this;
}

template <typename T>
typename _dinamic_vector<T>::_iterator _dinamic_vector<T>::_begin() {
    return _iterator(m_begin);
}

template <typename T>
typename _dinamic_vector<T>::_iterator _dinamic_vector<T>::_end() {
    return _iterator(m_end);
}

template <typename T>
typename _dinamic_vector<T>::_reverse_iterator _dinamic_vector<T>::_rbegin() {
    if(m_end != m_data) {
        return _reverse_iterator(m_end - 1);
    } else {
        return _reverse_iterator(m_capacity - 1);
    }
}

template <typename T>
typename _dinamic_vector<T>::_reverse_iterator _dinamic_vector<T>::_rend() {
    if(m_begin != m_data) {
        return _reverse_iterator(m_begin - 1);
    } else {
        return _reverse_iterator(m_capacity - 1);
    }
}

template <typename T>
size_t _dinamic_vector<T>::_size() const {
    if(m_end > m_begin) {
        return m_end - m_begin;
    } else {
        return _capacity() - (m_begin - m_end);
    }
}

template <typename T>
size_t _dinamic_vector<T>::_capacity() const {
    return m_capacity - m_data;
}

template <typename T>
void _dinamic_vector<T>::_reserve(int size) {
    if(size > _capacity()) {
        int count = _size();
        T* begin = new T[size]; 
        int pos = m_begin - m_data;
        for(int i = 0;i < _size(); i++) {
            begin[i] = m_data[pos];
            pos++;
            if(pos >= _capacity()) {
                pos = 0;
            }
        }
        delete m_data;
        m_data = begin;
        m_end = m_data + count;
        m_capacity = m_data + size; 
        m_begin = m_data;
    }
}

template <typename T>
void _dinamic_vector<T>::_shrink_to_fit() {
    int count = _size();
    T* begin = new T[_size()]; 
    int pos = m_begin - m_data;
    for(int i = 0;i < _size(); i++) {
        begin[i] = m_data[pos];
        pos++;
        if(pos > _capacity()) {
            pos = 0;
        }
    }
    delete m_data;
    m_data = begin;
    m_end = m_data + count;
    m_capacity = m_end; 
}

template <typename T>
T& _dinamic_vector<T>::operator[](int index) {
     int pos = m_begin - m_data + index;
    if(pos < _capacity()) {
        return m_data[pos];
    } else {
        return m_data[pos - _capacity()];
    }
}

template <typename T>
T& _dinamic_vector<T>::_at(int index) {
    if(_size() > index) {
        return m_data[index];
    } else {
        std::cerr << "terminate called after throwing an instance of 'std::out_of_range'\n what():  vector::_M_range_check: __n (which is " << index << ") >= this->size() (which is " << index << ")\nAborted (core dumped)"<<std::endl;
        exit(EXIT_FAILURE);
    }
}

template <typename T>
void _dinamic_vector<T>::_push_back(const T& value) {
    if(_size() + 1 <= _capacity()) {
        int* pos = m_end + 1;
        if(pos <= m_capacity) {
            m_data[_size()] = value;
            m_end++;
         } else {
            m_data[0] = value;
            m_end = m_data + 1;
         }
    } else {
        if(_size() + 1 <= 2) {
            _reserve(_size() + 1); 
        } else {
            int i = 2;
            while(i <= _size() + 1) {
                if(i * 2 >= _size() + 1) {
                    _reserve(i * 2);
                }
                i = i * 2;
            }
            m_data[_size()] = value;
            m_end++;
        }
    }
}

template <typename T>
void _dinamic_vector<T>::_pop_back() {
    int* pos = m_end - 1;
    if(pos >= m_data) {
        m_data[_size() - 1] = 0;
        m_end--;
     } else {
        m_data[_capacity() - 1] = 0;
        m_end = m_capacity;
     }
}

template <typename T>
void _dinamic_vector<T>::_push_front(const T& value) {
    if(_size() + 1 <= _capacity()) {
        int* pos = m_begin - 1;
        if(pos >= m_data) {
            m_data[m_begin - m_data - 1] = value;
            m_begin--;
         } else {
            m_data[_capacity() - 1] = value;
            m_begin = m_data + _capacity() - 1;
         }
    } else {
        if(_size() + 1 <= 2) {
            _reserve(_size() + 1); 
        } else {
            int i = 2;
            while(i <= _size() + 1) {
                if(i * 2 >= _size() + 1) {
                    _reserve(i * 2);
                }
                i = i * 2;
            }
            m_data[_capacity() - 1] = value;
            m_begin = m_data + _capacity() - 1;
        }
    }
}

template <typename T>
void _dinamic_vector<T>::_pop_front() {
    int* pos = m_begin + 1;
    if(pos <= m_capacity) {
        m_data[m_begin - m_data] = 0;
        m_begin++;
     } else {
        m_data[0] = 0;
        m_begin = m_data;
     }
}

template <typename T>
void _dinamic_vector<T>::_clear() {
    m_end = m_data;
    m_begin = m_data;
}

template <typename T>
void _dinamic_vector<T>::_resize(int number) {
    if(_size() < number) {
        if(number <= 2) {
            _reserve(number); 
        } else {
            int i = 2;
            while(i <= number) {
                if(i % 2 == 0 && i * 2 >= _size()) {
                    _reserve(i * 2);
                }
                i = i * 2;
            }
            m_end = m_data + number;
        }
    } else {
        if(m_begin - m_data + number <= _capacity()) {
            m_end = m_begin + number;
        } else {
            m_end = m_begin + number - _capacity();
        }
    }
}

template <typename T>
bool _dinamic_vector<T>::_empty() const {
    return _size() == 0;
}

template <typename T>
T& _dinamic_vector<T>::_front() {
    return m_data[m_begin - m_data];
}

template <typename T>
T& _dinamic_vector<T>::_back() {
    if(m_end - 1 < m_data) {
        return m_data[_capacity() - 1];
    }
    return m_data[m_end - m_data - 1];
}

template <typename T>
class _deque {
    _dinamic_vector<_dinamic_vector<T>>* m_dinamic_vector;
public:

    _deque();
    _deque(int size);
    ~_deque();

    void _push_back(const T& value);
    void _pop_back();
    void _push_front(const T& value);
    void _pop_front();
    size_t _size();
    size_t _capacity() const;
    bool _empty();
    T& operator[](int index);
    T& _at(int index);
    void _reserve(int new_capacity);
    void _shrink_to_fit();
    void _resize(int new_size);
    void _clear();
    T& _front();
    T& _back();

    class _iterator {
        T* _it;
    public:
        _iterator(T* it) : _it(it) {}
        _iterator& operator++() {
            ++_it;
            if(_it >= m_dinamic_vector->m_capacity) {
                _it = m_dinamic_vector->m_data;
            }
            return *this;
        }
        bool operator!=(const _iterator& other) { return _it != other._it; }
        T& operator*() { return *_it; }
        friend std::ostream& operator<<(std::ostream& os, const _iterator& it) {
            os << it._it;
            return os;
        }
    };

    class _reverse_iterator {
        T* _it;
    public:
        _reverse_iterator(T* it) : _it(it) {}
        _reverse_iterator& operator++() {
            --_it;
            if(_it < m_dinamic_vector->m_data) {
                _it = m_dinamic_vector->m_capacity - 1;
            }
            return *this;
        }
        bool operator!=(const _reverse_iterator& other) { return _it != other._it; }
        T& operator*() { return *_it; }
        friend std::ostream& operator<<(std::ostream& os, const _reverse_iterator& it) {
            os << it._it;
            return os;
        }
    };

    _iterator _begin();
    _iterator _end();
    _reverse_iterator _rbegin();
    _reverse_iterator _rend();

};

template <typename T>
_deque<T>::_deque() {
    m_dinamic_vector = new _dinamic_vector<_dinamic_vector<T>>();
}

template <typename T>
_deque<T>::_deque(int size) {
    if(size < 6) {
        m_dinamic_vector = new _dinamic_vector<_dinamic_vector<T>>( 1 );
        (*m_dinamic_vector)[0] = _dinamic_vector<T>(size);
    } else if(size % 6 == 0) {
        m_dinamic_vector = new _dinamic_vector<_dinamic_vector<T>>(size / 6);
    } else {
        m_dinamic_vector = new _dinamic_vector<_dinamic_vector<T>>(size / 6 + 1);
    }
    for(int i = 0;i < size / 6;i++) {
        (*m_dinamic_vector)[i] = _dinamic_vector<T>(6);
        //std::cout << (*m_dinamic_vector)[i]._capacity() << std::endl;
        if(size % 6 > 0 && i + 1 == size / 6) {
            (*m_dinamic_vector)[i + 1] = _dinamic_vector<T>(size % 6);
        }
    }
    // if(size % 6 > 0) {
    //     (*m_dinamic_vector)[size / 6 + 1] = _dinamic_vector<T>(size % 6);
    // }
}

template <typename T>
_deque<T>::~_deque() {
    delete m_dinamic_vector;
}

template <typename T>
size_t _deque<T>::_size() {
    size_t size;
    if(m_dinamic_vector->_size() == 0) {
        return 0;
    } else if (m_dinamic_vector->_size() == 1) {
        size = (*m_dinamic_vector)[m_dinamic_vector->m_begin - m_dinamic_vector->m_data]._size();
    } else {
         size = (m_dinamic_vector->_size() - 2) * 6;
         size += (*m_dinamic_vector)[m_dinamic_vector->m_begin - m_dinamic_vector->m_data]._size();
         size += (*m_dinamic_vector)[m_dinamic_vector->m_end - m_dinamic_vector->m_data - 1]._size();
    }
    return size;
}


template <typename T>
size_t _deque<T>::_capacity() const {
    return m_dinamic_vector->_capacity() * 6;
}

template <typename T>
bool _deque<T>::_empty()  {
    return _size() == 0;
}

template <typename T>
T& _deque<T>::operator[](int index) {
        return (*m_dinamic_vector)[m_dinamic_vector->m_begin - m_dinamic_vector->m_data + (index / 6)][index % 6];
}

template <typename T>
T& _deque<T>::_at(int index) {
    if(_size() > index) {
        return (*m_dinamic_vector)[m_dinamic_vector->m_begin - m_dinamic_vector->m_data + (index / 6)][index % 6];
    } else {
        std::cerr << "terminate called after throwing an instance of 'std::out_of_range'\n what():  vector::_M_range_check: __n (which is " << index << ") >= this->size() (which is " << index << ")\nAborted (core dumped)"<<std::endl;
        exit(EXIT_FAILURE);
    }
}

template <typename T>
void _deque<T>::_reserve(int size) {
    if(size > _capacity()) {
        int count = m_dinamic_vector->_size();
        _dinamic_vector<_dinamic_vector<T>>* begin;
        if(size < 6) {
            begin = new _dinamic_vector<_dinamic_vector<T>>( 1 );
        } else if(size % 6 == 0) {
            begin = new _dinamic_vector<_dinamic_vector<T>>(size / 6);
        } else {
            begin = new _dinamic_vector<_dinamic_vector<T>>(size / 6 + 1);
        }

        int pos = m_dinamic_vector->m_begin - m_dinamic_vector->m_data;
        for(int i = 0;i < m_dinamic_vector->_size(); i++) {
            //std::cout << pos << std::endl;
            //(*begin)[i] = _dinamic_vector<T>(size);
            //begin[i] = m_dinamic_vector[pos];
            begin[i].m_data = m_dinamic_vector[pos].m_data;
            begin[i].m_begin = m_dinamic_vector[pos].m_begin;
            begin[i].m_end = m_dinamic_vector[pos].m_end;
            begin[i].m_capacity = m_dinamic_vector[pos].m_capacity;
            for(int i = 0;i < 6;i++) {
                std::cout  << i << " : " << (*m_dinamic_vector)[i / 6][i % 6] << " ";
                  }
                  std::cout << std::endl;
                  for(int i = 0;i < 6;i++) {
                    std::cout  << i << " : " << (*begin)[i / 6][i % 6] << " ";
                      }

            pos++;
            if(pos >= m_dinamic_vector->_capacity()) {
                pos = 0;
            }
        }
        //delete m_dinamic_vector;
        //m_dinamic_vector = begin;
        m_dinamic_vector->m_data = begin->m_data;
        m_dinamic_vector->m_end = m_dinamic_vector->m_data + begin->_size();
        m_dinamic_vector->m_begin = m_dinamic_vector->m_data;
        if(size < 6) {
            m_dinamic_vector->m_capacity = m_dinamic_vector->m_data + 1;

        } else if(size % 6 == 0) {
            m_dinamic_vector->m_capacity = m_dinamic_vector->m_data + size / 6;

        } else {
            m_dinamic_vector->m_capacity = m_dinamic_vector->m_data + size / 6 + 1;

        }
        //  m_dinamic_vector->m_data = begin->m_data;
        //  m_dinamic_vector->m_begin = m_dinamic_vector->m_data;
        //   m_dinamic_vector->m_end = m_dinamic_vector->m_data + count;
        //   m_dinamic_vector->m_capacity = m_dinamic_vector->m_data + 3; 
          //std::cout << begin->_size() << std::endl;
    }
}

int main() {
    _deque<int> deque(15); // Create a deque of integers
    for(int i = 0 ;i < deque._size(); i ++) {
        deque[i] = i;
    }
    deque._reserve(25);
    std::cout << "Size of deque: " << deque._size() << std::endl;
    std::cout << "Capacity of deque: " << deque._capacity() << std::endl;
    std::cout << "Empty?: " << deque._empty() << std::endl;
    
    std::cout << "[]: " << deque[10] << std::endl;
    //std::cout << "at: " << deque._at(10) << std::endl;
    return 0;
    //deque._size();
    // _dinamic_vector<int> vec1(10);
    // vec1[9] = 100;
    // vec1._push_front(5);
    //  vec1._push_front(4);
    //  vec1._push_front(3);
    //  vec1._push_front(2);
    //  vec1._push_front(1);
    //  vec1._push_front(10);
    //  vec1._reserve(32);
    //  //vec1._push_front(10);
    // //vec1._pop_front();
    // //vec1._pop_back();
    // for(int i = 0; i < vec1._capacity();i++) {
    //     if(i == vec1._size()) {
    //         std::cout << "|";
    //     }
    //     std::cout << vec1[i] << " ";
    // }
    // std::cout << std::endl;
    // std::cout << vec1._capacity() << std::endl;  
    // std::cout << vec1._size() << std::endl;

    // std::cout << vec1._back() << std::endl;
    return 0;
}