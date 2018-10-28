//
//  pjvidwidget_win.cpp
//  pjsip demo
//
//  Created by Jimmy on 10/26/18.
//
//
#include "pjvidwidget.h"

#include "pjsipmanager.h"

#include <windows.h>


PjvidWidget::PjvidWidget(HWND vidWindow, QWidget *parent)
: QWidget(parent)
, _vidWindow(vidWindow)
, _originSize(0, 0)
{
    SetParent(_vidWindow, (HWND)winId());

    RECT wr;
    if (GetWindowRect(_vidWindow, &wr)) {
        _originSize = QSize(qMax(0L, wr.right - wr.left)
            , qMax(0L, wr.bottom - wr.top));
    }

    ShowWindow(_vidWindow, SW_HIDE);
}
void PjvidWidget::init() {
    if (!_originSize.isEmpty()) {
        MoveWindow(_vidWindow, 0, 0, _originSize.width(), _originSize.height(), true);
        resize(_originSize);
    } else {
        MoveWindow(_vidWindow, 0, 0, 300, 300, true);
        resize(300, 300);
    }
    ShowWindow(_vidWindow, SW_SHOW);
}

PjvidWidget::~PjvidWidget() {
    qDebug() << ">>>>>>>>>>>>>>~PjvidWidget";
}

void PjvidWidget::resizeEvent(QResizeEvent *e) {
    QWidget::resizeEvent(e);

    qint32 w = width();
    qint32 h = height();

    if (true || _originSize.isEmpty()) {
        MoveWindow(_vidWindow, 0, 0, w, h, true);

    } else {

    }


}




