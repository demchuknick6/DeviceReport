#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProgressBar>
#include <Qtimer>
#include <QLabel>
#include <aboutdlg.h>
#include <QTreeWidget>
#include <QVector>
#include <QDesktopServices>
#include <QUrl>
#include <fstream>
#include <windows.h>
#include <setupapi.h>
#include <devguid.h> //GUID class

#pragma comment(lib, "setupapi.lib")

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    GUID currGuid;
    QVector<QTreeWidgetItem*> roots;
    QTreeWidgetItem *mainRoot;
    QTimer *timer=new QTimer(this);
    QProgressBar *prRefresh= new QProgressBar;
    QLabel *lbProgress= new QLabel("Очікування");

private slots:
    void GetInfo(const short rowIndex, int classGuid);
    void GetOtherInfo(const short wOrder);
    void on_actAudio_Input_Output_triggered();
    void on_actDisplays_adapters_triggered();
    void on_actMonitors_triggered();
    void on_actImage_processing_devices_triggered();
    void on_actAbout_triggered();
    void on_actQuit_triggered();
    void on_actRefresh_triggered();
    void on_actSave_triggered();
    void saveFile(const ushort guid, std::ofstream &output);
    std::string GetDevRegInfo(DWORD SPDRP, const short wOrder, GUID currGuid);
    void progressAction();
    QTreeWidgetItem *addRootDevClass(QString name, short index, ushort guid);
    void addCildDev(QTreeWidgetItem *parent, QString name, short index, ushort guid);
    void getDevDesc(const ushort guid, QTreeWidgetItem *rootDevClass);
    void on_listwDevices_currentItemChanged(QTreeWidgetItem *current);
    void initStatusBar();
    void initListDevInfo();
    void initListDevice();
    void initActions();
    void on_actHelp_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
