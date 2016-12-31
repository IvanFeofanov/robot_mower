#ifndef MENU_ITEM_H
#define MENU_ITEM_H

class  MenuItem
{
public:
    typedef char IdType;

    MenuItem();
    MenuItem(IdType item_id, const char* item_text);

    MenuItem* select(IdType item_id);
    void addNext(MenuItem* next_item);
    void addChild(MenuItem* child_item);

    const char*  text;
    IdType       id;

    MenuItem* prev;
    MenuItem* next;
    MenuItem* parent;
    MenuItem* child;

protected:
    virtual MenuItem* callBack();
};


#endif
