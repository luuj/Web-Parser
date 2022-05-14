#include <iostream>
#include "set.h"
#include "web.h"
#include <string>
#include <fstream>
#include <stdexcept>
#include <ctype.h>
using namespace std;

WebPage::WebPage() //Constructor
{
}

WebPage::~WebPage() //Destructor
{}

WebPage::WebPage (string filename)
{
  fileName = filename;
  ifstream file(filename.c_str());
  string buffer;
  if (file.fail()) //Check file name
    throw invalid_argument ("Error when reading file");

  for (int i=0; !file.eof(); i++) //Read until end of file
  {
    getline(file, buffer);
    page.push_back(buffer);
  }
}

void WebPage::parse()
{
  string buffer, temp;
  bool flag;
  for (unsigned int i=0; i<page.size()-1; i++)
  {
    buffer = page[i];  //Retrieve string    
    while (1)
    {
      int k=0;
      while (1) //Until symbol/white space
      {
        if (buffer[0] == '[')
        {
          flag = true;
        }
        if (isspace(buffer[k]))     
	{
          k++;   
          break;       
	}
        if (isalnum(buffer[k])) //Iterate through string
          k++;     
        else
        {
          k++;
          break;
        }
      }

      if ((flag == true) && (buffer[k] == ']') && (buffer[k+1] != '('))
        flag = false;

      if (buffer.size() <2 && !isalnum(buffer[0])) //Check if last character is a symbol
        break;

      temp = buffer.substr(0, k-1);
      if (temp.empty()) //Check for double space
      {
	buffer = buffer.substr(k);
	continue;
      }

      for (int j=0; j<(signed)temp.length(); j++) //Change from upper-case to lower-case
      {
        if (isupper(temp[j]))
          temp[j] = tolower(temp[j]);
      }

      if (words.find(temp) == words.end())
        words.insert(temp); //Insert substring if not already in set

      try //Test for empty buffer
      {
        buffer = buffer.substr(k);
      }
      catch(out_of_range)
      {
        break;
      }

      if ( (flag == true) && (buffer[0] == '(') ) //If link is found, insert into webLink set and skip ( )
      {
        int tempCount = 1;
        while (buffer[tempCount] != ')')
          tempCount++;
        string insertString = buffer.substr(1, tempCount-1);
        webLink.insert(insertString);
        flag = false;

        buffer = buffer.substr(tempCount);
      }
    }
  }
}

string WebPage::removeParen()
{
  string buffer, temp, fileStore;
  bool flag = false;
  for (unsigned int i=0; i<page.size()-1; i++)
  {
    buffer = page[i];  //Retrieve string 
    for (unsigned int k=0; k<buffer.length(); k++)
    {
      char checker = buffer[k];
      if (checker == '[')
        flag = true;

      if ((flag == true) && (buffer[k] == ']') && (buffer[k+1] != '('))
        flag = false;
  
      if ( (flag == true) && (buffer[k] == '(') )
      {
        while (buffer[k] != ')')
        {
          k++;
        }
        continue;
      }
      fileStore = fileStore + checker;
    }
  }
  return fileStore;
}

ostream& operator<< (ostream& os, const WebPage& pg)
{
  for (unsigned int i=0; i<pg.page.size()-1; i++)
  {
    os << pg.page[i] << endl;
  }
  return os;
}

Set<string> WebPage::allWords () 
{
  parse();
  return words;
}

void WebPage::addIncomingLink (WebPage* start)
{
  webIn.insert(start);
}

void WebPage::addOutgoingLink (WebPage* target)
{
  webOut.insert(target);
}

Set<WebPage*> WebPage::allOutgoingLinks () const
{
  return webOut; 
}

Set<WebPage*> WebPage::allIncomingLinks () const
{
  return webIn;
}

string WebPage::filename () const
{
  return fileName;
}

Set<string> WebPage::queuedLinks ()
{
  return webLink;
}




