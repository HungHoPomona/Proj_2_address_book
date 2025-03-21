#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "address_book.h"
#include "address_book_menu.h"

void test_add_contacts(AddressBook *address_book)
{
    printf("Testing add_contacts...\n");
    add_contacts(address_book);
    if (address_book->count > 0)
    {
        printf("Contact added successfully. Total contacts: %d\n", address_book->count);
    }
    else
    {
        printf("Failed to add contact.\n");
    }
}

void test_search_contact_2(AddressBook *address_book)
{
    printf("Testing search_contact_2...\n");
    if (search_contact_2(address_book) == e_success)
    {
        printf("Search completed successfully.\n");
    }
    else
    {
        printf("No matching contact found.\n");
    }
}

void test_edit_contact_2(AddressBook *address_book)
{
    printf("Testing edit_contact_2...\n");
    if (edit_contact_2(address_book) == e_success)
    {
        printf("Contact edited successfully.\n");
    }
    else
    {
        printf("Failed to edit contact.\n");
    }
}

void test_delete_contact_2(AddressBook *address_book)
{
    printf("Testing delete_contact_2...\n");
    if (delete_contact_2(address_book) == e_success)
    {
        printf("Contact deleted successfully. Total contacts: %d\n", address_book->count);
    }
    else
    {
        printf("Failed to delete contact or no contacts available.\n");
    }
}

void test_list_contacts_2(AddressBook *address_book)
{
    printf("Testing list_contacts_2...\n");
    if (address_book->count > 0)
    {
        list_contacts_2(address_book, "List of Contacts", NULL, NULL, e_list);
        printf("Contacts listed successfully.\n");
    }
    else
    {
        printf("No contacts to list.\n");
    }
}

void test_save_prompt(AddressBook *address_book)
{
    printf("Testing save_prompt...\n");
    if (save_prompt(address_book) == e_success)
    {
        printf("Save prompt completed successfully.\n");
    }
    else
    {
        printf("Save prompt failed or canceled.\n");
    }
}

int main(void)
{
    AddressBook address_book;
    address_book.count = 0;
    address_book.list = (ContactInfo *)malloc(sizeof(ContactInfo) * MAX_CONTACTS); // Allocate memory for contacts

    printf("Starting tests for address_book_menu.c functions...\n");

    test_add_contacts(&address_book);
    test_search_contact_2(&address_book);
    test_edit_contact_2(&address_book);
    test_delete_contact_2(&address_book);
    test_list_contacts_2(&address_book);
    test_save_prompt(&address_book);

    free(address_book.list); // Free allocated memory
    printf("All tests completed.\n");

    return 0;
}