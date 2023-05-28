#include "GraphView.h"

#include <QApplication>
#include <QMainWindow>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    auto *widget = new inklink::graph::GraphView;

    QMainWindow mainWindow;
    mainWindow.setCentralWidget(widget);
    mainWindow.setWindowTitle("InkLink::Graph");
    mainWindow.show();
    return QApplication::exec();
}
