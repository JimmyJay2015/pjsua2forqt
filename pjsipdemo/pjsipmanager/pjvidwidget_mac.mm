//
//  pjvidwidget_mac.cpp
//  pjsip demo
//
//  Created by Jimmy on 10/26/18.
//
//
#include "pjvidwidget.h"

#import<Cocoa/Cocoa.h>

#include <QWindow>
#include <QWidget>


PjvidWidget::PjvidWidget(void *vidWindow, QWidget *tparent)
: QWidget(tparent)
, _vidWindow(vidWindow)
, _originSize(0, 0)
{

}

void PjvidWidget::init() {
    NSWindow *w = (NSWindow*)_vidWindow;
    if (!w) {
        return;
    }
    
}

PjvidWidget::~PjvidWidget() {
    qDebug() << ">>>>>>>>>>>>>>~PjvidWidget";
}

void PjvidWidget::resizeEvent(QResizeEvent *e) {
    QWidget::resizeEvent(e);

}



