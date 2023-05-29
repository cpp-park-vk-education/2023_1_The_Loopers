// #include "GraphView.h"
#include "DrawView.h"
#include "MenuView.h"

#include <QApplication>
#include <QMainWindow>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Установка стиля
    QApplication::setStyle(QStyleFactory::create("Fusion"));

    // Переопределение цветовой схемы стиля
    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(25, 25, 25));
    darkPalette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::Disabled, QPalette::WindowText, QColor(127, 127, 127));
    darkPalette.setColor(QPalette::Disabled, QPalette::Text, QColor(127, 127, 127));
    darkPalette.setColor(QPalette::Disabled, QPalette::ButtonText, QColor(127, 127, 127));
    app.setPalette(darkPalette);

    QWidget *central_widget = new QWidget;
    QHBoxLayout *lay = new QHBoxLayout(central_widget);
    central_widget->setLayout(lay);
    QMainWindow mainWindow;
    auto left_label = new inklink::menu::MenuView;
    left_label->setMaximumSize(200,2000);
    auto right_label = new inklink::draw::DrawView;
    left_label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    lay->addWidget(left_label);
    lay->addWidget(right_label);
    mainWindow.setCentralWidget(central_widget);
    mainWindow.setWindowTitle("InkLink");
    mainWindow.show();
    return QApplication::exec();
}
