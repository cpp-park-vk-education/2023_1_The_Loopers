#pragma once

#include <QListView>
#include <QPalette>
#include <QPushButton>
#include <QStyleFactory>
#include <QVBoxLayout>
#include <QWidget>

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
        setGeometry(0, 0, 262, 600);
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

        // QPalette buttonPalette = button->palette();
        // buttonPalette.setColor(QPalette::Button, QColor(25, 25, 25));
        // buttonPalette.setColor(QPalette::ButtonText, Qt::white);
        // buttonPalette.setColor(QPalette::Highlight, QColor(60, 60, 60));
        // button->setPalette(buttonPalette);

        // connect(button, &QPushButton::pressed, this,
        //         [button]()
        //         {
        //             QPalette pressedPalette = button->palette();
        //             pressedPalette.setColor(QPalette::Button, Qt::white);
        //             pressedPalette.setColor(QPalette::ButtonText, QColor(25, 25, 25));
        //             pressedPalette.setColor(QPalette::Highlight, QColor(200, 200, 200));
        //             button->setPalette(pressedPalette);
        //         });

        // connect(button, &QPushButton::released, this,
        //         [button]()
        //         {
        //             QPalette releasedPalette = button->palette();
        //             releasedPalette.setColor(QPalette::Button, QColor(25, 25, 25));
        //             releasedPalette.setColor(QPalette::ButtonText, Qt::white);
        //             releasedPalette.setColor(QPalette::Highlight, QColor(60, 60, 60));
        //             button->setPalette(releasedPalette);
        //         });

        // button->setStyleSheet(
        //         "QPushButton {"
        //         "    background-color: white;"
        //         "    color: rgb(25, 25, 25);"
        //         "}"
        //         "QPushButton:hover {"
        //         "    background-color: rgb(200, 200, 200);"
        //         "}"
        //         "QPushButton:pressed {"
        //         "    background-color: white;"
        //         "    color: rgb(25, 25, 25);"
        //         "    border-style: inset;"
        //         "}");

        return button;
    }
};
} // namespace inklink::menu
