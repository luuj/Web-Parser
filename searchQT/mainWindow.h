#ifndef main_Window
#define main_Window
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QString>
#include <QListWidget>
#include <QListWidgetItem>
#include <QRadioButton>
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
    Set<WebPage*> pageRank(Set<WebPage*>);

  private slots:
    void searchClicked();
    void ANDClicked();
    void ORClicked();
    void quitClicked();
    void popupWindow(QListWidgetItem*);
    void alphaClicked();
    void prClicked();
  private:
    QLineEdit* searchText;
    QPushButton* searchButton;
    QPushButton* ANDButton;
    QPushButton* ORButton;
    QPushButton* quitButton;
    QListWidget* bigList;
    QWidget* window;
    QRadioButton* alphaButton;
    QRadioButton* prButton;

    string Qinput;
    map<string, Set<WebPage*> > query;
    Set<WebPage*> compSet, tempSet;
    vector<WebPage*> webP;
    bool opened;
    string sortOrder;

    QListWidget* inList;
    QListWidget* outList;
};

#endif
