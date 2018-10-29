//
//  pjvidwidget.h
//  pjsip demo
//
//  Created by Jimmy on 10/26/18.
//
//
#ifndef PJVIDWIDGET_H
#define PJVIDWIDGET_H

#include <QtCore>
#include <QWidget>


class PjvidWidget : public QWidget {
    Q_OBJECT
public:
    explicit PjvidWidget(void *vidWindow, QWidget *parent = nullptr);
    ~PjvidWidget();

    void init();

protected:
    virtual void resizeEvent(QResizeEvent *) Q_DECL_OVERRIDE;

private:
    void  *_vidWindow;

    QSize _originSize;
};

#endif