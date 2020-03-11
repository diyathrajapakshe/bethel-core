#include "bteltechinit.h"
#include "ui_bteltechinit.h"
#include "net.h"
#include "util.h"
#include "skinize.h"

#include <string>
#include <iostream>

#include <QStringList>
#include <QUrl>

BtelTechInit::BtelTechInit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BtelTechInit)
{
    ui->setupUi(this);
}

BtelTechInit::~BtelTechInit()
{
    delete ui;
}

void BtelTechInit::ShowBteltechIntro(bool exitAfter)
{
    BtelTechInit bteltechinit;
    bteltechinit.setStyleSheet(Skinize());

    if(!bteltechinit.exec())
    {
        if(exitAfter)
            exit(0);
        else
            return;
    }

    QString ServersToAdd = bteltechinit.GetServers();
    QStringList ss = ServersToAdd.split('\n');

    for (int i = 0; i < ss.size(); ++i){
        QString Server = ss.at(i);
        QString strippedServer = Server.remove(' ');
        if(strippedServer != "")
        {
            WriteConfigFile("addanonserver",strippedServer.toStdString());
        }
    }
}

QString BtelTechInit::GetServers()
{
    return ui->plainTextEdit->toPlainText();
}
