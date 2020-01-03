#include "newaccdlg.h"
#include "ui_newaccdlg.h"

newaccdlg::newaccdlg(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::newaccdlg)
{
    ui->setupUi(this);
}

newaccdlg::~newaccdlg()
{
    delete ui;
}

