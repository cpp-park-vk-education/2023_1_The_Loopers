#pragma once

#include <QDialog>
#include <QGraphicsView>
#include <QWidget>

class AuthView : public QGraphicsView {
    //    Q_OBJECT
public:
    AuthView(QWidget *parent = nullptr);

    ~AuthView() {}

protected:
    void drawBackground(QPainter *painter, const QRectF &rect) override;
};

class AuthDialog : public QDialog {
    //    Q_OBJECT
public:
    AuthDialog(QWidget *parent = nullptr);

    ~AuthDialog() {}
};