#ifndef WEB_H
#define WEB_H
#include "set.h"
#include <string>
#include <vector>

class WebPage {
  public:
    WebPage ();    // constructor that creates an empty page
    WebPage (string filename);
      /* constructor that initializes the page from the given file.
         Should throw an exception if the file does not exist
         or has other problems. */
    ~WebPage ();   // destructor

    Set<string> allWords ();
      /* Returns a set containing all individual words on the web page. */   

    friend ostream & operator<< (ostream & os, const WebPage & page);
      /* Declares an operator we can use to print the web page. */

    Set<WebPage*> allOutgoingLinks () const;
      /* Returns "pointers" to all webpages that this page has links to. 
         As discussed above, this could be as a set or via an iterator,
         and it could be as actual pointers, or as strings, 
         or possibly other. */   

    Set<WebPage*> allIncomingLinks () const;
      /* Returns "pointers" to all webpages that link to this page. 
         Same consideration as previous function. */   

    string filename () const;
      /* Returns the filename from which this page was read. */

    void parse();

    void addIncomingLink (WebPage* start);
      /* Adds a link from start to the current page 
         in the current page's storage. */

    void addOutgoingLink (WebPage* target); 
      /* Adds a link from the current page to the target
         in the current page's storage. */

    Set<string> queuedLinks (); //returns Set of strings containing queued webpages to be parsed

    string removeParen();


  private:
    string fileName;
    vector<string> page;
    Set<string> words, webLink;
    Set<WebPage*> webOut, webIn;
    // you get to decide what goes here.
};

#endif
