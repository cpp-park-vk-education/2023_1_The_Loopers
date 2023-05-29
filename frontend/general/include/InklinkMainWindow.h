#pragma once

#include "DrawView.h"
#include "MenuView.h"

#include <QMainWindow>

namespace inklink::general
{
class InklinkMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    InklinkMainWindow(QWidget* parent = nullptr) : QMainWindow{parent}
    {
        InitUi();
    }

    void InitUi()
    {
        QWidget* centralWidget = new QWidget;
        QHBoxLayout* layout = new QHBoxLayout(centralWidget);
        centralWidget->setLayout(layout);

        m_menu = new inklink::menu::MenuView;
        m_menu->setMaximumSize(200, 2000);
        m_drawView = new inklink::draw::DrawView;
        left_label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

        layout->addWidget(m_menu);
        layout->addWidget(m_drawView);

        setCentralWidget(centralWidget);
        setWindowTitle("InkLink");
    }

    ~InklinkMainWindow() = default;

private:
    inklink::menu::MenuView* m_menu;
    inklink::draw::DrawView* m_drawView;
};
} // namespace inklink::general
