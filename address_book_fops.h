#ifndef ABK_FILEOPS_H
#define ABK_FILEOPS_H

#include "address_book.h"

Status load_file(AddressBook *address_book);
Status save_file(AddressBook *address_book);
Status add_contact(AddressBook *address_book, const ContactInfo *contact);
Status search_contact(const AddressBook *address_book, const char *name, ContactInfo *result);
Status edit_contact(AddressBook *address_book, int index, const ContactInfo *new_contact);
Status delete_contact(AddressBook *address_book, int index);
void list_contacts(const AddressBook *address_book);
void free_address_book(AddressBook *address_book);
#endif
