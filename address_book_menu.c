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
void print_pattern();

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
    if (fgets(input, sizeof(input), stdin) == NULL)
	{
		return -1; // Handle EOF or input error
	}

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
		option = toupper(option); // Convert input to uppercase

		if (option == 'Y')
		{
			save_file(address_book);
			printf("Exiting. Data saved in %s\n", DEFAULT_FILE);
			break;
		}
		else if (option == 'N')
		{
			printf("Exiting without saving.\n");
			return e_fail;
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
        menu_header(title);
        print_pattern();
        printf(": %6s : %32s : %32s : %32s :\n", "S.No", "Name", "Phone No.", "Email ID");
        print_pattern();

        // Dynamically display contacts based on the current page
        for (int i = page * WINDOW_SIZE; i < (page + 1) * WINDOW_SIZE && i < address_book->count; i++)
        {
            ContactInfo *contact = &address_book->list[i];

            // Print the pattern before each contact after the first one
            if (i > page * WINDOW_SIZE) 
            {
                print_pattern();
            }

            // Print the primary name, phone number, and email address
            printf(": %6d : %32s : %32s : %32s :\n",
                   contact->si_no,
                   contact->name[0],
                   strlen(contact->phone_numbers[0]) > 0 ? contact->phone_numbers[0] : "",
                   strlen(contact->email_addresses[0]) > 0 ? contact->email_addresses[0] : "");

            // Print additional phone numbers and email addresses in separate rows
            for (int j = 1; j < PHONE_NUMBER_COUNT || j < EMAIL_ID_COUNT; j++)
            {
                printf(": %6s : %32s : %32s : %32s :\n",
                       "",
                       "",
                       j < PHONE_NUMBER_COUNT && strlen(contact->phone_numbers[j]) > 0 ? contact->phone_numbers[j] : "",
                       j < EMAIL_ID_COUNT && strlen(contact->email_addresses[j]) > 0 ? contact->email_addresses[j] : "");
            }
        }

        print_pattern();
        printf("Page %d of %d\n", page + 1, (address_book->count + WINDOW_SIZE - 1) / WINDOW_SIZE);
        printf("Options: [0] Exit | [1] Next Page | [2] Previous Page\n");

        option = get_option(NUM, "Enter your choice: ");
        switch (option)
        {
            case 0:
                return e_success;
            case 1:
                if ((page + 1) * WINDOW_SIZE < address_book->count)
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
		system("cls");
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
				// Use save_prompt to validate before saving
				if (save_prompt(address_book) == e_success)
				{
					printf("Data saved successfully.\n");
				}
				else
				{
					printf("Returning to main menu.\n");
				}
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

bool_t is_valid_phone_number(const char *phone)
{
    int len = strlen(phone);
    if (len == 12 && phone[3] == '-' && phone[7] == '-') // Format: XXX-XXX-XXXX
    {
        for (int i = 0; i < len; i++)
        {
            if (i == 3 || i == 7) continue;
            if (!isdigit(phone[i])) return 0;
        }
        return 1;
    }
    else if (len == 10) // Format: 10 digits
    {
        for (int i = 0; i < len; i++)
        {
            if (!isdigit(phone[i])) return 0;
        }
        return 1;
    }
    return 0;
}

void format_phone_number(char *phone)
{
    if (strlen(phone) == 10) // Convert 10 digits to XXX-XXX-XXXX
    {
        char formatted[NUMBER_LEN];
        snprintf(formatted, sizeof(formatted), "%.3s-%.3s-%.4s", phone, phone + 3, phone + 6);
        strcpy(phone, formatted);
    }
}

bool_t is_valid_email(const char *email)
{
    const char *at = strchr(email, '@');
    const char *dot = strrchr(email, '.');
    return (at && dot && at < dot); // Ensure '@' exists before the last '.'
}

Status add_contacts(AddressBook *address_book)
{
    if (address_book->count >= MAX_CONTACTS)
    {
        printf("Address book is full!\n");
        return e_fail;
    }

    ContactInfo new_contact;
    memset(&new_contact, 0, sizeof(ContactInfo)); // Initialize memory to zero

    // Input name
    printf("Enter Name: ");
    fgets(new_contact.name[0], NAME_LEN, stdin);
    new_contact.name[0][strcspn(new_contact.name[0], "\n")] = '\0'; // Remove newline character
    if (strlen(new_contact.name[0]) == 0)
    {
        printf("Name cannot be empty.\n");
        return e_fail;
    }

    // Input up to 5 phone numbers
    for (int i = 0; i < PHONE_NUMBER_COUNT; i++)
    {
        printf("Enter Phone Number %d (press Enter to skip): ", i + 1);
        fgets(new_contact.phone_numbers[i], NUMBER_LEN, stdin);
        new_contact.phone_numbers[i][strcspn(new_contact.phone_numbers[i], "\n")] = '\0'; // Remove newline character
        if (strlen(new_contact.phone_numbers[i]) == 0) break; // Stop if input is blank

        while (!is_valid_phone_number(new_contact.phone_numbers[i]))
        {
            printf("Invalid phone number. Must be 10 digits or in XXX-XXX-XXXX format. Try again.\n");
            printf("Enter Phone Number %d (press Enter to skip): ", i + 1);
            fgets(new_contact.phone_numbers[i], NUMBER_LEN, stdin);
            new_contact.phone_numbers[i][strcspn(new_contact.phone_numbers[i], "\n")] = '\0'; // Remove newline character
            if (strlen(new_contact.phone_numbers[i]) == 0) break; // Stop if input is blank
        }

        if (strlen(new_contact.phone_numbers[i]) == 0) break; // Stop if input is blank after validation loop
        format_phone_number(new_contact.phone_numbers[i]); // Format to XXX-XXX-XXXX
    }

    // Input up to 5 email addresses
    for (int i = 0; i < EMAIL_ID_COUNT; i++)
    {
        printf("Enter Email Address %d (press Enter to skip): ", i + 1);
        fgets(new_contact.email_addresses[i], EMAIL_ID_LEN, stdin);
        new_contact.email_addresses[i][strcspn(new_contact.email_addresses[i], "\n")] = '\0'; // Remove newline character
        if (strlen(new_contact.email_addresses[i]) == 0) break; // Stop if input is blank

        while (!is_valid_email(new_contact.email_addresses[i]))
        {
            printf("Invalid email address. Must contain '@' and a valid domain. Try again.\n");
            printf("Enter Email Address %d (press Enter to skip): ", i + 1);
            fgets(new_contact.email_addresses[i], EMAIL_ID_LEN, stdin);
            new_contact.email_addresses[i][strcspn(new_contact.email_addresses[i], "\n")] = '\0'; // Remove newline character
            if (strlen(new_contact.email_addresses[i]) == 0) break; // Stop if input is blank
        }

        if (strlen(new_contact.email_addresses[i]) == 0) break; // Stop if input is blank after validation loop
    }

    // Assign serial number and add the contact to the address book
    new_contact.si_no = address_book->count + 1;
    address_book->list[address_book->count] = new_contact;
    address_book->count++;

    printf("Contact added successfully!\n");
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
    if (address_book->count == 0)
    {
        printf("No contacts available.\n");
        return e_fail;
    }

    int option;
    printf("Search by:\n");
    printf("1. ID\n");
    printf("2. Name\n");
    printf("3. Phone Number\n");
    printf("4. Email Address\n");
    printf("Enter your choice: ");
    option = get_option(NUM, "");

    char search_str[NAME_LEN];
    printf("Enter search term: ");
    fgets(search_str, sizeof(search_str), stdin);
    search_str[strcspn(search_str, "\n")] = '\0'; // Remove newline character

    int found = 0;
    for (int i = 0; i < address_book->count; i++)
    {
        ContactInfo *contact = &address_book->list[i];
        int match = 0;

        switch (option)
        {
            case 1: // Search by ID
                if (contact->si_no == atoi(search_str))
                    match = 1;
                break;
            case 2: // Search by Name
                if (strstr(contact->name[0], search_str) != NULL)
                    match = 1;
                break;
            case 3: // Search by Phone Number
                for (int j = 0; j < PHONE_NUMBER_COUNT; j++)
                {
                    if (strlen(contact->phone_numbers[j]) > 0 &&
                        strstr(contact->phone_numbers[j], search_str) != NULL)
                    {
                        match = 1;
                        break;
                    }
                }
                break;
            case 4: // Search by Email Address
                for (int j = 0; j < EMAIL_ID_COUNT; j++)
                {
                    if (strlen(contact->email_addresses[j]) > 0 &&
                        strstr(contact->email_addresses[j], search_str) != NULL)
                    {
                        match = 1;
                        break;
                    }
                }
                break;
            default:
                printf("Invalid option.\n");
                return e_fail;
        }

        if (match)
        {
            found = 1;
            printf("\nContact Found:\n");
            print_pattern();
            printf(": %6s : %32s : %32s : %32s :\n", "S.No", "Name", "Phone No.", "Email ID");
            print_pattern();
            printf(": %6d : %32s : %32s : %32s :\n",
                   contact->si_no,
                   contact->name[0],
                   strlen(contact->phone_numbers[0]) > 0 ? contact->phone_numbers[0] : "",
                   strlen(contact->email_addresses[0]) > 0 ? contact->email_addresses[0] : "");
            for (int j = 1; j < PHONE_NUMBER_COUNT || j < EMAIL_ID_COUNT; j++)
            {
                printf(": %6s : %32s : %32s : %32s :\n",
                       "",
                       "",
                       j < PHONE_NUMBER_COUNT && strlen(contact->phone_numbers[j]) > 0 ? contact->phone_numbers[j] : "",
                       j < EMAIL_ID_COUNT && strlen(contact->email_addresses[j]) > 0 ? contact->email_addresses[j] : "");
            }
            print_pattern();
        }
    }

    if (!found)
        printf("No matching contact found.\n");

    return e_success;
}

Status edit_contact_2(AddressBook *address_book)
{
    if (address_book->count == 0)
    {
        printf("No contacts available.\n");
        return e_fail;
    }

    int index = -1;
    int option;

    do
    {
        // Search Menu
        printf("\nSearch Options before EDIT:\n");
        printf("1. Search by Name\n");
        printf("2. Search by Phone Number\n");
        printf("3. Search by Email Address\n");
        printf("4. Search by Contact ID\n");
        printf("0. Back to Main Menu\n");
        printf("Enter your choice: ");
        option = get_option(NUM, "");

        switch (option)
        {
            case 1: // Search by Name
            {
                char name[NAME_LEN];
                printf("Enter the name to search: ");
                fgets(name, NAME_LEN, stdin);
                name[strcspn(name, "\n")] = '\0'; // Remove newline character

                int found = 0;
                for (int i = 0; i < address_book->count; i++)
                {
                    if (strstr(address_book->list[i].name[0], name) != NULL)
                    {
                        index = i;
                        found = 1;
                        break;
                    }
                }

                if (!found)
                {
                    printf("No contact found with name '%s'.\n", name);
                }
                else
                {
					system("cls");
					printf("\nContact Found:\n");
					print_pattern();
					printf(": %6s : %32s : %32s : %32s :\n", "S.No", "Name", "Phone No.", "Email ID");
					print_pattern();
					printf(": %6d : %32s : %32s : %32s :\n",
							address_book->list[index].si_no,
							address_book->list[index].name[0],
							strlen(address_book->list[index].phone_numbers[0]) > 0 ? address_book->list[index].phone_numbers[0] : "",
							strlen(address_book->list[index].email_addresses[0]) > 0 ? address_book->list[index].email_addresses[0] : "");
					for (int j = 1; j < PHONE_NUMBER_COUNT || j < EMAIL_ID_COUNT; j++)
					{
						printf(": %6s : %32s : %32s : %32s :\n",
							   "",
							   "",
							   j < PHONE_NUMBER_COUNT && strlen(address_book->list[index].phone_numbers[j]) > 0 ? address_book->list[index].phone_numbers[j] : "",
							   j < EMAIL_ID_COUNT && strlen(address_book->list[index].email_addresses[j]) > 0 ? address_book->list[index].email_addresses[j] : "");
					}
					print_pattern();
                }
                break;
            }

            case 2: // Search by Phone Number
            {
                char phone[NUMBER_LEN];
                printf("Enter the phone number to search: ");
                fgets(phone, NUMBER_LEN, stdin);
                phone[strcspn(phone, "\n")] = '\0'; // Remove newline character

                int found = 0;
                for (int i = 0; i < address_book->count; i++)
                {
                    for (int j = 0; j < PHONE_NUMBER_COUNT; j++)
                    {
                        if (strcmp(address_book->list[i].phone_numbers[j], phone) == 0)
                        {
                            index = i;
                            found = 1;
                            break;
                        }
                    }
                    if (found) break;
                }

                if (!found)
                {
                    printf("No contact found with phone number '%s'.\n", phone);
                }
                else
                {
					system("cls");
					printf("\nContact Found:\n");
					print_pattern();
					printf(": %6s : %32s : %32s : %32s :\n", "S.No", "Name", "Phone No.", "Email ID");
					print_pattern();
					printf(": %6d : %32s : %32s : %32s :\n",
							address_book->list[index].si_no,
							address_book->list[index].name[0],
							strlen(address_book->list[index].phone_numbers[0]) > 0 ? address_book->list[index].phone_numbers[0] : "",
							strlen(address_book->list[index].email_addresses[0]) > 0 ? address_book->list[index].email_addresses[0] : "");
					for (int j = 1; j < PHONE_NUMBER_COUNT || j < EMAIL_ID_COUNT; j++)
					{
						printf(": %6s : %32s : %32s : %32s :\n",
							   "",
							   "",
							   j < PHONE_NUMBER_COUNT && strlen(address_book->list[index].phone_numbers[j]) > 0 ? address_book->list[index].phone_numbers[j] : "",
							   j < EMAIL_ID_COUNT && strlen(address_book->list[index].email_addresses[j]) > 0 ? address_book->list[index].email_addresses[j] : "");
					}
					print_pattern();
                }
                break;
            }

            case 3: // Search by Email Address
            {
                char email[EMAIL_ID_LEN];
                printf("Enter the email address to search: ");
                fgets(email, EMAIL_ID_LEN, stdin);
                email[strcspn(email, "\n")] = '\0'; // Remove newline character

                int found = 0;
                for (int i = 0; i < address_book->count; i++)
                {
                    for (int j = 0; j < EMAIL_ID_COUNT; j++)
                    {
                        if (strcmp(address_book->list[i].email_addresses[j], email) == 0)
                        {
                            index = i;
                            found = 1;
                            break;
                        }
                    }
                    if (found) break;
                }

                if (!found)
                {
                    printf("No contact found with email address '%s'.\n", email);
                }
                else
                {
					system("cls");
                    printf("\nContact Found:\n");
					print_pattern();
					printf(": %6s : %32s : %32s : %32s :\n", "S.No", "Name", "Phone No.", "Email ID");
					print_pattern();
					printf(": %6d : %32s : %32s : %32s :\n",
							address_book->list[index].si_no,
							address_book->list[index].name[0],
							strlen(address_book->list[index].phone_numbers[0]) > 0 ? address_book->list[index].phone_numbers[0] : "",
							strlen(address_book->list[index].email_addresses[0]) > 0 ? address_book->list[index].email_addresses[0] : "");
					for (int j = 1; j < PHONE_NUMBER_COUNT || j < EMAIL_ID_COUNT; j++)
					{
						printf(": %6s : %32s : %32s : %32s :\n",
							   "",
							   "",
							   j < PHONE_NUMBER_COUNT && strlen(address_book->list[index].phone_numbers[j]) > 0 ? address_book->list[index].phone_numbers[j] : "",
							   j < EMAIL_ID_COUNT && strlen(address_book->list[index].email_addresses[j]) > 0 ? address_book->list[index].email_addresses[j] : "");
					}
					print_pattern();
                }
                break;
            }

            case 4: // Search by Contact ID
            {
                printf("Enter contact ID to search: ");
                int id;
                scanf("%d", &id);
                getchar(); // Consume newline character

                int found = 0;
                for (int i = 0; i < address_book->count; i++)
                {
                    if (address_book->list[i].si_no == id)
                    {
                        index = i;
                        found = 1;
                        break;
                    }
                }

                if (!found)
                {
                    printf("No contact found with ID %d.\n", id);
                }
                else
                {
					system("cls");
                    printf("\nContact Found:\n");
					print_pattern();
					printf(": %6s : %32s : %32s : %32s :\n", "S.No", "Name", "Phone No.", "Email ID");
					print_pattern();
					printf(": %6d : %32s : %32s : %32s :\n",
							address_book->list[index].si_no,
							address_book->list[index].name[0],
							strlen(address_book->list[index].phone_numbers[0]) > 0 ? address_book->list[index].phone_numbers[0] : "",
							strlen(address_book->list[index].email_addresses[0]) > 0 ? address_book->list[index].email_addresses[0] : "");
					for (int j = 1; j < PHONE_NUMBER_COUNT || j < EMAIL_ID_COUNT; j++)
					{
						printf(": %6s : %32s : %32s : %32s :\n",
							   "",
							   "",
							   j < PHONE_NUMBER_COUNT && strlen(address_book->list[index].phone_numbers[j]) > 0 ? address_book->list[index].phone_numbers[j] : "",
							   j < EMAIL_ID_COUNT && strlen(address_book->list[index].email_addresses[j]) > 0 ? address_book->list[index].email_addresses[j] : "");
					}
					print_pattern();
                }
                break;
            }

            case 0: // Back to Main Menu
                return e_success;

            default:
                printf("Invalid option. Try again.\n");
        }

        if (index != -1)
        {
            // Edit Menu
            int edit_option;
            do
            {
                printf("\nEdit Options:\n");
                printf("1. Edit Name\n");
                printf("2. Edit Phone Numbers\n");
                printf("3. Edit Email Addresses\n");
                printf("0. Back to Search Menu\n");
                printf("Enter your choice: ");
                edit_option = get_option(NUM, "");

                switch (edit_option)
                {
                    case 1: // Edit Name
                        for (int i = 0; i < NAME_COUNT; i++)
                        {
                            printf("Enter Name %d (leave blank to keep current): ", i + 1);
                            char new_name[NAME_LEN];
                            fgets(new_name, NAME_LEN, stdin);
                            new_name[strcspn(new_name, "\n")] = '\0'; // Remove newline character
                            if (strlen(new_name) > 0)
                                strncpy(address_book->list[index].name[i], new_name, NAME_LEN - 1);
                        }
                        break;

                    case 2: // Edit Phone Numbers
                        for (int i = 0; i < PHONE_NUMBER_COUNT; i++)
                        {
                            printf("Enter Phone Number %d (leave blank to keep current): ", i + 1);
                            char new_phone[NUMBER_LEN];
                            fgets(new_phone, NUMBER_LEN, stdin);
                            new_phone[strcspn(new_phone, "\n")] = '\0'; // Remove newline character
                            if (strlen(new_phone) > 0)
                                strncpy(address_book->list[index].phone_numbers[i], new_phone, NUMBER_LEN - 1);
                        }
                        break;

                    case 3: // Edit Email Addresses
                        for (int i = 0; i < EMAIL_ID_COUNT; i++)
                        {
                            printf("Enter Email Address %d (leave blank to keep current): ", i + 1);
                            char new_email[EMAIL_ID_LEN];
                            fgets(new_email, EMAIL_ID_LEN, stdin);
                            new_email[strcspn(new_email, "\n")] = '\0'; // Remove newline character
                            if (strlen(new_email) > 0)
                                strncpy(address_book->list[index].email_addresses[i], new_email, EMAIL_ID_LEN - 1);
                        }
                        break;

                    case 0: // Back to Search Menu
                        break;

                    default:
                        printf("Invalid option. Try again.\n");
                }
            } while (edit_option != 0);
        }
    } while (option != 0);

    return e_success;
}

Status delete_contact_2(AddressBook *address_book)
{
    if (address_book->count == 0)
    {
        printf("No contacts available.\n");
        return e_fail;
    }

    int option;
    do
    {
        printf("\nDelete Options:\n");
        printf("1. Delete by Name\n");
        printf("2. Delete by Phone Number\n");
        printf("3. Delete by Email Address\n");
        printf("4. Delete by Contact ID\n");
        printf("0. Done\n");
        printf("Enter your choice: ");
        option = get_option(NUM, "");

        switch (option)
        {
            case 1: // Delete by Name
            {
                char name[NAME_LEN];
                printf("Enter the name to delete: ");
                fgets(name, NAME_LEN, stdin);
                name[strcspn(name, "\n")] = '\0'; // Remove newline character

                int found = 0;
                for (int i = 0; i < address_book->count; i++)
                {
                    if (strcmp(address_book->list[i].name[0], name) == 0)
                    {
                        for (int j = i; j < address_book->count - 1; j++)
                        {
                            address_book->list[j] = address_book->list[j + 1];
                        }
                        address_book->count--;
                        printf("Contact with name '%s' deleted.\n", name);
                        found = 1;
                        break;
                    }
                }

                if (!found)
                {
                    printf("No contact found with name '%s'.\n");
                }
                break;
            }

            case 2: // Delete by Phone Number
            {
                char phone[NUMBER_LEN];
                printf("Enter the phone number to delete: ");
                fgets(phone, NUMBER_LEN, stdin);
                phone[strcspn(phone, "\n")] = '\0'; // Remove newline character

                int found = 0;
                for (int i = 0; i < address_book->count; i++)
                {
                    for (int j = 0; j < PHONE_NUMBER_COUNT; j++)
                    {
                        if (strcmp(address_book->list[i].phone_numbers[j], phone) == 0)
                        {
                            for (int k = i; k < address_book->count - 1; k++)
                            {
                                address_book->list[k] = address_book->list[k + 1];
                            }
                            address_book->count--;
                            printf("Contact with phone number '%s' deleted.\n", phone);
                            found = 1;
                            break;
                        }
                    }
                    if (found) break;
                }

                if (!found)
                {
                    printf("No contact found with phone number '%s'.\n");
                }
                break;
            }

            case 3: // Delete by Email Address
            {
                char email[EMAIL_ID_LEN];
                printf("Enter the email address to delete: ");
                fgets(email, EMAIL_ID_LEN, stdin);
                email[strcspn(email, "\n")] = '\0'; // Remove newline character

                int found = 0;
                for (int i = 0; i < address_book->count; i++)
                {
                    for (int j = 0; j < EMAIL_ID_COUNT; j++)
                    {
                        if (strcmp(address_book->list[i].email_addresses[j], email) == 0)
                        {
                            for (int k = i; k < address_book->count - 1; k++)
                            {
                                address_book->list[k] = address_book->list[k + 1];
                            }
                            address_book->count--;
                            printf("Contact with email address '%s' deleted.\n", email);
                            found = 1;
                            break;
                        }
                    }
                    if (found) break;
                }

                if (!found)
                {
                    printf("No contact found with email address '%s'.\n");
                }
                break;
            }

            case 4: // Delete by Contact ID
            {
                int id;
                printf("Enter the contact ID to delete: ");
                scanf("%d", &id);
                getchar(); // Consume newline character

                int found = 0;
                for (int i = 0; i < address_book->count; i++)
                {
                    if (address_book->list[i].si_no == id)
                    {
                        for (int j = i; j < address_book->count - 1; j++)
                        {
                            address_book->list[j] = address_book->list[j + 1];
                        }
                        address_book->count--;
                        printf("Contact with ID %d deleted.\n", id);
                        found = 1;
                        break;
                    }
                }

                if (!found)
                {
                    printf("No contact found with ID %d.\n");
                }
                break;
            }

            case 0: // Done
                printf("Finished deleting contacts.\n");
                break;

            default:
                printf("Invalid option. Try again.\n");
        }
    } while (option != 0);

    return e_success;
}