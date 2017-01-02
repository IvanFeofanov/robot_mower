#include "tree_menu.h"

#include <string.h>
#include <stdio.h>

TreeMenu::TreeMenu()
{
    current_item_       = 0;
    var_state_ptr_      = 0;
    state_print_help_   = 0;

    help_current_item_ = 0;
    print_help_state_  = 0;
}

TreeMenu::TreeMenu(MenuItem* root_item, uint8_t* var_state_ptr,
        uint16_t state_print_help)
{
    current_item_       = root_item;
    var_state_ptr_      = var_state_ptr;
    state_print_help_   = state_print_help;

    help_current_item_ = current_item_->child;
    print_help_state_  = state_print_header;
}

bool TreeMenu::select(MenuItem::IdType id)
{
    if(id == ' ' || id == '\n' || id == '\r')
        return true;

    if(id == 'H'){
        *var_state_ptr_ = state_print_help_;
        return true;
    }

    if(id == 'R'){
        toParent();
        return true;
    }

    MenuItem* new_item = current_item_->select(id);
    if(new_item != 0){
        current_item_ = new_item;
        return true;
    }

    return false;
}

void TreeMenu::getHelpOneLine(char* buffer)
{
    switch(print_help_state_)
    {
    case state_print_header:
        strcpy(buffer, current_item_->text);
        help_current_item_ = current_item_->child;
        print_help_state_ = state_print_h;
        break;

    case state_print_h:
        strcpy(buffer, "H - help");
        if(current_item_->parent != 0)
            print_help_state_ = state_print_r;
        else
            print_help_state_ = state_print_items;
        break;

    case state_print_r:
        strcpy(buffer, "R - return");
        print_help_state_ = state_print_items;
        break;

    case state_print_items:
        if(help_current_item_ != 0){
            sprintf(buffer, "%c - %s", help_current_item_->id,
                    help_current_item_->text);
            help_current_item_ = help_current_item_->next;
        }else{
            buffer[0] = '\0';
            print_help_state_ = state_print_header;
        }
        break;
    }
}

void TreeMenu::toParent()
{
    if(current_item_->parent != 0){
        current_item_ = current_item_->parent;
    }
}
