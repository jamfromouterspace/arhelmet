#include <iostream>
#include <stdio.h>
#include <string>
#include <unistd.h> // For usleep()
#include <iomanip> // For setprecision()
#include "Global.h"
#include "BNO.h"
#include "Tools.h"
#include "Menus.h"


bool validSelection(string s){
    if (s == "accel" || s == "gyro" || s == "compass") return true;
    if (s == "euler" || s == "quat" || s == "quaternion") return true;
    if (s == "x") return true;
    else return false;
}
void BNOMenu(){

    while(true)
    {
        cout << "(BNO Menu)\n";
        cout << "Press 'x' at any time to go back one menu\n\n";
        cout << "PICK THREE: \"accel\", \"gyro\", \"compass\","
                " \"euler\", or \"quaternion\"\n\n";
        
        string one;
        while (true) {
            cout << "1st : \n";
            getline(cin, one);
            if (validSelection(one)) break;
        }
        if (one == "x") break;

        string two;
        while(true){
            cout << "\n2nd : \n";                 
            getline(cin, two);
            if (validSelection(two)) break;
        }
        if (two == "x") break;

        string three;
        while(true){        
            cout << "\n3rd : \n";
            getline(cin, three);
            if (validSelection(three)) break;        
        }
        if (three == "x") break;

        while(true)
        {      
            displayBNOData(one);
            displayBNOData(two);
            displayBNOData(three);
            usleep(90000);

            // if statement suppresses annoying warning
            //      (unused return value)
            // and due to a bug with the MATE terminal, the
            // return type is bizzare, so we have to use complement logic...
            if (system("clear")) break;

            if GO_BACK break;

        }
    }

}
