#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QListWidgetItem>
#include <QLabel>
#include <QTextEdit>
#include <QDialog>
#include <QRadioButton>
#include <sstream>
#include "mainWindow.h"
#include "msort.h"
#define R_PROB 0.15
using namespace std;
 
mainWindow::mainWindow(QWidget* parent)
{
  searchButton = new QPushButton("&Search");
  quitButton = new QPushButton("&Quit");
  ANDButton = new QPushButton("&AND");
  ORButton = new QPushButton("&OR"); 
  alphaButton = new QRadioButton("Alphabetical");
  prButton = new QRadioButton("PageRank");
  
  opened = false;

  searchText = new QLineEdit("Enter one word for search; Two+ words for AND/OR");
  searchText->selectAll();
  bigList = new QListWidget;

  QVBoxLayout* textBox = new QVBoxLayout;
  textBox->addWidget(searchText);
  textBox->addWidget(bigList);
  
  QHBoxLayout* radioBox = new QHBoxLayout;
  radioBox->addWidget(alphaButton);
  radioBox->addWidget(prButton); 

  QHBoxLayout* buttonBox = new QHBoxLayout;
  buttonBox->addWidget(searchButton);
  buttonBox->addWidget(ANDButton);
  buttonBox->addWidget(ORButton);
  buttonBox->addWidget(quitButton);

  QVBoxLayout* mainLayout = new QVBoxLayout;
  mainLayout->addLayout(textBox);
  mainLayout->addLayout(radioBox);
  mainLayout->addLayout(buttonBox);

  connect(quitButton, SIGNAL(clicked()), this, SLOT(quitClicked()));
  connect(searchButton, SIGNAL(clicked()), this, SLOT(searchClicked()));
  connect(ANDButton, SIGNAL (clicked()), this, SLOT(ANDClicked()));
  connect(ORButton, SIGNAL (clicked()), this, SLOT(ORClicked()));
  connect(bigList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(popupWindow(QListWidgetItem*)));
  connect(alphaButton, SIGNAL(clicked()), this, SLOT(alphaClicked()));
  connect(prButton, SIGNAL(clicked()), this, SLOT(prClicked()));

  setLayout(mainLayout);
}

void mainWindow::popupWindow(QListWidgetItem* inputFile)
{
  if (opened == true){
    delete window->layout();
      window->close();}

  window = new QWidget;
  QString temp = inputFile->text(); //Title of window
  window->setWindowTitle(temp);

  string fileName = temp.toStdString(); //File text
  string tempText;
  int saveK = 0;
  bool testFile = false;
  for (unsigned int k=0; k<webP.size(); k++)
  {
    if (fileName == webP[k]->filename() )
    {
      testFile = true;
      saveK = k;
      tempText = webP[k]->removeParen();
      break;
    }
  }
  if (testFile == false) //Stop if nothing is found
    return;

  Set<WebPage*> outLink = webP[saveK]->allOutgoingLinks();
  Set<WebPage*> inLink = webP[saveK]->allIncomingLinks();

  QVBoxLayout* sideBars = new QVBoxLayout;
  inList = new QListWidget;
  outList = new QListWidget;
  QTextEdit* inputFileText = new QTextEdit;
  inputFileText->append(QString::fromStdString(tempText));

  Set<WebPage*>::iterator outIT = outLink.begin(); //Display outgoing links
  for (outIT = outLink.begin(); outIT != outLink.end(); ++outIT)
  {
    string outText = (**outIT).filename();
    outList->addItem(QString::fromStdString(outText));
  }

  Set<WebPage*>::iterator inIT = inLink.begin();
  for (inIT = inLink.begin(); inIT != inLink.end(); ++inIT)
  {
    string inText = (**inIT).filename();
    inList->addItem(QString::fromStdString(inText));
  }

  QLabel* incoming = new QLabel;
  QLabel* outgoing = new QLabel;
  incoming->setText("Incoming Links");
  outgoing->setText("Outgoing Links");
  sideBars->addWidget(incoming);
  sideBars->addWidget(inList);
  sideBars->addWidget(outgoing);
  sideBars->addWidget(outList);

  QHBoxLayout* mainWindow = new QHBoxLayout;
  mainWindow->addWidget(inputFileText);
  mainWindow->addLayout(sideBars);

  window->setLayout(mainWindow);
  window->show();
  opened = true;

  connect(inList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(popupWindow(QListWidgetItem*)));
  connect(outList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(popupWindow(QListWidgetItem*)));
}

void mainWindow::quitClicked()
{
  cout << "Exiting program. Bye!" << endl;
  close();
  if (opened == true)
    window->close();
}

void mainWindow::searchClicked()
{
  Qinput = searchText->text().toStdString();
  bigList->clear();
  tempSet.clear();

  if (query.find(Qinput) != query.end() ) //If word is found, place in tempSet  
    tempSet = query.find(Qinput)->second;
  else
  {
    bigList->addItem("Either no word was found or more than one word was entered. Try again."); 
    bigList->addItem("Also make sure there is no space after the first word");
    return;
  }    
 
  AlphaSort compare;
  vector<string> webNameArray; 
  vector<double> tempPRholder, PRholder;
  tempSet = pageRank(tempSet);


  if (sortOrder == "Alpha")
  {
    Set<WebPage*>::iterator stepPrint = tempSet.begin();
    for (stepPrint = tempSet.begin(); stepPrint != tempSet.end(); ++stepPrint) //Get text file names 
    {
      string temp = (**stepPrint).filename();
      webNameArray.push_back(temp);
    }

    mergeSort(webNameArray, compare); //Sort file names alphabetically
  }
  
  if (sortOrder == "PR")
  { 
    prSort comparePR; 
    Set<WebPage*>::iterator stepPrint = tempSet.begin();
    for (stepPrint = tempSet.begin(); stepPrint != tempSet.end(); ++stepPrint)//Retrieve all values of pageRank
      tempPRholder.push_back((**stepPrint).getPageRank());
 
    mergeSort(tempPRholder, comparePR); //Sort pageRank from greatest to least

    for (int i=0; i<(signed)tempPRholder.size(); i++) //Associate sorted pageRanks with webpage names
    {
      for (stepPrint = tempSet.begin(); stepPrint != tempSet.end(); ++stepPrint)
      {
        if ( (**stepPrint).getPageRank() == tempPRholder[i] )
        { 
          webNameArray.push_back( (**stepPrint).filename());
          tempSet.erase(*stepPrint);
          break;
        }
      }
    }
  }
 

  for (int i=0; i<(signed)webNameArray.size(); i++) //Display file names
  {
    string temp = webNameArray[i];
    QString temp2 = QString::fromStdString(temp);
    bigList->addItem(temp2);
  }
}

void mainWindow::ANDClicked()
{
  Qinput = searchText->text().toStdString();
  bigList->clear();
  tempSet.clear();
  compSet.clear();

  stringstream parse;
  parse << Qinput;
  vector<string> tempHold;

  while (1) //Place all strings into tempHold
  {
    parse >> Qinput;
    if (parse.fail())
      break;
    tempHold.push_back(Qinput);
  }

  if (tempHold.size() < 2) //Check if more than one word
  {
    bigList->addItem("Please enter more than one word");
    return;
  }

  unsigned int i=0;
  for (i=0; i<tempHold.size(); i++)
  {
    if (query.find(tempHold[i]) != query.end() ) //If word is found, place in tempSet
    {
      tempSet = query.find(tempHold[i])->second;
      break;
    }
    else 
    {
      bigList->clear();
      bigList->addItem("No results found");
      return;
    }
  }
  
  if (i < tempHold.size())
  {
    for (unsigned int k =i+1; k<tempHold.size(); k++)
    {
      if (query.find(tempHold[k]) != query.end())
        compSet = query.find(tempHold[k])->second;
      else
      {
        bigList->clear();
        bigList->addItem("No results found");
        return;
      }
      if (!compSet.empty())
        tempSet = tempSet.setIntersection(compSet);      
    } 
  }
  if (tempSet.empty() || compSet.empty())
  {
    bigList->addItem("No results found");
    return;
  }
  else
  {
    AlphaSort compare;
    vector<string> webNameArray; 
    vector<double> tempPRholder, PRholder;
    tempSet = pageRank(tempSet);

    if (sortOrder == "Alpha")
    {
      Set<WebPage*>::iterator stepPrint = tempSet.begin();
      for (stepPrint = tempSet.begin(); stepPrint != tempSet.end(); ++stepPrint) //Get text file names 
      {
        string temp = (**stepPrint).filename();
        webNameArray.push_back(temp);
      }

      mergeSort(webNameArray, compare); //Sort file names alphabetically
    }
  
    if (sortOrder == "PR")
    { 
      prSort comparePR; 
      Set<WebPage*>::iterator stepPrint = tempSet.begin();
      for (stepPrint = tempSet.begin(); stepPrint != tempSet.end(); ++stepPrint)//Retrieve all values of pageRank
        tempPRholder.push_back((**stepPrint).getPageRank());
 
      mergeSort(tempPRholder, comparePR); //Sort pageRank from greatest to least

      for (int i=0; i<(signed)tempPRholder.size(); i++) //Associate sorted pageRanks with webpage names
      {
        for (stepPrint = tempSet.begin(); stepPrint != tempSet.end(); ++stepPrint)
        {
          if ( (**stepPrint).getPageRank() == tempPRholder[i] )
          { 
            webNameArray.push_back( (**stepPrint).filename());
            tempSet.erase(*stepPrint);
            break;
          }
        }
      }
    }

    for (int i=0; i<(signed)webNameArray.size(); i++) //Display file names
    {
      string temp = webNameArray[i];
      QString temp2 = QString::fromStdString(temp);
      bigList->addItem(temp2);
    }
  }
}


void mainWindow::ORClicked()
{
  Qinput = searchText->text().toStdString();
  bigList->clear();
  tempSet.clear();
  compSet.clear();

  stringstream parse;
  parse << Qinput;
  vector<string> tempHold;

  while (1) //Place all strings into tempHold
  {
    parse >> Qinput;
    if (parse.fail())
      break;
    tempHold.push_back(Qinput);
  }
 
  if (tempHold.size() < 2) //Check if more than one word
  {
    bigList->addItem("Please enter more than one word");
    return;
  }

  unsigned int i=0;
  for (i=0; i<tempHold.size(); i++)
  {
    if (query.find(tempHold[i]) != query.end() ) //If word is found, place in tempSet
    {
      tempSet = query.find(tempHold[i])->second;
      break;
    }
  }
  
  if (i < tempHold.size())
  {
    for (unsigned int k =i+1; k<tempHold.size(); k++)
    {
      if (query.find(tempHold[k]) != query.end())
        compSet = query.find(tempHold[k])->second;
      if (!compSet.empty())
        tempSet = tempSet.setUnion(compSet);      
    } 
  }
  if (tempSet.empty())
  {
    bigList->addItem("No results found");
    return;
  }
  else
  {
    AlphaSort compare;
    vector<string> webNameArray; 
    vector<double> tempPRholder, PRholder;
    tempSet = pageRank(tempSet);

    if (sortOrder == "Alpha")
    {
      Set<WebPage*>::iterator stepPrint = tempSet.begin();
      for (stepPrint = tempSet.begin(); stepPrint != tempSet.end(); ++stepPrint) //Get text file names 
      {
        string temp = (**stepPrint).filename();
        webNameArray.push_back(temp);
      }

      mergeSort(webNameArray, compare); //Sort file names alphabetically
    }
  
    if (sortOrder == "PR")
    { 
      prSort comparePR; 
      Set<WebPage*>::iterator stepPrint = tempSet.begin();
      for (stepPrint = tempSet.begin(); stepPrint != tempSet.end(); ++stepPrint)//Retrieve all values of pageRank
        tempPRholder.push_back((**stepPrint).getPageRank());
 
      mergeSort(tempPRholder, comparePR); //Sort pageRank from greatest to least

      for (int i=0; i<(signed)tempPRholder.size(); i++) //Associate sorted pageRanks with webpage names
      {
        for (stepPrint = tempSet.begin(); stepPrint != tempSet.end(); ++stepPrint)
        {
          if ( (**stepPrint).getPageRank() == tempPRholder[i] )
          { 
            webNameArray.push_back( (**stepPrint).filename());
            tempSet.erase(*stepPrint);
            break;
          }
        }
      }
    }

    for (int i=0; i<(signed)webNameArray.size(); i++) //Display file names
    {
      string temp = webNameArray[i];
      QString temp2 = QString::fromStdString(temp);
      bigList->addItem(temp2);
    }
  }
}

void mainWindow::copyQuery(map<string, Set<WebPage*> > q)
{
  query = q;
}

void mainWindow::copyWeb(vector<WebPage*> temp)
{
  webP = temp;
}

Set<WebPage*> mainWindow::pageRank(Set<WebPage*> tempList)
{
  Set<WebPage*>::iterator pageIT = tempList.begin();
  Set<WebPage*> unsortedList = tempList;

  for (pageIT = tempList.begin(); pageIT != tempList.end(); ++pageIT) //Find outgoing and incoming links from all Webpage hits
  {
    Set<WebPage*> tempOutSet = (**pageIT).allOutgoingLinks(); //All outgoing links to current page
    Set<WebPage*> tempInSet = (**pageIT).allIncomingLinks(); //All incoming links to current page

    unsortedList = unsortedList.setUnion(tempOutSet);
    unsortedList = unsortedList.setUnion(tempInSet); //UnsortedList now has all hits + incoming/outgoing from all hit pages
  }

  double vertices = unsortedList.size();  
  double totalPR;
  double distribution = 1/vertices;

  int tempI = 0; //Initialize pageRank scores;
  for (pageIT = unsortedList.begin(); pageIT != unsortedList.end(); ++pageIT)
  {
    (**pageIT).setPageRank(distribution);
    tempI++;    
  }

  for (int i=0; i<30; i++)
  {
    for (pageIT = unsortedList.begin(); pageIT != unsortedList.end(); ++pageIT)
    {
      Set<WebPage*> tempInSet = unsortedList.setIntersection((**pageIT).allIncomingLinks());
      Set<WebPage*> testSet = unsortedList.setIntersection((**pageIT).allOutgoingLinks());

      Set<WebPage*>::iterator linkIT = tempInSet.begin(); //Iterate through incoming links
      for (linkIT = tempInSet.begin(); linkIT != tempInSet.end(); ++linkIT)
      {
        Set<WebPage*> tempOutSet = unsortedList.setIntersection((**linkIT).allOutgoingLinks());
        double outDegree = tempOutSet.size();  
        totalPR += (((1-R_PROB)*(**linkIT).getPageRank())/outDegree); // X(I) * .85 / # of outgoing links 
      }

      Set<WebPage*>::iterator randomIT = unsortedList.begin();
      for (randomIT = unsortedList.begin(); randomIT != unsortedList.end(); ++randomIT)
      {
        if (*randomIT != *pageIT)
          totalPR += ( (**randomIT).getPageRank() * R_PROB ) / (vertices-1);
      }

      if (testSet.empty()) //Test if it is a sink or not
      {
        double tempNode = (**pageIT).getPageRank() / vertices; //x(i)/n if sink node
        totalPR += tempNode;
        tempNode -= ( (**pageIT).getPageRank() * R_PROB ) / (vertices-1);
        Set<WebPage*>::iterator sinkIT = unsortedList.begin();
        for (sinkIT = unsortedList.begin(); sinkIT != unsortedList.end(); ++sinkIT) //Iterate through all nodes
        {
          if (*sinkIT != *pageIT)
            (**sinkIT).addPageRank(tempNode); //Add x(i)/n of the sink node to all   
        }
      }

      (**pageIT).setTempRank(totalPR);
      totalPR = 0; //Reset for next website
    }

    for (pageIT = unsortedList.begin(); pageIT != unsortedList.end(); ++pageIT)
    {
      (**pageIT).setPageRank((**pageIT).getTempRank());   
    } 
  }
  return unsortedList;
}


void mainWindow::alphaClicked()
{
  sortOrder = "Alpha";
}

void mainWindow::prClicked()
{
  sortOrder = "PR";
}


