#pragma once

template<typename T>
struct ListNode
{
    ListNode(const char* _k, const T& e) : key(_k), data(e), next(0), prev(0) {}

    T data;
    const char* key = 0;
    ListNode<T>* next = 0;
    ListNode<T>* prev = 0;
};

template<typename T>
class List
{
public:
    List() : head_(0), tail_(0), size_(0)
    {
    }

    ~List()
    {
        this->Clear(); /* Remove all the items from the list */
    }

    int size() { return size_; }
    void push_back(const char* key, const T& e);
    void push_front(const char* key, const T& e);
    void Clear();

    T GetAt(int index);
    ListNode<T>* GetNodeAt(int index);
    T operator[](int index);
    int IndexOf(const T& e);

    void Remove(const char* key);
    void Remove(int index);
    void Remove(const T& e);
private:
    ListNode<T>* head_;
    ListNode<T>* tail_;

    int size_;

    ListNode<T>* insertInternal(const char* key, const T& e, ListNode<T>* pos);
    void removeInternal(const char* key, ListNode<T>* pos);

    //Iterators
public:
    class iterator
    {
    public:
        iterator(ListNode<T>* p = 0) : pos_(p) { }

        T& operator*()
        {
            return pos_->data;
        }

        T* operator->()
        {
            return &(pos_->data);
        }

        bool operator!=(const iterator& rhs)
        {
            return this->pos_ != rhs.pos_;
        }

        iterator operator++()
        {
            pos_ = pos_->next; return *this;
        }

        iterator operator--()
        {
            pos_ = pos_->prev; return *this;
        }

    private:
        ListNode<T>* pos_;
    };

    iterator begin()
    {
        return iterator(head_);
    }

    iterator end()
    {
        return iterator(0);
    }
};

/////////////
// Implementations
////////////
template<typename T>
ListNode<T>* List<T>::insertInternal(const char* key, const T& e, ListNode<T>* pos)
{
    ListNode<T>* n = new ListNode<T>(key, e);
    size_++;
    // no operation below this should throw
    // as state of the list has changed and memory allocated
    n->next = pos;
    if (pos)
    {
        n->prev = pos->prev;
        pos->prev = n;
    }
    else
    {
        // pos is null that is at the very end of the list
        n->prev = tail_;
        tail_ = n;
    }
    if (n->prev)
    {
        n->prev->next = n;
    }
    else
    {
        // at the very begining of the list
        head_ = n;
    }

    return n;
}

template<typename T>
void List<T>::push_back(const char* key, const T& e)
{
    // inserts before the position, 
    // 0 is the end() iterator
    // hence insert at the end
    insertInternal(key, e, 0);
}
template<typename T>
void List<T>::push_front(const char* key, const T& e)
{
    // insert before the head
    insertInternal(key, e, head_);
}

template<typename T>
void List<T>::removeInternal(const char* key, ListNode<T>* pos)
{
    if (pos)
    {
        if (pos->prev)
            pos->prev->next = pos->next;
        if (pos->next)
            pos->next->prev = pos->prev;
        if (pos == head_)
            head_ = pos->next;
        if (pos == tail_)
            tail_ = pos->prev;
        delete pos;
        size_--;
    }
}

template<typename T>
void List<T>::Remove(int index)
{
    ListNode<T>* cur = head_;
    for (int i = 0; i < index; ++i)
        cur = cur->next;
    removeInternal(cur->key, cur);
}

template<typename T>
void List<T>::Remove(const T& e)
{
    for (int i = 0; i < size_; i++)
        if (GetAt(i) == e)
            Remove(i);
}

template<typename T>
void List<T>::Remove(const char* key)
{
    for (int i = size_; i > 0; i--)
        if (*GetNodeAt(i)->key == *key)
            Remove(i);
}

template<typename T>
void List<T>::Clear()
{
    ListNode<T>* current(head_);

    while (current)
    {
        ListNode<T>* next(current->next);
        delete current;
        current = next;
    }
    size_ = 0; //Reset the size to 0
    head_ = 0;
    tail_ = 0;
}

template<typename T>
ListNode<T>* List<T>::GetNodeAt(int index)
{
    if (index == 0)
        return this->head_;
    else
    {
        ListNode<T>* cur = head_;
        for (int i = 0; i < index; ++i)
            cur = cur->next;

        return cur;
    }
}

template<typename T>
T List<T>::GetAt(int index)
{
    if (index == 0)
        return this->head_->data;
    else
    {
        ListNode<T>* cur = head_;
        for (int i = 0; i < index; ++i)
            cur = cur->next;

        T ret = cur->data;

        return ret;
    }
}

template<typename T>
T List<T>::operator[](int index)
{
    return GetAt(index);
}

template<typename T>
int List<T>::IndexOf(const T& e)
{
    for (int i = 0; i < size_; i++)
        if (GetAt(i) == e)
            return i;

    return -1;
}
