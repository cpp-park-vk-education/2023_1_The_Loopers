#include "LoginView.h"

#include "AuthModel.h"
#include "AuthView.h"

#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

namespace
{
QString myStyleSheet = R"(
            #customButton {
                background-color: rgb(25, 25, 25);
                color: white;
            }
            #customButton:hover {
                background-color: rgb(200, 200, 200);
                color: rgb(25, 25, 25);
            }
            #customButton:pressed {
                background-color: white;
                color: rgb(25, 25, 25);
            }
        )";
} // namespace

namespace inklink::auth
{
LoginDialog::LoginDialog(QWidget* parent) : QDialog(parent)
{
    m_model = new AuthModel;

    connect(this, &LoginDialog::GotResultFromNetwork, this, &LoginDialog::DoOnGotResultFromNetwork);

    setWindowTitle("Login");

    setWindowFlags(Qt::CoverWindow | Qt::WindowTitleHint);

    setFixedSize(300, 200);

    m_usernameLine = new QLineEdit(this);
    m_passwordLine = new QLineEdit(this);

    auto* usernameLabel = new QLabel(tr("Username"), this);
    auto* passwordLabel = new QLabel(tr("Password"), this);

    auto* enterButton = new QPushButton(tr("Login"), this);
    enterButton->setFlat(true);
    enterButton->setStyleSheet(myStyleSheet);
    connect(enterButton, &QPushButton::clicked, this, &LoginDialog::OnEnterButtonClicked);

    auto* registrationButton = new QPushButton(tr("Register"), this);
    registrationButton->setFlat(true);
    registrationButton->setStyleSheet(myStyleSheet);
    connect(registrationButton, &QPushButton::clicked, this, &LoginDialog::OnRegisterButtonClicked);

    auto* buttonsLayout = new QHBoxLayout;
    buttonsLayout->setAlignment(Qt::AlignCenter);
    buttonsLayout->addWidget(enterButton);
    buttonsLayout->addWidget(registrationButton);

    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(usernameLabel);
    mainLayout->addWidget(m_usernameLine);

    mainLayout->addWidget(passwordLabel);
    mainLayout->addWidget(m_passwordLine);

    mainLayout->addLayout(buttonsLayout);

    setLayout(mainLayout);
}
void LoginDialog::OnRegisterButtonClicked()
{
    close();
    auto* registration = new AuthDialog();
    registration->show();
}
void LoginDialog::OnEnterButtonClicked()
{
    std::string username;
    std::string password;

    const QString usernameEnter = m_usernameLine->text();
    const QString passwordEnter = m_passwordLine->text();

    username = usernameEnter.toStdString();
    password = passwordEnter.toStdString();

    QMessageBox warningBox;
    warningBox.setStyleSheet(myStyleSheet);

    if (username.empty())
    {
        warningBox.warning(this, "error", "Enter username");
    }
    else if (password.empty())
    {
        warningBox.warning(this, "error", "Enter password");
    }
    else
    {
        const auto message = m_model->LoginParseToSend(username, password);
        m_model->LoginSend(message);
    }
}
void LoginDialog::NotifyGotResultFromNetwork(int result)
{
    emit GotResultFromNetwork(result);
}
void LoginDialog::DoOnGotResultFromNetwork(int result)
{
    if (result == 0)
    {
        QMessageBox warningBox;
        warningBox.setStyleSheet(myStyleSheet);
        warningBox.warning(this, "usernameEnter", "Uncorrect username or password");
    }
    else
    {
        close();
    }
}
} // namespace inklink::auth
