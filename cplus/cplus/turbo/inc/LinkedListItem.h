// ---------------------------------------------------------------------------------
// LinkedListItem.h
//
// The item can be put into any double linked list
//
// ----------------------------------------------------------------------------------
#ifndef __TURBO_LINKED_LIST_ITEM_H__
#define __TURBO_LINKED_LIST_ITEM_H__

namespace turbo {

template <typename T>
struct LinkedListItem {

    T item;
    LinkedListItem* pPrev;
    LinkedListItem* pNext;

    LinkedListItem()
        : item()
        , pPrev(0)
        , pNext(0)
    {
    }
    LinkedListItem(const T& another)
        : item(another)
        , pPrev(0)
        , pNext(0)
    {
    }

    LinkedListItem& operator=(const LinkedListItem& another)
    {
        item = another.item;
        return *this;
    }

}; // LinkedListItem

} // namespace turbo

#endif // __TURBO_LINKED_LIST_ITEM_H__