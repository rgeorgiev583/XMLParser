#include "ptrlist.h"
#pragma once
/*
template<typename T>
PointerList<T>::PointerList(size_t n, const T& val = T()): std::list<T*>(n, &val) {}

template<typename T>
PointerList<T>::PointerList(const PointerListIterator& first, const PointerListIterator& last): std::list<T*>(first, last) {}
*/
template<typename T>
PointerListConstIterator<T> PointerList<T>::begin() const
{
    return std::list<T*>::begin();
}

template<typename T>
PointerListConstIterator<T> PointerList<T>::end() const
{
    return std::list<T*>::end();
}

template<typename T>
PointerListReverseConstIterator<T> PointerList<T>::rbegin() const
{
    return std::list<T*>::rbegin();
}

template<typename T>
PointerListReverseConstIterator<T> PointerList<T>::rend() const
{
    return std::list<T*>::rend();
}

template<typename T>
PointerListIterator<T> PointerList<T>::begin()
{
    return begin();//std::list<T*>::begin();
}

template<typename T>
PointerListIterator<T> PointerList<T>::end()
{
    return end();//std::list<T*>::end();
}

template<typename T>
PointerListReverseIterator<T> PointerList<T>::rbegin()
{
    return std::list<T*>::rbegin();
}

template<typename T>
PointerListReverseIterator<T> PointerList<T>::rend()
{
    return std::list<T*>::rend();
}

template<typename T>
const T& PointerList<T>::front() const
{
    return *std::list<T*>::front();
}

template<typename T>
const T& PointerList<T>::back() const
{
    return *std::list<T*>::back();
}

template<typename T>
T& PointerList<T>::front()
{
    return *std::list<T*>::front();
}

template<typename T>
T& PointerList<T>::back()
{
    return *std::list<T*>::back();
}
/*
template<typename T>
void PointerList<T>::assign(const PointerListIterator& first, const PointerListIterator& last)
{
    std::list<T*>::assign(first, last);
}

template<typename T>
void PointerList<T>::assign(size_t n, const T& val)
{
    std::list<T*>::assign(n, &val);
}
*/
template<typename T>
void PointerList<T>::push_front(const T& val)
{
    std::list<T*>::push_front(&val);
}

template<typename T>
void PointerList<T>::push_back(const T& val)
{
    std::list<T*>::push_back(&val);
}

template<typename T>
PointerListIterator<T> PointerList<T>::insert(const PointerListIterator<T>& position, const T& val)
{
    std::list<T*>::insert(position, &val);
}
/*
template<typename T>
void PointerList<T>::insert(const PointerListIterator& position, size_t n, const T& val)
{
    std::list<T*>::insert(position, n, &val);
}

template<typename T>
void PointerList<T>::insert(const PointerListIterator&, const PointerListIterator&, const PointerListIterator&)
{
    std::list<T*>::insert(position, first, last);
}
*/
template<typename T>
PointerListIterator<T> PointerList<T>::erase(const PointerListIterator<T>& position)
{
    std::list<T*>::erase(position);
}

/*
PointerListIterator PointerList<T>::erase(const PointerListIterator&, const PointerListIterator&);
template<typename T>
void PointerList<T>::resize(size_t n, const T& val = T())
{
    std::list<T*>::resize(n, &val);
}
*/


template<typename T>
T& PointerListIterator<T>::operator*()
{
    return *std::list<T*>::iterator::operator*();
}


template<typename T>
const T& PointerListConstIterator<T>::operator*() const
{
    return *std::list<T*>::const_iterator::operator*();
}
