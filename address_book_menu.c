#include <stdio.h>
//#include <stdio_ext.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "address_book.h"
#include "address_book_menu.h"

#include "address_book_fops.h"
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

	int option;

	printf("%s", msg);

	switch (type) 
	{
		case NONE:
			break;
		case CHAR:
			scanf("%c", (char *)&option);
			break;
		case NUM:
			scanf("%d", &option);
			break;
		default:
			printf("Invalid input.\n");
			return -1;
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

Status list_contacts(AddressBook *address_book, const char *title, int *index, const char *msg, Modes mode)
{
	/* 
	 * Add code to list all the contacts availabe in address_book.csv file
	 * Should be menu based
	 * The menu provide navigation option if the entries increase the page size
	 */ 

	return e_success;
}

void menu_header(const char *str)
{
	fflush(stdout);

	system("clear");

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
				search_contact(address_book);
				break;
			case e_edit_contact:
				edit_contact(address_book);
				break;
			case e_delete_contact:
				delete_contact(address_book);
				break;
			case e_list_contacts:
				break;
				/* Add your implementation to call list_contacts function here */
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
}

Status search(const char *str, AddressBook *address_book, int loop_count, int field, const char *msg, Modes mode)
{
	/* Add the functionality for adding contacts here */
}

Status search_contact(AddressBook *address_book)
{
	/* Add the functionality for search contacts here */
}

void edit_print_result(AddressBook *address_book, int index)
{
	printf("%d | ", address_book->list[index].si_no);
	for (int i = 0; i < 5; i++) // 5 is the max length	
	{ 
		if (i != 0) { printf("\t"); }
		printf("%s | ", address_book->list[index].name[i]);
		printf("%s | ", address_book->list[index].phone_numbers[i]);
		printf("%s", address_book->list[index].email_addresses[i]);
	}
}

void edit_contact_editor(AddressBook *address_book)
{
	// Edit contact menu
	menu_header("Edit Contact:\n");
	
	printf("0. Back\n");
	printf("1. Name : %s\n", address_book->list[index].name[0]);
	printf("2. Phone No ");
	for (int i = 0; i < PHONE_NUMBER_COUNT; i++)
	{
		printf("%d : %s\n", 
			i, address_book->list[index].phone_numbers[i]);
	}
	printf("3. Email Id ");
	for (int i = 0; i < EMAIL_ID_COUNT; i++)
	{
		printf("%d : %s\n", 
			i, address_book->list[index].email_addresses[i]);
	}
	printf("\n");
	// End of edit contact menu

	int edit_option;
	int edit_index;
	char * edit_changes;

	do
	{
		edit_option = get_option(NUM, "Please select an option: ");

		switch (edit_option)
		{
			case 0: // Back
				continue;	
			case 1: // Name
				printf("Enter name: ");
				scanf("%s", edit_changes);
				strcpy(address_book->list[index].name[0], edit_changes);
				break;
			case 2: // Phone no
				do
				{
					edit_index = get_option(NUM, "Enter phone no index to be changed [Max 5]: ");
					if (edit_index <= 0 || edit_index >= 5)
					{
						printf(" | Invalid index. Try again\n");
					}
				} while (edit_index <= 0 || edit_index >= 5);

				printf("Enter phone no %d: ", edit_index);
				scanf("%s", edit_changes);
				strcpy(address_book->list[index].phone_numbers[edit_index], edit_changes);
				break;
			case 3: // Email id
				do
				{
					edit_index = get_option(NUM, "Enter email id index to be changed [Max 5]: ");
					if (edit_index <= 0 || edit_index >= 5)
					{
						printf(" | Invalid index. Try again\n");
					}
				} while (edit_index <= 0 || edit_index >= 5);

				printf("Enter email id %d: ", edit_index);
				scanf("%s", edit_changes);
				strcpy(address_book->list[index].email_addresses[edit_index], edit_changes);
				break;
			default:
				printf("Invalid input. Try again\n");
				continue;
		}
		
	} while (edit_option != 0);
}

Status edit_contact(AddressBook *address_book)
{
	int option;

	do 
	{
		// Print edit menu
		menu_header("Search Contact to Edit by:\n");

		printf("0. Back\n");
		printf("1. Name\n");
		printf("2. Phone No\n");
		printf("3. Email ID\n");
		printf("4. Serial No\n");
		printf("\n");
		// End of edit menu
		option = get_option(NUM, "Please select an option: ");
		
		int book_length = address_book->count;
		int info_length;
		char * search_pattern;
		
		printf("Enter a pattern: "); // TODO: replace pattern with name of search method
		scanf("%s", search_pattern);
	
		for (int i = 0; i < book_length; i++) 
		{
			ContactInfo * contact = &address_book->list[i];

			switch (option) // search by option selected
			{
				case 0: // back
					continue;	
				case 1: // name
					if (stricmp(search_pattern, contact->name[0]) == 0)
					{
						edit_print_result(address_book, i);
					}
					break;
				case 2: // phone no 
					for (int j = 0; j < PHONE_NUMBER_COUNT; j++) {
						if (stricmp(search_pattern, contact->phone_numbers[j]) == 0)
						{
							edit_print_result(address_book, i);
						}
					}
					break;
				case 3: // email id 
					for (int j = 0; j < EMAIL_ID_COUNT; j++) {
						if (stricmp(search_pattern, contact->email_addresses[j]) == 0)
						{
							edit_print_result(address_book, i);
						}
					}
					break;
				case 4: // serial no 
					if (atoi(search_pattern) == contact->si_no)
					{
						edit_print_result(address_book, i);
					}
					break;
				default:
					printf("Invalid input.\n");
					continue;
				}
		}

		// Contact selection menu via serial number 
		char select_option;

		do
		{
			select_option = get_option(CHAR, "Press [s] = Select. [q] | Cancel: ");

			if (select_option == 's') 
			{
				int serial_selection = get_option(NUM, "Select a Serial Number (S.No) to Edit: ");
		
				/*
					Check for invalid serial numbers	
				*/				

				int index;
				for (index = 0; index < address_book->count; index++)
				{ // NOTE: May not be necessary if si_no corresponds to its list index
					if (serial_selection == address_book->list[index].si_no)	
					{
						break;
					}
				}
			
				edit_contact_editor(address_book); // Enter contact editor
			
			}
			else if (select_option != 'q') 
			{
				printf("Invalid input. Try again\n");
			}

		} while (select_option != 's' || select_option != 'q');
		
	} while (option != e_back);

	return e_success;
}

Status delete_contact(AddressBook *address_book)
{
	/* Add the functionality for delete contacts here */
}
