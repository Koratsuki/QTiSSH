#include "add_server.h"
#include "ui_add_server.h"

add_Server::add_Server(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::add_Server)
{
    ui->setupUi(this);
}

add_Server::~add_Server()
{
    delete ui;
}
