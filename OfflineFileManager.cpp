#include "OfflineFileManager.h"
#include <QCommandLineParser>
#include <QFileIconProvider>
#include <QFileSystemModel>
#include <QDesktopServices>
#include <QTableWidgetItem>
#include <QMessageBox>
#include <QFileDialog>
#include <QScreen>
#include <Qscroller>
#include <QStringList>
#include <QVBoxLayout>
#include <QPushButton>
#include <QThread>
#include <QUrl>
#include <QMenu>
#include <QSpinBox>

#include "PropertiesWindow.h"
#include "PropertiesLogic.h"
#include "AddDataToFolder.h"
#include "SearchWindow.h"



OfflineFileManager::OfflineFileManager(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    
    model = new QFileInfoModel();

    saveMessage.setText("Save data");
    saveMessage.setInformativeText("You have unsaved changes. Do you want to save it?");
    saveMessage.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    saveMessage.setDefaultButton(QMessageBox::Yes);

    connect(ui.actionClose, &QAction::triggered, this, &OfflineFileManager::close);
    connect(ui.updateButton, &QToolButton::triggered, this, &OfflineFileManager::on_updateButton_clicked);
    connect(ui.upButton, &QToolButton::triggered, this, &OfflineFileManager::on_upButton_clicked);
    connect(ui.homeButton, &QToolButton::clicked, this, &OfflineFileManager::on_homeButton_clicked);
    connect(ui.actionSave, &QAction::triggered, this, &OfflineFileManager::on_saveAction_triggered);
    connect(ui.actionOpen, &QAction::triggered, this, &OfflineFileManager::on_openAction_triggered);
    connect(ui.addFolderButton, &QToolButton::triggered, this, &OfflineFileManager::on_addFolderButton_clicked);
    connect(ui.fileSystemTree, &QTreeView::doubleClicked, model, &QFileInfoModel::fetchMore);
    connect(ui.addressLine, &QLineEdit::editingFinished, this, &OfflineFileManager::on_editLine_editingFinished);
    connect(ui.fileSystemTree, &QTreeView::activated, this, &OfflineFileManager::on_treeWidget_clicked);
    connect(ui.fileSystemTree, &QTreeView::customContextMenuRequested, this, &OfflineFileManager::on_customContextMenu);
    connect(ui.fileSystemTree, &QTreeView::clicked, this, &OfflineFileManager::on_treeWidget_clicked);
    connect(ui.actionAll_drives, &QAction::triggered, this, &OfflineFileManager::setAllDrivesregime);
    connect(ui.actionExternal_drives, &QAction::triggered, this, &OfflineFileManager::setExternalDrivesregime);
    connect(ui.actionMaximum_depth, &QAction::triggered, this, &OfflineFileManager::setMaxDepth);
    connect(ui.searchButton, &QToolButton::clicked, this, &OfflineFileManager::search);
    

    treeViewInit(ui.fileSystemTree, model);
    on_homeButton_clicked();
}

OfflineFileManager::~OfflineFileManager()
{
    if (model->rowCount() != 0) {
        int ret = saveMessage.exec();
        switch (ret) {
        case QMessageBox::Yes:
            on_saveAction_triggered();
            break;
        case QMessageBox::No:
            break;
        case QMessageBox::Close:
            break;
        default:
            Q_ASSERT(1 == 0);
        }
    }
    delete model;
}

void OfflineFileManager::on_treeWidget_clicked(QModelIndex index)
{
    QList<QString> path = model->getPath(index);
    QString spath;
    foreach(auto file, path) { 
        file.removeIf([](QChar c) {return c == QChar('/') || c == QChar('\\'); });
        spath += '/' + file; 
    }

    ui.addressLine->blockSignals(true);
    ui.addressLine->setText(spath);
    ui.addressLine->blockSignals(false);
}

void OfflineFileManager::action_openInFileExplorer()
{
    QModelIndex index = ui.fileSystemTree->currentIndex();

    if (model->isLink(index)) {
        QString path = model->getPathfFromInfo(index);
        index = model->byPath(path);
    }

    QString path = model->getPathfFromInfo(index);
    if (!ui.fileSystemTree->model()->hasChildren(index))
    {
        int i = path.length() - 1, j = 0;
        while (i >= 0 && path[i] != QChar('/') && path[i] != QChar('\\')) { j++; i--; }
        path.remove(i, j+1);
    }

    if (path.isEmpty())
        QDesktopServices::openUrl(QUrl::fromLocalFile("/"));
    else {
        QFileInfo info(path);
        if (info.exists())
            QDesktopServices::openUrl(QUrl::fromLocalFile(path));
        else
            QMessageBox::critical(this, tr("Offline File Manager"),
                tr("No access"), QMessageBox::Close);
    }
}

void OfflineFileManager::action_openFile()
{
    QModelIndex index = ui.fileSystemTree->currentIndex();
    
    if (model->isLink(index)) {
        QString path = model->getPathfFromInfo(index);
        index = model->byPath(path);
        QString d = index.data().toString();
    }

    QFileInfo info(index.data().toString());
    if (ui.fileSystemTree->model()->hasChildren(index))
    {
        ui.fileSystemTree->expand(index);
        ui.fileSystemTree->setCurrentIndex(index);
    }
    else
    {
        QString spath = model->getPathfFromInfo(index);
        QFileInfo info(spath);
        if (info.exists())
            QDesktopServices::openUrl(QUrl::fromLocalFile(spath));
        else
            QMessageBox::critical(this, tr("Offline File Manager"),
                tr("No access"), QMessageBox::Close);
    }
}

void OfflineFileManager::action_Properties()
{
    QModelIndex index = ui.fileSystemTree->currentIndex();

    QDialog* widget = new QDialog(this);
    QVBoxLayout* layout = new QVBoxLayout(widget);
    widget->setWindowTitle("Properties");
    widget->setFixedHeight(550);
    widget->setFixedWidth(400);
    PropertiesWindow* properties_window = new PropertiesWindow(widget, index, model);
    QPushButton* buttonOk = new QPushButton(widget);
    QPushButton* buttonCancel = new QPushButton(widget);
    QPushButton* buttonSave = new QPushButton(widget);
    buttonCancel->setText("Cancel");
    buttonOk->setText("OK");
    buttonSave->setText("Save");

    connect(buttonCancel, &QPushButton::clicked, widget, &QWidget::close);
    connect(buttonOk, &QPushButton::clicked, widget, &QWidget::close);
    connect(buttonOk, &QPushButton::clicked, properties_window, &PropertiesWindow::saveTextSlot);
    connect(buttonSave, &QPushButton::clicked, properties_window, &PropertiesWindow::saveTextSlot);
    connect(properties_window, &PropertiesWindow::saveTextSignal, this, &OfflineFileManager::saveMeta);

    layout->addWidget(properties_window, 0);
    layout->addWidget(buttonCancel);
    layout->addWidget(buttonSave);
    layout->addWidget(buttonOk);
    widget->setLayout(layout);
    widget->show();

}

void OfflineFileManager::action_addDataToVirtualFolder()
{
    AddDataToFolder* dialog = new AddDataToFolder(this, model);
    connect(dialog, &AddDataToFolder::infoSent, this, &OfflineFileManager::addDataToVirtualFolder);
    connect(dialog, &AddDataToFolder::finished, dialog, &AddDataToFolder::close);
    dialog->exec();
}

void OfflineFileManager::on_customContextMenu(const QPoint& point)
{
    QPoint globalPos = ui.fileSystemTree->mapToGlobal(point);

    QMenu menu(this);
    menu.addAction("Open",
        this, &OfflineFileManager::action_openFile);
    menu.addAction("Open in file explorer",
        this, &OfflineFileManager::action_openInFileExplorer);
    
    QModelIndex index = ui.fileSystemTree->currentIndex();
    if (!index.siblingAtColumn((int)ColunmsOrder::TYPE).data().toString().compare(virtualFolderType))
        menu.addAction("Add data",
            this, &OfflineFileManager::action_addDataToVirtualFolder);

    menu.addAction("Remove",
        this, &OfflineFileManager::removeElement);
    menu.addAction("Rename",
        this, &OfflineFileManager::editFileNameA);
    menu.addAction("Properties",
        this, &OfflineFileManager::action_Properties);

    menu.exec(globalPos);
}

void OfflineFileManager::on_updateButton_clicked()
{
    if (model->rowCount() != 0) {
        int ret = saveMessage.exec();
        switch (ret) {
        case QMessageBox::Yes:
            on_saveAction_triggered();
            break;
        case QMessageBox::No:
            break;
        case QMessageBox::Close:
            break;
        default:
            Q_ASSERT(1 == 0);
        }
    }

    switch (regime)
    {
    case OfflineFileManager::FILESYSTEM:
    {
        treeViewInit(ui.fileSystemTree, model);

        this->model->genStaticSystemModel(maxDepth);
        treeViewInit(ui.fileSystemTree, model);
        break;
    }
    case OfflineFileManager::EXTERNAL_DRIVES:
    {
        this->model->genExternalDrivesModel(maxDepth);
        // treeViewInit(ui.fileSystemTree, model->readFile("tmp.bin"));
        treeViewInit(ui.fileSystemTree, model);
        break;
    }
    default:
        break;
    }
    on_homeButton_clicked();
}

void OfflineFileManager::on_saveAction_triggered() 
{
    QString fileName = QFileDialog::getSaveFileName(this, 
        ("Save File"),"", ("File system hierarchy (*.fsh)"));

    try {
        model->writeFile(fileName, maxDepth);
    }
    catch (const std::exception& e) {
        QMessageBox::critical(this, tr("Offline File Manager"),
            tr(e.what()), QMessageBox::Close);
    }
}

void OfflineFileManager::on_openAction_triggered()
{
    if (model->rowCount() != 0) {
        int ret = saveMessage.exec();
        switch (ret) {
        case QMessageBox::Yes:
            on_saveAction_triggered();
            break;
        case QMessageBox::No:
            break;
        case QMessageBox::Close:
            break;
        default:
            Q_ASSERT(1 == 0);
        }
    }

    QString fileName = QFileDialog::getOpenFileName(this, ("Open File"),
        "", ("File system hierarchy (*.fsh)"));
    
    try {
        model->readFile(fileName);
    }
    catch(const std::exception &e) {
        QMessageBox::critical(this, tr("Offline File Manager"),
            tr(e.what()), QMessageBox::Close);
    }
    treeViewInit(ui.fileSystemTree, model);
}

void OfflineFileManager::on_homeButton_clicked()
{
    ui.addressLine->setText("");
    ui.fileSystemTree->setCurrentIndex(QModelIndex());
    ui.fileSystemTree->collapseAll();
}

void OfflineFileManager::on_editLine_editingFinished()
{
    QString path = ui.addressLine->text();
    QModelIndex index = model->byPath(path);
    
    if (index.isValid()) {
        ui.fileSystemTree->expand(index);
        ui.fileSystemTree->setCurrentIndex(index);
    }
}

void OfflineFileManager::on_upButton_clicked()
{
    QString path = ui.addressLine->text();
    QModelIndex index = model->byPath(path);
    
    ui.fileSystemTree->collapseAll();
    ui.fileSystemTree->setCurrentIndex(index.parent());

    QStringList newPath = model->getPath(index.parent());
    QString spath;
    foreach(auto file, newPath) { 
        file.removeIf([](QChar c) {return c == QChar('/') || c == QChar('\\'); });
        spath += '/' + file; };

    ui.addressLine->blockSignals(true);
    ui.addressLine->setText(spath);
    ui.addressLine->blockSignals(false);
}

void OfflineFileManager::on_addFolderButton_clicked()
{

    QModelIndex cur = ui.fileSystemTree->currentIndex();

    if (model->isFolder(cur) || cur == QModelIndex())
    {
        QString name = "New folder";
        QModelIndex appended = model->appendFolder(QFileInfo(name), cur);
        ui.fileSystemTree->expand(appended.parent());
        editFileName(appended);
    treeViewInit(ui.fileSystemTree, model);
    }
    else QMessageBox::warning(this, "Adding error",
        "Unable to insert a folder into a non-folder",
        QMessageBox::Close);
}

void OfflineFileManager::saveMeta(const QString& str, const QModelIndex& index)
{
    QModelIndex newindex = index.siblingAtColumn((int)ColunmsOrder::CUSTOM_METHADATA);
    QStandardItem* item = model->itemFromIndex(newindex);
    item->setData(str, 0);
}

void OfflineFileManager::editFileName(const QModelIndex& index)
{
    ui.fileSystemTree->edit(index);
}

void OfflineFileManager::editFileNameA()
{
    editFileName(ui.fileSystemTree->currentIndex());
}

void OfflineFileManager::addDataToVirtualFolder(QModelIndexList indexes, QStringList paths)
{
    QModelIndex cur = ui.fileSystemTree->currentIndex();

    foreach(auto &ind, indexes)
        model->insertFileLinkToTheFolder(ind, cur);

    foreach(auto &path, paths)
        model->insertFileToTheFolder(path, cur);
}

void OfflineFileManager::removeElement()
{
    QModelIndex index = ui.fileSystemTree->currentIndex();
    QMessageBox mes(this);
    mes.setText("Delete file");
    mes.setInformativeText("Are you sure you want to delete this record?");
    mes.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
    mes.setDefaultButton(QMessageBox::Cancel);

    int ret = mes.exec();
    switch (ret) {
    case QMessageBox::Yes:
        model->deleteFile(index);
        break;
    case QMessageBox::Cancel:
        break;
    case QMessageBox::Close:
        break;
    default:
        Q_ASSERT(1 == 0);
    }
}

void OfflineFileManager::errorString(QString e)
{
    this->error = e;
    QMessageBox::critical(this, tr("Error"),
        e, QMessageBox::Close);
}

void OfflineFileManager::setExternalDrivesregime()
{
    regime = Regime::EXTERNAL_DRIVES;
}

void OfflineFileManager::setAllDrivesregime()
{
    regime = Regime::FILESYSTEM;
}

void OfflineFileManager::setMaxDepth()
{
    QDialog* dialog = new QDialog(this);
    QVBoxLayout* layout = new QVBoxLayout(dialog);
    QLabel* label = new QLabel(dialog);
    label->setText("Maximum depth: ");
    QSpinBox* box = new QSpinBox(dialog);
    dialog->setWindowTitle("Set maximum depth");
    QPushButton* buttonOk = new QPushButton(dialog);
    QPushButton* buttonCancel = new QPushButton(dialog);
    buttonCancel->setText("Cancel");
    buttonOk->setText("OK");

    connect(buttonCancel, &QPushButton::clicked, dialog, &QWidget::close);
    connect(box, &QSpinBox::valueChanged, this, &OfflineFileManager::setTempValue);
    connect(buttonOk, &QPushButton::clicked, dialog, &QWidget::close);
    connect(buttonOk, &QPushButton::clicked, 
        this, &OfflineFileManager::setMaxDepthFromTempValue);

    layout->addWidget(label, 0);
    layout->addWidget(box);
    layout->addWidget(buttonCancel);
    layout->addWidget(buttonOk);
    dialog->setLayout(layout);
    dialog->exec();

    delete dialog;
}

void OfflineFileManager::setTempValue(int i)
{
    tempValue = i;
}

void OfflineFileManager::setMaxDepthFromTempValue()
{
    maxDepth = tempValue;
}

void OfflineFileManager::highlightElem(QModelIndex index)
{
    ui.fileSystemTree->setCurrentIndex(index);
    ui.fileSystemTree->expand(index);
}

void OfflineFileManager::search()
{
    QModelIndex cur = ui.fileSystemTree->currentIndex();
    SearchWindow* window = new SearchWindow(model, model->pathFromStringList(model->getPath(cur)), this);
    connect(window, &SearchWindow::infoSent, this, &OfflineFileManager::highlightElem);
    window->show();
}

void OfflineFileManager::treeViewInit(QTreeView* tree, QFileInfoModel* model1)
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
