#pragma once

#include <QListView>
#include <QPalette>
#include <QPushButton>
#include <QStyleFactory>
#include <QVBoxLayout>
#include <QWidget>

#include <iostream>

namespace inklink::menu
{
class MenuView : public QWidget
{
    Q_OBJECT

public:
    MenuView(QWidget* parent = nullptr) : QWidget(parent)
    {
        InitUi();
    }

signals:
    void WhiteboardClicked(bool);
    void GraphClicked(bool);
    void AuthClicked(bool);
    void LinkClicked(bool);
    void NewClicked(bool);
    void QuitClicked(bool);

private:
    void InitUi()
    {
        setGeometry(0, 0, 200, 600);
        setWindowTitle("Widget");

        QVBoxLayout* verticalLayout = new QVBoxLayout(this);

        QPushButton* pushButton = createButton("Graph", this);
        verticalLayout->addWidget(pushButton);
        connect(pushButton, &QPushButton::clicked, this, &MenuView::GraphClicked);

        pushButton = createButton("Whiteboard", this);
        verticalLayout->addWidget(pushButton);
        connect(pushButton, &QPushButton::clicked, this, &MenuView::WhiteboardClicked);

        QListView* listView = new QListView(this);
        verticalLayout->addWidget(listView);

        pushButton = createButton("Link", this);
        verticalLayout->addWidget(pushButton);
        connect(pushButton, &QPushButton::clicked, this, &MenuView::LinkClicked);

        pushButton = createButton("Create new", this);
        verticalLayout->addWidget(pushButton);
        connect(pushButton, &QPushButton::clicked, this, &MenuView::NewClicked);

        pushButton = createButton("Auth", this);
        verticalLayout->addWidget(pushButton);
        connect(pushButton, &QPushButton::clicked, this, &MenuView::AuthClicked);

        pushButton = createButton("Quit", this);
        verticalLayout->addWidget(pushButton);
        connect(pushButton, &QPushButton::clicked, this, &MenuView::QuitClicked);
    }

    QPushButton* createButton(const QString& text, QWidget* parent)
    {
        QPushButton* button = new QPushButton(text, parent);
        button->setFlat(true);
        button->setObjectName("customButton");

        QString styleSheet = R"(
            #customButton {
                background-color: rgb(25, 25, 25);
                color: white;
            }
            #customButton:hover {
                background-color: rgb(200, 200, 200);
                color: rgb(25, 25, 25);
            }
            #customButton:pressed {
                background-color: white;
                color: rgb(25, 25, 25);
            }
        )";

        button->setStyleSheet(styleSheet);

        return button;
    }
};
} // namespace inklink::menu
