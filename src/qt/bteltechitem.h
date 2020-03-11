#ifndef BTELTECHITEM_H
#define BTELTECHITEM_H

#include <QWidget>

namespace Ui {
class bteltechitem;
}

class bteltechitem : public QWidget
{
    Q_OBJECT

public:
    explicit bteltechitem(QWidget *parent = 0);
    ~bteltechitem();
    void setHost(QString hostStr);
    void setName(QString nameStr);

private:
    Ui::bteltechitem *ui;
    QString host;
    QString name;
};

#endif // BTELTECHITEM_H
