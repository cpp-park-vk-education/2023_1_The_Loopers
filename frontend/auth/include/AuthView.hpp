#pragma once

#include "AuthModel.hpp"

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
class AuthDialog : public QDialog
{
    Q_OBJECT

public:
    AuthDialog(QWidget* parent = nullptr);
    ~AuthDialog() = default;

    void NotifyGotResultFromNetwork(int result);

private:
    void OnLoginButtonClicked();
    void OnCreateButtonClicked();
    void DoOnGotResultFromNetwork(int result);

signals:
    void GotResultFromNetwork(int result);

private:
    QLineEdit* m_usernameLine;
    QLineEdit* m_passwordLine;
    AuthModel* m_model;
};
} // namespace inklink::auth
