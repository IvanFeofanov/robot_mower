#ifndef TO_STATE_MENU_ITEM
#define TO_STATE_MENU_ITEM

#include "../utils/menu_item.h"
#include <avr/io.h>
class ToStateMenuItem: public MenuItem
{
public:
    ToStateMenuItem()
    {
        state_var_ptr_ = 0;
        target_state_ = 0;
    }

    ToStateMenuItem(char id, const char* text, uint8_t* state_var_ptr, uint16_t target_state):
        MenuItem(id, text)
    {
        state_var_ptr_  = state_var_ptr;
        target_state_   = target_state;
    }

protected:
    MenuItem* callBack()
    {
        *state_var_ptr_ = target_state_;

        //to root
        MenuItem* item = this;
        while(item->parent != 0)
            item = item->parent;

        return item;
        // return MenuItem::callBack();
    }

private:
    uint8_t* state_var_ptr_;
    uint16_t target_state_;
};

#endif
