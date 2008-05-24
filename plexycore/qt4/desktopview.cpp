/*******************************************************************************
* This file is part of PlexyDesk.
*  Maintained by : Siraj Razick <siraj@kde.org>
*  Authored By  :
*
*  PlexyDesk is free software: you can redistribute it and/or modify
*  it under the terms of the GNU Lesser General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  PlexyDesk is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU Lesser General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with PlexyDesk. If not, see <http://www.gnu.org/licenses/lgpl.html>
*******************************************************************************/
#include "desktopview.h"
#include <desktopwidget.h>
#include <backdropinterface.h>
#include <pluginloader.h>
#include <baseplugin.h>
#include <backdropplugin.h>
#include <widgetplugin.h>
#include <viewlayer.h>
#include <QGLWidget>


namespace PlexyDesk
{
class  DesktopView::Private
{
    public:
    Private(){}
    ~Private(){}
    AbstractPluginInterface * bIface ;
    BackdropPlugin * bgPlugin;
    WidgetPlugin * widgets;
    QGraphicsGridLayout * gridLayout;
    ViewLayer *  layer;
    int row;
    int column;
};

bool getLessThanWidget(const QGraphicsItem* it1, const QGraphicsItem* it2)
{
	return it1->zValue() < it2->zValue();
}

DesktopView::DesktopView ( QGraphicsScene * scene, QWidget * parent ):QGraphicsView(scene,parent),d(new Private)
{
       setWindowFlags(Qt::FramelessWindowHint);
       setAttribute(Qt::WA_ContentsPropagated );
       setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
       setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
       setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
       setFrameStyle(QFrame::NoFrame);
       setAlignment(Qt::AlignLeft | Qt::AlignTop);
       d->bgPlugin  = static_cast<BackdropPlugin*> (PluginLoader::getInstance()->instance("classicbackdrop"));
       d->widgets = 0;
       d->gridLayout = new QGraphicsGridLayout ();
       d->row = QDesktopWidget().availableGeometry().center().x()-(140*4)/2;
       d->column =  QDesktopWidget().availableGeometry().center().y()/2;;
       d->layer = new ViewLayer(this);
       d->gridLayout = new QGraphicsGridLayout ;
}

DesktopView::~DesktopView()
{
    delete d;
}

void DesktopView::backgroundChanged()
{
    invalidateScene();
    scene()->update();
}

void DesktopView::addExtension(const QString& name)
{

        d->widgets = static_cast<WidgetPlugin*> ( PluginLoader::getInstance()->instance(name));
        if (d->widgets){
                //scene()->addItem(d->widgets->item());
		DesktopWidget * widget = (DesktopWidget*) d->widgets->item();		
		if(widget){
		      widget->configState(DesktopWidget::DOCK);
		      scene()->addItem(widget);
                      widget->setPos(d->row,d->column);
                      d->row += widget->boundingRect().width()+10;
                  //    d->column += widget->boundingRect().height()+10;
                      d->layer->addItem("Widgets",widget);
                      //d->gridLayout->addItem(widget,d->row++,d->column++);

		}
	}

}

void DesktopView::addCoreExtension(const QString& name)
{

        d->widgets = static_cast<WidgetPlugin*> ( PluginLoader::getInstance()->instance(name));
        if (d->widgets){
                //scene()->addItem(d->widgets->item());
		QGraphicsRectItem  * widget = (QGraphicsRectItem*) d->widgets->item();		
		if(widget){
		     // widget->configState(DesktopWidget::DOCK);
		      scene()->addItem(widget);
                      widget->setPos(d->row,d->column);
                      d->row += widget->boundingRect().width();
                  //    d->column += widget->boundingRect().height()+10;
                      //d->layer->addItem("Widgets",widget);
                      //d->gridLayout->addItem(widget,d->row++,d->column++);

		}
	}

}

void DesktopView::drawBackground ( QPainter * painter, const QRectF & rect )
{
    painter->save();
    painter->setClipRect(rect);
    if( d->bgPlugin ) {
  	d->bgPlugin->render(painter,QRectF(rect.x(),rect.y(),rect.width(),rect.height()));
    }

    painter->restore();
}

void DesktopView::mousePressEvent(QMouseEvent *event)
{
	setTopMostWidget(event->pos());

	QGraphicsView::mousePressEvent(event);
}

void DesktopView::setTopMostWidget(const QPoint &pt)
{
	int i = 0;
	QGraphicsItem *clickedItem = scene()->itemAt(pt);
	if(clickedItem == 0)
		return;

	QList<QGraphicsItem *> itemsList = scene()->items();
	qStableSort(itemsList.begin(), itemsList.end(), getLessThanWidget);

	clickedItem->setZValue(itemsList.size());

	foreach(QGraphicsItem* item, itemsList)
	{
		if(item == clickedItem)
			continue;

		item->setZValue(i);
		i++;
	}

	clickedItem->update();
}

}
#include "desktopview.moc"
