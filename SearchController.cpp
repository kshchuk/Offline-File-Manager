#include "SearchController.h"


manager::SearchController::SearchController(Searcher *searcher, SearchWindow *view)
    : searcher(searcher), view(view)
{
    connect(searcher, &Searcher::found, view, &SearchWindow::AppendRecordInTheList);
}

void manager::SearchController::search() 
{
	this->foundRecords.clear();
	if (model->rowCount() > 0)
	{
	
	QVariant* criterias = new QVariant[searcher->criteriasCount + 1];
	
	if (!view->getNameLine().isEmpty())
		criterias[searcher->NAME] = ui.nameLine->text();
	if (!view->getTypeLine().isEmpty())
		criterias[searcher->TYPE] = ui.typeLine->text();

	if (!view->getHashLine().isEmpty())
		criterias[searcher->HASH] = ui.hashLine->text();
	if (!view->getMetadataLine()->text().isEmpty())
		criterias[searcher->CUSTOM_META] = ui.metadataLine->text();

	if (ui.timeCreatedMinCheckBox->isChecked())
		criterias[searcher->CREATION_TIME_MIN] = view->getMinCreatedTime();
	if (ui.timeCreatedMaxCheckBox->isChecked())
		criterias[searcher->CREATION_TIME_MAX] = view->getMaxCreatedTime();

	if (ui.timeEditedMinCheckBox->isChecked())
		criterias[searcher->EDIT_TIME_MIN] = view->getMinEditedTime();
	if (ui.timeEditedMaxCheckBox->isChecked())
		criterias[searcher->EDIT_TIME_MAX] = view->getMaxEditedTime();

	    searcher->search(criterias, view->getPath());
	}
}

