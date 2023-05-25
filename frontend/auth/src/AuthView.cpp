#include "AuthView.hpp"

#include "AuthModel.hpp"

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

namespace inklink::auth
{
AuthDialog::AuthDialog(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("Registration");

    setFixedSize(300, 200);

    usernameLine = new QLineEdit(this);
    passwordLine = new QLineEdit(this);

    auto *usernameLabel = new QLabel(tr("Username"), this);
    auto *passwordLabel = new QLabel(tr("Password"), this);

    auto *acceptButton = new QPushButton(tr("Registrate"), this);
    acceptButton->setAutoDefault(false);
    connect(acceptButton, &QPushButton::clicked, this, &AuthDialog::OnPushButtonCreate);

    auto *loginButton = new QPushButton(tr("Log in"), this);
    loginButton->setAutoDefault(false);
    connect(loginButton, &QPushButton::clicked, this, &AuthDialog::OnPushButtonLogin);

    auto *buttonsLayout = new QHBoxLayout;
    buttonsLayout->setAlignment(Qt::AlignCenter);
    buttonsLayout->addWidget(acceptButton);
    buttonsLayout->addWidget(loginButton);

    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(usernameLabel);
    mainLayout->addWidget(usernameLine);

    mainLayout->addWidget(passwordLabel);
    mainLayout->addWidget(passwordLine);

    mainLayout->addLayout(buttonsLayout);

    setLayout(mainLayout);
}

void AuthDialog::OnPushButtonLogin()
{
    auto login = new LoginDialog(this);
    close();
    login->show();
}

void AuthDialog::OnPushButtonCreate()
{
    std::string username;
    std::string password;

    QString usernameEnter = usernameLine->text();
    QString passwordEnter = passwordLine->text();

    username = usernameEnter.toStdString();
    password = passwordEnter.toStdString();

    auto *authModel = new AuthModel;

    if (username.empty())
    {
        QMessageBox::warning(this, usernameEnter, "Enter username");
    }
    else if (password.empty())
    {
        QMessageBox::warning(this, passwordEnter, "Enter password");
    }
    else if (!authModel->CreateUser(username, password))
    {
        QMessageBox::warning(this, usernameEnter, "Existing username");
    }
    else
        close();
}

LoginDialog::LoginDialog(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("Login");

    setFixedSize(300, 200);

    usernameLine = new QLineEdit(this);
    passwordLine = new QLineEdit(this);

    auto *usernameLabel = new QLabel(tr("Username"), this);
    auto *passwordLabel = new QLabel(tr("Password"), this);

    auto *acceptButton = new QPushButton(tr("Login"), this);
    acceptButton->setAutoDefault(false);
    connect(acceptButton, &QPushButton::clicked, this, &LoginDialog::OnPushButtonEnter);

    auto *registrationButton = new QPushButton(tr("Registrate"), this);
    registrationButton->setAutoDefault(false);
    connect(registrationButton, &QPushButton::clicked, this, &LoginDialog::OnPushButtonRegistration);

    auto *buttonsLayout = new QHBoxLayout;
    buttonsLayout->setAlignment(Qt::AlignCenter);
    buttonsLayout->addWidget(acceptButton);
    buttonsLayout->addWidget(registrationButton);

    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(usernameLabel);
    mainLayout->addWidget(usernameLine);

    mainLayout->addWidget(passwordLabel);
    mainLayout->addWidget(passwordLine);

    mainLayout->addLayout(buttonsLayout);

    setLayout(mainLayout);
}

void LoginDialog::OnPushButtonRegistration()
{
    auto registration = new AuthDialog(this);
    close();
    registration->show();
}

void LoginDialog::OnPushButtonEnter()
{
    std::string username;
    std::string password;

    QString usernameEnter = usernameLine->text();
    QString passwordEnter = passwordLine->text();

    username = usernameEnter.toStdString();
    password = passwordEnter.toStdString();

    auto *authModel = new AuthModel;

    if (username.empty())
    {
        QMessageBox::warning(this, usernameEnter, "Enter username");
    }
    else if (password.empty())
    {
        QMessageBox::warning(this, passwordEnter, "Enter password");
    }
    else if (!authModel->Login(username, password))
    {
        QMessageBox::warning(this, usernameEnter, "Uncorrect username or password");
    }
    else
        close();
}
} // namespace inklink::auth