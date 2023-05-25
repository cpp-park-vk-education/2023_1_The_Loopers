#pragma once

#include "AuthModel.hpp"

#include <QDialog>
#include <QGraphicsView>
#include <QLineEdit>
#include <QString>
#include <QWidget>

namespace inklink::auth
{
class LoginDialog : public QDialog
{
public:
    LoginDialog(QWidget *parent = nullptr);

    ~LoginDialog() = default;

private:
    void OnRegisterButtonClicked();

    void OnEnterButtonClicked();

private:
    QLineEdit *usernameLine;
    QLineEdit *passwordLine;
};
}