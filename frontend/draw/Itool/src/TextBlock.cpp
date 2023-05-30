#include "TextBlock.hpp"

namespace inklink::draw
{
TextRedactor::TextRedactor(QWidget* parent) : QWidget(parent), textBlock(nullptr), isResizing(false)
{
    setMouseTracking(true);
}

void TextRedactor::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        if (!textBlock)
        {
            textBlock = new QTextEdit(this);
            textBlock->setGeometry(event->pos().x(), event->pos().y(), 100, 50);
            textBlock->setReadOnly(false);
            textBlock->show();
            textBlock->setFocus();

            isResizing = true;
        }
        else
        {
            if (textBlock->geometry().contains(event->pos()))
            {
                if (textBlock->isReadOnly())
                {
                    textBlock->setReadOnly(false);
                    textBlock->setFocus();
                }
            }
            else
            {
                textBlock->setReadOnly(true);
                textBlock->clearFocus();

                if (textBlock->geometry().contains(event->pos()))
                {
                    textBlock->setReadOnly(false);
                    textBlock->setFocus();
                }
            }
        }
    }

    QWidget::mousePressEvent(event);
}

void TextRedactor::mouseMoveEvent(QMouseEvent* event)
{
    if (isResizing)
    {
        int width = event->pos().x() - textBlock->pos().x();
        int height = event->pos().y() - textBlock->pos().y();
        textBlock->resize(width, height);
    }

    QWidget::mouseMoveEvent(event);
}

void TextRedactor::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        if (textBlock && isResizing)
        {
            isResizing = false;
        }
    }

    QWidget::mouseReleaseEvent(event);
}
} // namespace inklink::draw
