#pragma once

// #include "DrawView.h"
#include "AuthView.h"
// #include "DrawSceneModel.h"
#include "GraphicsDrawView.h"
#include "MenuView.h"
#include "ToolBox.h"

#include <QMainWindow>

#include <iostream>

namespace inklink::draw
{
class DrawSceneModel;
}

namespace inklink::general
{
class InklinkMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    InklinkMainWindow(QWidget* parent = nullptr);

    void InitUi();

    void ConnectSignals();

    ~InklinkMainWindow() = default;

private:
    void OnAuthBtnClicked(bool);

private:
    inklink::menu::MenuView* m_menu;
    inklink::draw::GraphicsDrawView* m_drawView;
    inklink::draw::DrawSceneModel* m_model;
    inklink::draw::ToolBox* m_toolBox;
};
} // namespace inklink::general
