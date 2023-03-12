#pragma once

#include "Searcher.h"
#include "SearchWindow.h"
#include "QFileInfoModel.h"


namespace manager 
{

class SearchController : public QObject
{

    Q_OBJECT    

public:
    SearchController(Searcher* searcher, SearchWindow* view);
    
    inline void setPath(const QString& path) {
        this->path = path;
    }

public slots:
    void search();
    void AppendRecordInTheList(Record*) {
        // TODO
    };

private:
    Searcher* searcher;
    SearchWindow* view;

    QString path;
    QList<const Record*> foundRecords;
};

}