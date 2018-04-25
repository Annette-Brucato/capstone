#include "Client.h"
#include <iostream>
#include <conio.h>

using namespace std;

bool isusername(string);
bool ispassword(string);
string getpass(const char *prompt, bool show_asterisk = true);

int main()
{
	string userPass, usern;
	int tries = 0;

	cout << "Enter your username: " << endl;
	cin >> usern;
	if (isusername(usern)) {
		while (!ispassword(userPass) && tries < 3) {
			userPass = getpass("Enter your password: ", true); // Show asterisks
			if (ispassword(userPass))
				cout << "Correct password" << endl;
			else {
				cout << "Incorrect password. Try again" << endl;
				tries++;
				if (tries == 3) {
					userPass = getpass("Last try!: ", true); // Show asterisks
					if (ispassword(userPass))
						cout << "Correct password" << endl;
					else {
						cout << "Wrong password, exiting program." << endl;
						system("PAUSE");
						return 0;
					}
				}
			}
		}


	}

	if (isusername(usern) && ispassword(userPass)) {
		Client myClient("172.31.223.104", 1111); //Create client to connect to server 127.0.0.1 [localhost] on port 1111
		if (!myClient.Connect()) //If client fails to connect...
		{
			std::cout << "Failed to connect to server..." << std::endl;
			system("pause");
			return -1;
		}
		std::string userinput; //holds the user's chat message
		while (true)
		{
			std::getline(std::cin, userinput); //Get line if user presses enter and fill the buffer
			myClient.SendString(userinput); //Send string to server
		}

		std::cin.get();
		return 0;
	}
	else {
		cout << "Username and/or password did not match.  " << endl;
		system("PAUSE");
		return 0;
	}
}

bool isusername(string usern) {
	string usename = "jon";
	if (usern == usename | usern == "user1" | usern == "user2")
		return true;
	else
		return false;
}

bool ispassword(string password) {
	string pass = "Senior2018";
	if (pass == password)
		return true;
	else
		return false;
}

string getpass(const char *prompt, bool show_asterisk)
{
	const char BACKSPACE = 8;
	const char RETURN = 13;

	string password;
	unsigned char ch = 0;

	cout << prompt << endl;

	while ((ch = _getch()) != RETURN)
	{
		if (ch == BACKSPACE)
		{
			if (password.length() != 0)
			{
				if (show_asterisk)
					cout << "\b \b";
				password.resize(password.length() - 1);
			}
		}
		else if (ch == 0 || ch == 224) // handle escape sequences
		{
			_getch(); // ignore non printable chars
			continue;
		}
		else
		{
			password += ch;
			if (show_asterisk)
				cout << '*';
		}
	}
	cout << endl;
	return password;
}