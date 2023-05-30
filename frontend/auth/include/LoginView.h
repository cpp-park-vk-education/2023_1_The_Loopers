#pragma once

#include "AuthModel.h"

#include <QGraphicsView>
#include <QLineEdit>
#include <QString>
#include <QWidget>

// forward declaration
namespace inklink::auth
{
class AuthModel;
} // namespace inklink::auth

namespace inklink::auth
{
class LoginDialog : public QWidget
{
    Q_OBJECT

public:
    LoginDialog(QWidget* parent = nullptr);
    ~LoginDialog() = default;

    void NotifyGotResultFromNetwork(int result);

private:
    void OnRegisterButtonClicked();
    void OnEnterButtonClicked();
    void DoOnGotResultFromNetwork(int result);

signals:
    void GotResultFromNetwork(int result);

private:
    QLineEdit* m_usernameLine;
    QLineEdit* m_passwordLine;
    AuthModel* m_model;
};
} // namespace inklink::auth
