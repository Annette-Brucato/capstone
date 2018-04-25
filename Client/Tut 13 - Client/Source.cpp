/*
Tutorial 13 Client Solution
Author: Pindrought
Date: 02/26/2017
*/
#include "Client.h"
#include <iostream>
using namespace std;

bool isusername(string);
bool ispassword(string);

int main()
{
	string userPass, usern;
	cout << "Enter your username: " << endl;
	cin >> usern;
	if (isusername(usern)) {
		cout << "Enter your password: " << endl;
		cin >> userPass;
	}

	if (isusername(usern) && ispassword(userPass)) {
		Client myClient("172.20.10.2",1111); //Create client to connect to server 127.0.0.1 [localhost] on port 1111
			if (!myClient.Connect()) //If client fails to connect...
			{
				std::cout << "Failed to connect to server..." << std::endl;
				system("pause");
				return -1;
			}
			std::string userinput; //holds the user's chat message
			while (true)
			{
				std::getline(std::cin,userinput); //Get line if user presses enter and fill the buffer
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
	if (usern == usename)
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