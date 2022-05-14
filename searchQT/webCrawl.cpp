#include <iostream>
#include <map>
#include <fstream>
#include <vector>
#include <ctype.h>
#include <string>
#include "../webparse/set.h"
using namespace std;

void webCrawl (Set<string>& textFiles, string textName); 

int main(int argc, char* argv[])
{
  Set<string> allLinks;
  
  if (argc !=3)
  {
    cout << "Please enter the seed file name and output file name" << endl;
    return 0;
  }

  ifstream seedFile(argv[1]);
  if (seedFile.fail())
  {
    cout << "Could not open seed file" << endl;
    return 0;
  }

  vector<string> filename;
  string temp;
  seedFile >> temp;
  while (!seedFile.eof()) //Read in data files
  {    
    filename.push_back(temp);
    seedFile >> temp;
  }

  for (int i=0; i<(signed)filename.size(); i++)
  {
    allLinks.insert(filename[i]);
    webCrawl(allLinks, filename[i]);
  }
  
  ofstream outFile(argv[2]);
  Set<string>::iterator parseIT = allLinks.begin();
  for (parseIT =allLinks.begin(); parseIT != allLinks.end(); ++parseIT)
  {
    outFile << *parseIT << " ";
  }
}


void webCrawl (Set<string>& textFiles, string textName) 
{
  char fileNameTemp[textName.length()+2];
  textName.copy(fileNameTemp, textName.length());
  fileNameTemp[textName.length()] = '\0';

  ifstream tempCrawl(fileNameTemp);

  if (tempCrawl.fail())
  {
    cout << textName << " could not be found." << endl;
    return;
  }

  vector<string> wordHolder;
  string buffer;
  for (int i=0; !tempCrawl.eof(); i++) //Read until end of file
  {
    getline(tempCrawl, buffer);
    wordHolder.push_back(buffer);
  }

  string addedFile;
  bool flag = false; //Parsing through webpage contents
  for (unsigned int i=0; i<wordHolder.size()-1; i++)
  {
    buffer = wordHolder[i];  //Retrieve string 
    for (unsigned int k=0; k<buffer.length(); k++)
    {
      char checker = buffer[k];
      if (checker == '[')     
        flag = true;      

      if ((flag == true) && (buffer[k] == ']') && (buffer[k+1] != '('))
        flag = false;

  
      if ( (flag == true) && (buffer[k] == '(') ) //If ( ) is found for link, insert into set and DFS search it
      {
        k++;
        while (buffer[k] != ')')
        {
          addedFile = addedFile + buffer[k];
          k++;
        }
        if (textFiles.find(addedFile) != textFiles.end() )
        {
          addedFile.clear();
          continue;
        }
        textFiles.insert(addedFile);
        webCrawl(textFiles, addedFile);
        addedFile.clear();
        continue;
      }
    }
  }
}
