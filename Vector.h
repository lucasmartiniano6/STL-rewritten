#include <initializer_list>
#include <iterator>
#include <cstddef>

template<typename T>
class Vector
{
public:
    Vector()
    {
        ReAlloc(2);
    }
    Vector(std::initializer_list<T> args)
    {
        ReAlloc(2);
        for(auto i : args) push_back(i);
    }
    Vector(const Vector<T>& other)
    {
        this->m_Data = (T*)::operator new(other.m_Capacity*sizeof(T));
        for(size_t i=0; i<other.size(); ++i)
            new(&m_Data[i]) T(other.m_Data[i]);
        
        this->m_Capacity = other.m_Capacity;
        this->m_Size = other.m_Size;
    }
    Vector(Vector<T>&& other) noexcept
    {
        this->m_Data = other.m_Data;
        this->m_Capacity = other.m_Capacity; 
        this->m_Size = other.m_Size;

        other.m_Data = nullptr;
        other.m_Capacity = 0;
        other.m_Size = 0;
    }   
    ~Vector()
    {
        clear();
        ::operator delete(m_Data, m_Capacity*sizeof(T));
    }

    void clear()
    {
        for(size_t i=0; i<m_Size; ++i)
            m_Data[i].~T();
        m_Size = 0;
    }

    void push_back(const T& value)
    {
        if(m_Size >= m_Capacity)
        {
            ReAlloc(m_Capacity + m_Capacity/2);
        }

        new(&m_Data[m_Size]) T(value);
        m_Size++;
    }

    void pop_back()
    {
        if(m_Size>0)
        {
            m_Size--;
            m_Data[m_Size].~T();
        }
    }

    T& front() { return m_Data[0];}
    T& back() { return m_Data[m_Size-1];}

    size_t size() const {return m_Size;}

    bool empty() const noexcept { return m_Size==0 ? true : false;}

    T& operator[](size_t index)
    {
        return m_Data[index];
    }

    const T& operator[](size_t index) const
    {
        return m_Data[index];
    }

    Vector& operator=(const Vector& other)
    {
        if(this == &other) return *this;

        this->clear();
        ::operator delete(this->m_Data, this->m_Capacity*sizeof(T));

        this->m_Data = (T*)::operator new(other.m_Capacity*sizeof(T));
        for(size_t i=0; i<other.size(); ++i)
            new(&m_Data[i]) T(other.m_Data[i]);
        
        this->m_Capacity = other.m_Capacity;
        this->m_Size = other.m_Size;

        return *this;
    }
    Vector& operator=(Vector&& other) noexcept
    {
        if(this == &other) return *this;

        this->clear();
        ::operator delete(this->m_Data, this->m_Capacity*sizeof(T));
         
        this->m_Data = other.m_Data;
        this->m_Capacity = other.m_Capacity; 
        this->m_Size = other.m_Size;

        other.m_Data = nullptr;
        other.m_Capacity = 0;
        other.m_Size = 0;
        
        return *this;
    }   
public:
    class iterator 
    {
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = T*;
        using reference = T&;
    public:
        explicit iterator(pointer __ptr) : m_ptr(__ptr) {}
        iterator& operator++() {m_ptr++; return *this;} 
        iterator operator++(int) {iterator retval=*this; ++(*this); return retval;} 
        bool operator==(iterator other) const { return m_ptr == other.m_ptr;}
        bool operator!=(iterator other) const { return !(*this==other);}
        reference operator*() const { return *m_ptr;}
        reference operator[](size_t offset) { return *(m_ptr+offset);}
        pointer operator->() { return m_ptr;}

        iterator operator+(size_t val) {iterator temp=*this; temp.m_ptr+=val; return temp; }
        iterator operator-(size_t val) {iterator temp=*this; temp.m_ptr-=val; return temp; }
    private:
        pointer m_ptr;
     };

    iterator begin() { return iterator(m_Data); }
    iterator end() { return iterator(m_Data+m_Size); }

    iterator erase(iterator pos) 
    {
        (*pos).~T();
        --m_Size;
        for(; pos != this->end(); ++pos){
            new(&(*pos)) T(std::move(*(pos+1)));
        }
        return pos;
    }
    iterator erase(iterator first, iterator last)
    {
        size_t delCount = 0;
        for(auto it = first; it != last; ++it){
            (*it).~T();
            ++delCount;
        }
        for(; last != this->end(); ++last, ++first)
            new(&(*(first))) T(std::move(*last));

        m_Size -= delCount;
        return last;
    }
private:
void ReAlloc(size_t newCapacity)
{
    if(newCapacity < m_Size)
        m_Size = newCapacity;

    T* newBlock = (T*)::operator new(newCapacity*sizeof(T));

    for(size_t i=0; i<m_Size; ++i)
        new(&newBlock[i]) T(std::move(m_Data[i]));

    for(size_t i=0; i<m_Size; ++i)
        m_Data[i].~T();

    ::operator delete(m_Data, m_Capacity*sizeof(T));
    m_Data = newBlock;
    m_Capacity = newCapacity;
}
private:
    T* m_Data = nullptr;
    size_t m_Capacity = 0;
    size_t m_Size = 0;
};
