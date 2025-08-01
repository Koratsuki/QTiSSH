#ifndef ADD_SERVER_H
#define ADD_SERVER_H

#include <QDialog>

namespace Ui {
class add_Server;
}

class add_Server : public QDialog
{
    Q_OBJECT

public:
    explicit add_Server(QWidget *parent = nullptr);
    ~add_Server();

private:
    Ui::add_Server *ui;
};

#endif // ADD_SERVER_H
