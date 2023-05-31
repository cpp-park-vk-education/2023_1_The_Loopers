#include "LoginView.h"

#include "AuthModel.h"
#include "AuthView.h"

#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

namespace inklink::auth
{
LoginDialog::LoginDialog(QWidget* parent) : QWidget(parent)
{
    connect(this, &LoginDialog::GotResultFromNetwork, this, &LoginDialog::DoOnGotResultFromNetwork);

    setWindowTitle("Login");

    setFixedSize(300, 200);

    m_usernameLine = new QLineEdit(this);
    m_passwordLine = new QLineEdit(this);

    auto* usernameLabel = new QLabel(tr("Username"), this);
    auto* passwordLabel = new QLabel(tr("Password"), this);

    auto* enterButton = new QPushButton(tr("Login"), this);
    enterButton->setAutoDefault(false);
    connect(enterButton, &QPushButton::clicked, this, &LoginDialog::OnEnterButtonClicked);

    auto* registrationButton = new QPushButton(tr("Register"), this);
    registrationButton->setAutoDefault(false);
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

    connect(this, &LoginDialog::GotResultFromNetwork, this, &LoginDialog::DoOnGotResultFromNetwork);
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

    if (username.empty())
    {
        QMessageBox::warning(this, usernameEnter, "Enter username");
    }
    else if (password.empty())
    {
        QMessageBox::warning(this, passwordEnter, "Enter password");
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
        QMessageBox::warning(this, "UsernameEnter", "Uncorrect username or password");
    }
    else
    {
        close();
    }
}
} // namespace inklink::auth
