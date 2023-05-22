#include "AuthView.hpp"

#include <QDialog>
#include <QGraphicsView>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

AuthView::AuthView(QWidget *parent) : QGraphicsView(parent)
{
    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->setSceneRect(-640, -460, 1280, 920);
    setScene(scene);
    setFrameShape(QFrame::NoFrame);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void AuthView::drawBackground(QPainter *painter, const QRectF &rect)
{
    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(255, 255, 255));
    painter->drawRect(rect);
}

AuthDialog::AuthDialog(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("Login");

    QLineEdit *usernameLine = new QLineEdit(this);
    QLineEdit *passwordLine = new QLineEdit(this);

    QLabel *usernameLabel = new QLabel(tr("Username"), this);
    QLabel *passwordLabel = new QLabel(tr("Password"), this);

    QPushButton *acceptButton = new QPushButton(tr("Log in"), this);
    acceptButton->setAutoDefault(false);

    QPushButton *registrationButton = new QPushButton(tr("Registration"), this);
    registrationButton->setAutoDefault(false);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->setAlignment(Qt::AlignCenter);
    buttonsLayout->addWidget(acceptButton);
    buttonsLayout->addWidget(registrationButton);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(usernameLabel);
    mainLayout->addWidget(usernameLine);

    mainLayout->addWidget(passwordLabel);
    mainLayout->addWidget(passwordLine);

    mainLayout->addLayout(buttonsLayout);

    setLayout(mainLayout);
}