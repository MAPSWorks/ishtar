#include "situation_view_qquick_painted_item.h"

#include <QWheelEvent>
#include <QMouseEvent>
#include <QTouchEvent>
#include <QPainter>
#include <QCursor>

#include "presenters/situation_node_presenter.h"
#include "drawers/situation_node_drawer/situation_node_drawer_qpainter.h"

namespace
{
    const double mouseWheelDegrees = 1200.0d;
}

using namespace presentation;

class SituationViewQQuickPaintedItem::SituationViewQQuickPaintedItemPrivate
{
public:
    SituationNodePresenterPtr presenter;
    SituationNodeDrawerQPainter drawer;
    QPointF lastMousePosition;
    QLineF touchLine;
};

SituationViewQQuickPaintedItem::
    SituationViewQQuickPaintedItem(QQuickItem* parent):
    QQuickPaintedItem(parent),
    ISituationNodeView(),
    d(new SituationViewQQuickPaintedItemPrivate())
{
    d->presenter.reset(new SituationNodePresenter(this));
    this->setFlag(QQuickItem::ItemHasContents, true);

    this->setCursor(QCursor(Qt::CrossCursor));
    this->setAcceptedMouseButtons(Qt::AllButtons);
    this->setFiltersChildMouseEvents(true);
    this->setAcceptHoverEvents(true);
}

SituationViewQQuickPaintedItem::~SituationViewQQuickPaintedItem()
{
    delete d;
}

ISituationNodeDrawer* SituationViewQQuickPaintedItem::drawer() const
{
    return &d->drawer;
}

void presentation::SituationViewQQuickPaintedItem::paint(QPainter* painter)
{
    d->drawer.draw(painter, this->transformationMatrix());

    painter->setPen(Qt::cyan);
    painter->setBrush(Qt::cyan);

    if (!d->touchLine.isNull())
    {
        painter->drawLine(d->touchLine);
    }
}

void SituationViewQQuickPaintedItem::wheelEvent(QWheelEvent* event)
{
    this->scaleToPoint(1 + (event->delta() / ::mouseWheelDegrees),
                       QPointF(event->x(), event->y()));
    this->update();
}

void SituationViewQQuickPaintedItem::mouseMoveEvent(QMouseEvent* event)
{
    if (d->lastMousePosition.isNull())
    {
        d->lastMousePosition = event->pos();
    }
    else if (event->buttons() == Qt::LeftButton)
    {
        QPointF dPosition = event->pos() - d->lastMousePosition;
        this->transformationMatrix().translate(
                    dPosition.x() / this->transformationMatrix().m11(),
                    dPosition.y() / this->transformationMatrix().m22());
        this->update();
    }

    d->lastMousePosition = event->pos();
}

void SituationViewQQuickPaintedItem::mousePressEvent(QMouseEvent* event)
{
    Q_UNUSED(event)
}

void SituationViewQQuickPaintedItem::mouseReleaseEvent(QMouseEvent* event)
{
    d->lastMousePosition = QPointF();
}

void SituationViewQQuickPaintedItem::touchEvent(QTouchEvent* event)
{
    if (event->touchPoints().count() > 1)
    {
        QLineF line;
        line.setP1(event->touchPoints().first().pos());
        line.setP2(event->touchPoints().last().pos());

        if (!d->touchLine.isNull())
        {
            this->scaleToPoint(line.length() / d->touchLine.length(),
                               QPointF((line.p2().x() + line.p1().x()) / 2,
                                       (line.p2().y() + line.p1().y()) / 2));
        }

        d->touchLine = line;
    }
    else
    {
        d->touchLine = QLine();
    }

    this->update();
}
