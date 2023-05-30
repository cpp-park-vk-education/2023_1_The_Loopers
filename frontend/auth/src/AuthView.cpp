#include "AuthView.hpp"

#include "AuthModel.hpp"
#include "LoginView.hpp"

#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

namespace inklink::auth
{
AuthDialog::AuthDialog(QWidget* parent) : QWidget(parent)
{
    connect(this, &AuthDialog::GotResultFromNetwork, this, &AuthDialog::DoOnGotResultFromNetwork);

    setWindowTitle("Registration");

    setFixedSize(300, 200);

    usernameLine = new QLineEdit(this);
    passwordLine = new QLineEdit(this);

    auto* usernameLabel = new QLabel(tr("Username"), this);
    auto* passwordLabel = new QLabel(tr("Password"), this);

    auto* createButton = new QPushButton(tr("Register"), this);
    createButton->setAutoDefault(false);
    connect(createButton, &QPushButton::clicked, this, &AuthDialog::OnCreateButtonClicked);

    auto* loginButton = new QPushButton(tr("Log in"), this);
    loginButton->setAutoDefault(false);
    connect(loginButton, &QPushButton::clicked, this, &AuthDialog::OnLoginButtonClicked);

    auto* buttonsLayout = new QHBoxLayout;
    buttonsLayout->setAlignment(Qt::AlignCenter);
    buttonsLayout->addWidget(createButton);
    buttonsLayout->addWidget(loginButton);

    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(usernameLabel);
    mainLayout->addWidget(usernameLine);

    mainLayout->addWidget(passwordLabel);
    mainLayout->addWidget(passwordLine);

    mainLayout->addLayout(buttonsLayout);

    setLayout(mainLayout);
}
void AuthDialog::OnLoginButtonClicked()
{
    auto* login = new LoginDialog(this);
    close();
    login->show();
}
void AuthDialog::OnCreateButtonClicked()
{
    std::string username;
    std::string password;

    const QString usernameEnter = usernameLine->text();
    const QString passwordEnter = passwordLine->text();

    username = usernameEnter.toStdString();
    password = passwordEnter.toStdString();

    auto* authModel = new AuthModel;

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
        const auto message = m_model->RegParseToSend(login, password);
        m_model->RegSend(message);
    }
}
void NotifyGotResultFromNetwork(bool result)
{
    emit GotResultFromNetwork(result);
}
void DoOnGotResultFromNetwork(bool result)
{
    if (!result)
    {
        QMessageBox::warning(this, usernameEnter, "Existing username");
    }
    else
    {
        close();
    }
}
}
} // namespace inklink::auth