#pragma once

#include <QListView>
#include <QPalette>
#include <QPushButton>
#include <QStyleFactory>
#include <QVBoxLayout>
#include <QWidget>

namespace inklink::draw
{
class ToolBox : public QWidget
{
    Q_OBJECT

public:
    ToolBox(QWidget* parent = nullptr) : QWidget(parent)
    {
        InitUi();
    }

signals:
    void LineClicked(bool);
    void FreeLineClicked(bool);
    void RectangleClicked(bool);
    void EllipseClicked(bool);

private:
    void InitUi()
    {
        setGeometry(0, 0, 200, 600);
        setWindowTitle("Widget");

        QVBoxLayout* verticalLayout = new QVBoxLayout(this);

        QPushButton* pushButton = createButton("Line", this);
        verticalLayout->addWidget(pushButton);
        connect(pushButton, &QPushButton::clicked, this, &ToolBox::LineClicked);

        pushButton = createButton("FreeLine", this);
        verticalLayout->addWidget(pushButton);
        connect(pushButton, &QPushButton::clicked, this, &ToolBox::FreeLineClicked);

        pushButton = createButton("Rectangle", this);
        verticalLayout->addWidget(pushButton);
        connect(pushButton, &QPushButton::clicked, this, &ToolBox::RectangleClicked);

        pushButton = createButton("Ellipse", this);
        verticalLayout->addWidget(pushButton);
        connect(pushButton, &QPushButton::clicked, this, &ToolBox::EllipseClicked);
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
} // namespace inklink::draw
