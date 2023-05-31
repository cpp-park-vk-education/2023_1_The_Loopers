#pragma once

#include <QDialog>
#include <QGraphicsView>
#include <QLineEdit>
#include <QString>

// forward declaration
namespace inklink::auth
{
class AuthModel;
} // namespace inklink::auth

namespace inklink::auth
{
class LoginDialog : public QDialog
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
