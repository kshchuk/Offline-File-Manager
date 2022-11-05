#pragma once

#include <QDialog>
#include "ui_SearchWindow.h"

class SearchWindow : public QDialog
{
	Q_OBJECT

public:
	SearchWindow(QWidget *parent = nullptr);
	~SearchWindow();

private:
	Ui::SearchWindowClass ui;
};
