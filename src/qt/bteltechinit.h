#ifndef BTELTECHINIT_H
#define BTELTECHINIT_H

#include <QDialog>

namespace Ui {
class BtelTechInit;
}

class BtelTechInit : public QDialog
{
    Q_OBJECT

public:
    explicit BtelTechInit(QWidget *parent = 0);
    ~BtelTechInit();
    QString GetServers();
    void ShowBteltechIntro(bool exitAfter = true);

private:
    Ui::BtelTechInit *ui;
};

#endif // BTELTECHINIT_H
