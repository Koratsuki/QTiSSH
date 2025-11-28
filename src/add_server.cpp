#include "add_server.h"
#include "ui_add_server.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QStandardPaths>

add_Server::add_Server(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::add_Server)
{
    ui->setupUi(this);
    setupConnections();
    
    // Set default port
    ui->portLineEdit->setText("22");
    
    // Setup authentication combo box
    ui->authTypeCombo->addItem("Password", static_cast<int>(AuthType::Password));
    ui->authTypeCombo->addItem("Public Key", static_cast<int>(AuthType::PublicKey));
    
    // Initially show password field
    onAuthTypeChanged(0);
}

add_Server::~add_Server()
{
    delete ui;
}

void add_Server::setupConnections()
{
    connect(ui->authTypeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &add_Server::onAuthTypeChanged);
    connect(ui->browseKeyButton, &QPushButton::clicked, this, &add_Server::onBrowseKeyClicked);
    connect(ui->addButton, &QPushButton::clicked, this, &add_Server::onAddClicked);
    connect(ui->cancelButton, &QPushButton::clicked, this, &add_Server::onCancelClicked);
}

void add_Server::onAuthTypeChanged(int index)
{
    AuthType authType = static_cast<AuthType>(ui->authTypeCombo->itemData(index).toInt());
    
    if (authType == AuthType::Password) {
        ui->passwordLineEdit->setEnabled(true);
        ui->passwordLineEdit->setVisible(true);
        ui->keyPathLineEdit->setEnabled(false);
        ui->keyPathLineEdit->setVisible(false);
        ui->browseKeyButton->setEnabled(false);
        ui->browseKeyButton->setVisible(false);
    } else {
        ui->passwordLineEdit->setEnabled(false);
        ui->passwordLineEdit->setVisible(false);
        ui->keyPathLineEdit->setEnabled(true);
        ui->keyPathLineEdit->setVisible(true);
        ui->browseKeyButton->setEnabled(true);
        ui->browseKeyButton->setVisible(true);
    }
}

void add_Server::onBrowseKeyClicked()
{
    QString defaultPath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/.ssh";
    QString keyPath = QFileDialog::getOpenFileName(this, 
                                                    tr("Select SSH Private Key"),
                                                    defaultPath,
                                                    tr("All Files (*);;PEM Files (*.pem);;Key Files (id_*)"));
    if (!keyPath.isEmpty()) {
        ui->keyPathLineEdit->setText(keyPath);
    }
}

void add_Server::onAddClicked()
{
    if (validateInput()) {
        accept();
    }
}

void add_Server::onCancelClicked()
{
    reject();
}

bool add_Server::validateInput()
{
    if (ui->aliasLineEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, tr("Validation Error"), tr("Please enter an alias for the server."));
        ui->aliasLineEdit->setFocus();
        return false;
    }
    
    if (ui->hostLineEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, tr("Validation Error"), tr("Please enter a host address."));
        ui->hostLineEdit->setFocus();
        return false;
    }
    
    if (ui->usernameLineEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, tr("Validation Error"), tr("Please enter a username."));
        ui->usernameLineEdit->setFocus();
        return false;
    }
    
    bool ok;
    int port = ui->portLineEdit->text().toInt(&ok);
    if (!ok || port < 1 || port > 65535) {
        QMessageBox::warning(this, tr("Validation Error"), tr("Please enter a valid port number (1-65535)."));
        ui->portLineEdit->setFocus();
        return false;
    }
    
    AuthType authType = static_cast<AuthType>(ui->authTypeCombo->currentData().toInt());
    if (authType == AuthType::Password) {
        if (ui->passwordLineEdit->text().isEmpty()) {
            QMessageBox::warning(this, tr("Validation Error"), tr("Please enter a password."));
            ui->passwordLineEdit->setFocus();
            return false;
        }
    } else {
        if (ui->keyPathLineEdit->text().trimmed().isEmpty()) {
            QMessageBox::warning(this, tr("Validation Error"), tr("Please select an SSH key file."));
            return false;
        }
    }
    
    return true;
}

ServerConfig add_Server::getServerConfig() const
{
    ServerConfig config;
    config.setAlias(ui->aliasLineEdit->text().trimmed());
    config.setHost(ui->hostLineEdit->text().trimmed());
    config.setPort(ui->portLineEdit->text().toInt());
    config.setUsername(ui->usernameLineEdit->text().trimmed());
    config.setGroup(ui->groupLineEdit->text().trimmed());
    config.setTags(ui->tagsLineEdit->text().trimmed());
    
    AuthType authType = static_cast<AuthType>(ui->authTypeCombo->currentData().toInt());
    config.setAuthType(authType);
    
    if (authType == AuthType::Password) {
        config.setPassword(ui->passwordLineEdit->text());
    } else {
        config.setKeyPath(ui->keyPathLineEdit->text().trimmed());
    }
    
    return config;
}

void add_Server::setServerConfig(const ServerConfig &config)
{
    ui->aliasLineEdit->setText(config.alias());
    ui->hostLineEdit->setText(config.host());
    ui->portLineEdit->setText(QString::number(config.port()));
    ui->usernameLineEdit->setText(config.username());
    ui->groupLineEdit->setText(config.group());
    ui->tagsLineEdit->setText(config.tags());
    
    if (config.authType() == AuthType::Password) {
        ui->authTypeCombo->setCurrentIndex(0);
        ui->passwordLineEdit->setText(config.password());
    } else {
        ui->authTypeCombo->setCurrentIndex(1);
        ui->keyPathLineEdit->setText(config.keyPath());
    }
}

