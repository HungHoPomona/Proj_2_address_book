#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "address_book.h"
#include "address_book_fops.h"

void print_contact(const ContactInfo *contact)
{
    printf("Name: %s\n", contact->name[0]);
    printf("Phone: %s\n", contact->phone_numbers[0]);
    printf("Email: %s\n", contact->email_addresses[0]);
}

int main()
{
    AddressBook address_book;
    address_book.list = (ContactInfo *)malloc(sizeof(ContactInfo) * 100); // Allocate memory for 100 contacts
    address_book.count = 0;

    // Test loading contacts from file
    if (load_file(&address_book) == e_success)
    {
        printf("Contacts loaded successfully.\n");
    }
    else
    {
        printf("Failed to load contacts.\n");
    }

    // Test adding a contact
    ContactInfo new_contact;
    strcpy(new_contact.name[0], "John Doe");
    strcpy(new_contact.phone_numbers[0], "1234567890");
    strcpy(new_contact.email_addresses[0], "john.doe@example.com");
    new_contact.si_no = address_book.count + 1;

    if (add_contact(&address_book, &new_contact) == e_success)
    {
        printf("Contact added successfully.\n");
    }
    else
    {
        printf("Failed to add contact.\n");
    }

    // Test saving contacts to file
    if (save_file(&address_book) == e_success)
    {
        printf("Contacts saved successfully.\n");
    }
    else
    {
        printf("Failed to save contacts.\n");
    }

    // Test searching for a contact
    ContactInfo search_result;
    if (search_contact(&address_book, "John Doe", &search_result) == e_success)
    {
        printf("Contact found:\n");
        print_contact(&search_result);
    }
    else
    {
        printf("Contact not found.\n");
    }

    // Test editing a contact
    strcpy(new_contact.phone_numbers[0], "0987654321");
    if (edit_contact(&address_book, 0, &new_contact) == e_success)
    {
        printf("Contact edited successfully.\n");
    }
    else
    {
        printf("Failed to edit contact.\n");
    }

    // Test deleting a contact
    if (delete_contact(&address_book, 0) == e_success)
    {
        printf("Contact deleted successfully.\n");
    }
    else
    {
        printf("Failed to delete contact.\n");
    }

    // Test listing all contacts
    printf("Listing all contacts:\n");
    list_contacts(&address_book);

    // Free allocated memory
    free_address_book(&address_book);

    return 0;
}