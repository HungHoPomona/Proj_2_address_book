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
    if (address_book->count == 0)
    {
        printf("No contacts available to search.\n");
        return;
    }

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
    if (address_book->count == 0)
    {
        printf("No contacts available to edit.\n");
        return;
    }

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
    if (address_book->count == 0)
    {
        printf("No contacts available to delete.\n");
        return;
    }

    if (delete_contact_2(address_book) == e_success)
    {
        printf("Contact deleted successfully. Total contacts: %d\n", address_book->count);
    }
    else
    {
        printf("Failed to delete contact.\n");
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

void test_is_valid_phone_number()
{
    printf("Testing is_valid_phone_number...\n");
    const char *valid_numbers[] = {"123-456-7890", "9876543210"};
    const char *invalid_numbers[] = {"12345", "abc-def-ghij", "123-4567-890"};

    for (int i = 0; i < 2; i++)
    {
        if (is_valid_phone_number(valid_numbers[i]))
        {
            printf("Valid phone number passed: %s\n", valid_numbers[i]);
        }
        else
        {
            printf("Valid phone number failed: %s\n", valid_numbers[i]);
        }
    }

    for (int i = 0; i < 3; i++)
    {
        if (!is_valid_phone_number(invalid_numbers[i]))
        {
            printf("Invalid phone number passed: %s\n", invalid_numbers[i]);
        }
        else
        {
            printf("Invalid phone number failed: %s\n", invalid_numbers[i]);
        }
    }
}

void test_is_valid_email()
{
    printf("Testing is_valid_email...\n");
    const char *valid_emails[] = {"test@example.com", "user@domain.org"};
    const char *invalid_emails[] = {"test@", "@domain.com", "userdomain.com"};

    for (int i = 0; i < 2; i++)
    {
        if (is_valid_email(valid_emails[i]))
        {
            printf("Valid email passed: %s\n", valid_emails[i]);
        }
        else
        {
            printf("Valid email failed: %s\n", valid_emails[i]);
        }
    }

    for (int i = 0; i < 3; i++)
    {
        if (!is_valid_email(invalid_emails[i]))
        {
            printf("Invalid email passed: %s\n", invalid_emails[i]);
        }
        else
        {
            printf("Invalid email failed: %s\n", invalid_emails[i]);
        }
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
    test_is_valid_phone_number();
    test_is_valid_email();

    free(address_book.list); // Free allocated memory
    printf("All tests completed.\n");

    return 0;
}