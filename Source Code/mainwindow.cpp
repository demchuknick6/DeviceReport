#include "mainwindow.h"
#include "ui_mainwindow.h"

int countOfDevices=0;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initStatusBar();
    initActions();
    initListDevice();
    initListDevInfo();
    connect(timer, SIGNAL(timeout()),this,SLOT(progressAction()));
    on_actRefresh_triggered();
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initStatusBar()
{
    prRefresh->setFixedSize(200,15);
    lbProgress->setFixedSize(70,20);
    ui->statusBar->setContentsMargins(10,2,0,2);
    ui->statusBar->addWidget(lbProgress);
    ui->statusBar->addWidget(prRefresh);
}

void MainWindow::initActions()
{
    ui->actAudio_Input_Output->setChecked(true);
    ui->actDisplays_adapters->setChecked(true);
    ui->actMonitors->setChecked(true);
    ui->actImage_processing_devices->setChecked(true);
}

void MainWindow::initListDevice()
{
    ui->listwDevices->setColumnCount(3);
    ui->listwDevices->setColumnHidden(1,true);
    ui->listwDevices->setColumnHidden(2,true);
    mainRoot=addRootDevClass("Number of devices: ", -1, roots.size());

    //addCildDev(mainRoot, "",-1,0);
    //mainRoot->setText(0,"Number of devices: "+QString::number(countOfDevices));
    //QString::number(countOfDevices)
    //mainRoot->setHidden(true);
    mainRoot->setExpanded(true);
}

void MainWindow::initListDevInfo()
{
    ui->listwDeviceInfo->setColumnCount(2);
    ui->listwDeviceInfo->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->listwDeviceInfo->setRowCount(10);

    for(int i=0; i<ui->listwDeviceInfo->rowCount(); i++)
    {
        for(int j=0; j<ui->listwDeviceInfo->columnCount(); j++)
        {
            QTableWidgetItem *it1= new QTableWidgetItem();
            ui->listwDeviceInfo->setItem(i,j,it1);
        }
    }
    ui->listwDeviceInfo->setColumnWidth(0,120);
    ui->listwDeviceInfo->horizontalHeader()->setStretchLastSection(true);
    ui->listwDeviceInfo->item(0, 0)->setText("GUID");
    ui->listwDeviceInfo->item(1, 0)->setText("Class");
    ui->listwDeviceInfo->item(2, 0)->setText("Enumerator");
    ui->listwDeviceInfo->item(3, 0)->setText("Device Manufacturer");
    ui->listwDeviceInfo->item(4, 0)->setText("Physical Device Object Name");
    ui->listwDeviceInfo->item(5, 0)->setText("Service");
    ui->listwDeviceInfo->item(6, 0)->setText("Hardware ID");
    ui->listwDeviceInfo->item(7, 0)->setText("File Path");
    ui->listwDeviceInfo->item(8, 0)->setText("Driver Date");
    ui->listwDeviceInfo->item(9, 0)->setText("Section Name");
}

void MainWindow::progressAction()
{
    timer->start(3);
    lbProgress->setText("Refreshing");
    prRefresh->setValue(prRefresh->value()+1);
    if(prRefresh->value()>=100)
    {
        timer->stop();
        prRefresh->reset();
        lbProgress->setText("Done");
    }
}

void MainWindow::on_actAudio_Input_Output_triggered()
{
    on_actRefresh_triggered();
}

void MainWindow::on_actDisplays_adapters_triggered()
{
    on_actRefresh_triggered();
}

void MainWindow::on_actMonitors_triggered()
{
    on_actRefresh_triggered();
}

void MainWindow::on_actImage_processing_devices_triggered()
{
    on_actRefresh_triggered();
}

void MainWindow::on_actAbout_triggered()
{
    aboutdlg *dlg=new aboutdlg(this);
    dlg->show();
    dlg->exec();
    return;
}

void MainWindow::on_actQuit_triggered()
{
    close();
}

void MainWindow::getDevDesc(const ushort guid, QTreeWidgetItem *rootDevClass)
{
    HDEVINFO hDevInfo;
    SP_DEVINFO_DATA DeviceInfoData;
    DWORD i;

    switch(guid)
    {
    case 0: hDevInfo = SetupDiGetClassDevs(&GUID_DEVCLASS_MEDIA,NULL,NULL,DIGCF_PRESENT);
        break;
    case 1: hDevInfo = SetupDiGetClassDevs(&GUID_DEVCLASS_DISPLAY,NULL,NULL,DIGCF_PRESENT);
        break;
    case 2: hDevInfo = SetupDiGetClassDevs(&GUID_DEVCLASS_MONITOR,NULL,NULL,DIGCF_PRESENT);
        break;
    case 3: hDevInfo = SetupDiGetClassDevs(&GUID_DEVCLASS_IMAGE,NULL,NULL,DIGCF_PRESENT);
        break;
    }

    DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
    for (i=0;SetupDiEnumDeviceInfo(hDevInfo,i,&DeviceInfoData);i++)
    {
      DWORD DataT;
      LPTSTR buffer = NULL;
      DWORD buffersize = 0;

      //LIST
      while (!SetupDiGetDeviceRegistryProperty(
          hDevInfo,
          &DeviceInfoData,
          SPDRP_DEVICEDESC,
          &DataT,
          (PBYTE)buffer,
          buffersize,
          &buffersize))
      {
          if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
          {
              // Change the buffer size.
              if (buffer) LocalFree(buffer);
              buffer = (LPTSTR)LocalAlloc(LPTR,buffersize * 2);
          }
      }

      addCildDev(rootDevClass, QString::fromWCharArray((TCHAR*)buffer), i, guid);
      countOfDevices++;
      if (buffer) LocalFree(buffer);
    }

    SetupDiDestroyDeviceInfoList(hDevInfo);
}

void MainWindow::on_actRefresh_triggered()
{
    for(int i=0; i<10; i++)
        ui->listwDeviceInfo->item(i,1)->setText("");

    countOfDevices=0;
    for(int i=roots.size()-1; i>=0; i--)
        delete roots[i];

    roots.clear();

    if(ui->actAudio_Input_Output->isChecked())
    {
        QTreeWidgetItem *root=addRootDevClass("Audio Input&Output", -1, 0);

        root->setIcon(0, QIcon(":/icon/main/res/Audio.png"));
        getDevDesc(0, root);
        roots.push_back(root);
    }
    if(ui->actDisplays_adapters->isChecked())
    {
        QTreeWidgetItem *root=addRootDevClass("Display adapters", -1, 1);
        root->setIcon(0, QIcon(":/icon/main/res/Display.png"));
        getDevDesc(1, root);
        roots.push_back(root);
    }
    if(ui->actMonitors->isChecked())
    {
        QTreeWidgetItem *root=addRootDevClass("Monitors", -1, 2);
        root->setIcon(0, QIcon(":/icon/main/res/Monitor.png"));
        getDevDesc(2, root);
        roots.push_back(root);
    }
    if(ui->actImage_processing_devices->isChecked())
    {
        QTreeWidgetItem *root=addRootDevClass("Image processing devices", -1, 3);
        root->setIcon(0, QIcon(":/icon/main/res/Image.png"));
        getDevDesc(3, root);
        roots.push_back(root);
    }

    mainRoot->setText(0,"Number of devices: "+QString::number(countOfDevices));
    progressAction();
}

void MainWindow::on_actSave_triggered()
{
    std::ofstream out;
    out.open("info.txt", std::ios::out);
    out.close();
    out.open("info.txt", std::ios::app);
    if (out.is_open())
    {
        if (ui->actAudio_Input_Output->isChecked())
        {
            saveFile(0, out);
        }
        if (ui->actDisplays_adapters->isChecked())
        {
            saveFile(1, out);
        }
        if (ui->actMonitors->isChecked())
        {
            saveFile(2, out);
        }
        if (ui->actImage_processing_devices->isChecked())
        {
            saveFile(3, out);
        }
            /*for (int i = 0; i < roots.size(); i++)
            {
                saveFile(i, out);
                //out << "hello" << std::endl;
            }*/
    }
    out.close();
    progressAction();
    /*QMessageBox msgBox;
    msgBox.setText("The information has been succesfully\nsaved to \"info.txt\" file!");
    msgBox.exec();*/
}


void MainWindow::saveFile(const ushort guid, std::ofstream &output)
{
    HDEVINFO hDevInfo;
    SP_DEVINFO_DATA DeviceInfoData;
    DWORD i;
    GUID Guid;
    switch (guid)
    {
    case 0: hDevInfo = SetupDiGetClassDevs(&GUID_DEVCLASS_MEDIA, NULL, NULL, DIGCF_PRESENT);
        Guid = GUID_DEVCLASS_MEDIA;
        break;
    case 1: hDevInfo = SetupDiGetClassDevs(&GUID_DEVCLASS_DISPLAY, NULL, NULL, DIGCF_PRESENT);
        Guid = GUID_DEVCLASS_DISPLAY;
        break;
    case 2: hDevInfo = SetupDiGetClassDevs(&GUID_DEVCLASS_MONITOR, NULL, NULL, DIGCF_PRESENT);
        Guid = GUID_DEVCLASS_MONITOR;
        break;
    case 3: hDevInfo = SetupDiGetClassDevs(&GUID_DEVCLASS_IMAGE, NULL, NULL, DIGCF_PRESENT);
        Guid = GUID_DEVCLASS_IMAGE;
        break;
    }
    DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
    for (i = 0; SetupDiEnumDeviceInfo(hDevInfo, i, &DeviceInfoData); i++)
    {
        DWORD DataT;
        LPTSTR buffer = NULL;
        DWORD buffersize = 0;

        while (!SetupDiGetDeviceRegistryProperty(hDevInfo, &DeviceInfoData, SPDRP_DEVICEDESC,
            &DataT, (PBYTE)buffer, buffersize,&buffersize))
        {
            if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
            {
                // Change the buffer size.
                if (buffer) LocalFree(buffer);
                buffer = (LPTSTR)LocalAlloc(LPTR, buffersize * 2);
            }
        }
        std::string name = QString::fromWCharArray((TCHAR*)buffer).toLocal8Bit().constData();
        output << "Name: " << name << std::endl <<
            "GUID: " << GetDevRegInfo(SPDRP_CLASSGUID, i, Guid) << std::endl <<
            "Class: " << GetDevRegInfo(SPDRP_CLASS, i, Guid) << std::endl <<
            "Enumerator: " << GetDevRegInfo(SPDRP_ENUMERATOR_NAME, i, Guid) << std::endl <<
            "Device Manufacturer: " << GetDevRegInfo(SPDRP_MFG, i, Guid) << std::endl <<
            "Physical Device Object Name: " << GetDevRegInfo(SPDRP_PHYSICAL_DEVICE_OBJECT_NAME, i, Guid) << std::endl <<
            "Service: " << GetDevRegInfo(SPDRP_SERVICE, i, Guid) << std::endl << std::endl;
        if (buffer) LocalFree(buffer);
    }
    SetupDiDestroyDeviceInfoList(hDevInfo);
}

std::string MainWindow::GetDevRegInfo(DWORD SPDRP, const short wOrder, GUID currGuid)
{
    SP_DEVINFO_DATA DeviceInfoData;
    HDEVINFO hDevInfo = SetupDiGetClassDevs(&currGuid, NULL, NULL, DIGCF_PRESENT);
    DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
    SetupDiEnumDeviceInfo(hDevInfo, wOrder, &DeviceInfoData);
    DWORD DataT;
    LPTSTR buffer = NULL;
    DWORD buffersize = 0;
    while (!SetupDiGetDeviceRegistryProperty(hDevInfo, &DeviceInfoData, SPDRP,
        &DataT, (PBYTE)buffer, buffersize, &buffersize))
    {
        if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
        {
            // Change the buffer size.
            if (buffer) LocalFree(buffer);
            buffer = (LPTSTR)LocalAlloc(LPTR, buffersize * 2);
        }
    }

    std::string res = QString::fromWCharArray((TCHAR*)buffer).toLocal8Bit().constData();
    if (buffer) LocalFree(buffer);
    SetupDiDestroyDeviceInfoList(hDevInfo);
    return res;
}

//guid to int : 0-media, 1-display, 2 - monitor, 3 - image//USE SWITCH
QTreeWidgetItem *MainWindow::addRootDevClass(QString name, short index, ushort guid)
{
    QTreeWidgetItem *itm= new QTreeWidgetItem(ui->listwDevices);
    itm->setText(0, name);
    itm->setText(1, QString::number(index));
    itm->setText(2, QString::number(guid));
    itm->setExpanded(true);
    ui->listwDevices->addTopLevelItem(itm);

    return itm;
}

void MainWindow::addCildDev(QTreeWidgetItem *parent, QString name, short index, ushort guid)
{
    QTreeWidgetItem *itm= new QTreeWidgetItem();
    itm->setText(0, name);
    itm->setText(1, QString::number(index));
    itm->setText(2, QString::number(guid));
    parent->addChild(itm);
}

QString GetDevRegProperty(DWORD SPDRP, const short wOrder, GUID currGuid)
{
    SP_DEVINFO_DATA DeviceInfoData;

    HDEVINFO hDevInfo = SetupDiGetClassDevs(&currGuid,NULL,NULL,DIGCF_PRESENT);

    DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
    SetupDiEnumDeviceInfo(hDevInfo,wOrder,&DeviceInfoData);

    DWORD DataT;
    LPTSTR buffer = NULL;
    DWORD buffersize = 0;

    while (!SetupDiGetDeviceRegistryProperty(
        hDevInfo,
        &DeviceInfoData,
        SPDRP,
        &DataT,
        (PBYTE)buffer,
        buffersize,
        &buffersize))
    {
        if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
        {
            // Change the buffer size.
            if (buffer) LocalFree(buffer);
            buffer = (LPTSTR)LocalAlloc(LPTR,buffersize * 2);
        }
    }

    QString res=QString::fromWCharArray((TCHAR*)buffer);
    if (buffer) LocalFree(buffer);

    SetupDiDestroyDeviceInfoList(hDevInfo);
    return res;
}

void MainWindow::GetInfo(const short rowIndex, int classGuid)
{
    //CLASS GUID
    switch (classGuid) {
    case 0:
        currGuid=GUID_DEVCLASS_MEDIA;
        break;
    case 1:
        currGuid=GUID_DEVCLASS_DISPLAY;
        break;
    case 2:
        currGuid=GUID_DEVCLASS_MONITOR;
        break;
    case 3:
        currGuid=GUID_DEVCLASS_IMAGE;
        break;
    }

    ui->listwDeviceInfo->item(0,1)->setText(GetDevRegProperty(SPDRP_CLASSGUID, rowIndex, currGuid));
    ui->listwDeviceInfo->item(1,1)->setText(GetDevRegProperty(SPDRP_CLASS, rowIndex, currGuid));
    ui->listwDeviceInfo->item(2,1)->setText(GetDevRegProperty(SPDRP_ENUMERATOR_NAME, rowIndex, currGuid));
    ui->listwDeviceInfo->item(3,1)->setText(GetDevRegProperty(SPDRP_MFG, rowIndex, currGuid));
    ui->listwDeviceInfo->item(4,1)->setText(GetDevRegProperty(SPDRP_PHYSICAL_DEVICE_OBJECT_NAME, rowIndex, currGuid));
    ui->listwDeviceInfo->item(5,1)->setText(GetDevRegProperty(SPDRP_SERVICE, rowIndex, currGuid));
}


void MainWindow::GetOtherInfo( const short wOrder)
{
    HDEVINFO         hDevInfo      = 0L;
    SP_DEVINFO_DATA  spDevInfoData = {0};
    //
    hDevInfo = SetupDiGetClassDevs(&currGuid, 0L, 0L, DIGCF_PRESENT);
    if (hDevInfo == (void*)-1)
    {
        return;
    };
    spDevInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
    if (SetupDiEnumDeviceInfo(hDevInfo,
                              wOrder,
                              &spDevInfoData))
    {
        SP_DRVINFO_DATA        spDrvInfoData      = {0};
        SP_DRVINFO_DETAIL_DATA spDrvInfoDetail    = {0};

        DWORD                  dwRequireSize;
        short                  wIdx;

        if (!SetupDiBuildDriverInfoList(hDevInfo,
                                        &spDevInfoData,
                                        SPDIT_COMPATDRIVER))

        wIdx = 0;
        spDrvInfoData.cbSize = sizeof(SP_DRVINFO_DATA);
        if (SetupDiEnumDriverInfo(hDevInfo,
                                     &spDevInfoData,
                                     SPDIT_COMPATDRIVER,
                                     wOrder,
                                     &spDrvInfoData))
        {

            char szBuf[2048] = {0};
    //
            memcpy(&spDrvInfoDetail, szBuf, sizeof(SP_DRVINFO_DETAIL_DATA));
            spDrvInfoDetail.cbSize = sizeof(SP_DRVINFO_DETAIL_DATA);
            dwRequireSize = 0;
            if (SetupDiGetDriverInfoDetail(hDevInfo,
                                           &spDevInfoData,
                                           &spDrvInfoData,
                                           &spDrvInfoDetail,
                                           2048,
                                           &dwRequireSize))
            {
                SYSTEMTIME sysTime   = {0};
                char       szTmp[64] = {0};

                QString str=QString::fromWCharArray((TCHAR*)spDrvInfoDetail.HardwareID);
                ui->listwDeviceInfo->item(6,1)->setText(str);

                str=QString::fromWCharArray((TCHAR*)spDrvInfoDetail.InfFileName);
                ui->listwDeviceInfo->item(7,1)->setText(str);

                FileTimeToSystemTime(&spDrvInfoData.DriverDate, &sysTime);
                sprintf(szTmp, "%02d/%02d/%04d", sysTime.wMonth, sysTime.wDay, sysTime.wYear);
                str=(QString::fromStdString(szTmp));
                ui->listwDeviceInfo->item(8,1)->setText(str);

                str=(QString::fromWCharArray((TCHAR*)spDrvInfoDetail.SectionName));
                ui->listwDeviceInfo->item(9,1)->setText(str);
            }

        }
        SetupDiDestroyDriverInfoList(hDevInfo, &spDevInfoData, SPDIT_COMPATDRIVER);
        SetupDiDestroyDeviceInfoList(hDevInfo);
    }
}

//void MainWindow::on_listwDevices_currentItemChanged(QTreeWidgetItem *itm, QTreeWidgetItem *previous)
void MainWindow::on_listwDevices_currentItemChanged(QTreeWidgetItem *itm)
{
    itm= ui->listwDevices->currentItem();

    if(itm->text(1)!="-1")
    {
        GetInfo(itm->text(1).toInt(),itm->text(2).toInt());
        GetOtherInfo(itm->text(1).toInt());
    }
    else{
        for(int i=0; i<10; i++)
            ui->listwDeviceInfo->item(i,1)->setText("");
    }
}

void MainWindow::on_actHelp_triggered()
{
    QString put= QCoreApplication::applicationDirPath ();
            put.append("/Help.chm");
        QDesktopServices::openUrl( QUrl::fromLocalFile(put) );
}
