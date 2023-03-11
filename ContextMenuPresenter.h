#pragma once

#include "QFileInfoModel.h"
#include "OfflineFileManagerView.h"
#include "FileInteractor.h"


namespace manager {

    class ContextMenuView;

    class ContextMenuPresenter
    {
    public:
        explicit ContextMenuPresenter(ContextMenuView* view, QFileInfoModel* model, OfflineFileManagerView* managerView);
        explicit ContextMenuPresenter(QFileInfoModel* model, OfflineFileManagerView* managerView);

        void setView(ContextMenuView* view);
        void open();
        void openInExplorer();
        void addData();
        void remove();
        void rename();
        void properties();

    private:
        ContextMenuView* view{ nullptr };
        QFileInfoModel* model{ nullptr };
        OfflineFileManagerView* managerView{ nullptr };
        QModelIndex index;
        FileInteractor interactor;
    };

} // namespace manager
