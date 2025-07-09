#include "secmainwindow.h"
#include "ui_secmainwindow.h"

SecMainWindow::SecMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SecMainWindow)
{
    ui->setupUi(this);
}

SecMainWindow::~SecMainWindow()
{
    delete ui;
}
