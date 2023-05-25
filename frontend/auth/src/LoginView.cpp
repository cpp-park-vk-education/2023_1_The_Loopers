#include "AuthView.hpp"
#include "LoginView.hpp"

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
LoginDialog::LoginDialog(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("Login");

    setFixedSize(300, 200);

    usernameLine = new QLineEdit(this);
    passwordLine = new QLineEdit(this);

    auto *usernameLabel = new QLabel(tr("Username"), this);
    auto *passwordLabel = new QLabel(tr("Password"), this);

    auto *enterButton = new QPushButton(tr("Login"), this);
    enterButton->setAutoDefault(false);
    connect(enterButton, &QPushButton::clicked, this, &LoginDialog::OnEnterButtonClicked);

    auto *registrationButton = new QPushButton(tr("Register"), this);
    registrationButton->setAutoDefault(false);
    connect(registrationButton, &QPushButton::clicked, this, &LoginDialog::OnRegisterButtonClicked);

    auto *buttonsLayout = new QHBoxLayout;
    buttonsLayout->setAlignment(Qt::AlignCenter);
    buttonsLayout->addWidget(enterButton);
    buttonsLayout->addWidget(registrationButton);

    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(usernameLabel);
    mainLayout->addWidget(usernameLine);

    mainLayout->addWidget(passwordLabel);
    mainLayout->addWidget(passwordLine);

    mainLayout->addLayout(buttonsLayout);

    setLayout(mainLayout);
}

void LoginDialog::OnRegisterButtonClicked()
{
    auto registration = new AuthDialog(this);
    close();
    registration->show();
}

void LoginDialog::OnEnterButtonClicked()
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
