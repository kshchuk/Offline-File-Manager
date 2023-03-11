#pragma once

#include <QtGui>
#include <QStandardItemModel>


namespace manager {


struct BasicTraits {
    BasicTraits() {}
    /// The base model that the serializer operates on
    typedef QAbstractItemModel Model;
    /// The streamable representation of model's configuration
    typedef bool ModelConfig;
    /// The streamable representation of an item's data
    typedef QMap<int, QVariant> Roles;
    /// The streamable representation of a section of model's header data
    typedef Roles HeaderRoles;
    /// Returns a streamable representation of an item's data.
    Roles itemData(const Model* model, const QModelIndex& index) {
        return model->itemData(index);
    }
    /// Sets the item's data from the streamable representation.
    bool setItemData(Model* model, const QModelIndex& index, const Roles& data) {
        return model->setItemData(index, data);
    }
    /// Returns a streamable representation of a model's header data.
    HeaderRoles headerData(const Model* model, int section, Qt::Orientation ori) {
        Roles data;
        data.insert(Qt::DisplayRole, model->headerData(section, ori));
        return data;
    }
    /// Sets the model's header data from the streamable representation.
    bool setHeaderData(Model* model, int section, Qt::Orientation ori, const HeaderRoles& data) {
        return model->setHeaderData(section, ori, data.value(Qt::DisplayRole));
    }
    /// Should horizontal header data be serialized?
    bool doHorizontalHeaderData() const { return true; }
    /// Should vertical header data be serialized?
    bool doVerticalHeaderData() const { return false; }
    /// Sets the number of rows and columns for children on a given parent item.
    bool setRowsColumns(Model* model, const QModelIndex& parent, int rows, int columns) {
        bool rc = model->insertRows(0, rows, parent);
        rc = rc && model->insertColumns(0, columns, parent);
        return rc;
    }
    /// Returns a streamable representation of the model's configuration.
    ModelConfig modelConfig(const Model*) {
        return true;
    }
    /// Sets the model's configuration from the streamable representation.
    bool setModelConfig(Model*, const ModelConfig&) {
        return true;
    }
};


struct Status {
    enum SubStatus { StreamOk = 1, ModelOk = 2, IgnoreModelFailures = 4 };
    QFlags<SubStatus> flags;
    Status(SubStatus s) : flags(StreamOk | ModelOk | s) {}
    Status() : flags(StreamOk | ModelOk) {}
    bool ok() const {
        return (flags & StreamOk && (flags & IgnoreModelFailures || flags & ModelOk));
    }
    bool operator()(QDataStream& str) {
        return stream(str.status() == QDataStream::Ok);
    }
    bool operator()(Status s) {
        if (flags & StreamOk && !(s.flags & StreamOk)) flags ^= StreamOk;
        if (flags & ModelOk && !(s.flags & ModelOk)) flags ^= ModelOk;
        return ok();
    }
    bool model(bool s) {
        if (flags & ModelOk && !s) flags ^= ModelOk;
        return ok();
    }
    bool stream(bool s) {
        if (flags & StreamOk && !s) flags ^= StreamOk;
        return ok();
    }
};

}
