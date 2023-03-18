#pragma once

#include <QtGui>
#include <QStandardItemModel>

namespace manager
{
    /**
     * @brief The BasicTraits struct defines a set of traits that can be used by a TableView.
     * This struct defines a set of item functions, header functions, and configuration functions
     * that can be used by a TableView to display and manipulate data. The traits defined in this
     * struct are intended to be used with QAbstractItemModel-based models.
     */
    struct BasicTraits
    {
        /*
         * @brief Constructs a BasicTraits object.
         */
        BasicTraits() = default;

        /**
         * @brief Destroys the BasicTraits object.
         */
        ~BasicTraits() = default;

        // Type Definitions
        using Model = QAbstractItemModel;  /**< The model type */
        using ModelConfig = bool;          /**< The model configuration type */
        using Roles = QMap<int, QVariant>; /**< The role map type */
        using HeaderRoles = Roles;         /**< The header role map type */

        // Item Functions
        /**
         * @brief Returns the role map for the item at the given index in the given model.
         * @param model The model.
         * @param index The index of the item.
         * @return The role map for the item.
         */
        Roles itemData(const Model *model, const QModelIndex &index) const
        {
            return model->itemData(index);
        }

        /**
         * @brief Sets the role map for the item at the given index in the given model.
         * @param model The model.
         * @param index The index of the item.
         * @param roles The role map to set.
         * @return True if successful, false otherwise.
         */
        bool setItemData(Model *model, const QModelIndex &index, const Roles &roles)
        {
            return model->setItemData(index, roles);
        }

        // Header Functions
        /**
         * @brief Returns the header role map for the given section and orientation in the given model.
         * @param model The model.
         * @param section The section.
         * @param orientation The orientation.
         * @return The header role map.
         */
        HeaderRoles headerData(const Model *model, int section, Qt::Orientation orientation) const
        {
            return {{Qt::DisplayRole, model->headerData(section, orientation)}};
        }

        /**
         * @brief Sets the header role map for the given section and orientation in the given model.
         * @param model The model.
         * @param section The section.
         * @param orientation The orientation.
         * @param roles The header role map to set.
         * @return True if successful, false otherwise.
         */
        bool setHeaderData(Model *model, int section, Qt::Orientation orientation, const HeaderRoles &roles)
        {
            return model->setHeaderData(section, orientation, roles.value(Qt::DisplayRole));
        }

        // Child Functions
        /**
         * @brief Sets the number of rows and columns for the child items of the item at the given index in the given model.
         * @param model The model.
         * @param parent The index of the parent item.
         * @param rows The number of rows to set.
         * @param columns The number of columns to set.
         * @return True if successful, false otherwise.
         */
        bool setRowsColumns(Model *model, const QModelIndex &parent, int rows, int columns) const
        {
            bool rc = model->insertRows(0, rows, parent);
            rc = rc && model->insertColumns(0, columns, parent);
            return rc;
        }

        // Configuration Functions
        /**
         * @brief Determines whether horizontal header data should be displayed.
         * @return true if horizontal header data should be displayed, false otherwise.
         */
        bool doHorizontalHeaderData() const
        {
            return true;
        };

        /**
        * @brief Determines whether vertical header data should be displayed.
        * @return true if vertical header data should be displayed, false otherwise.
        */
        bool doVerticalHeaderData() const
        {
            return false;
        };

        /**
        * @brief Gets the configuration of the model.
        * @param model Pointer to the model object.
        * @return The configuration of the model.
        */
        ModelConfig modelConfig(const Model *model) const
        {
            return true;
        };

        /**
        * @brief Sets the configuration of the model.
        * @param model Pointer to the model object.
        * @param config The new configuration to set.
        * @return true if the configuration was set successfully, false otherwise.
        */
        bool setModelConfig(Model *model, const ModelConfig &config) const
        {
            return true;
        };
    };

    /**
     * @brief The Status struct represents the status of a data stream model.
     */
    struct Status
    {
        /**
         * @brief The SubStatus enum represents the substatuses of the Status object.
         */
        enum SubStatus
        {
            StreamOk = 1,
            ModelOk = 2,
            IgnoreModelFailures = 4
        };

        /**
         * @brief QFlags<SubStatus> is a type-safe way of storing OR-combined enum values.
         */
        QFlags<SubStatus> flags;

        /**
         * @brief Constructs a Status object with the given substatus.
         * @param s The substatus to be set.
         */
        Status(SubStatus s) : flags(StreamOk | ModelOk | s) {}

        /**
         * @brief Constructs a Status object with default substatuses.
         */
        Status() : flags(StreamOk | ModelOk) {}

        /**
         * @brief Checks whether the Status object is in an "ok" state.
         * @return true if the Status object is in an "ok" state, false otherwise.
         */
        bool ok() const
        {
            return (flags & StreamOk && (flags & IgnoreModelFailures || flags & ModelOk));
        }

        /**
         * @brief Checks whether the given QDataStream object is in an "ok" state.
         * @param str The QDataStream object to be checked.
         * @return true if the QDataStream object is in an "ok" state, false otherwise.
         */
        bool operator()(QDataStream &str)
        {
            return stream(str.status() == QDataStream::Ok);
        }

        /**
         * @brief Checks whether the given Status object is in an "ok" state.
         * @param s The Status object to be checked.
         * @return true if the Status object is in an "ok" state, false otherwise.
         */
        bool operator()(Status s)
        {
            if (flags & StreamOk && !(s.flags & StreamOk))
                flags ^= StreamOk;
            if (flags & ModelOk && !(s.flags & ModelOk))
                flags ^= ModelOk;
            return ok();
        }

        /**
         * @brief Sets the model substatus to the given value and checks whether the Status object is in an "ok" state.
         * @param s The value to set the model substatus to.
         * @return true if the Status object is in an "ok" state, false otherwise.
         */
        bool model(bool s)
        {
            if (flags & ModelOk && !s)
                flags ^= ModelOk;
            return ok();
        }

        /**
         * @brief Sets the stream substatus to the given value and checks whether the Status object is in an "ok" state.
         * @param s The value to set the stream substatus to.
         * @return true if the Status object is in an "ok" state, false otherwise.
         */
        bool stream(bool s)
        {
            if (flags & StreamOk && !s)
                flags ^= StreamOk;
            return ok();
        }
    };
}
