#ifndef BTELTECHSETUP_H
#define BTELTECHSETUP_H

#include "wallet/bteltech.h"

#include <QDialog>
#include <QVBoxLayout>

namespace Ui {
class bteltechsetup;
}

class bteltechsetup : public QDialog
{
    Q_OBJECT

public:
    explicit bteltechsetup(QWidget *parent = 0);
    ~bteltechsetup();

private:
    Ui::bteltechsetup *ui;

public Q_SLOTS:
    void reloadBteltechServers();
    void addBteltechServer();
    void removeBteltechServer();
    void getinfoBteltechServer();
    void showButtons(bool show=true);
    void showBteltechIntro();

};

#endif // BTELTECHSETUP_H
