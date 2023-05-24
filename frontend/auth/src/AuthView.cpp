#include "AuthModel.hpp"
#include "AuthView.hpp"

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QWidget>


AuthDialog::AuthDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle("Registration");

    setFixedSize(300, 200);

    usernameLine = new QLineEdit(this);
    passwordLine = new QLineEdit(this);

    QLabel *usernameLabel = new QLabel(tr("Username"), this);
    QLabel *passwordLabel = new QLabel(tr("Password"), this);

    QPushButton *acceptButton = new QPushButton(tr("Registrate"), this);
    acceptButton->setAutoDefault(false);
    connect(acceptButton, &QPushButton::clicked, this, &AuthDialog::CreatePushButton);

    QPushButton *loginButton = new QPushButton(tr("Log in"), this);
    loginButton->setAutoDefault(false);
    connect(loginButton, &QPushButton::clicked, this, &AuthDialog::OnPushButtonLogin);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->setAlignment(Qt::AlignCenter);
    buttonsLayout->addWidget(acceptButton);
    buttonsLayout->addWidget(loginButton);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(usernameLabel);
    mainLayout->addWidget(usernameLine);

    mainLayout->addWidget(passwordLabel);
    mainLayout->addWidget(passwordLine);

    mainLayout->addLayout(buttonsLayout);

    setLayout(mainLayout);
}

void AuthDialog::OnPushButtonLogin() {
    auto login = new LoginDialog(this);
    close();
    login->show();
}

void AuthDialog::CreatePushButton() {
    std::string username;
    std::string password;

    QString usernameEnter = usernameLine->text();
    QString passwordEnter = passwordLine->text();

    username = usernameEnter.toStdString();
    password = passwordEnter.toStdString();


    if (username.empty()) {
        QMessageBox::warning(this, usernameEnter, "Enter username");
    } else if (password.empty()) {
        QMessageBox::warning(this, passwordEnter, "Enter password");
    }
    //Закомменченный кусок не хочет работать
    //    else if(!CreateUser(username, password)){
    //        QMessageBox::warning(this, usernameEnter, "Existing username");
    //    }
    else close();
}

LoginDialog::LoginDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle("Login");

    setFixedSize(300, 200);

    usernameLine = new QLineEdit(this);
    passwordLine = new QLineEdit(this);

    QLabel *usernameLabel = new QLabel(tr("Username"), this);
    QLabel *passwordLabel = new QLabel(tr("Password"), this);

    QPushButton *acceptButton = new QPushButton(tr("Login"), this);
    acceptButton->setAutoDefault(false);
    connect(acceptButton, &QPushButton::clicked, this, &LoginDialog::EnterPushButton);

    QPushButton *registrationButton = new QPushButton(tr("Registrate"), this);
    registrationButton->setAutoDefault(false);
    connect(registrationButton, &QPushButton::clicked, this, &LoginDialog::OnPushButtonRegistration);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->setAlignment(Qt::AlignCenter);
    buttonsLayout->addWidget(acceptButton);
    buttonsLayout->addWidget(registrationButton);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(usernameLabel);
    mainLayout->addWidget(usernameLine);

    mainLayout->addWidget(passwordLabel);
    mainLayout->addWidget(passwordLine);

    mainLayout->addLayout(buttonsLayout);

    setLayout(mainLayout);
}

void LoginDialog::OnPushButtonRegistration() {
    auto registration = new AuthDialog(this);
    close();
    registration->show();
}

void LoginDialog::EnterPushButton() {
    std::string username;
    std::string password;

    QString usernameEnter = usernameLine->text();
    QString passwordEnter = passwordLine->text();

    username = usernameEnter.toStdString();
    password = passwordEnter.toStdString();

    if (username.empty()) {
        QMessageBox::warning(this, usernameEnter, "Enter username");
    } else if (password.empty()) {
        QMessageBox::warning(this, passwordEnter, "Enter password");
    }
    //    Закомменченный кусок не хочет работать
    //    else if(!Login(username, password)){
    //        QMessageBox::warning(this, usernameEnter, "Uncorrect username or password");
    //    }
    else close();
}
