//
//  pjvidwidget_linux.cpp
//  pjsip demo
//
//  Created by Jimmy on 10/26/18.
//
//
#include "pjvidwidget.h"




PjvidWidget::PjvidWidget(void *vidWindow, QWidget *tparent)
: QWidget(tparent)
, _vidWindow(vidWindow)
, _originSize(0, 0)
{

}

void PjvidWidget::init() {
    
}

PjvidWidget::~PjvidWidget() {
    qDebug() << ">>>>>>>>>>>>>>~PjvidWidget";
}

void PjvidWidget::resizeEvent(QResizeEvent *e) {
    QWidget::resizeEvent(e);

    qint32 w = width();
    qint32 h = height();
}


