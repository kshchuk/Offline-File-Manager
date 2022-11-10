#include "MyExplorer.h"
#include <qscreen.h>
#include <QScroller>


MyExplorer::MyExplorer(QWidget* parent, QFileInfoModel* model)
	: QTreeView(parent)
{
	treeViewInit(this, model);
}

MyExplorer::~MyExplorer()
{}

void MyExplorer::sendPathOfTheSelectedFile()
{
	emit pathSent(this->currentIndex().data((int)ColunmsOrder::FULL_PATH).toString());
}

void MyExplorer::treeViewInit(QTreeView* tree, QFileInfoModel* model1)
{
    tree->setModel(model1);

    for (size_t i = 4; i < tree->model()->columnCount(); i++)
        tree->hideColumn(i); // only 4 columns need to be displayed

    tree->setAnimated(false);
    tree->setIndentation(20);
    tree->setSortingEnabled(true);
    const QSize availableSize = tree->screen()->availableGeometry().size();
    tree->setColumnWidth(0, tree->width() / 2);
    QScroller::grabGesture(tree, QScroller::TouchGesture);
    tree->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tree->setContextMenuPolicy(Qt::CustomContextMenu);
}
