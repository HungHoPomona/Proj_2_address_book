#ifndef ADDRESS_BOOK_MENU_H
#define ADDRESS_BOOK_MENU_H

#include "address_book.h"

#define	NONE							0
#define	NUM								1
#define	CHAR							2

#define WINDOW_SIZE						5

Status save_file(AddressBook *address_book);

void menu_header(const char *str);
void main_menu(void);
Status menu(AddressBook *address_book);
Status save_prompt(AddressBook *address_book);
Status add_contacts(AddressBook *address_book);
Status search_contact_2(AddressBook *address_book);
Status edit_contact_2(AddressBook *address_book);
Status delete_contact_2(AddressBook *address_book);
Status list_contacts_2(AddressBook *address_book, const char *title, int *index, const char *msg, Modes mode);
void edit_print_result(AddressBook *address_book, int index);
int get_option(int type, const char *msg);

bool_t is_valid_phone_number(const char *phone);
void format_phone_number(char *phone);
bool_t is_valid_email(const char *email);
 
#endif
