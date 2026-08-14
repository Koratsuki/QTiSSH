#include "mainwindow.h"
#include "applockmanager.h"
#include "applockdialog.h"
#include "settingsmanager.h"

#include <QApplication>
#include <QDir>
#include <QLockFile>
#include <QMessageBox>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("qtissh");
    QCoreApplication::setOrganizationDomain("qtissh.tld");
    QCoreApplication::setApplicationName("QTiSSH");

    QApplication a(argc, argv);

    // Interface language (English is the built-in source language).
    QTranslator translator;
    const QString lang = SettingsManager::instance().language();
    if (lang == QLatin1String("es")
        && translator.load(QStringLiteral(":/i18n/qtissh_es"))) {
        a.installTranslator(&translator);
    }

    auto tmpDir = QDir::tempPath();
        QLockFile lockFile(tmpDir + "/qtissh.lock");

        if (!lockFile.tryLock(100)) {
            QMessageBox msgBox;
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setText(QObject::tr("You already have QTiSSH running."
                            "\r\nOnly one instance is allowed."));
            msgBox.exec();
            return 1;
        }

    // Global application password: gate access to the whole app.
    if (AppLockManager::instance().isEnabled()
        && !AppLockDialog::prompt()) {
        return 0;
    }

    MainWindow w;
    w.show();
    return a.exec();
}
