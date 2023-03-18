#pragma once

#include <QDesktopServices>
#include <QUrl>

#include "QFileInfoModel.h"


namespace manager {

class FileInteractor
{
public:
    bool open(FileInfoModelHelper* helper, QModelIndex index)
    {
        if (helper->isLink(index))
        {
            QString path = helper->getPathFromInfo(index);
            index = helper->byPath(path);
        }

        if (!helper->isFolder(index))
        {
            QString spath = helper->getPathFromInfo(index);
            QFileInfo info(spath);
            if (info.exists())
                QDesktopServices::openUrl(QUrl::fromLocalFile(spath));
            else
                return false;
        }
        return true;
    };
    bool openInExplorer(FileInfoModelHelper* helper, QModelIndex index)
    {
        if (helper->isLink(index))
        {
            QString path = helper->getPathFromInfo(index);
            index = helper->byPath(path);
        }

        QString path = helper->getPathFromInfo(index);
        if (!helper->isFolder(index))
        {
            int i = path.length() - 1, j = 0;
            while (i >= 0 && path[i] != QChar('/') && path[i] != QChar('\\'))
            {
                j++;
                i--;
            }
            path.remove(i, j + 1);
        }

        if (path.isEmpty())
            QDesktopServices::openUrl(QUrl::fromLocalFile("/"));
        else
        {
            QFileInfo info(path);
            if (info.exists())
                QDesktopServices::openUrl(QUrl::fromLocalFile(path));
            else
                return false;
        }
        return true;
    };
};
}
