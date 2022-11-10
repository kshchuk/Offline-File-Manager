#include "Robocopy.h"

#include <QDesktopServices>
#include <QScrollBar>
#include <QFile>

#include "MyExplorer.h"


Robocopy::Robocopy(QWidget *parent, QFileInfoModel* model)
	: QMainWindow(parent), model(model)
{
	ui.setupUi(this);

    this->setCentralWidget(ui.widget);

    checkBoxes.append({ ui.MOT,"/MOT:m" });
    checkBoxes.append({ ui.MON, "/MON:n" });
    checkBoxes.append({ ui.NOCOPY,"/NOCOPY" });
    checkBoxes.append({ ui.LEV, "/LEV:n" });
    checkBoxes.append({ ui.ZB, "/ZB" });
    checkBoxes.append({ ui.PURGE, "/PURGE" });
    checkBoxes.append({ ui.d_256, "/256" });
    checkBoxes.append({ ui.CREATE, "/CREATE" });
    checkBoxes.append({ ui.TIMFIX, "/TIMFIX" });
    checkBoxes.append({ ui.EFSRAW, "/EFSRAW" });
    checkBoxes.append({ ui.E, "/E" });
    checkBoxes.append({ ui.MIR, "/MIR" });
    checkBoxes.append({ ui.RH, "/RH:hhmm-hhmm" });
    checkBoxes.append({ ui.S, "/S" });
    checkBoxes.append({ ui.MOV, "/MOV" });
    checkBoxes.append({ ui.DCOPY, "/DCOPY:T" });
    checkBoxes.append({ ui.A_p, "/A+:[RASHCNET]" });
    checkBoxes.append({ ui.COPYALL, "/COPYALL" });
    checkBoxes.append({ ui.B, "/B" });
    checkBoxes.append({ ui.FAT, "/FAT" });
    checkBoxes.append({ ui.A_m, "/A-:[RASHCNET]" });
    checkBoxes.append({ ui.PF, "/PF" });
    checkBoxes.append({ ui.COPY, "/COPY:copyflag[s]" });
    checkBoxes.append({ ui.Z, "/Z" });
    checkBoxes.append({ ui.MOVE, "/MOVE" });
    checkBoxes.append({ ui.TIMFIX_2, "/TIMFIX" });
    checkBoxes.append({ ui.A, "/A" });
    checkBoxes.append({ ui.XC, "/XC" });
    checkBoxes.append({ ui.M, "/M" });
    checkBoxes.append({ ui.XN, "/XN" });
    checkBoxes.append({ ui.IA, "/IA:[RASHCNETO]" });
    checkBoxes.append({ ui.XJD, "/XJD" });
    checkBoxes.append({ ui.XA, "/XA:[RASHCNETO]" });
    checkBoxes.append({ ui.MINAGE, "/MINAGE:n" });
    checkBoxes.append({ ui.XF, "/XF file [file]..." });
    checkBoxes.append({ ui.XO, "/XO" });
    checkBoxes.append({ ui.XD, "/XD dirs [dirs]..." });
    checkBoxes.append({ ui.DST, "/DST" });
    checkBoxes.append({ ui.MAX, "/MAX:n" });
    checkBoxes.append({ ui.FFT, "/FFT" });
    checkBoxes.append({ ui.MIN, "/MIN:n" });
    checkBoxes.append({ ui.XL, "/XL" });
    checkBoxes.append({ ui.XJ, "/XJ" });
    checkBoxes.append({ ui.IS, "/IS" });
    checkBoxes.append({ ui.MAXLAD, "/MAXLAD:n" });
    checkBoxes.append({ ui.MAXAGE, "/MAXAGE:n" });
    checkBoxes.append({ ui.MINLAD, "/MINLAD:n" });
    checkBoxes.append({ ui.XJF, "/XJF" });
    checkBoxes.append({ ui.XX, "/XX" });
    checkBoxes.append({ ui.IT, "/IT" });
    checkBoxes.append({ ui.W, "/W:n" });
    checkBoxes.append({ ui.TBD, "/TBD" });
    checkBoxes.append({ ui.L, "/L" });
    checkBoxes.append({ ui.NS, "/NS" });
    checkBoxes.append({ ui.X, "/X" });
    checkBoxes.append({ ui.NC, "/NC" });
    checkBoxes.append({ ui.V, "/V" });
    checkBoxes.append({ ui.NFL, "/NFL" });
    checkBoxes.append({ ui.TS, "/TS" });
    checkBoxes.append({ ui.NDL, "/NDL" });
    checkBoxes.append({ ui.FP, "/FP" });
    checkBoxes.append({ ui.NP, "/NP" });
    checkBoxes.append({ ui.BYTES, "/BYTES" });
    checkBoxes.append({ ui.ETA, "/ETA" });
    checkBoxes.append({ ui.LOG, "/LOG:file" });
    checkBoxes.append({ ui.TEE, "/TEE" });
    checkBoxes.append({ ui.LOG_p, "/LOG+:file" });
    checkBoxes.append({ ui.NJH, "/NJH" });
    checkBoxes.append({ ui.UNILOG, "/UNILOG:file" });
    checkBoxes.append({ ui.NJS, "/NJS" });
    checkBoxes.append({ ui.UNILOG_p, "/UNILOG+:file" });
    checkBoxes.append({ ui.JOB, "/JOB:jobname" });
    checkBoxes.append({ ui.NOSD, "/NOSD" });
    checkBoxes.append({ ui.SAVE, "/SAVE:jobname" });
    checkBoxes.append({ ui.NODD, "/NODD" });
    checkBoxes.append({ ui.QUIT, "/QUIT" });
    checkBoxes.append({ ui.IF, "/IF" });

	connect(ui.actionShort_description, &QAction::triggered, this, &Robocopy::seeShortDescription);
	connect(ui.actionVisit_reference_site, &QAction::triggered, this, &Robocopy::visitReferenceSite);
	connect(ui.exploreSource, &QPushButton::clicked, this, &Robocopy::setSource);
    connect(ui.runButton, &QPushButton::clicked, this, &Robocopy::runCommand);
	connect(ui.exploreDest, &QPushButton::clicked, this, &Robocopy::setDest);
	connect(ui.lineEdit, &QLineEdit::editingFinished, this, &Robocopy::updatePsevdoConsole);
	connect(ui.lineEdit_2, &QLineEdit::editingFinished, this, &Robocopy::updatePsevdoConsole);
    connect(ui.lineEdit, &QLineEdit::textEdited, this, &Robocopy::updatePsevdoConsole);
    connect(ui.lineEdit_2, &QLineEdit::textEdited, this, &Robocopy::updatePsevdoConsole);
    connect(ui.lineEdit_2, &QLineEdit::textChanged, this, &Robocopy::updatePsevdoConsole);
    connect(ui.lineEdit, &QLineEdit::textChanged, this, &Robocopy::updatePsevdoConsole);

    for (auto box : checkBoxes)
        connect(box.first, &QCheckBox::toggled, this, & Robocopy::updatePsevdoConsole);

}

Robocopy::~Robocopy()
{}

void Robocopy::visitReferenceSite()
{
	QDesktopServices::openUrl(QUrl::fromLocalFile("https://learn.microsoft.com/en-us/windows-server/administration/windows-commands/robocopy"));
}

void Robocopy::setSource()
{
	QWidget* widget = new QWidget();
	QVBoxLayout* layout = new QVBoxLayout(widget);

	widget->setWindowTitle("Offline File Manager Explorer");
	MyExplorer* explorer = new MyExplorer(widget, model);
    explorer->setMinimumSize({ 600, 400 });
	QPushButton* selectButton = new QPushButton(widget);
	selectButton->setText("Select");

	connect(selectButton, &QPushButton::clicked, explorer, &MyExplorer::sendPathOfTheSelectedFile);
	connect(explorer, &MyExplorer::pathSent, this, &Robocopy::setSourceSlot);
	connect(selectButton, &QPushButton::clicked, widget, &QWidget::close);

	layout->addWidget(explorer);
	layout->addWidget(selectButton);
	widget->setLayout(layout);
	widget->show();
}

void Robocopy::setSourceSlot(QString str)
{
    ui.lineEdit->setText(str);
}

void Robocopy::setDest()
{
	QWidget* widget = new QWidget();
	QVBoxLayout* layout = new QVBoxLayout(widget);

	widget->setWindowTitle("Offline File Manager Explorer");
	MyExplorer* explorer = new MyExplorer(widget, model);
    explorer->setMinimumSize({ 600, 400 });
	QPushButton* selectButton = new QPushButton(widget);
	selectButton->setText("Select");

	connect(selectButton, &QPushButton::clicked, explorer, &MyExplorer::sendPathOfTheSelectedFile);
	connect(explorer, &MyExplorer::pathSent, this, &Robocopy::setDestSlot);
	connect(selectButton, &QPushButton::clicked, widget, &QWidget::close);

	layout->addWidget(explorer);
	layout->addWidget(selectButton);
	widget->setLayout(layout);
	widget->show();
}

void Robocopy::setDestSlot(QString str)
{
    ui.lineEdit_2->setText(str);
}

void Robocopy::runCommand()
{
    QString command, text = ui.psevdoConsole->toPlainText();
    QStringList com = text.split(">");
    command = com.last();

    ui.psevdoConsole->appendPlainText("\n");
    QStringList args = command.split(" ");
    QString program = args[0];  args.remove(0);
    process = new QProcess();
    process->start(program, args);

    connect(process, &QProcess::readyReadStandardOutput, this, &Robocopy::updateRunning);
    connect(process, &QProcess::finished, this, &Robocopy::processFinished);
}

void Robocopy::updateRunning()
{
    ui.psevdoConsole->appendPlainText(this->process->readAllStandardOutput());
}

void Robocopy::processFinished(int code)
{
    ui.psevdoConsole->appendPlainText("\n");
    ui.psevdoConsole->appendPlainText(">");
}

void Robocopy::updatePsevdoConsole()
{
    QString text = ui.psevdoConsole->toPlainText();
    for (size_t i = text.size() - 1; i >= 0; --i)
    {
        if (text[i] == '>') {
            text.remove(i + 1, text.size() - i);
            break;
        }
    }

    text += "robocopy " + ui.lineEdit->text().replace('/', '\\') + " " +
        ui.lineEdit_2->text().replace('/', '\\');

    foreach(auto elem, this->checkBoxes)
    {
        if (elem.first->isChecked())
            text += " " + elem.second;
    }

    ui.psevdoConsole->setPlainText(text);
    ui.psevdoConsole->verticalScrollBar()->setValue(ui.psevdoConsole->verticalScrollBar()->maximum());
}

void Robocopy::seeShortDescription()
{
	QWidget* widget = new QWidget();
	widget->setFixedSize(QSize(600, 500));
	QVBoxLayout* layout = new QVBoxLayout(widget);
	widget->setWindowTitle("Robocopy Description");
	QTextEdit* text = new QTextEdit(widget);
	auto *f = new QFile("res/RobocopyDescription.html");
	f->open(QFile::ReadOnly | QFile::Text);
	auto istream = QTextStream(f);
	text->setHtml(istream.readAll());
	f->close();
	delete f;
	layout->addWidget(text);
	widget->setLayout(layout);
	widget->show();
}

