#ifndef main_Window
#define main_Window
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QString>
#include <QListWidget>
#include <QListWidgetItem>
#include <vector>
#include "../webparse/web.h"
using namespace std;

class mainWindow : public QWidget
{
  Q_OBJECT

  public:
    mainWindow (QWidget* parent = NULL);
    QSize sizeHint() const { return QSize( 650, 500 ); }
    void copyQuery(map<string, Set<WebPage*> > q);
    void copyWeb(vector<WebPage*>);

  private slots:
    void searchClicked();
    void ANDClicked();
    void ORClicked();
    void quitClicked();
    void popupWindow(QListWidgetItem*);
  private:
    QLineEdit* searchText;
    QPushButton* searchButton;
    QPushButton* ANDButton;
    QPushButton* ORButton;
    QPushButton* quitButton;
    QListWidget* bigList;
    QWidget* window;

    string Qinput;
    map<string, Set<WebPage*> > query;
    Set<WebPage*> compSet, tempSet;
    vector<WebPage*> webP;
    bool opened;

    QListWidget* inList;
    QListWidget* outList;
};

#endif
