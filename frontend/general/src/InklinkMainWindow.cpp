#include "InklinkMainWindow.h"

#include "DrawSceneModel.h"

#include <QMainWindow>

#include <iostream>

namespace inklink::general
{
InklinkMainWindow::InklinkMainWindow(QWidget* parent) : QMainWindow{parent}
{
    InitUi();
}

void InklinkMainWindow::InitUi()
{
    QWidget* centralWidget = new QWidget;
    QHBoxLayout* layout = new QHBoxLayout(centralWidget);
    centralWidget->setLayout(layout);

    m_menu = new menu::MenuView;
    m_menu->setMaximumWidth(200);
    m_drawView = new inklink::draw::GraphicsDrawView;
    m_model = m_drawView->GetSceneModel();
    m_toolBox = new inklink::draw::ToolBox;

    m_menu->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    layout->addWidget(m_menu);
    layout->addWidget(m_drawView);
    layout->addWidget(m_toolBox);

    setCentralWidget(centralWidget);
    setWindowTitle("InkLink");

    ConnectSignals();
}

void InklinkMainWindow::ConnectSignals()
{
    connect(m_menu, &menu::MenuView::AuthClicked, this, &InklinkMainWindow::OnAuthBtnClicked);
    connect(m_toolBox, &draw::ToolBox::LineClicked, m_model, &draw::DrawSceneModel::SetLineMode);
    connect(m_toolBox, &draw::ToolBox::FreeLineClicked, m_model, &draw::DrawSceneModel::SetFreeLineMode);
    connect(m_toolBox, &draw::ToolBox::EllipseClicked, m_model, &draw::DrawSceneModel::SetEllipseMode);
    connect(m_toolBox, &draw::ToolBox::RectangleClicked, m_model, &draw::DrawSceneModel::SetRectangleMode);
}

void InklinkMainWindow::OnAuthBtnClicked(bool)
{
    auth::AuthDialog dialog(this);
    dialog.exec();
}
} // namespace inklink::general
