#include "aboutbox.h"
#include "ui_aboutbox.h"

aboutbox::aboutbox(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::aboutbox)
{
    ui->setupUi(this);
}

aboutbox::~aboutbox()
{
    delete ui;
}
