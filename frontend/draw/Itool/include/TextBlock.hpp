#include <QApplication>
#include <QMouseEvent>
#include <QTextEdit>
#include <QWidget>

namespace inklink::draw
{
class TextRedactor : public QWidget
{
public:
    TextRedactor(QWidget* parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    QTextEdit* textBlock;
    bool isResizing;
};
} // namespace inklink::draw
