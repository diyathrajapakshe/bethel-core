#include "bteltechitem.h"
#include "ui_bteltechitem.h"

bteltechitem::bteltechitem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::bteltechitem),
    host(""),
    name("")
{
    ui->setupUi(this);
}

void bteltechitem::setHost(QString hostStr)
{
    host = hostStr;
    ui->serverHostLabel->setText(hostStr);
}

void bteltechitem::setName(QString nameStr)
{
    name = nameStr;
    ui->serverNameLabel->setText(nameStr);
}

bteltechitem::~bteltechitem()
{
    delete ui;
}
