#include <QApplication>
#include "mainWindow.h"
#include <iostream>
#include <map>
#include <fstream>
#include <vector>
#include <ctype.h>
#include <string>
using namespace std;

int main(int argc, char* argv[])
{
  map<string, Set<WebPage*> > query;

  if (argc != 2){
    cout << "Please enter in one data file" << endl;
    return 0;
  }
  
  ifstream datafile(argv[1]);
  if (datafile.fail())
  {
    cout << "Could not open file" << endl;
    return 0;
  }

  vector<string> filename;
  vector<WebPage*> website;
  string temp;
  datafile >> temp;
  while (!datafile.eof()) //Read in data files
  {    
    filename.push_back(temp);
    datafile >> temp;
  }

  for (int i=0; i<(signed)filename.size(); i++)
  {
    WebPage* temp2 = new WebPage(filename[i]);
    website.push_back(temp2);
  }

  for (int i=0; i<(signed)website.size(); i++)
  {
    Set<string> tempList = website[i]->allWords();
    Set<WebPage*> tempSet;
   
    Set<string>::iterator tempWord = tempList.begin();
    tempSet.insert(website[i]);

    for (tempWord = tempList.begin(); tempWord != tempList.end(); ++tempWord)
    {
      if (query.count(*tempWord) == 0) //If word is not already in map, add to map
      {
        query.insert(make_pair(*tempWord, tempSet)); //Add query and associated set to map
        continue;
      }
      if (query.count(*tempWord) >0)
        query.find(*tempWord)->second.insert(website[i]); //If in set, add to existing Webpage set
    }

    Set<string> tempLinks = website[i]->queuedLinks(); //Adding incoming/outgoing links
    Set<string>::iterator linkWord = tempLinks.begin();
    for (linkWord = tempLinks.begin(); linkWord != tempLinks.end(); ++linkWord) //Iterate through all outgoing links in current website
    {
      for (int k=0; k<(signed)website.size(); k++) //Check all other websites for matches
      {
        if (*linkWord == website[k]->filename() )
        {
          website[i]->addOutgoingLink(website[k]); //Add outgoing link to current website
          website[k]->addIncomingLink(website[i]); //Add incoming link to target website
        } 
      }
    }
  }

  QApplication app(argc, argv);
  mainWindow form;
  form.copyQuery(query);
  form.copyWeb(website);

  form.show();
  return app.exec();
  return 0;
}

