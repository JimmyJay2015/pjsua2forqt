//
//  pjvidwidget_linux.cpp
//  pjsip demo
//
//  Created by Jimmy on 10/26/18.
//
//
#include "pjvidwidget.h"


#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <QX11Info>


PjvidWidget::PjvidWidget(void *hwnd, QWidget *tparent)
: QWidget(tparent)
, _vidWindow(hwnd)
, _originSize(0, 0)
{

    XReparentWindow(QX11Info::display(), (XID)_vidWindow, (XID)winId(), 0, 0);


    // get _originSize


    // show


}

void PjvidWidget::init() {
    // resize to origin

    // move to 0, 0
} 

PjvidWidget::~PjvidWidget() {
    qDebug() << ">>>>>>>>>>>>>>~PjvidWidget";
}

void PjvidWidget::resizeEvent(QResizeEvent *e) {
    QWidget::resizeEvent(e);

    qint32 w = width();
    qint32 h = height();


    // resize to w, h
    XResizeWindow(QX11Info::display(), (XID)_vidWindow, w, h);

}


