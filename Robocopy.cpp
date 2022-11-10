#include "Robocopy.h"

#include <QDesktopServices>
#include <QFile>

#include "MyExplorer.h"


Robocopy::Robocopy(QWidget *parent, QFileInfoModel* model)
	: QMainWindow(parent), model(model)
{
	ui.setupUi(this);

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

    connect(ui.MOT, &QCheckBox::toggled, this, &Robocopy::updatePsevdoConsole);
    connect(ui.MON, &QCheckBox::toggled, this, &Robocopy::updatePsevdoConsole);
    connect(ui.NOCOPY, &QCheckBox::toggled, this, &Robocopy::updatePsevdoConsole);
    connect(ui.LEV, &QCheckBox::toggled, this, &Robocopy::updatePsevdoConsole);
    connect(ui.ZB, &QCheckBox::toggled, this, &Robocopy::updatePsevdoConsole);
    connect(ui.PURGE, &QCheckBox::toggled, this, &Robocopy::updatePsevdoConsole);
    connect(ui.d_256, &QCheckBox::toggled, this, &Robocopy::updatePsevdoConsole);
    connect(ui.CREATE, &QCheckBox::toggled, this, &Robocopy::updatePsevdoConsole);
    connect(ui.TIMFIX, &QCheckBox::toggled, this, &Robocopy::updatePsevdoConsole);
    connect(ui.EFSRAW, &QCheckBox::toggled, this, &Robocopy::updatePsevdoConsole);
    connect(ui.E, &QCheckBox::toggled, this, &Robocopy::updatePsevdoConsole);
    connect(ui.MIR, &QCheckBox::toggled, this, &Robocopy::updatePsevdoConsole);
    connect(ui.RH, &QCheckBox::toggled, this, &Robocopy::updatePsevdoConsole);
    connect(ui.S, &QCheckBox::toggled, this, &Robocopy::updatePsevdoConsole);
    connect(ui.MOV, &QCheckBox::toggled, this, &Robocopy::updatePsevdoConsole);
    connect(ui.DCOPY, &QCheckBox::toggled, this, &Robocopy::updatePsevdoConsole);
    connect(ui.A_p, &QCheckBox::toggled, this, &Robocopy::updatePsevdoConsole);
    connect(ui.COPYALL, &QCheckBox::toggled, this, &Robocopy::updatePsevdoConsole);
    connect(ui.B, &QCheckBox::toggled, this, &Robocopy::updatePsevdoConsole);
    connect(ui.FAT, &QCheckBox::toggled, this, &Robocopy::updatePsevdoConsole);
    connect(ui.A_m, &QCheckBox::toggled, this, &Robocopy::updatePsevdoConsole);
    connect(ui.PF, &QCheckBox::toggled, this, &Robocopy::updatePsevdoConsole);
    connect(ui.COPY, &QCheckBox::toggled, this, &Robocopy::updatePsevdoConsole);
    connect(ui.Z, &QCheckBox::toggled, this, &Robocopy::updatePsevdoConsole);
    connect(ui.MOVE, &QCheckBox::toggled, this, &Robocopy::updatePsevdoConsole);
    connect(ui.TIMFIX_2, &QCheckBox::toggled, this, &Robocopy::updatePsevdoConsole);
    connect(ui.A, &QCheckBox::toggled, this, &Robocopy::updatePsevdoConsole);
    connect(ui.XC, &QCheckBox::toggled, this, &Robocopy::updatePsevdoConsole);
    connect(ui.M, &QCheckBox::toggled, this, &Robocopy::updatePsevdoConsole);
    connect(ui.XN, &QCheckBox::toggled, this, &Robocopy::updatePsevdoConsole);
    connect(ui.IA, &QCheckBox::toggled, this, &Robocopy::updatePsevdoConsole);
    connect(ui.XJD, &QCheckBox::toggled, this, &Robocopy::updatePsevdoConsole);
    connect(ui.XA, &QCheckBox::toggled, this, &Robocopy::updatePsevdoConsole);
    connect(ui.MINAGE, &QCheckBox::toggled, this, &Robocopy::updatePsevdoConsole);
    connect(ui.XF, &QCheckBox::toggled, this, &Robocopy::updatePsevdoConsole);
    connect(ui.XO, &QCheckBox::toggled, this, &Robocopy::updatePsevdoConsole);
    connect(ui.XD, &QCheckBox::toggled, this, &Robocopy::updatePsevdoConsole);
    connect(ui.DST, &QCheckBox::toggled, this, &Robocopy::updatePsevdoConsole);
    connect(ui.MAX, &QCheckBox::toggled, this, &Robocopy::updatePsevdoConsole);
    connect(ui.FFT, &QCheckBox::toggled, this, &Robocopy::updatePsevdoConsole);
    connect(ui.MIN, &QCheckBox::toggled, this, &Robocopy::updatePsevdoConsole);
    connect(ui.XL, &QCheckBox::toggled, this, &Robocopy::updatePsevdoConsole);
    connect(ui.XJ, &QCheckBox::toggled, this, &Robocopy::updatePsevdoConsole);
    connect(ui.IS, &QCheckBox::toggled, this, &Robocopy::updatePsevdoConsole);
    connect(ui.MAXLAD, &QCheckBox::toggled, this, &Robocopy::updatePsevdoConsole);
    connect(ui.MAXAGE, &QCheckBox::toggled, this, &Robocopy::updatePsevdoConsole);
    connect(ui.MINLAD, &QCheckBox::toggled, this, &Robocopy::updatePsevdoConsole);
    connect(ui.XJF, &QCheckBox::toggled, this, &Robocopy::updatePsevdoConsole);
    connect(ui.XX, &QCheckBox::toggled, this, &Robocopy::updatePsevdoConsole);
    connect(ui.IT, &QCheckBox::toggled, this, &Robocopy::updatePsevdoConsole);
    connect(ui.L, &QCheckBox::toggled, this, &Robocopy::updatePsevdoConsole);
    connect(ui.NS, &QCheckBox::toggled, this, &Robocopy::updatePsevdoConsole);
    connect(ui.X, &QCheckBox::toggled, this, &Robocopy::updatePsevdoConsole);
    connect(ui.NC, &QCheckBox::toggled, this, &Robocopy::updatePsevdoConsole);
    connect(ui.V, &QCheckBox::toggled, this, &Robocopy::updatePsevdoConsole);
    connect(ui.NFL, &QCheckBox::toggled, this, &Robocopy::updatePsevdoConsole);
    connect(ui.TS, &QCheckBox::toggled, this, &Robocopy::updatePsevdoConsole);
    connect(ui.NDL, &QCheckBox::toggled, this, &Robocopy::updatePsevdoConsole);
    connect(ui.FP, &QCheckBox::toggled, this, &Robocopy::updatePsevdoConsole);
    connect(ui.NP, &QCheckBox::toggled, this, &Robocopy::updatePsevdoConsole);
    connect(ui.BYTES, &QCheckBox::toggled, this, &Robocopy::updatePsevdoConsole);
    connect(ui.ETA, &QCheckBox::toggled, this, &Robocopy::updatePsevdoConsole);
    connect(ui.LOG, &QCheckBox::toggled, this, &Robocopy::updatePsevdoConsole);
    connect(ui.TEE, &QCheckBox::toggled, this, &Robocopy::updatePsevdoConsole);
    connect(ui.LOG_p, &QCheckBox::toggled, this, &Robocopy::updatePsevdoConsole);
    connect(ui.NJH, &QCheckBox::toggled, this, &Robocopy::updatePsevdoConsole);
    connect(ui.UNILOG, &QCheckBox::toggled, this, &Robocopy::updatePsevdoConsole);
    connect(ui.NJS, &QCheckBox::toggled, this, &Robocopy::updatePsevdoConsole);
    connect(ui.UNILOG_p, &QCheckBox::toggled, this, &Robocopy::updatePsevdoConsole);
    connect(ui.JOB, &QCheckBox::toggled, this, &Robocopy::updatePsevdoConsole);
    connect(ui.NOSD, &QCheckBox::toggled, this, &Robocopy::updatePsevdoConsole);
    connect(ui.SAVE, &QCheckBox::toggled, this, &Robocopy::updatePsevdoConsole);
    connect(ui.NODD, &QCheckBox::toggled, this, &Robocopy::updatePsevdoConsole);
    connect(ui.QUIT, &QCheckBox::toggled, this, &Robocopy::updatePsevdoConsole);
    connect(ui.IF, &QCheckBox::toggled, this, &Robocopy::updatePsevdoConsole);
}

Robocopy::~Robocopy()
{}

void Robocopy::visitReferenceSite()
{
	QDesktopServices::openUrl(QUrl::fromLocalFile("https://learn.microsoft.com/en-us/windows-server/administration/windows-commands/robocopy"));
}

void Robocopy::setSource()
{
	QWidget* widget = new QWidget(this);
	QVBoxLayout* layout = new QVBoxLayout(widget);

	widget->setWindowTitle("Offline File Manager Explorer");
	MyExplorer* explorer = new MyExplorer(widget, model);
	QPushButton* selectButton = new QPushButton(widget);
	selectButton->setText("Select");

	connect(selectButton, &QPushButton::clicked, explorer, &MyExplorer::sendPathOfTheSelectedFile);
	connect(explorer, &MyExplorer::pathSent, ui.lineEdit, &QLineEdit::setText);
	connect(selectButton, &QPushButton::clicked, widget, &QWidget::close);

	layout->addWidget(explorer);
	layout->addWidget(selectButton);
	widget->setLayout(layout);
	widget->show();
}

void Robocopy::setDest()
{
	QWidget* widget = new QWidget(this);
	QVBoxLayout* layout = new QVBoxLayout(widget);

	widget->setWindowTitle("Offline File Manager Explorer");
	MyExplorer* explorer = new MyExplorer(widget, model);
	QPushButton* selectButton = new QPushButton(widget);
	selectButton->setText("Select");

	connect(selectButton, &QPushButton::clicked, explorer, &MyExplorer::sendPathOfTheSelectedFile);
	connect(explorer, &MyExplorer::pathSent, ui.lineEdit_2, &QLineEdit::setText);
	connect(selectButton, &QPushButton::clicked, widget, &QWidget::close);

	layout->addWidget(explorer);
	layout->addWidget(selectButton);
	widget->setLayout(layout);
	widget->show();
}

void Robocopy::runCommand()
{
    QString command, text;
    QString::iterator end = text.end(); end--;
    while (*end != '>' && end != text.begin()) {
        command.append(*end);
        end--;
    }

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
    for (size_t i = text.size() - 1; i > 0; --i)
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

