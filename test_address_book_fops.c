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

int main()
{
    AddressBook address_book;
    address_book.list = (ContactInfo *)malloc(sizeof(ContactInfo) * MAX_CONTACTS); // Allocate memory for contacts
    address_book.count = 0;

    // Test loading contacts from file
    printf("Testing load_file...\n");
    if (load_file(&address_book) == e_success)
    {
        printf("Contacts loaded successfully. Total contacts: %d\n", address_book.count);
    }
    else
    {
        printf("Failed to load contacts.\n");
    }

    // Test adding a contact
    printf("\nTesting add_contact...\n");
    ContactInfo new_contact;
    memset(&new_contact, 0, sizeof(ContactInfo));
    strcpy(new_contact.name[0], "John Doe");
    strcpy(new_contact.phone_numbers[0], "123-456-7890");
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
    printf("\nTesting save_file...\n");
    if (save_file(&address_book) == e_success)
    {
        printf("Contacts saved successfully.\n");
    }
    else
    {
        printf("Failed to save contacts.\n");
    }

    // Test searching for a contact
    printf("\nTesting search_contact...\n");
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
    printf("\nTesting edit_contact...\n");
    strcpy(new_contact.phone_numbers[0], "098-765-4321");
    if (edit_contact(&address_book, 0, &new_contact) == e_success)
    {
        printf("Contact edited successfully.\n");
    }
    else
    {
        printf("Failed to edit contact.\n");
    }

    // Test deleting a contact
    printf("\nTesting delete_contact...\n");
    if (delete_contact(&address_book, 0) == e_success)
    {
        printf("Contact deleted successfully.\n");
    }
    else
    {
        printf("Failed to delete contact.\n");
    }

    // Test listing all contacts
    printf("\nTesting list_contacts...\n");
    list_contacts(&address_book);

    // Free allocated memory
    printf("\nTesting free_address_book...\n");
    free_address_book(&address_book);
    printf("Memory freed successfully.\n");

    return 0;
}