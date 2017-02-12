#ifndef TREE_MENU_H
#define TREE_MENU_H

#include <stdint.h>

#include "menu_item.h"

class TreeMenu
{
public:
    TreeMenu();
    TreeMenu(MenuItem* root_item, uint8_t* var_state_ptr, uint16_t state_print_help);
    bool select(MenuItem::IdType id);
    void getHelpLine(char* buffer);

private:
    void toParent();

private:
    MenuItem* current_item_;
    uint8_t* var_state_ptr_;
    uint16_t state_print_help_;

    MenuItem* help_current_item_;
    uint8_t print_help_state_;
    enum{
        state_print_header,
        state_print_h,
        state_print_r,
        state_print_items
    };
};

#endif
