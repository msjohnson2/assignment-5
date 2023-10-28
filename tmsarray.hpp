// tmsarray.hpp
// Matthew Johnson
// 10/26/2023
// 

#pragma once
// for single inclusion

#include <cstddef>
// For std::size_t
#include <algorithm>
// For std::max
// For std::copy
// For std::swap
// For std::rotate


template <typename Valtype>
class TMSArray
{

public:


    using value_type = Valtype;

    using size_type  = std::size_t;

    using iterator = value_type*;

    using const_iterator = const value_type*;


private:


    // Capacity of default-constructed object
    enum { DEFAULT_CAP = 42 };


// ***** TMSArray: ctors, op=, dctor *****
public:


    // Default ctor & ctor from size
    // Strong Guarantee
    explicit TMSArray(size_type thesize=0)
        :_capacity(std::max(thesize, size_type(DEFAULT_CAP))), // _capacity must be declared before _data
         _size(thesize),
         _data(_capacity == 0 ? nullptr : new value_type[_capacity]) // ? : acts as if else statment
    {}


    // Copy ctor
    // Strong Guarantee
    TMSArray(const TMSArray & other)
        :_capacity(other._capacity),
         _size(other.size()),
         _data(other._capacity == 0 ? nullptr : new value_type[other._capacity]) // ? : acts as if else statment
        // The above call to std::copy does not throw, since it copies int
        // values. But if value_type is changed, then the call may throw, in
        // which case this copy ctor may need to be rewritten.
    {
       // std::copy(other.begin(), other.end(), begin());
        
        try
        {
            std::copy(other.begin(), other.end(), begin());
        }
        catch(...)
        {
            delete [] _data;
            throw;
        }
        
        
    }


    // Move ctor
    // No-Throw Guarantee
    TMSArray(TMSArray && other) noexcept
        :_capacity(other._capacity),
        _size(other._size),
        _data(other._data)
    {
        other._capacity = 0;
        other._size = 0;
        other._data = nullptr;
    }


    // Copy assignment operator
    // Strong Guarantee
    TMSArray & operator=(const TMSArray & other)
    {
        TMSArray copy(other);
        swap(copy);
        return *this;
    }


    // Move assignment operator
    // No-Throw Guarantee
    TMSArray & operator=(TMSArray && other) noexcept
    {
        swap(other);
        return *this; // DUMMY
    }


    // Dctor
    // No-Throw Guarantee
    ~TMSArray()
    {
        delete [] _data;
    }



// ***** TMSArray: general public operators *****
public:


    // operator[] - non-const & const
    // Pre:
    //     ???
    // No-Throw Guarantee
    value_type & operator[](size_type index)
    {
        return _data[index];
    }
    const value_type & operator[](size_type index) const
    {
        return _data[index];
    }


// ***** TMSArray: general public functions *****
public:


    // size
    // No-Throw Guarantee
    size_type size() const noexcept
    {
        return _size;
    }


    // empty
    // No-Throw Guarantee
    bool empty() const noexcept
    {
        return size() == 0;
    }


    // begin - non-const & const
    // No-Throw Guarantee
    iterator begin() noexcept
    {
        return _data;
    }
    const_iterator begin() const noexcept
    {
        return _data;
    }


    // end - non-const & const
    // No-Throw Guarantee
    iterator end() noexcept
    {
        return begin() + size();
    }
    const_iterator end() const noexcept
    {
        return begin() + size();
    }


    // resize
    // ??? Guarantee
    void resize(size_type newsize)
    {
        if(newsize > _capacity)
        {
            size_type newCapacity = std::max(_capacity * 2, newsize);
            value_type * newData = new value_type[newCapacity];

            try
            {
                std::copy(this->begin(), this->end(), newData);
            }
            catch(...)
            {
                delete [] newData;
                throw;
            }

            _capacity = newCapacity;

            delete [] _data;
            _data = newData;
            
        }   

        _size = newsize;
            
    }


    // insert
    // Pre:
    //     ???
    // ??? Guarantee
    iterator insert(iterator pos, value_type item)
        // Above, passing by value is appropriate, since our value type
        // is int. However, if the value type is changed, then a
        // different parameter-passing method may need to be used.
    {
        size_type diff = pos - begin();
        resize(size() + 1);
        _data[size()-1] = item;
        std::rotate(begin()+diff, end() - 1, end());
        return begin() + diff;  // need to be fixed
    }
        


    // erase
    // Pre:
    //     ???
    // ??? Guarantee
    iterator erase(iterator pos)
    {
        std::rotate(pos, pos+1, end()); 
        this->resize(size() - 1);
        return pos;  // DUMMY
    }


    // push_back
    // ??? Guarantee
    void push_back(value_type item)
        // Above, passing by value is appropriate, since our value type
        // is int. However, if the value type is changed, then a
        // different parameter-passing method may need to be used.
    {
        insert(end(), item);
    }

    // pop_back
    // Pre:
    //     ???
    // ??? Guarantee
    void pop_back()
    {
        erase(end()-1);
    }


    // swap
    // No-Throw Guarantee
    void swap(TMSArray & other) noexcept
    {
        std::swap(this->_capacity, other._capacity);
        std::swap(this->_size, other._size);
        std::swap(this->_data, other._data);

    }

// ***** TMSArray: data members *****
private:

    size_type    _capacity;  
    size_type    _size;     
    value_type * _data;     

}; // end of class