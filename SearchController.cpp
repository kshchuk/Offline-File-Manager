#include "SearchController.h"
#include "SearchWindow.h"


manager::SearchController::SearchController(Searcher *searcher, SearchWindow *view)
    : searcher(searcher), view(view)
{
    connect(searcher, &Searcher::found, this, &SearchController::AppendRecordInTheList);
}

void manager::SearchController::search() 
{
	this->foundRecords.clear();
	
	QVariant* criterias = new QVariant[searcher->criteriasCount + 1];
	
	if (!view->getNameLine().isEmpty())
		criterias[searcher->NAME] = view->getNameLine();
	if (!view->getTypeLine().isEmpty())
		criterias[searcher->TYPE] = view->getTypeLine();

	if (!view->getHashLine().isEmpty())
		criterias[searcher->HASH] = view->getHashLine();
	if (!view->getMetadataLine().isEmpty())
		criterias[searcher->CUSTOM_META] = view->getMetadataLine();

	// TODO: Get information about time

	/*if (ui.timeCreatedMinCheckBox->isChecked())
		criterias[searcher->CREATION_TIME_MIN] = view->getMinCreatedTime();
	if (ui.timeCreatedMaxCheckBox->isChecked())
		criterias[searcher->CREATION_TIME_MAX] = view->getMaxCreatedTime();

	if (ui.timeEditedMinCheckBox->isChecked())
		criterias[searcher->EDIT_TIME_MIN] = view->getMinEditedTime();
	if (ui.timeEditedMaxCheckBox->isChecked())
		criterias[searcher->EDIT_TIME_MAX] = view->getMaxEditedTime();*/

	    searcher->search(criterias, view->getPath());
}

