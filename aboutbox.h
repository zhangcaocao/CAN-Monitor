#ifndef ABOUTBOX_H
#define ABOUTBOX_H

#include <QMainWindow>

namespace Ui {
class aboutbox;
}

class aboutbox : public QMainWindow
{
    Q_OBJECT

public:
    explicit aboutbox(QWidget *parent = nullptr);
    ~aboutbox();

private:
    Ui::aboutbox *ui;
};

#endif // ABOUTBOX_H
