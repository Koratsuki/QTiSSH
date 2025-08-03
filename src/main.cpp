#include "mainwindow.h"

#include <QApplication>
#include <QDir>
#include <QLockFile>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("qtissh");
    QCoreApplication::setOrganizationDomain("qtissh.cu");
    QCoreApplication::setApplicationName("QTiSSH");

    QApplication a(argc, argv);

    auto tmpDir = QDir::tempPath();
        QLockFile lockFile(tmpDir + "/qtissh.lock");

        if (!lockFile.tryLock(100)) {
            QMessageBox msgBox;
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setText("You already have QTiSSH running."
                           "\r\nOnly one instance is allowed.");
            msgBox.exec();
            return 1;
        }

    MainWindow w;
    w.show();
    return a.exec();
}
