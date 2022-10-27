#include "QModelLoader.h"

#include <cassert>

#include <QFile>
#include <QFileSystemModel>



void QModelLoader::readModel(QString fileName, QList<QString> headers)
{
	QJsonModel* jmodel = new QJsonModel;
	for (size_t i = 0; i < headers.size(); i++)
	{
		jmodel->setHeaderData(i, Qt::Horizontal, headers[i]);
	}

	jmodel->load(fileName);
	model = jmodel;
}

void QModelLoader::writeModel(QString fileName)
{
	QFile* file = new QFile(fileName);
	file->open(QIODeviceBase::WriteOnly);

	QFileSystemModel* fmodel = dynamic_cast<QFileSystemModel*>(model);
	if (fmodel != nullptr)
	{
		std::string json = this->convertRecursively();
		file->write(QByteArray::fromStdString(json));
	}
	else
	{
		QJsonModel* jmodel = dynamic_cast<QJsonModel*>(model);

		assert(jmodel != nullptr);

		file->write(jmodel->json());
	}

	file->close();
	delete file;
}

void QModelLoader::makeFileSystemModel(QString root, int maxLoadedDepth)
{
	QFileSystemModel* fmodel = new QFileSystemModel;
	fmodel->setRootPath(root);

	model = fmodel;

	//QObject::connect(fmodel, &QFileSystemModel::directoryLoaded,
	//	this, &QModelLoader::loadRecursively);

	// loadRecursively(); // force load file system model into ram
}



QAbstractItemModel* QModelLoader::getModel()
{
	QAbstractItemModel* res = model;
	model = nullptr;
	return res;
}

std::string QModelLoader::convertRecursively(QModelIndex parent)
{
	std::string str;
	for (int r = 0; r < model->rowCount(parent); ++r) 
	{
		QModelIndex index = model->index(r, 0, parent);
		QVariant name = model->data(index);
		qDebug() << name;

		str =  name.toString().toStdString() + " | ";

		model->fetchMore(index);

		if (model->hasChildren(index)) {
			convertRecursively(index);
		}
	}
		return str;
}

std::string QModelLoader::toJson(const std::string& str)
{
	std::string json;
		return json;
}

void QModelLoader::loadRecursively(int currentDepth, int maxDepth, QModelIndex parent)
{
	for (int r = 0; r < model->rowCount(parent); ++r)
	{
		QModelIndex index = model->index(r, 0, parent);

		if (model->canFetchMore(index) && currentDepth < maxDepth) {
			model->fetchMore(index);
			loadRecursively(currentDepth + 1, maxDepth, index);
		}

		//if (model->hasChildren(index) && currentDepth < maxDepth) {
		//	loadRecursively(currentDepth + 1, maxDepth, index);
		//}
	}
}

