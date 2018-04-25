#include "Client.h"
#include "SerialClass.h"
#include <stdio.h>
#include <string>
#include <iostream>
#include <conio.h>

using namespace std;
bool isusername(string);
bool ispassword(string);
string getpass(const char *prompt, bool show_asterisk = true);


int main()
{
	Serial SP = Serial("COM4");
	char incomingData[1] = "";			// don't forget to pre-allocate memory
	int dataLength = 1;
	int readResult = 0;

	if (SP.IsConnected())
		printf("Welcome to CypherChat! \n");
	
	while (true)
	{
		while (SP.IsConnected())
		{
			readResult = SP.ReadData(incomingData, dataLength);
			incomingData[readResult] = 0;
			if (readResult == 1)
				break;
			Sleep(500);
		}

		Sleep(500);
		break;
	}
	string userPass, usern;
	int tries = 0;

	cout << "Enter your username: " << endl;
	cin >> usern;

	if (isusername(usern))
	{
		while (!ispassword(userPass) && tries < 3)
		{
			userPass = getpass("Enter your password: ", true); // Show asterisks
			if (ispassword(userPass))
				cout << "Correct password" << endl;
			else
			{
				cout << "Incorrect password. Try again" << endl;
				tries++;
				if (tries == 3)
				{
					userPass = getpass("Last try!: ", true); // Show asterisks
					if (ispassword(userPass))
						cout << "Correct password" << endl;
					else
					{
						cout << "Wrong password, exiting program." << endl;
						return -2;
					}
				}
			}
		}
	}

	if (isusername(usern) && ispassword(userPass)) {
		Client myClient("172.20.10.2", 1111); //Create client to connect to server 127.0.0.1 [localhost] on port 1111

		if (!myClient.Connect()) //If client fails to connect...
		{
			std::cout << "Failed to connect to server..." << std::endl;
			system("pause");
			return -66;
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
		return -3;
	}
}

bool isusername(string usern)
{
	string usename = "jon";
	if (usern == usename)
		return true;
	else
		return false;
}



bool ispassword(string password)
{
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