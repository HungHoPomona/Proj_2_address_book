#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <ctype.h>

#include "address_book.h"
#include "address_book_fops.h"

Status load_file(AddressBook *address_book)
{
	struct stat st;
	int ret = stat(DEFAULT_FILE, &st);

	/* 
	 * Check for file existance
	 */

	if (ret == 0)
	{
		/* 
		 * Do the neccessary step to open the file
		 * Do error handling
		 */
		address_book->fp = fopen(DEFAULT_FILE, "r");
        if (address_book->fp == NULL)
        {
            perror("Error opening file");
            return e_fail;
        }

        /* Read the file and populate address_book */
        address_book->count = 0; // Initialize count
        while (fscanf(address_book->fp, "%[^,],%[^,],%[^\n]\n",
                      address_book->list[address_book->count].name[0],
                      address_book->list[address_book->count].phone_numbers[0],
                      address_book->list[address_book->count].email_addresses[0]) == 3)
        {
            address_book->list[address_book->count].si_no = address_book->count + 1;
            address_book->count++;
            if (address_book->count >= MAX_CONTACTS) break; // Prevent overflow
        }
        fclose(address_book->fp);

	}
	else
	{
		/* Create a file for adding entries */
		/* Create a file for adding entries */
        address_book->fp = fopen(DEFAULT_FILE, "w");
        if (address_book->fp == NULL)
        {
            return e_fail;
        }
        fclose(address_book->fp);

	}

	return e_success;
}

Status save_file(AddressBook *address_book)
{
	/*
	 * Write contacts back to file.
	 * Re write the complete file currently
	 */ 

	
    address_book->fp = fopen(DEFAULT_FILE, "w");

	if (address_book->fp == NULL)
	{
		return e_fail;
	}

	/* 
	 * Add the logic to save the file
	 * Make sure to do error handling
	 */

    for (int i = 0; i < address_book->count; i++)
    {
        fprintf(address_book->fp, "%s,%s,%s\n",
                address_book->list[i].name[0],
                address_book->list[i].phone_numbers[0],
                address_book->list[i].email_addresses[0]);
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
