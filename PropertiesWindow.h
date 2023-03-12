#include <QTableWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QDialog>

#include "PropertiesLogic.h"

namespace manager {

class PropertiesWindow : public QDialog
{
    Q_OBJECT

    enum PropertiesOrder
    {
        NAME,
        FULL_PATH,
        TYPE,
        SIZE,
        ICON_NAME,
        GROUP,
        OWNER,
        OWNER_ID,
        DATE_CREATED,
        DATE_MODIDFIED,
        HASH,
        CUSTOM_METHADATA,
    };

public:
    PropertiesWindow(PropertiesLogic* logic, QWidget* parent = nullptr);

    inline size_t getRowsNumber() { return rowsNumber; }

private slots:
    void save();

private:
    PropertiesLogic* logic;

    QVBoxLayout layout;
    QTableWidget table;
    QPushButton buttonOk;
    QPushButton buttonCancel;
    QPushButton buttonSave;

    size_t rowsNumber = 12;
};

}
