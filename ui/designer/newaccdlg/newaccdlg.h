#ifndef NEWACCDLG_H
#define NEWACCDLG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class newaccdlg; }
QT_END_NAMESPACE

class newaccdlg : public QDialog
{
    Q_OBJECT

public:
    newaccdlg(QWidget *parent = nullptr);
    ~newaccdlg();

private:
    Ui::newaccdlg *ui;
};
#endif // NEWACCDLG_H
