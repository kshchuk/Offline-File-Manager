#pragma once

#include <QMenu>
#include <QAction>
#include "ContextMenuPresenter.h"


namespace manager {
class ContextMenuView : public QMenu
{
    Q_OBJECT

public:
    explicit ContextMenuView(ContextMenuPresenter* presenter, QWidget* parent) : QMenu(parent), presenter(presenter)
    {
        this->addAction("Open", this, &ContextMenuView::open);
        this->addAction("Open in file explorer", this, &ContextMenuView::openInExplorer);
        this->addAction("Insert data", this, &ContextMenuView::addData);
        this->addAction("Remove", this, &ContextMenuView::remove);
        this->addAction("Rename", this, &ContextMenuView::rename);
        this->addAction("Properties", this, &ContextMenuView::properties);
    };
    virtual ~ContextMenuView() {};

private slots:
    inline void open() {
        presenter->open();
    };
    inline void openInExplorer(){
        presenter->openInExplorer();
    };
    inline void addData(){
        presenter->addData();
    };
    inline void remove(){
        presenter->remove();
    };
    inline void rename(){
        presenter->rename();
    };
    inline void properties(){
        presenter->properties();
    };

private:
    ContextMenuPresenter* presenter;
};
}
