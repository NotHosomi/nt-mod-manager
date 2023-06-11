#include "MainWindow.h"
#include <qmessagebox.h>

#define DEFAULT_ALPHA 65535u // Max unsigned short

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    setFixedWidth(540);
    setFixedHeight(413);
    Refresh();
    std::vector<std::string> preinstalled = m_Files.GetInstalledList();
    for (auto& s : preinstalled)
    {
        ui.lst_TargetMods->addItem(QString::fromStdString(s));
    }
}

MainWindow::~MainWindow()
{}

// Slots
void MainWindow::on_btn_Apply_clicked()
{
    ui.lbl_Status->setText("Installing...");
    std::vector<std::string> vModList;
    for (auto item : ui.lst_TargetMods->findItems("*", Qt::MatchWildcard))
    {
        vModList.push_back(item->text().toStdString());
    }
    std::string errorText;
    std::vector<int> errorIndices;
    if (!m_Files.InstallMods(vModList, errorText, errorIndices))
    {
        for (auto& index : errorIndices)
        {
            QList<QListWidgetItem*> matches = ui.lst_TargetMods->findItems(QString::fromStdString(vModList[index]), Qt::MatchExactly);
            for (auto p : matches)
            {
                p->setBackgroundColor(QColor(255, 0, 0));
            }
        }
        ui.lbl_Status->setText("Done with error(s)");
        ErrorDialogue(errorText);
    }
    else
    {
        ui.lbl_Status->setText("Done");
    }
}

void MainWindow::on_btn_Refresh_clicked()
{
    Refresh();
}

void MainWindow::on_lst_AllMods_itemDoubleClicked(QListWidgetItem* item)
{
    // Add item if already in install list
    if (item->font().bold())
    {
        Untarget(item->text());
        //Untarget(item->text());
        //RemoveFromList(ui.lst_TargetMods, item->text());
        return;
    }

    // otherwise, add item
    Target(item);
}

void MainWindow::on_lst_TargetMods_itemDoubleClicked(QListWidgetItem* item)
{
    Untarget(item->text());
    //QList<QListWidgetItem*> matches = ui.lst_AllMods->findItems(item->text(), Qt::MatchExactly);
    //for (auto p : matches)
    //{
    //    p->setBackground(QColor(0, 0, 0));
    //    QFont font = item->font();
    //    font.setBold(false);
    //    item->setFont(font);
    //}
    //RemoveFromList(ui.lst_TargetMods, item);
}

// Key functionality
void MainWindow::Refresh()
{
    std::vector<std::string> vDiscovered = m_Files.Discover();
    for (auto& iter : vDiscovered)
    {
        ui.lst_AllMods->addItem(QString::fromStdString(iter));
    }
}

void MainWindow::Target(QListWidgetItem* item)
{
    ui.lbl_Status->setText("Changes pending");
    ui.lst_TargetMods->insertItem(0, item->text());
    item->setBackgroundColor(QColor(184, 255, 207));
    QFont font = item->font();
    font.setBold(true);
    item->setFont(font);
}

void MainWindow::Untarget(const QString& name)
{
    ui.lbl_Status->setText("Changes pending");
    QList<QListWidgetItem*> matches = ui.lst_AllMods->findItems(name, Qt::MatchExactly);
    for (auto p : matches)
    {
        p->setBackground(QColor(255, 255, 255));
        QFont font = p->font();
        font.setBold(false);
        p->setFont(font);
    }
    RemoveFromList(ui.lst_TargetMods, name);
}

// Utils
void MainWindow::ErrorDialogue(std::string what)
{
    QMessageBox messageBox;
    messageBox.critical(0, "Error", QString::fromStdString(what));
    messageBox.setFixedSize(500, 200);
}

void MainWindow::RemoveFromList(QListWidget* list, QListWidgetItem* item)
{
    list->removeItemWidget(item);
    delete item;
}

bool MainWindow::RemoveFromList(QListWidget* list, const QString& name)
{
    QList<QListWidgetItem*> matches = list->findItems(name, Qt::MatchExactly);
    bool found = matches.size() > 0;
    for (auto p : matches)
    {
        list->removeItemWidget(p);
        delete p;
    }
    return found;
}