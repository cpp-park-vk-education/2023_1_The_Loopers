#include "AuthView.h"

#include "AuthModel.h"
#include "LoginView.h"

#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

#include <iostream>

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
AuthDialog::AuthDialog(QWidget* parent) : QDialog(parent)
{
    m_model = new AuthModel;

    connect(this, &AuthDialog::GotResultFromNetwork, this, &AuthDialog::DoOnGotResultFromNetwork);

    setWindowTitle("Authentication");

    setWindowFlags(Qt::CoverWindow | Qt::WindowTitleHint);

    setFixedSize(300, 200);

    m_usernameLine = new QLineEdit(this);
    m_passwordLine = new QLineEdit(this);

    auto* usernameLabel = new QLabel(tr("Username"), this);
    auto* passwordLabel = new QLabel(tr("Password"), this);

    auto* createButton = new QPushButton(tr("Register"), this);
    createButton->setFlat(true);
    createButton->setStyleSheet(myStyleSheet);
    connect(createButton, &QPushButton::clicked, this, &AuthDialog::OnCreateButtonClicked);

    auto* loginButton = new QPushButton(tr("Log in"), this);
    loginButton->setFlat(true);
    loginButton->setStyleSheet(myStyleSheet);
    connect(loginButton, &QPushButton::clicked, this, &AuthDialog::OnLoginButtonClicked);

    auto* buttonsLayout = new QHBoxLayout;
    buttonsLayout->setAlignment(Qt::AlignCenter);
    buttonsLayout->addWidget(createButton);
    buttonsLayout->addWidget(loginButton);

    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(usernameLabel);
    mainLayout->addWidget(m_usernameLine);

    mainLayout->addWidget(passwordLabel);
    mainLayout->addWidget(m_passwordLine);

    mainLayout->addLayout(buttonsLayout);

    setLayout(mainLayout);
}

void AuthDialog::OnLoginButtonClicked()
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

void AuthDialog::OnCreateButtonClicked()
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
        const auto message = m_model->RegParseToSend(username, password);
        m_model->RegSend(message);
    }
}

void AuthDialog::NotifyGotResultFromNetwork(int result)
{
    emit GotResultFromNetwork(result);
}

void AuthDialog::DoOnGotResultFromNetwork(int result)
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