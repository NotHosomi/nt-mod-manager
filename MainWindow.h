#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"
#include "FileManager.h"

struct TModStatus
{
    bool targetted = false;
    bool errored = false;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btn_Apply_clicked();
    void on_btn_Refresh_clicked();
    void on_lst_AllMods_itemDoubleClicked(QListWidgetItem* item);
    void on_lst_TargetMods_itemDoubleClicked(QListWidgetItem* item);

private:
    Ui::MainWindowClass ui;
    FileManager m_Files;

    void Refresh();
    void Target(QListWidgetItem* item);
    void Untarget(const QString& name);

    void ErrorDialogue(std::string what);
    void RemoveFromList(QListWidget* list, QListWidgetItem* item);
    bool RemoveFromList(QListWidget* list, const QString& name);
};
