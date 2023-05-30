#pragma once

#include "AuthModel.hpp"

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
class AuthDialog : public QWidget
{
    Q_OBJECT

public:
    AuthDialog(QWidget* parent = nullptr);
    ~AuthDialog() = default;

    void NotifyGotResultFromNetwork(bool result);

private:
    void OnLoginButtonClicked();
    void OnCreateButtonClicked();
    void DoOnGotResultFromNetwork(bool result);

signals:
    void GotResultFromNetwork(bool result);

private:
    QLineEdit* m_usernameLine;
    QLineEdit* m_passwordLine;
    AuthModel* m_model;
};
} // namespace inklink::auth
