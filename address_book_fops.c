#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <ctype.h>

#include "address_book.h"
#include "address_book_fops.h"
#include "address_book_menu.h" // Include this header for is_valid_phone_number, format_phone_number, and is_valid_email

Status load_file(AddressBook *address_book)
{
    struct stat st;
    int ret = stat(DEFAULT_FILE, &st);

    if (ret != 0) // File does not exist
    {
        printf("File '%s' does not exist. Creating a new file.\n", DEFAULT_FILE);
        address_book->fp = fopen(DEFAULT_FILE, "w");
        if (address_book->fp == NULL)
        {
            perror("Error creating file");
            return e_fail;
        }
        fclose(address_book->fp);

        address_book->count = 0; // Initialize count to 0
        return e_success;
    }

    address_book->fp = fopen(DEFAULT_FILE, "r");
    if (address_book->fp == NULL)
    {
        perror("Error opening file");
        return e_fail;
    }

    // Preallocate memory for contacts
    address_book->list = (ContactInfo *)malloc(MAX_CONTACTS * sizeof(ContactInfo));
    if (address_book->list == NULL)
    {
        perror("Error allocating memory for contacts");
        fclose(address_book->fp);
        return e_fail;
    }

    address_book->count = 0; // Initialize count to 0
    char line[1024];

    while (fgets(line, sizeof(line), address_book->fp) != NULL && address_book->count < MAX_CONTACTS)
    {
        // Skip empty lines
        if (line[0] == '\n' || line[0] == '\r')
            continue;

        ContactInfo *contact = &address_book->list[address_book->count];
        memset(contact, 0, sizeof(ContactInfo)); // Initialize memory to zero

        char *token = strtok(line, ",");
        int field_index = 0;
        int phone_index = 0, email_index = 0;

        while (token != NULL)
        {
            token[strcspn(token, "\r\n")] = '\0'; // Remove trailing newline or carriage return characters

            if (field_index == 0) // Name
            {
                strncpy(contact->name[0], token, NAME_LEN - 1);
                contact->name[0][NAME_LEN - 1] = '\0';
            }
            else if (field_index >= 1) // Phone numbers or email addresses
            {
                if (is_valid_phone_number(token) && phone_index < PHONE_NUMBER_COUNT)
                {
                    strncpy(contact->phone_numbers[phone_index], token, NUMBER_LEN - 1);
                    contact->phone_numbers[phone_index][NUMBER_LEN - 1] = '\0';
                    phone_index++;
                }
                else if (is_valid_email(token) && email_index < EMAIL_ID_COUNT)
                {
                    strncpy(contact->email_addresses[email_index], token, EMAIL_ID_LEN - 1);
                    contact->email_addresses[email_index][EMAIL_ID_LEN - 1] = '\0';
                    email_index++;
                }
            }

            token = strtok(NULL, ",");
            field_index++;
        }

        // Validate that at least a name exists
        if (strlen(contact->name[0]) == 0)
        {
            printf("Warning: Skipping invalid contact with no name.\n");
        }
        else
        {
            contact->si_no = address_book->count + 1; // Assign serial number
            address_book->count++;
        }
    }

    fclose(address_book->fp);
    return e_success;
}

Status save_file(AddressBook *address_book)
{
    address_book->fp = fopen(DEFAULT_FILE, "w");
    if (address_book->fp == NULL)
    {
        perror("Error opening file for saving");
        return e_fail;
    }

    for (int i = 0; i < address_book->count; i++)
    {
        ContactInfo *contact = &address_book->list[i];

        // Save the name
        fprintf(address_book->fp, "%s,", contact->name[0]);

        // Save up to 5 phone numbers
        for (int j = 0; j < PHONE_NUMBER_COUNT; j++)
        {
            fprintf(address_book->fp, "%s,", contact->phone_numbers[j]);
        }

        // Save up to 5 email addresses
        for (int j = 0; j < EMAIL_ID_COUNT; j++)
        {
            fprintf(address_book->fp, "%s,", contact->email_addresses[j]);
        }

        fprintf(address_book->fp, "\n"); // End of contact
    }

    fclose(address_book->fp);
    return e_success;
}

Status add_contact(AddressBook *address_book, const ContactInfo *contact)
{
    if (address_book->count >= 100) // Assuming a maximum of 100 contacts
    {
        return e_fail;
    }

    address_book->list[address_book->count++] = *contact;
    return e_success;
}

Status search_contact(const AddressBook *address_book, const char *name, ContactInfo *result)
{
    for (int i = 0; i < address_book->count; i++)
    {
        if (strcmp(address_book->list[i].name[0], name) == 0)
        {
            *result = address_book->list[i];
            return e_success;
        }
    }
    return e_no_match;
}

Status edit_contact(AddressBook *address_book, int index, const ContactInfo *new_contact)
{
    if (index < 0 || index >= address_book->count)
    {
        return e_fail;
    }

    address_book->list[index] = *new_contact;
    return e_success;
}

Status delete_contact(AddressBook *address_book, int index)
{
    if (index < 0 || index >= address_book->count)
    {
        return e_fail;
    }

    for (int i = index; i < address_book->count - 1; i++)
    {
        address_book->list[i] = address_book->list[i + 1];
    }
    
    address_book->count--;
    return e_success;
}

void print_pattern(); // Forward declaration of print_pattern

void list_contacts(const AddressBook *address_book)
{
	if (address_book->count == 0)
	{
		printf("No contacts available.\n");
		return;
	}

	print_pattern();
	printf(": %6s : %32s : %32s : %32s :\n", "S.No", "Name", "Phone No.", "Email ID");
	print_pattern();

	for (int i = 0; i < address_book->count; i++)
	{
		printf(": %6d : %32s : %32s : %32s :\n",
		       address_book->list[i].si_no,
		       address_book->list[i].name[0],
		       address_book->list[i].phone_numbers[0],
		       address_book->list[i].email_addresses[0]);
	}

	print_pattern();
}

void free_address_book(AddressBook *address_book)
{
    free(address_book->list);
}
