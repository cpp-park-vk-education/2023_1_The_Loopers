#pragma once

#include "AuthModel.hpp"

#include <QDialog>
#include <QGraphicsView>
#include <QLineEdit>
#include <QString>
#include <QWidget>


class AuthDialog : public QDialog {
    //    Q_OBJECT
public:
    AuthDialog(QWidget *parent = nullptr);

    ~AuthDialog() {}

private:
    QLineEdit *usernameLine;
    QLineEdit *passwordLine;
private slots:

    void OnPushButtonLogin();

    void CreatePushButton();
};

class LoginDialog : public QDialog {
    //    Q_OBJECT
public:
    LoginDialog(QWidget *parent = nullptr);

    ~LoginDialog() {}

private:
    QLineEdit *usernameLine;
    QLineEdit *passwordLine;
private slots:

    void OnPushButtonRegistration();

    void EnterPushButton();
};
