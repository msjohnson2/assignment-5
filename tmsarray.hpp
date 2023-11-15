// tmsarray.hpp
// Matthew Johnson
// 10/26/2023
// class that implements a mildly smart array via dynamic memory allocation

#pragma once
// for single inclusion

#include <cstddef>
// For std::size_t

#include <algorithm>
// For std::max
// For std::copy
// For std::swap
// For std::rotate



// *********************************************************************
// class MSArray - Class definition
// *********************************************************************


// class MSArray
// Marvelously Smart Array of int.
// Resizable, copyable/movable, exception-safe.
// Invariants:
//     0 <= _size <= _capacity.
//     _data points to an array of value_type, allocated with new [],
//      owned by *this, holding _capacity value_type values -- UNLESS
//      _capacity == 0, in which case _data may be nullptr.

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
    // Exception-Neutral
    // Pre: None
    // Post: 
    //      TMSArray is constructed with set size capacity and allocated memory
    explicit TMSArray(size_type thesize=0) // =0 makes size 0 if parameter is not filled
        :_capacity(std::max(thesize, size_type(DEFAULT_CAP))), // _capacity must be declared before _data
         _size(thesize),
         _data(_capacity == 0 ? nullptr : new value_type[_capacity]) // ? : acts as if else statment
    {}


    // Copy ctor
    // Strong Guarantee
    // Exception-Neutral
    // Pre:
    //      other must be same type as this
    // Post: 
    //      TMSArray is constructed with set size capacity and allocated memory
    //      TMSArray is a copy of other and other is unmodifed
    TMSArray(const TMSArray & other)
        :_capacity(other._capacity),
         _size(other.size()),
         _data(other._capacity == 0 ? nullptr : new value_type[other._capacity]) // ? : acts as if else statment
    {   
        try
        {
            // copy must be in a try block because it might fail and leak memory
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
    // Exception-Neutral
    // Pre:
    //      other must be same type as this
    // Post: 
    //      TMSArray is constructed with set size capacity and allocated memory
    //      TMSArray is a copy of other and other is set to nothing
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
    // Exception-Neutral
    // Pre:
    //      other must be same type as this
    // Post: 
    //      TMSArray is constructed with set size capacity and allocated memory
    //      TMSArray is a copy of other and other is unmodifed
    TMSArray & operator=(const TMSArray & other)
    {
        TMSArray copy(other);
        swap(copy);
        return *this;
    }


    // Move assignment operator
    // No-Throw Guarantee
    // Exception-Neutral
    // Pre:
    //      other must be same type as this
    // Post: 
    //      TMSArray is constructed with set size capacity and allocated memory
    //      TMSArray is a copy of other and other is modifed
    TMSArray & operator=(TMSArray && other) noexcept
    {
        swap(other);
        return *this; // DUMMY
    }


    // Dctor
    // No-Throw Guarantee
    // Pre: None
    // Post: None
    ~TMSArray()
    {
        delete [] _data;
    }



// ***** TMSArray: general public operators *****
public:


    // operator[] - non-const & const
    // No-Throw Guarantee
    // Exception-Neutral
    // Pre:
    //      0 <= index < size
    // Post: 
    //      Returns _data at index
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
    // Exception-Neutral
    // Pre: None
    // Post: 
    //      Returns _size
    size_type size() const noexcept
    {
        return _size;
    }


    // empty
    // No-Throw Guarantee
    // Exception-Neutral
    // Pre: None
    // Post: 
    //      Returns size() == 0
    bool empty() const noexcept
    {
        return size() == 0;
    }


    // begin - non-const & const
    // No-Throw Guarantee
    // Exception-Neutral
    // Pre: None
    // Post: 
    //      Returns iterator to first positiion in array
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
    // Exception-Neutral
    // Pre: None
    // Post: 
    //      Returns iterator to positiion past last data point in array
    iterator end() noexcept
    {
        return begin() + size();
    }
    const_iterator end() const noexcept
    {
        return begin() + size();
    }


    // resize
    // Strong Guarantee
    // Exception-Neutral
    // Pre: 
    //      newsize > 0
    // Post: 
    //      _size == newsize
    //      if new capacity is needed: _capacity == newCapacity and _data == newData
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
                delete [] newData; // if copy fails and data that was copied is deleted
                throw;
            }

            _capacity = newCapacity;

            delete [] _data; // _data is no longer needed bc newData has been sucsessfully allocated and copied
            _data = newData;
            
        }   

        _size = newsize;
            
    }


    // insert
    // Strong Guarantee
    // Exception-Neutral
    // Pre:
    //     0 <= pos < _size
    // Post: 
    //      ++_size
    //      item is inserted at correct position and rest of the data points are moved back
    //      returns iterator at new item position
    iterator insert(iterator pos, const value_type & item)
    {
        size_type diff = pos - begin(); // gets items distance from begining

        resize(size() + 1);
        _data[size()-1] = item;
        std::rotate(begin()+diff, end() - 1, end());

        return begin() + diff;  // uses diff to return correct iterator
    }
        


    // erase
    // No-Throw Guarantee
    // Exception-Neutral
    // Pre:
    //     0 <= pos < _size
    //     _size > 0
    // Post: 
    //      --_size
    //      item is erased at correct position and rest of the data points are moved foward
    //      returns iterator at erased item position
    iterator erase(iterator pos) noexcept
    {
        std::rotate(pos, pos+1, end()); 
        this->resize(size() - 1);
        return pos;
    }


    // push_back
    // Strong Guarantee
    // Exception-Neutral
    // Pre: None
    // Post: 
    //      ++_size
    //      item is inserted at end of list
    void push_back(const value_type & item)
    {
        insert(end(), item);
    }

    // pop_back
    // No-Throw Guarantee
    // Exception-Neutral
    // Pre:
    //     container size must be greater than 0 
    // Post: 
    //      --_size
    //      item is removed from end of list
    void pop_back() noexcept
    {
        erase(end()-1);
    }


    // swap
    // No-Throw Guarantee
    // Exception-Neutral
    // Pre: None
    // Post: None
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