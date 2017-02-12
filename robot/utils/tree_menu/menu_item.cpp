#include "menu_item.h"

MenuItem::MenuItem()
{
    id  = 0;

    prev    = 0;
    next    = 0;
    parent  = 0;
    child   = 0;
}

MenuItem::MenuItem(IdType item_id, const char* item_text)
{
    id   = item_id;
    text = item_text;

    prev   = 0;
    next   = 0;
    parent = 0;
    child  = 0;
}

MenuItem* MenuItem::select(IdType item_id)
{
    MenuItem* item = child;
    while(item != 0 && item->id != item_id)
        item = item->next;

    if(item != 0 && item->child == 0){
        item = item->callBack();
    }

    return item;
}

void MenuItem::addNext(MenuItem* next_item)
{
    next = next_item;
    next->prev = this;
    next->parent = this->parent;
}

void MenuItem::addChild(MenuItem* child_item)
{
    child = child_item;
    child_item->parent = this;
}

MenuItem* MenuItem::callBack()
{
    return parent == 0 ? this : parent;
}


