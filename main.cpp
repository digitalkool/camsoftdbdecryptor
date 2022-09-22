/*
Name:               CPOCamsoftDBDecrytor
Description:       A command line utility program to convert a Camsoft customer database TDB file
                    to plain text file.
URI:                
Author:            Ruben Ordonez
Author URI:        https://rubenordonez.com/
Version:           1.0
Stable tag:        1.0
License:           GPL v2 or later
License URI:       https://www.gnu.org/licenses/gpl-2.0.txt

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 
2 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
with this program. If not, visit: https://www.gnu.org/licenses/
*/

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <cstring>

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{

    string filename ("TDB.DAT");
    string outfilename ("TDB.TXT");

    ifstream read;
    ofstream write;

    read.open(filename,ios::binary);
    write.open(outfilename,ios::binary);

    if(read.is_open())
    {
        unsigned char letter;
        string line;
        string strBuf;
        string end("END\r");
        char seed = 0x1F;
        unsigned char encKey;
        int res;
        int lineLength;
        int charIndex; 
        int lineCounter;

        // read and disregard first line in file.
        getline(read,line);
        line.clear();
        lineCounter = 0;

get_line:
/****************************************
/ read next line from file
*/

        getline(read,line);

        res = line.compare(end);

        if (res == 0) goto close_files;

        strBuf.clear();
   
        lineLength = line.length();
        charIndex = 0;
        lineCounter ++;
        if(lineCounter == 118) lineCounter = 0;
        encKey = seed;

get_char:
/****************************************
/ read char from file
/ decrypt char
/ sav char to buffer
*/
        if((charIndex+1) == lineLength) goto write_line;

        encKey = encKey + 0x2;
        encKey = encKey + lineCounter;

        if (encKey >= 0x80) encKey = seed;

        letter = line.at(charIndex);

        letter = letter - encKey;

        if(letter == 0x0D ) letter = 0x24;

        strBuf.insert(0, 1, letter);

        charIndex ++;
        goto get_char;

write_line:
/****************************************
/ write line to new file
*/

        strBuf.append("\r\n");
        write.write(strBuf.data(), strBuf.length());
        goto get_line;

    }

/****************************************
/ close files
*/
close_files:

    read.close();
    write.close();
      

	return 0;
}

