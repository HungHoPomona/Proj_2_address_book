#include <stdio.h>
//#include <stdio_ext.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "address_book.h"
#include "address_book_menu.h"
//#include "address_book_fops.h"

#include "address_book_fops.h"
//#include "abk_log.h"
//#include "abk_menus.h"
//#include "abk.h"
void print_pattern();

int get_option(int type, const char *msg)
{
	char input[32];
	int option = -1;
		
	printf("%s", msg);
	
	if (fgets(input, sizeof(input), stdin) == NULL)
	{
		return -1; // Handle EOF or input error
	}

	switch (type) 
	{
		case NONE:
			break;
		case CHAR:
			option = input[0];
			break;
		case NUM:
			option = atoi(input);
			break;
		default:
			printf("Invalid type.\n");
			break;
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
			system("pause");
			break;
		}
		else if (option == 'N')
		{
			printf("Exiting without saving.\n");
			break;
		}
		else
		{
			printf("Invalid option. Please enter 'Y' or 'N'.\n");
		}

	} while (option != 'N');

	free(address_book->list);

	return e_success;
}

Status list_contacts_2(AddressBook *address_book, const char *title, int *index, const char *msg, Modes mode)
{
	int page = 0;
	int option = 1;

	do
	{
		menu_header(title); // Use updated menu_header for screen clearing
		print_pattern();
		printf(": %6s : %32s : %32s : %32s :\n", "S.No", "Name", "Phone No.", "Email ID");
		print_pattern();

		for (int i = page * 5; i < (page + 1) * 5 && i < address_book->count; i++)
		{
			printf(": %6d : %32s : %32s : %32s :\n",
			       address_book->list[i].si_no,
			       address_book->list[i].name[0],
			       address_book->list[i].phone_numbers[0],
			       address_book->list[i].email_addresses[0]);
		}

		print_pattern();
		printf("Page %d of %d\n", page + 1, (address_book->count + 4) / 5);
		printf("Options: [0] Exit | [1] Next Page | [2] Previous Page\n");

		option = get_option(NUM, "Enter your choice: ");
		switch (option)
		{
			case 0:
				return e_success;
			case 1:
				if ((page + 1) * 5 < address_book->count)
					page++;
				else
					printf("No more pages.\n");
				break;
			case 2:
				if (page > 0)
					page--;
				else
					printf("Already on the first page.\n");
				break;
			default:
				printf("Invalid option. Try again.\n");
		}
        system("pause");
    } while (option != 0);
	return e_fail;
}

void print_pattern()
{
for(int a = 0; a < 115; a++)
       {
       printf("=");
       }
       printf("\n");
}


void menu_header(const char *str)
{
	fflush(stdout);

	system("cls");

	printf("#######  Address Book  #######\n");
	if (str != NULL)
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
				add_contacts(address_book);
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
				/* Add your implementation to call list_contacts function here */
				//list_contacts(address_book, "List of Contacts", NULL, "Press any key to continue...", e_list_contacts);
				list_contacts_2(address_book, "List of Contacts", NULL, NULL, e_list);
				break;
			case e_save:
				save_file(address_book);
				break;
			case e_exit:
				exit(0);
				break;
		}

		system("pause");
		system("cls");

	} while (option != e_exit);

	return e_success;
}

Status add_contacts(AddressBook *address_book)
{
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

	printf("Contact added successfully!\n");
    return e_success;
}

Status search(const char *str, AddressBook *address_book, int loop_count, int field, const char *msg, Modes mode)
{
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
    char search_str[NAME_LEN];
    printf("Enter name to search: ");
    fgets(search_str, NAME_LEN, stdin);
    search_str[strcspn(search_str, "\n")] = '\0'; // Remove newline character

    return search(search_str, address_book, address_book->count, 0, NULL, e_search);
}

// utility for printing results based on search
void search_result(AddressBook *address_book, int option, const char *search_pattern)
{
	menu_header("Search results:\n");
	printf(": %6s : %32s : %32s : %32s :\n", "S.No", "Name", "Phone No.", "Email ID");
	printf("===================================================================================================================\n");
	int has_results = 0;

	for (int i = 0; i < address_book->count; i++) 
	{
		int index = -1;
		switch (option) // search by option selected
		{
			case 1: // name
				if (stricmp(search_pattern, address_book->list[i].name[0]) == 0)
				{
					has_results = 1;
					index = i;	
				}
				break;
			case 2: // phone no 
				for (int j = 0; j < PHONE_NUMBER_COUNT; j++) 
				{
					if (stricmp(search_pattern, address_book->list[i].phone_numbers[j]) == 0)
					{
						has_results = 1;
						index = i;
						break;
					}
				}
				break;
			case 3: // email id 
				for (int j = 0; j < EMAIL_ID_COUNT; j++) 
				{
					if (stricmp(search_pattern, address_book->list[i].email_addresses[j]) == 0)
					{
						has_results = 1;
						index = i;
						break;
					}
				}
				break;
			case 4: // serial no 
				if (atoi(search_pattern) == address_book->list[i].si_no)
				{
					has_results = 1;
					index = i;	
				}
				break;
			default:
				printf("Invalid input.\n");
				continue;
		}

		if (index != -1) 
		{
			printf(": %6d : %32s : %32s : %32s :\n",
				address_book->list[index].si_no,
				address_book->list[index].name[0],
				address_book->list[index].phone_numbers[0],
				address_book->list[index].email_addresses[0]);

			for (int j = 1; j < 5; j++) // 5 is the max length	
			{ 
				printf(": %6s : %32s : %32s : %32s :\n",
					" ",
					" ",
					address_book->list[index].phone_numbers[j],
					address_book->list[index].email_addresses[j]);
			}
			printf("===================================================================================================================\n");
		}
	}

	if (has_results == 0)
	{
		printf("No matching contacts found.\n");
		printf("===================================================================================================================\n");
	}
}

void edit_contact_editor(AddressBook *address_book, int index)
{
	int edit_option;
	int edit_index;
	char edit_changes[32];

	do
	{
		// Edit contact menu
		menu_header("Edit Contact:\n");
		
		printf("0. Back\n");
		printf("1. %-10s : %s\n", "Name", address_book->list[index].name[0]);
		printf("2. Phone No 1 : %s\n", address_book->list[index].phone_numbers[0]);
		for (int i = 1; i < PHONE_NUMBER_COUNT; i++)
		{
			printf("%13d : %s\n", 
				i+1, address_book->list[index].phone_numbers[i]);
		}
		printf("3. Email Id 1 : %s\n", address_book->list[index].email_addresses[0]);
		for (int i = 1; i < EMAIL_ID_COUNT; i++)
		{
			printf("%13d : %s\n", 
				i+1, address_book->list[index].email_addresses[i]);
		}
		printf("\n");
		// End of edit contact menu

		do
		{
			edit_option = get_option(NUM, "Please select an option: ");
			if (edit_option < 0 || edit_option > 3)
			{
				printf("Invalid input.\n");
			}
		} while (edit_option < 0 || edit_option > 3);
	
		switch (edit_option)
		{
			case 0: // Back
				continue;	
			case 1: // Name
				printf("Enter name: ");
				fgets(edit_changes, sizeof(edit_changes), stdin);
				edit_changes[strcspn(edit_changes, "\n")] = '\0';
				strcpy(address_book->list[index].name[0], edit_changes);
				break;
			case 2: // Phone no
				do
				{
					edit_index = get_option(NUM, "Enter phone no index to be changed [Max 5]: ");
					if (edit_index <= 1 || edit_index >= 5)
					{
						printf(" | Invalid index. Try again\n");
					}
				} while (edit_index <= 1 || edit_index >= 5);

				printf("Enter phone no %d: ", edit_index);
				fgets(edit_changes, sizeof(edit_changes), stdin);
				edit_changes[strcspn(edit_changes, "\n")] = '\0';
				strcpy(address_book->list[index].phone_numbers[edit_index-1], edit_changes);
				break;
			case 3: // Email id
				do
				{
					edit_index = get_option(NUM, "Enter email id index to be changed [Max 5]: ");
					if (edit_index <= 1 || edit_index >= 5)
					{
						printf(" | Invalid index. Try again\n");
					}
				} while (edit_index <= 1 || edit_index >= 5);

				printf("Enter email id %d: ", edit_index);
				fgets(edit_changes, sizeof(edit_changes), stdin);
				edit_changes[strcspn(edit_changes, "\n")] = '\0';
				strcpy(address_book->list[index].email_addresses[edit_index-1], edit_changes);
				break;
			default:
				printf("Invalid input. Try again\n");
				continue;
		}
	} while (edit_option != 0);
}

Status edit_contact_2(AddressBook *address_book)
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
		do
		{
			option = get_option(NUM, "Please select an option: ");
			if (option < 0 || option > 4)
			{
				printf("Invalid input.\n");
			}
		} while (option < 0 || option > 4);
	
		if (option == 0)
		{
			continue;
		}

		char search_pattern[32];
		
		printf("Enter a pattern: "); // TODO: replace pattern with name of search method
		fgets(search_pattern, sizeof(search_pattern), stdin);
		search_pattern[strcspn(search_pattern, "\n")] = '\0';
	
		search_result(address_book, option, search_pattern);

		// Contact selection menu via serial number 
		char select_option = 's';

		do
		{
			select_option = get_option(CHAR, "Press [s] = Select. [q] | Cancel: ");

			if (select_option == 's') 
			{
				int serial_selection = get_option(NUM, "Select a Serial Number (S.No) to Edit: ");
		
				int index;
				for (index = 0; index < address_book->count; index++)
				{ // NOTE: May not be necessary if si_no corresponds to its list index
					if (serial_selection == address_book->list[index].si_no)	
					{
						edit_contact_editor(address_book, index); // Enter contact editor
						break; // halt search
					}
				}
				if (index == address_book->count)
				{
					printf("No such contact.\n");
					continue;
				}
			
				search_result(address_book, option, search_pattern);		
			}
			else if (select_option != 's' && select_option != 'q') 
			{
				printf("Invalid input. Try again\n");
			}

		} while (select_option != 'q');
		
	} while (option != 0);

	return e_success;
}

Status delete_contact_2(AddressBook *address_book)
{
	int option;

	do 
	{
		// Print delete menu
		menu_header("Search Contact to Delete by:\n");

		printf("0. Back\n");
		printf("1. Name\n");
		printf("2. Phone No\n");
		printf("3. Email ID\n");
		printf("4. Serial No\n");
		printf("\n");
		// End of delete menu
		do
		{
			option = get_option(NUM, "Please select an option: ");
			if (option < 0 || option > 4)
			{
				printf("Invalid input.\n");
			}
		} while (option < 0 || option > 4);
	
		if (option == 0)
		{
			continue;
		}

		char search_pattern[32];
		
		printf("Enter a pattern: "); // TODO: replace pattern with name of search method
		fgets(search_pattern, sizeof(search_pattern), stdin);
		search_pattern[strcspn(search_pattern, "\n")] = '\0';
	
		search_result(address_book, option, search_pattern);

		// Contact selection menu via serial number 
		char select_option = 's';

		do
		{
			select_option = get_option(CHAR, "Press [s] = Select. [q] | Cancel: ");

			if (select_option == 's') 
			{
				int serial_selection = get_option(NUM, "Select a Serial Number (S.No) to Delete: ");

				int index;
				for (index = 0; index < address_book->count; index++)
				{ // NOTE: May not be necessary if si_no corresponds to its list index
					if (serial_selection == address_book->list[index].si_no)	
					{
						// Delete contact preview
						menu_header("Edit Contact:\n");
						
						printf("0. Back\n");
						printf("1. %-10s : %s\n", "Name", address_book->list[index].name[0]);
						printf("2. Phone No 1 : %s\n", address_book->list[index].phone_numbers[0]);
						for (int i = 1; i < PHONE_NUMBER_COUNT; i++)
						{
							printf("%13d : %s\n", 
								i+1, address_book->list[index].phone_numbers[i]);
						}
						printf("3. Email Id 1 : %s\n", address_book->list[index].email_addresses[0]);
						for (int i = 1; i < EMAIL_ID_COUNT; i++)
						{
							printf("%13d : %s\n", 
								i+1, address_book->list[index].email_addresses[i]);
						}
						printf("\n");
						// End of delete contact preview 
						
						// Delete prompt
						if (get_option(CHAR, "Enter 'Y' to delete. [Press any key to ignore]: ") == 'Y')
						{ // set list entry to empty
							address_book->list[index] = *(ContactInfo *)malloc(sizeof(ContactInfo));
						}
						
						break; // halt search
					}
				}
				// Serial number does not exist 
				if (index == address_book->count)
				{
					printf("No such contact.\n");
					continue;
				}
			
				search_result(address_book, option, search_pattern);		
			}
			else if (select_option != 's' && select_option != 'q') 
			{
				printf("Invalid input. Try again\n");
			}

		} while (select_option != 'q');
		
	} while (option != 0);
}
