#pragma once

#include <QListView>
#include <QPalette>
//#include <QToolButton>
#include <QStyleFactory>
#include <QToolButton>
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

        QToolButton* freeLineButton = createToolButton(tr("Pen"), QIcon("../pen.svg") , this);
        verticalLayout->addWidget(freeLineButton);
        connect(freeLineButton, &QToolButton::clicked, this, &ToolBox::FreeLineClicked);

        QToolButton* lineButton = createToolButton(tr("Line"), QIcon("../line.svg") , this);
        verticalLayout->addWidget(lineButton);
        connect(lineButton, &QToolButton::clicked, this, &ToolBox::LineClicked);

        QToolButton* rectangleButton = createToolButton(tr("Rectangle"), QIcon("../rectangle.svg") , this);
        verticalLayout->addWidget(rectangleButton);
        connect(rectangleButton, &QToolButton::clicked, this, &ToolBox::RectangleClicked);

        QToolButton* ellipseButton = createToolButton(tr("Ellipse"), QIcon("../ellipse.svg") , this);
        verticalLayout->addWidget(ellipseButton);
        connect(ellipseButton, &QToolButton::clicked, this, &ToolBox::EllipseClicked);
    }

    QToolButton* createToolButton(const QString &toolTip, const QIcon &icon, QWidget* parent)
    {
        QToolButton *button = new QToolButton(parent);
        button->setToolTip(toolTip);
        button->setIcon(icon);
        button->setIconSize(QSize(32, 32));
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
