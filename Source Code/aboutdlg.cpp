#include "aboutdlg.h"
#include "ui_aboutdlg.h"

aboutdlg::aboutdlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::aboutdlg)
{
    ui->setupUi(this);
}

aboutdlg::~aboutdlg()
{
    delete ui;
}

void aboutdlg::on_btnOk_clicked()
{
    close();
}
