#pragma once

#include "AuthModel.hpp"

#include <QGraphicsView>
#include <QLineEdit>
#include <QString>
#include <QWidget>

class AuthModel;

namespace inklink::auth
{
class AuthDialog : public QWidget
{
    Q_OBJECT
public:
    AuthDialog(QWidget* parent = nullptr);
    ~AuthDialog() = default;

private:
    void OnLoginButtonClicked();
    void OnCreateButtonClicked();
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