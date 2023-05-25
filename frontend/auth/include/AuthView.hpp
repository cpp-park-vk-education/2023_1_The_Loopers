#pragma once

#include "AuthModel.hpp"

#include <QDialog>
#include <QGraphicsView>
#include <QLineEdit>
#include <QString>
#include <QWidget>

namespace inklink::auth
{
class AuthDialog : public QDialog
{
public:
    AuthDialog(QWidget *parent = nullptr);

    ~AuthDialog() = default;

private:
    void OnPushButtonLogin();

    void OnPushButtonCreate();

private:
    QLineEdit *usernameLine;
    QLineEdit *passwordLine;
};

class LoginDialog : public QDialog
{
public:
    LoginDialog(QWidget *parent = nullptr);

    ~LoginDialog() = default;

private:
    void OnPushButtonRegistration();

    void OnPushButtonEnter();

private:
    QLineEdit *usernameLine;
    QLineEdit *passwordLine;
};
} // namespace inklink::auth