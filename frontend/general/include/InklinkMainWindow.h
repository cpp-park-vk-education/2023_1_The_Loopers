#pragma once

// #include "DrawView.h"
#include "AuthView.h"
#include "GraphicsDrawView.h"
#include "DrawSceneModel.h"
#include "MenuView.h"
#include "ToolBox.h"

#include <QMainWindow>

#include <iostream>

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

        m_menu = new menu::MenuView;
        m_menu->setMaximumWidth(200);
        m_drawView = new inklink::draw::GraphicsDrawView;
        m_toolBox = new inklink::draw::ToolBox;

        m_menu->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

        layout->addWidget(m_menu);
        layout->addWidget(m_drawView);
        layout->addWidget(m_toolBox);

        setCentralWidget(centralWidget);
        setWindowTitle("InkLink");

        ConnectSignals();
    }

    void ConnectSignals()
    {
        connect(m_menu, &menu::MenuView::AuthClicked, this, &InklinkMainWindow::OnAuthBtnClicked);
        connect(m_toolBox, &draw::ToolBox::LineClicked, draw::DrawSceneModel, &draw::DrawSceneModel::SetLineMode);
        connect(m_toolBox, &draw::ToolBox::FreeLineClicked, draw::DrawSceneModel, &draw::DrawSceneModel::SetFreeLineMode);
        connect(m_toolBox, &draw::ToolBox::EllipseClicked, draw::DrawSceneModel, &draw::DrawSceneModel::SetEllipseMode);
        connect(m_toolBox, &draw::ToolBox::RectangleClicked, draw::DrawSceneModel, &draw::DrawSceneModel::SetRectangleMode);
    }

    ~InklinkMainWindow() = default;

private:
    void OnAuthBtnClicked(bool)
    {
        auth::AuthDialog dialog(this);
        dialog.exec();
    }

private:
    inklink::menu::MenuView* m_menu;
    inklink::draw::GraphicsDrawView* m_drawView;
    inklink::draw::ToolBox* m_toolBox;
};
} // namespace inklink::general
