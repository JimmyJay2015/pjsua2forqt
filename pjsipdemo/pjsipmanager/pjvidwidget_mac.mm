//
//  pjvidwidget_mac.cpp
//  pjsip demo
//
//  Created by Jimmy on 10/26/18.
//
//
#include "pjvidwidget.h"

#import<Cocoa/Cocoa.h>


PjvidWidget::PjvidWidget(void *vidWindow, QWidget *tparent)
: QWidget(tparent)
, _vidWindow(vidWindow)
, _originSize(0, 0)
{
    NSWindow *w = (NSWindow*)_vidWindow;
    //if (w) {
        NSRect r = [w bounds];
        //_originSize = QSize([r width, w.height);
        NSWindow *parent = [(NSView*)winId() window];
        [parent addChildWindow:w ordered:NSWindowAbove];

        //r.x = 0;
        //r.y = 0;

        [w setFrame:r display:YES];
    //}



}

void PjvidWidget::init() {
    NSWindow *w = (NSWindow*)_vidWindow;
    if (!w) {
        return;
    }

    //NSRect r = [w bounds];;
    //[w setFrame:r display:YES];

    // if (!_originSize.isEmpty()) {
    //     w.frame = (0, 0, _originSize.width(), _originSize.height());
    // } else {
    //     w.frame = (0, 0, 300, 300;
    // }
}

PjvidWidget::~PjvidWidget() {
    qDebug() << ">>>>>>>>>>>>>>~PjvidWidget";
}

void PjvidWidget::resizeEvent(QResizeEvent *e) {
    QWidget::resizeEvent(e);

    qint32 w = width();
    qint32 h = height();

    // NSWindow *w = (NSWindow*)_vidWindow;
    // if (!w) {
    //     return;
    // }

    // if (!_originSize.isEmpty()) {
    //     w.frame = (0, 0, w, h);
    // } else {
    //     w.frame = (0, 0, w, h);
    // }

}



