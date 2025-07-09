#include "form.h"
#include "ui_form.h"

Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);
    connect(ui->backButton, &QPushButton::clicked, this, &Form::backClicked);
}

Form::~Form()
{
    delete ui;
}





void Form::on_closeButton_clicked()
{
    this->close();
}
