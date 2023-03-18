#pragma once

#include "QFileInfoModel.h"
#include "OfflineFileManagerView.h"


namespace manager 
{
    static constexpr size_t defaultDepth = 5;


    class OfflineFileManagerController : public QObject
    {
        Q_OBJECT

        enum class Mode { FILESYSTEM, EXTERNAL_DRIVES, GOOGLE_DRIVE };

    public:
        explicit OfflineFileManagerController(QFileInfoModel* model, OfflineFileManagerView* view, QObject* parent = nullptr);

    signals:
        void save(const QString&);
        void load(const QString&,size_t);

    public slots:
        void saveModelToFile();
        void loadModelFromFile();
        void update();
        void goUpper();
        void addFolder();
        void setMaximumReadingDepth();
        void search();
        void openRobocopyTool() {
            // TODO
        };

        void goHome() {
            view->hideAllItems();
        };
        void setExternalDrivesMode() {
            mode = Mode::EXTERNAL_DRIVES;
            delete model; model = new ExternalDrivesModel();
        };
        void setAllDrivesMode() {
            mode = Mode::FILESYSTEM;
            delete model; model = new AllDrivesModel();
        };
        void setGoogleDriveMode() {
            mode = Mode::GOOGLE_DRIVE;
            delete model; model = new GoogleDriveModel();
        };

        void openCustomContextMenu(const QPoint& point);
        void editLineChanged() {
            auto path = view->getEditLineText();
            QModelIndex index = model->byPath(path);

            if (index.isValid())
            {
                view->expandFolder(index);
                view->highlightItem(index);
            }
        };

        void alertUser();

    public:
        size_t maxReadingDepth = defaultDepth;

    private:
        QFileInfoModel* model = nullptr;
        OfflineFileManagerView* view = nullptr;

        Mode mode = Mode::EXTERNAL_DRIVES;
    };



    /*class PropertiesPresenter : public QObject
    {
        Q_OBJECT

    public:
        explicit PropertiesPresenter(PropertiesLogic* logic, PropertiesWindow* window, QObject* parent = nullptr);

    private slots:
        void onSaveButtonClicked();
        void onCloseButtonClicked();
        void onOkButtonClicked();

    private:
        PropertiesLogic* logic;
        PropertiesWindow* window;
    };*/

}
