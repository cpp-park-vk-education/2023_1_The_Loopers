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
public:
    MenuView(QWidget* parent = nullptr) : QWidget(parent)
    {
        setGeometry(0, 0, 262, 600);
        setWindowTitle("Widget");

        QVBoxLayout* verticalLayout = new QVBoxLayout(this);

        QPushButton* pushButton_3 = createButton("Graph", this);
        verticalLayout->addWidget(pushButton_3);

        QPushButton* pushButton_2 = createButton("Whiteboard", this);
        verticalLayout->addWidget(pushButton_2);

        QListView* listView = new QListView(this);
        verticalLayout->addWidget(listView);

        QPushButton* pushButton_4 = createButton("Link", this);
        verticalLayout->addWidget(pushButton_4);

        QPushButton* pushButton = createButton("Create new", this);
        verticalLayout->addWidget(pushButton);

        QPushButton* pushButton_5 = createButton("Quit", this);
        verticalLayout->addWidget(pushButton_5);
    }

private:
    QPushButton* createButton(const QString& text, QWidget* parent)
    {
        QPushButton* button = new QPushButton(text, parent);
        button->setFlat(true);

        QPalette buttonPalette = button->palette();
        buttonPalette.setColor(QPalette::Button, QColor(25, 25, 25));
        buttonPalette.setColor(QPalette::ButtonText, Qt::white);
        buttonPalette.setColor(QPalette::Highlight, QColor(60, 60, 60));
        button->setPalette(buttonPalette);

        connect(button, &QPushButton::pressed, this,
                [button]()
                {
                    QPalette pressedPalette = button->palette();
                    pressedPalette.setColor(QPalette::Button, Qt::white);
                    pressedPalette.setColor(QPalette::ButtonText, QColor(25, 25, 25));
                    pressedPalette.setColor(QPalette::Highlight, QColor(200, 200, 200));
                    button->setPalette(pressedPalette);
                });

        connect(button, &QPushButton::released, this,
                [button]()
                {
                    QPalette releasedPalette = button->palette();
                    releasedPalette.setColor(QPalette::Button, QColor(25, 25, 25));
                    releasedPalette.setColor(QPalette::ButtonText, Qt::white);
                    releasedPalette.setColor(QPalette::Highlight, QColor(60, 60, 60));
                    button->setPalette(releasedPalette);
                });

        return button;
    }
};
} // namespace inklink::menu
