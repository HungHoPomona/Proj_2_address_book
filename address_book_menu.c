#include <stdio.h>
//#include <stdio_ext.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "address_book.h"
#include "address_book_menu.h"
//#include "address_book_fops.h"

//#include "abk_fileops.h"
//#include "abk_log.h"
//#include "abk_menus.h"
//#include "abk.h"

int get_option(int type, const char *msg)
{
	/*
	 * Mutilfuction user intractions like
	 * Just an enter key detection
	 * Read an number
	 * Read a charcter
	 */ 

	/* Fill the code to add above functionality */
    char input[32];
    int option = -1;

    printf("%s", msg);
    fgets(input, sizeof(input), stdin);

    if (type == NUM)
    {
        option = atoi(input);
    }
    else if (type == CHAR)
    {
        option = input[0];
    }

    return option;
}

Status save_prompt(AddressBook *address_book)
{
	char option;

	do
	{
		main_menu();

		option = get_option(CHAR, "\rEnter 'N' to Ignore and 'Y' to Save: ");

		if (option == 'Y')
		{
			save_file(address_book);
			printf("Exiting. Data saved in %s\n", DEFAULT_FILE);

			break;
		}
	} while (option != 'N');

	free(address_book->list);

	return e_success;
}

Status list_contacts_2(AddressBook *address_book, const char *title, int *index, const char *msg, Modes mode)
{
	/* 
	 * Add code to list all the contacts availabe in address_book.csv file
	 * Should be menu based
	 * The menu provide navigation option if the entries increase the page size
	 */ 

     printf("%s\n", title);
     for (int i = 0; i < address_book->count; i++)
     {
         printf("%d. %s\n", i + 1, address_book->list[i].name[0]);
     }
     return e_success;
}

void menu_header(const char *str)
{
	fflush(stdout);

	system("cls");

	printf("#######  Address Book  #######\n");
	if (str != '\0')
	{
		printf("#######  %s\n", str);
	}
}

void main_menu(void)
{
	menu_header("Features:\n");

	printf("0. Exit\n");
	printf("1. Add Contact\n");
	printf("2. Search Contact\n");
	printf("3. Edit Contact\n");
	printf("4. Delete Contact\n");
	printf("5. List Contacts\n");
	printf("6. Save\n");
	printf("\n");
	printf("Please select an option: ");
}

Status menu(AddressBook *address_book)
{
	ContactInfo backup;
	Status ret;
	int option;

	do
	{
		main_menu();

		option = get_option(NUM, "");

		if ((address_book-> count == 0) && (option != e_add_contact))
		{
			get_option(NONE, "No entries found!!. Would you like to add? Use Add Contacts");

			continue;
		}

		switch (option)
		{
			case e_add_contact:
				/* Add your implementation to call add_contacts function here */
				break;
			case e_search_contact:
				search_contact_2(address_book);
				break;
			case e_edit_contact:
				edit_contact_2(address_book);
				break;
			case e_delete_contact:
				delete_contact_2(address_book);
				break;
			case e_list_contacts:
                //list_contacts(address_book, "List of Contacts", NULL, "Press any key to continue...", e_list_contacts);
				list_contacts_2(address_book, "List of Contacts", NULL, NULL, e_list);
                system("pause");
				break;
			case e_save:
				save_file(address_book);
				break;
			case e_exit:
				break;
		}
	} while (option != e_exit);

	return e_success;
}

Status add_contacts(AddressBook *address_book)
{
	/* Add the functionality for adding contacts here */
    if (address_book->count >= 100) // Assuming a maximum of 100 contacts
    {
        printf("Address book is full!\n");
        return e_fail;
    }

    ContactInfo new_contact;
    printf("Enter Name: ");
    fgets(new_contact.name[0], NAME_LEN, stdin);
    new_contact.name[0][strcspn(new_contact.name[0], "\n")] = '\0'; // Remove newline character

    printf("Enter Phone Number: ");
    fgets(new_contact.phone_numbers[0], NUMBER_LEN, stdin);
    new_contact.phone_numbers[0][strcspn(new_contact.phone_numbers[0], "\n")] = '\0'; // Remove newline character

    printf("Enter Email Address: ");
    fgets(new_contact.email_addresses[0], EMAIL_ID_LEN, stdin);
    new_contact.email_addresses[0][strcspn(new_contact.email_addresses[0], "\n")] = '\0'; // Remove newline character

    new_contact.si_no = address_book->count + 1;
    address_book->list[address_book->count] = new_contact;
    address_book->count++;

    return e_success;
}

Status search(const char *str, AddressBook *address_book, int loop_count, int field, const char *msg, Modes mode)
{
	/* Add the functionality for adding contacts here */
    for (int i = 0; i < address_book->count; i++)
    {
        if (strstr(address_book->list[i].name[0], str) != NULL)
        {
            printf("Found: %s\n", address_book->list[i].name[0]);
            return e_success;
        }
    }
    printf("No match found.\n");
    return e_no_match;
    
}

Status search_contact_2(AddressBook *address_book)
{
	/* Add the functionality for search contacts here */
    // Example:
    char search_str[NAME_LEN];
    printf("Enter name to search: ");
    fgets(search_str, NAME_LEN, stdin);
    search_str[strcspn(search_str, "\n")] = '\0'; // Remove newline character

    return search(search_str, address_book, address_book->count, 0, NULL, e_search);
}

Status edit_contact_2(AddressBook *address_book)
{
    /* Add the functionality for edit contacts here */
	int index;
    printf("Enter contact index to edit: ");
    scanf("%d", &index);
    getchar(); // Consume newline character

    if (index < 1 || index > address_book->count)
    {
        printf("Invalid index!\n");
        return e_fail;
    }

    ContactInfo *contact = &address_book->list[index - 1];
    printf("Editing contact: %s\n", contact->name[0]);

    printf("Enter new Name (leave blank to keep current): ");
    char new_name[NAME_LEN];
    fgets(new_name, NAME_LEN, stdin);
    if (new_name[0] != '\n')
    {
        new_name[strcspn(new_name, "\n")] = '\0'; // Remove newline character
        strcpy(contact->name[0], new_name);
    }

    printf("Enter new Phone Number (leave blank to keep current): ");
    char new_phone[NUMBER_LEN];
    fgets(new_phone, NUMBER_LEN, stdin);
    if (new_phone[0] != '\n')
    {
        new_phone[strcspn(new_phone, "\n")] = '\0'; // Remove newline character
        strcpy(contact->phone_numbers[0], new_phone);
    }

    printf("Enter new Email Address (leave blank to keep current): ");
    char new_email[EMAIL_ID_LEN];
    fgets(new_email, EMAIL_ID_LEN, stdin);
    if (new_email[0] != '\n')
    {
        new_email[strcspn(new_email, "\n")] = '\0'; // Remove newline character
        strcpy(contact->email_addresses[0], new_email);
    }

    return e_success;
}

Status delete_contact_2(AddressBook *address_book)
{
	/* Add the functionality for delete contacts here */
    /* Add the functionality for delete contacts here */

	int index;
    printf("Enter contact index to delete: ");
    scanf("%d", &index);
    getchar(); // Consume newline character

    if (index < 1 || index > address_book->count)
    {
        printf("Invalid index!\n");
        return e_fail;
    }

    for (int i = index - 1; i < address_book->count - 1; i++)
    {
        address_book->list[i] = address_book->list[i + 1];
    }
    address_book->count--;

    return e_success;
}
