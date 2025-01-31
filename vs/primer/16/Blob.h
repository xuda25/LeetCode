#ifndef BLOB_H
#define BLOB_H

#include <vector>
#include <memory>

template <typename> class BlobPtr;
template <typename> class Blob;

template <typename T>
bool operator==(const Blob<T>&, const Blob<T>&);

template <typename T>
class Blob
{
public:
    friend class BlobPtr<T>;
    friend bool operator==(const Blob<T>&, const Blob<T>&);

    typedef T value_type;
    typedef typename std::vector<T>::size_type size_type;
    template <typename I>
    Blob(I b, I e);
    Blob();
    Blob(std::initializer_list<T> il);
    size_type size() const {return data->size();}
    bool empty() const {return data->empty();}
    void push_back(const T& t) {data->push_back(t);}
    void push_back(T&& t) {data->push_back(std::move(T));}
    void pop_back();
    T& back();
    T& operator[] (size_type i);
private:
    std::shared_ptr<std::vector<T>> data;
    void check(size_type i, const std::string& msg) const;
};

template <typename T>
template <typename I>
Blob<T>::Blob(I b, I e) : data(std::make_shared<std::vector<T>> (b, e)) {}

template <typename T>
void Blob<T>::check(size_type i, const std::string& msg) const
{
    if (i >= data->size())
        throw std::out_of_range(msg);
}

template <typename T>
T& Blob<T>::back()
{
    check(0, "back on empty Blob.");
    return data->back();
}

template <typename T>
T& Blob<T>::operator[](size_type i)
{
    check(i, "subscript out of range");
    return (*data)[i];
}

template <typename T>
void Blob<T>::pop_back()
{
    check(0, "pop_back on empty Blob.");
    data->pop_back();
}

template <typename T>
Blob<T>::Blob() : data(std::make_shared<std::vector<T>>()) {}

template <typename T>
Blob<T>::Blob(std::initializer_list<T> il) : data(std::make_shared<std::vector<T>(il)) {}



template <typename T>
class BlobPtr
{
public:
    BlobPtr() : curr(0) {};
    BlobPtr(Blob<T>& a, size_t sz = 0) : wptr(a.data), curr(sz) {}
    T& operator*() const
    {
        auto p = check(curr, "dereference past end");
        return (*p)[curr];
    }  
    BlobPtr& operator++();
    BlobPtr& operator--();

private:
    std::shared_ptr<std::vector<T>> check(std::size_t, const std::string&) const;
    std::weak_ptr<std::vector<T>> wptr;
    std::size_t curr;
};


template <typename T>
BlobPtr<T>& BlobPtr<T>::operator++()
{
    return ++*this;
}


#endif