#pragma once

// #include "DrawView.h"
#include "DrawUsingQGraphicsView.h"
#include "MenuView.h"
#include "ToolBox.h"

#include <QMainWindow>

namespace inklink::general
{
class InklinkMainWindow : public QMainWindow
{
    // Q_OBJECT

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
        m_drawView = new inklink::draw::DrawingView;
        m_toolBox = new inklink::draw::ToolBox;

        m_menu->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

        layout->addWidget(m_menu);
        layout->addWidget(m_drawView);
        layout->addWidget(m_toolBox);

        setCentralWidget(centralWidget);
        setWindowTitle("InkLink");
    }

    ~InklinkMainWindow() = default;

private:
    inklink::menu::MenuView* m_menu;
    inklink::draw::DrawingView* m_drawView;
    inklink::draw::ToolBox* m_toolBox;
};
} // namespace inklink::general
