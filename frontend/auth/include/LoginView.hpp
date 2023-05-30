#pragma once

#include "AuthModel.hpp"

#include <QGraphicsView>
#include <QLineEdit>
#include <QString>
#include <QWidget>

class AuthModel;

namespace inklink::auth
{
class LoginDialog : public QWidget
{
    Q_OBJECT
public:
    LoginDialog(QWidget* parent = nullptr);
    ~LoginDialog() = default;

private:
    void OnRegisterButtonClicked();
    void OnEnterButtonClicked();
    void NotifyGotResultFromNetwork(bool result);
    void DoOnGotResultFromNetwork(bool result);
signals:
    void GotResultFromNetwork(bool result);

private:
    QLineEdit* usernameLine;
    QLineEdit* passwordLine;
    AuthModel* m_model;
};
} // namespace inklink::auth