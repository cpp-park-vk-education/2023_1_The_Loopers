//#include "GraphView.h"
#include "DrawView.h"

#include <QApplication>
#include <QMainWindow>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    auto *widget = new inklink::draw::DrawView;

    QMainWindow mainWindow;
    mainWindow.setCentralWidget(widget);
    mainWindow.setWindowTitle("InkLink::Graph");
    mainWindow.show();
    return QApplication::exec();
}
