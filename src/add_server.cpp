#include "add_server.h"
#include "ui_add_server.h"
#include "foldermanager.h"
#include "profilemanager.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QStandardPaths>

add_Server::add_Server(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::add_Server),
    m_folderManager(nullptr)
{
    ui->setupUi(this);
    setupConnections();
    
    // Set default port
    ui->portLineEdit->setText("22");
    
    // Setup authentication combo box
    ui->authTypeCombo->addItem("Password", static_cast<int>(AuthType::Password));
    ui->authTypeCombo->addItem("Public Key", static_cast<int>(AuthType::PublicKey));
    ui->authTypeCombo->addItem("SSH Agent", static_cast<int>(AuthType::SSHAgent));
    
    // Populate profiles combo
    ui->profileCombo->addItem(tr("(None)"), QString());
    for (const QString &name : ProfileManager::instance().profileNames()) {
        ui->profileCombo->addItem(name, name);
    }
    connect(ui->profileCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &add_Server::onProfileChanged);
    
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
    
    bool showPassword = (authType == AuthType::Password);
    bool showKey = (authType == AuthType::PublicKey);
    
    ui->passwordLineEdit->setEnabled(showPassword);
    ui->passwordLineEdit->setVisible(showPassword);
    ui->keyPathLineEdit->setEnabled(showKey);
    ui->keyPathLineEdit->setVisible(showKey);
    ui->browseKeyButton->setEnabled(showKey);
    ui->browseKeyButton->setVisible(showKey);
}

void add_Server::onProfileChanged(int index)
{
    QString profileName = ui->profileCombo->itemData(index).toString();
    if (profileName.isEmpty()) {
        return;
    }
    QString options = ProfileManager::instance().optionsFor(profileName);
    if (!options.trimmed().isEmpty()) {
        ui->sshOptionsEdit->setPlainText(options.trimmed());
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
        if (ui->passwordLineEdit->text().isEmpty() && !m_hasStoredPassword) {
            QMessageBox::warning(this, tr("Validation Error"), tr("Please enter a password."));
            ui->passwordLineEdit->setFocus();
            return false;
        }
    } else if (authType == AuthType::PublicKey) {
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
    config.setJumpHost(ui->jumpHostLineEdit->text().trimmed());
    config.setStrictHostKeyChecking(ui->strictHostKeyCheckBox->isChecked());
    config.setForwardAgent(ui->forwardAgentCheckBox->isChecked());
    config.setTunnels(ui->tunnelsEdit->toPlainText().trimmed());
    config.setSshOptions(ui->sshOptionsEdit->toPlainText().trimmed());
    config.setAutoReconnect(ui->autoReconnectCheckBox->isChecked());
    config.setProfileName(ui->profileCombo->currentData().toString());
    
    AuthType authType = static_cast<AuthType>(ui->authTypeCombo->currentData().toInt());
    config.setAuthType(authType);
    
    if (authType == AuthType::Password) {
        QString entered = ui->passwordLineEdit->text();
        if (entered.isEmpty() && m_hasStoredPassword) {
            config.setPassword(m_originalPassword);
        } else {
            config.setPassword(entered);
        }
    } else if (authType == AuthType::PublicKey) {
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
    ui->jumpHostLineEdit->setText(config.jumpHost());
    ui->strictHostKeyCheckBox->setChecked(config.strictHostKeyChecking());
    ui->forwardAgentCheckBox->setChecked(config.forwardAgent());
    ui->tunnelsEdit->setPlainText(config.tunnels());
    ui->sshOptionsEdit->setPlainText(config.sshOptions());
    ui->autoReconnectCheckBox->setChecked(config.autoReconnect());

    // Select profile (or None)
    int profileIndex = ui->profileCombo->findData(config.profileName());
    if (profileIndex != -1) {
        ui->profileCombo->setCurrentIndex(profileIndex);
    } else {
        ui->profileCombo->setCurrentIndex(0);
    }
    
    m_originalPassword = config.password();
    m_hasStoredPassword = !config.password().isEmpty();
    
    if (config.authType() == AuthType::Password) {
        ui->authTypeCombo->setCurrentIndex(0);
        if (m_hasStoredPassword && config.password().startsWith("enc:")) {
            ui->passwordLineEdit->clear();
            ui->passwordLineEdit->setPlaceholderText(
                tr("Password stored encrypted - leave empty to keep"));
        } else {
            ui->passwordLineEdit->setText(config.password());
        }
    } else if (config.authType() == AuthType::PublicKey) {
        ui->authTypeCombo->setCurrentIndex(1);
        ui->keyPathLineEdit->setText(config.keyPath());
    } else {
        ui->authTypeCombo->setCurrentIndex(2);
    }
}

void add_Server::setFolderManager(FolderManager *folderManager)
{
    m_folderManager = folderManager;
    populateFolderComboBox();
}

void add_Server::setDefaultFolder(const QString &folderId)
{
    m_defaultFolderId = folderId;
    populateFolderComboBox();
}

void add_Server::populateFolderComboBox()
{
    // For now, we'll keep using the group line edit
    // In a future update, we could replace this with a combo box
    if (!m_defaultFolderId.isEmpty() && m_folderManager) {
        FolderNode folder = m_folderManager->getFolder(m_defaultFolderId);
        if (!folder.name.isEmpty()) {
            ui->groupLineEdit->setText(m_defaultFolderId);
        }
    }
}
