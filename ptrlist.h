#include <list>
#pragma once

template<typename T>
class PointerListIterator: public std::list<T*>::iterator
{
public:
    //PointerListIterator(const std::list<T*>::iterator&);

    //operator std::list<T*>::iterator();

    T& operator*();
};

template<typename T>
class PointerListConstIterator: public std::list<T*>::const_iterator
{
public:
    //PointerListConstIterator(const std::list<T*>::const_iterator&);

    //operator std::list<T*>::const_iterator();

    const T& operator*() const;
};

template<typename T>
class PointerListReverseIterator: public std::reverse_iterator<PointerListIterator<T> > {};
template<typename T>
class PointerListReverseConstIterator: public std::reverse_iterator<PointerListConstIterator<T> > {};

template<typename T>
class PointerList: public std::list<T*>
{
public:
    //PointerList();
    //PointerList(size_t, const T& = T());
    //PointerList(const PointerListIterator<T>&, const PointerListIterator<T>&);

    PointerListConstIterator<T> begin() const;
    PointerListConstIterator<T> end() const;
    PointerListReverseConstIterator<T> rbegin() const;
    PointerListReverseConstIterator<T> rend() const;
    PointerListIterator<T> begin();
    PointerListIterator<T> end();
    PointerListReverseIterator<T> rbegin();
    PointerListReverseIterator<T> rend();

    const T& front() const;
    const T& back() const;
    T& front();
    T& back();

    /*
    void assign(const PointerListIterator<T>&, const PointerListIterator<T>&);
    void assign(size_t, const T&);
    */
    void push_front(const T&);
    void push_back(const T&);
    PointerListIterator<T> insert(const PointerListIterator<T>&, const T&);
    /*
    void insert(const PointerListIterator<T>&, size_t, const T&);
    void insert(const PointerListIterator<T>&, const PointerListIterator<T>&, const PointerListIterator<T>&);
    */
    PointerListIterator<T> erase(const PointerListIterator<T>&);
    /*
    PointerListIterator<T> erase(const PointerListIterator<T>&, const PointerListIterator<T>&);
    void resize(size_t, T = T());
    */
};
