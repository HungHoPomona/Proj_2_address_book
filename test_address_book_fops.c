#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "address_book.h"
#include "address_book_fops.h"

void print_contact(const ContactInfo *contact)
{
    printf("ID: %d\n", contact->si_no);
    printf("Name: %s\n", contact->name[0]);
    for (int i = 0; i < PHONE_NUMBER_COUNT; i++)
    {
        if (strlen(contact->phone_numbers[i]) > 0)
            printf("Phone %d: %s\n", i + 1, contact->phone_numbers[i]);
    }
    for (int i = 0; i < EMAIL_ID_COUNT; i++)
    {
        if (strlen(contact->email_addresses[i]) > 0)
            printf("Email %d: %s\n", i + 1, contact->email_addresses[i]);
    }
    printf("\n");
}

void test_load_file(AddressBook *address_book)
{
    printf("Testing load_file...\n");
    if (load_file(address_book) == e_success)
    {
        printf("Contacts loaded successfully. Total contacts: %d\n", address_book->count);
    }
    else
    {
        printf("Failed to load contacts.\n");
    }
}

void test_save_file(AddressBook *address_book)
{
    printf("Testing save_file...\n");
    if (save_file(address_book) == e_success)
    {
        printf("Contacts saved successfully.\n");
    }
    else
    {
        printf("Failed to save contacts.\n");
    }
}

void test_add_contact(AddressBook *address_book)
{
    printf("Testing add_contact...\n");
    ContactInfo new_contact;
    memset(&new_contact, 0, sizeof(ContactInfo));
    strcpy(new_contact.name[0], "John Doe");
    strcpy(new_contact.phone_numbers[0], "123-456-7890");
    strcpy(new_contact.email_addresses[0], "john.doe@example.com");
    new_contact.si_no = address_book->count + 1;

    if (add_contact(address_book, &new_contact) == e_success)
    {
        printf("Contact added successfully. Total contacts: %d\n", address_book->count);
    }
    else
    {
        printf("Failed to add contact.\n");
    }
}

void test_search_contact(AddressBook *address_book)
{
    printf("Testing search_contact...\n");
    ContactInfo search_result;
    if (search_contact(address_book, "John Doe", &search_result) == e_success)
    {
        printf("Contact found:\n");
        print_contact(&search_result);
    }
    else
    {
        printf("Contact not found.\n");
    }
}

void test_edit_contact(AddressBook *address_book)
{
    printf("Testing edit_contact...\n");
    if (address_book->count == 0)
    {
        printf("No contacts available to edit.\n");
        return;
    }

    ContactInfo updated_contact = address_book->list[0];
    strcpy(updated_contact.phone_numbers[0], "098-765-4321");

    if (edit_contact(address_book, 0, &updated_contact) == e_success)
    {
        printf("Contact edited successfully.\n");
        print_contact(&address_book->list[0]);
    }
    else
    {
        printf("Failed to edit contact.\n");
    }
}

void test_delete_contact(AddressBook *address_book)
{
    printf("Testing delete_contact...\n");
    if (address_book->count == 0)
    {
        printf("No contacts available to delete.\n");
        return;
    }

    if (delete_contact(address_book, 0) == e_success)
    {
        printf("Contact deleted successfully. Total contacts: %d\n", address_book->count);
    }
    else
    {
        printf("Failed to delete contact.\n");
    }
}

void test_list_contacts(AddressBook *address_book)
{
    printf("Testing list_contacts...\n");
    if (address_book->count > 0)
    {
        list_contacts(address_book);
    }
    else
    {
        printf("No contacts to list.\n");
    }
}

void test_free_address_book(AddressBook *address_book)
{
    printf("Testing free_address_book...\n");
    free_address_book(address_book);
    printf("Memory freed successfully.\n");
}

int main()
{
    AddressBook address_book;
    address_book.list = (ContactInfo *)malloc(sizeof(ContactInfo) * MAX_CONTACTS); // Allocate memory for contacts
    address_book.count = 0;

    test_load_file(&address_book);
    test_add_contact(&address_book);
    test_save_file(&address_book);
    test_search_contact(&address_book);
    test_edit_contact(&address_book);
    test_delete_contact(&address_book);
    test_list_contacts(&address_book);
    test_free_address_book(&address_book);

    return 0;
}