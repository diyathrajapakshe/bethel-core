#include "bteltechsetup.h"
#include "bteltechitem.h"
#include "ui_bteltechsetup.h"
#include "net.h"
#include "skinize.h"
#include "util.h"

#include <QVBoxLayout>
#include <QListWidgetItem>
#include <QListWidget>
#include <QMessageBox>
#include <QtNetwork>
#include <QWidget>

bteltechsetup::bteltechsetup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::bteltechsetup)
{
    ui->setupUi(this);
    ui->getInfoButton->setVisible(false);
    ui->removeButton->setVisible(false);

    ui->saveButton->setVisible(false);

    this->setWindowTitle("Bteltech");

    connect(ui->addNewButton,SIGNAL(clicked()),this,SLOT(addBteltechServer()));
    connect(ui->getInfoButton,SIGNAL(clicked()),this,SLOT(getinfoBteltechServer()));
    connect(ui->removeButton,SIGNAL(clicked()),this,SLOT(removeBteltechServer()));
    connect(ui->serversListWidget,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(showButtons()));
    connect(ui->saveButton,SIGNAL(clicked()),this,SLOT(close()));

    reloadBteltechServers();
}

bteltechsetup::~bteltechsetup()
{
    delete ui;
}

void bteltechsetup::addBteltechServer()
{
    QString serverToAdd = ui->addServerText->text();

    if(serverToAdd == "")
        return;

    serverToAdd = serverToAdd.remove(' ').remove('\n').remove('\r').remove('\t');

    LOCK(cs_vAddedAnonServers);
    std::vector<std::string>::iterator it = vAddedAnonServers.begin();
    for(; it != vAddedAnonServers.end(); it++)
        if (serverToAdd.toStdString() == *it)
            break;

    WriteConfigFile("addanonserver", serverToAdd.toStdString());
    if (it == vAddedAnonServers.end())
        vAddedAnonServers.push_back(serverToAdd.toStdString());

    ui->addServerText->clear();

    reloadBteltechServers();
}

void bteltechsetup::reloadBteltechServers()
{
    ui->serversListWidget->clear();

    const std::vector<std::string>& confAnonServers = mapMultiArgs["-addanonserver"];

    BOOST_FOREACH(std::string confAnonServer, confAnonServers) {
        ui->serversListWidget->addItem(QString::fromStdString(confAnonServer));
    }

    BOOST_FOREACH(std::string vAddedAnonServer, vAddedAnonServers) {
        ui->serversListWidget->addItem(QString::fromStdString(vAddedAnonServer));
    }
}

void bteltechsetup::showBteltechIntro()
{
    setWindowIcon(QIcon(":icons/bethel"));
    setStyleSheet(Skinize());

    ui->saveButton->setVisible(true);

    exec();
}

void bteltechsetup::removeBteltechServer()
{
    QString serverToRemove = ui->serversListWidget->currentItem()->text();

    if(serverToRemove == "")
        return;

    QMessageBox::StandardButton btnRetVal = QMessageBox::question(this, tr("Remove Bteltech server"),
        tr("You are about to remove the following Bteltech server: ") + "<br><br>" + serverToRemove + "<br><br>" + tr("Are you sure?"),
        QMessageBox::Yes | QMessageBox::Cancel, QMessageBox::Cancel);

    if(btnRetVal == QMessageBox::Cancel)
        return;

    LOCK(cs_vAddedAnonServers);
    std::vector<std::string>::iterator it = vAddedAnonServers.begin();

    RemoveConfigFile("addanonserver", serverToRemove.toStdString());
    for(; it != vAddedAnonServers.end(); it++)
        if (serverToRemove == QString::fromStdString(*it))
            break;

    if (it != vAddedAnonServers.end())
    {
      vAddedAnonServers.erase(it);
      QMessageBox::critical(this, windowTitle(),
          tr("Removed."),
          QMessageBox::Ok, QMessageBox::Ok);
    }

    showButtons(false);
    reloadBteltechServers();
}

void bteltechsetup::showButtons(bool show)
{
    ui->getInfoButton->setVisible(show);
    ui->removeButton->setVisible(show);
}

void bteltechsetup::getinfoBteltechServer()
{
    QString serverToCheck = ui->serversListWidget->currentItem()->text();

    if(serverToCheck == "")
        return;

    QStringList server = serverToCheck.split(':');

    if(server.length() != 2)
        return;

    try {
        Bteltech *bteltech = new Bteltech();
        UniValue data = bteltech->GetServerInfo(serverToCheck.toStdString());

        QMessageBox::information(this, windowTitle(),
            tr("Bteltech server") + "<br><br>" +  tr("Address: ") + serverToCheck + "<br>" + tr("Min amount: ") + QString::number(data["min_amount"].get_real()) + " <br>" + tr("Max amount: ") + QString::number(data["max_amount"].get_real()) + "<br>" + tr("Tx fee: ") + QString::number(data["transaction_fee"].get_real()) + "%",
            QMessageBox::Ok, QMessageBox::Ok);
    }
    catch(const std::runtime_error &e)
    {
        QMessageBox::warning(this, tr("Bteltech server"),
                             "<qt>Could not get info:<br/><br/>" +
                             tr(e.what())+"</qt>");
    }
}
