#include "ContextMenuPresenter.h"
#include "ContextMenuView.h"

#include "PropertiesLogic.h"
#include "PropertiesWindow.h"

namespace manager {

    ContextMenuPresenter::ContextMenuPresenter(ContextMenuView* view, QFileInfoModel* model, OfflineFileManagerView* managerView)
        : view(view), model(model), managerView(managerView)
    {
        index = managerView->getCurrentIndex();
    }

    ContextMenuPresenter::ContextMenuPresenter(QFileInfoModel* model, OfflineFileManagerView* managerView)
        : model(model), managerView(managerView)
    {
        index = managerView->getCurrentIndex();
    }

    void ContextMenuPresenter::setView(ContextMenuView* view)
    {
        this->view = view;
    }

    void ContextMenuPresenter::open()
    {
        if (!interactor.open(model, index))
            QMessageBox::critical(managerView, "Offline File Manager", "No access", QMessageBox::Close);
    }

    void ContextMenuPresenter::openInExplorer()
    {
        if (!interactor.openInExplorer(model, index))
            QMessageBox::critical(managerView, "Offline File Manager", "No access", QMessageBox::Close);
    }

    void ContextMenuPresenter::addData()
    {
        // TODO
    }

    void ContextMenuPresenter::remove()
    {
        QMessageBox mes(managerView);
        mes.setText("Delete file");
        mes.setInformativeText("Are you sure you want to delete this record?");
        mes.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
        mes.setDefaultButton(QMessageBox::Cancel);

        int ret = mes.exec();
        switch (ret)
        {
        case QMessageBox::Yes:
            model->deleteFile(index);
            break;
        case QMessageBox::Cancel:
            break;
        case QMessageBox::Close:
            break;
        default:
            break;
        }
    }

    void ContextMenuPresenter::rename()
    {
        managerView->editItemName(managerView->getCurrentIndex());
    }

    void ContextMenuPresenter::properties()
    {
        PropertiesLogic logic(dynamic_cast<Record*>(model->itemFromIndex(index)), model);
        PropertiesWindow window(&logic);
        window.show();
    }

} // namespace manager
