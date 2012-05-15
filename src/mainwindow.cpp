#include "mainwindow.h"
#include "note.h"
#include "findfilemodel.h"
#include "filesystemmodel.h"
#include "preferences.h"
#include <QTextStream>
#include <QFile>
#include <QModelIndex>
#include <QInputDialog>
#include <QMouseEvent>
#include <QDesktopServices>
#include <QtConcurrentRun>
#include <QSettings>
#include <QMessageBox>
#include <QDebug>
#include <QProgressDialog>
#include <QFileIconProvider>


NobleNote::NobleNote()
{
     setupUi(this);

   //TrayIcon
     QIcon icon = QIcon(":nobleNote");

     minimize_restore_action = new QAction(tr("&Minimize"),this);
     quit_action = new QAction(tr("&Quit"),this);

#ifndef NO_SYSTEM_TRAY_ICON
     TIcon = new QSystemTrayIcon(this);
     TIcon->setIcon(icon);
     TIcon->show();

   //TrayIconContextMenu
     iMenu = new QMenu(this);
     iMenu->addAction(minimize_restore_action);
     iMenu->addAction(quit_action);

     TIcon->setContextMenu(iMenu);  //setting contextmenu for the systray
#endif

   //Configuration file
     QSettings settings; // ini format does save but in the executables directory, use native format
     if(!settings.isWritable()){
       QMessageBox::critical(this,tr("Settings not writable"),tr("W: nobelNote settings not writable!"));
     }
     if(!settings.value("rootPath").isValid()) // root path has not been set before
         settings.setValue("rootPath"           ,QDir::homePath() + "/.nobleNote/notes");
     if(!settings.value("journalFolderPath").isValid())
         settings.setValue("journalFolderPath"  ,QDir::homePath() + "/.nobleNote/Journals");

     if(!QDir(settings.value("rootPath").toString()).exists())
       QDir().mkpath(settings.value("rootPath").toString());

     if(!QDir(settings.value("journalFolderPath").toString()).exists())
       QDir().mkpath(settings.value("journalFolderPath").toString());

   //Setup preferences
     pref = new Preferences(this);

   //Collabsable search
     hBoxLayout = new QHBoxLayout();
     gridLayout->addLayout(hBoxLayout, 0, 0);

     showHideAdvancedSearchButton = new QToolButton(this);
     showHideAdvancedSearchButton->setText("+");
     showHideAdvancedSearchButton->setMinimumSize(25, 20);
     hBoxLayout->addWidget(showHideAdvancedSearchButton);

     findLabel = new QLabel(this);
     findLabel->setText(tr("Search"));
     hBoxLayout->addWidget(findLabel);
     QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
     sizePolicy.setHeightForWidth(findLabel->sizePolicy().hasHeightForWidth());
     findLabel->setSizePolicy(sizePolicy);

     hLine = new QFrame(this);
     hLine->setFrameStyle(QFrame::HLine);
     hBoxLayout->addWidget(hLine);

     searchBoolean = false;

   //Search line edits
     searchName = new QLineEdit(this);
     searchName->setPlaceholderText(tr("Search for note"));
     gridLayout->addWidget(searchName, 1, 0);
     searchName->setHidden(true);
     connect(searchName, SIGNAL(returnPressed()), this, SLOT(find()));
     searchText = new QLineEdit(this);
     searchText->setPlaceholderText(tr("Search for content"));
     gridLayout->addWidget(searchText, 2, 0);
     searchText->setHidden(true);
     connect(searchText, SIGNAL(returnPressed()), this, SLOT(find()));

     splitter = new QSplitter(centralwidget);
     gridLayout->addWidget(splitter, 3, 0);

     folderModel = new FileSystemModel(this);
     folderModel->setRootPath(settings.value("rootPath").toString());
     folderModel->setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
     folderModel->setReadOnly(false);

     noteFSModel = new FileSystemModel(this);
     noteFSModel->setFilter(QDir::Files);
     noteFSModel->setReadOnly(false);

     model = new FindFileModel(this);

     noteModel = new FindFileSystemModel(this);
     noteModel->setSourceModel(noteFSModel);

     folderList = new QListView(splitter);
     noteList = new QListView(splitter);

     QList<QListView*> listViews;
     listViews << folderList << noteList;
     foreach(QListView* list, listViews) // add drag drop options
     {
        list->setContextMenuPolicy(Qt::CustomContextMenu);
        //list->setSelectionMode(QAbstractItemView::SingleSelection); // single item can be draged or droped
        list->setDragDropMode(QAbstractItemView::DragDrop);
        list->viewport()->setAcceptDrops(true);
        list->setDropIndicatorShown(true);
        list->setDefaultDropAction(Qt::MoveAction);
     }
     noteList->setDragEnabled(true);
     folderList->setDragEnabled(false);

     folderList->setModel(folderModel);
     folderList->setRootIndex(folderModel->index(settings.value("rootPath").toString()));
     noteList->setEditTriggers(QListView::EditKeyPressed);
     noteList->setModel(noteModel);

     // make sure there's at least one folder
     QStringList dirList = QDir(settings.value("rootPath").toString()).entryList(QDir::Dirs);
     dirList.removeOne(".");
     dirList.removeOne("..");
     if(dirList.isEmpty())
     {
         QString defaultDirName = tr("default");
         QDir(settings.value("rootPath").toString()).mkdir(defaultDirName);
         noteList->setRootIndex(noteModel->setRootPath(settings.value("rootPath").toString() + "/" + defaultDirName)); // set default dir as current note folder
     }
     else
         noteList->setRootIndex(noteModel->setRootPath(settings.value("rootPath").toString() + "/" + dirList.first())); // dirs exist, set first dir as current note folder

//TODO: make it possible to import notes from some other folder or even another program

//     // selects first folder as soon as the folderModel has populated its first folder
//     // "single shot" slot
     connect(folderModel,SIGNAL(directoryLoaded(QString)), this,
       SLOT(selectFirstFolder(QString)),Qt::QueuedConnection);

     connect(actionQuit, SIGNAL(triggered()), qApp, SLOT(quit()));
    #ifndef NO_SYSTEM_TRAY_ICON
     connect(TIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
       this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason))); //handles systray-symbol     
     connect(minimize_restore_action, SIGNAL(triggered()), this, SLOT(tray_actions()));
      #endif
     connect(quit_action, SIGNAL(triggered()), qApp, SLOT(quit())); //contextmenu "Quit" for the systray
     connect(folderList, SIGNAL(clicked(const QModelIndex &)), this,
       SLOT(setCurrentFolder(const QModelIndex &)));
     connect(folderList,SIGNAL(activated(QModelIndex)), this,
       SLOT(setCurrentFolder(QModelIndex)));
     connect(noteList,SIGNAL(activated(QModelIndex)), this,
       SLOT(openNote(QModelIndex)));
     connect(folderList, SIGNAL(customContextMenuRequested(const QPoint &)),
       this, SLOT(showContextMenuF(const QPoint &)));
     connect(noteList, SIGNAL(customContextMenuRequested(const QPoint &)),
       this, SLOT(showContextMenuN(const QPoint &)));
     connect(action_Configure, SIGNAL(triggered()), pref, SLOT(show()));
     connect(showHideAdvancedSearchButton, SIGNAL(clicked(bool)), this,
       SLOT(showHideAdvancedSearch()));
     //connect(pref, SIGNAL(sendPathChanged()), this, SLOT(changeRootIndex()));
}

NobleNote::~NobleNote(){}

void NobleNote::showHideAdvancedSearch(){
     if(!searchBoolean){
       searchName->show();
       searchText->show();
       showHideAdvancedSearchButton->setText("-");
       searchBoolean = true;
     }
     else{
       searchName->hide();
       searchText->hide();
       showHideAdvancedSearchButton->setText("+");
       searchBoolean = false;
     }
}

void NobleNote::find(){
//TODO: Reset or delete old entries for new search here
     noteList->setModel(model);
     QString fileName = searchName->text();
     QString text = searchText->text();
     QString path = folderModel->rootPath();

     searchDir = QDir(path);
     QStringList files;
     if(fileName.isEmpty())
       fileName = "*";
     QDirIterator it(path, QDirIterator::Subdirectories);
     while(it.hasNext())
       files << it.next();

     if(!text.isEmpty())
       files = findFiles(files, text);

     while(!files.isEmpty()){
       QString file = files.takeFirst();
       QFileInfo info(file);
       if(!info.isDir())
         model->appendFile(file);
     }
//TODO: when should the model swich back?
//TODO: show folder and file name
}

QStringList NobleNote::findFiles(const QStringList &files, const QString &text){
     QProgressDialog progressDialog(this);
     progressDialog.setCancelButtonText(tr("&Cancel"));
     progressDialog.setRange(0, files.size());
     progressDialog.setWindowTitle(tr("Find Files"));

     QStringList foundFiles;

     for(int i = 0; i < files.size(); ++i){
        progressDialog.setValue(i);
        progressDialog.setLabelText(tr("Searching file number %1 of %2...")
          .arg(i).arg(files.size()));
        qApp->processEvents();

        if(progressDialog.wasCanceled())
          break;

        QFile file(searchDir.absoluteFilePath(files[i]));

        if(file.open(QIODevice::ReadOnly)){
          QString line;
          QTextStream in(&file);
          while(!in.atEnd()){
            if(progressDialog.wasCanceled())
              break;
            line = in.readLine();
            if(line.contains(text)){
              foundFiles << files[i];
              break;
            }
          }
        }
     }
     return foundFiles;
}

void NobleNote::selectFirstFolder(QString path)
{
     // this slot gets (probably) called by the QFileSystemModel gatherer thread
     // due to some race conditions:
     // disconnecting this slot will not work button disconnect() will return true
     // qDebug() may work or not work depending how many time has elapsed in this function

     // only call once
     static bool thisMethodHasBeenCalled = false;

     if(thisMethodHasBeenCalled)
       return;

     QModelIndex idx = folderList->indexAt(QPoint(0,0));
     if(!idx.isValid())
       return;

     folderList->selectionModel()->select(idx,QItemSelectionModel::Select);

     thisMethodHasBeenCalled = true;
}

void NobleNote::setCurrentFolder(const QModelIndex &ind){
     noteList->setRootIndex(noteModel->setRootPath(folderModel->filePath(ind)));
}

//void NobleNote::changeRootIndex(){
//     QSettings s;
//     folderModel->setRootPath(s.value("rootPath").toString());
//     noteModel->setRootPath(s.value("rootPath").toString());
//     folderList->setRootIndex(folderModel->index(s.value("rootPath").toString()));
//     noteList->setRootIndex(noteModel->index(s.value("rootPath").toString()));
//}

#ifndef NO_SYSTEM_TRAY_ICON
void NobleNote::iconActivated(QSystemTrayIcon::ActivationReason reason){
     if(reason == QSystemTrayIcon::Trigger)
       tray_actions();
}
#endif

#ifndef NO_SYSTEM_TRAY_ICON
void NobleNote::tray_actions(){
     if(isMinimized() || isHidden())  //in case that the window is minimized or hidden
       showNormal();
     else
       hide();
}
#endif

void NobleNote::showEvent(QShowEvent* show_window){
     minimize_restore_action->setText(tr("&Minimize"));
     QWidget::showEvent(show_window);
}

void NobleNote::hideEvent(QHideEvent* window_hide){
     minimize_restore_action->setText(tr("&Restore"));
     QWidget::hideEvent(window_hide);
}

void NobleNote::closeEvent(QCloseEvent* window_close){
     if(pref->dontQuit->isChecked())
       hide();
     else
       qApp->quit();
     QWidget::closeEvent(window_close);
}

void NobleNote::openNote(const QModelIndex &index /* = new QModelIndex*/){
     QModelIndex ind = index;
     if(!ind.isValid()) // default constructed model index
       ind = noteList->currentIndex();

     QString notesPath = noteModel->filePath(ind);
     QFile noteFile(notesPath);
     if(!noteFile.open(QIODevice::ReadOnly))
       return;
     QTextStream streamN(&noteFile);
     text = streamN.readAll();
     noteFile.close();

     QString journalFilesPath = QSettings().value("journalFolderPath").toString() + "/" +
       folderModel->fileName(folderList->currentIndex()) + "_" + noteModel->fileName(ind) + ".journal";
     QFile journalFile(journalFilesPath);
     if(!journalFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
       return;
     if(!journalFile.exists()){
       QTextStream streamJ(&journalFile);
       streamJ << text;
     }
     journalFile.close();

     //TODO:
     //if(QFileInfo(journalsPath).lastModified().toString() == QFileInfo(notesPath).lastModified().toString());

     Note *notes = new Note(this);
     notes->text = text;
     notes->notesPath = notesPath;
     notes->journalsPath = journalFilesPath;
     if(pref->pSpin->value() > 0)
       notes->timer->start(pref->pSpin->value() * 60000);
     notes->show();
     notes->setAttribute(Qt::WA_DeleteOnClose);
}

void NobleNote::newFolder(){
     QString path = folderModel->rootPath() + "/" + tr("new folder");
     int counter = 0;
     while(QDir(path).exists())
     {
         ++counter;
         path = folderModel->rootPath() + "/" + tr("new folder (%1)").arg(QString::number(counter));
     }
     folderModel->mkdir(folderList->rootIndex(),QDir(path).dirName());
}

void NobleNote::newNote(){
    QString name = noteModel->rootPath() + "/" + tr("new note");
    int counter = 0;
    while(QFile::exists(name))
    {
        ++counter;
        name = noteModel->rootPath() + "/" + tr("new note (%1)").arg(QString::number(counter));
    }

     QFile file(name);
     if(!file.open(QIODevice::WriteOnly))
       return;
     file.close();
}

void NobleNote::renameFolder(){
     folderList->edit(folderList->currentIndex());
}

void NobleNote::renameNote(){
     noteList->edit(noteList->currentIndex());
}

void NobleNote::removeFolder(){

     QStringList dirList = QDir(QSettings().value("rootPath").toString()).entryList(QDir::Dirs | QDir::NoDotAndDotDot);

     // only needed if journal folder resides in the same folder
     //dirList.removeOne(QDir(QSettings().value("journalFolderPath").toString()).dirName());

     // keep at least one folder
     if(dirList.size() == 1)
     {
        QMessageBox::information(this,tr("One notebook must remain"),tr("At least one notebook must remain."));
        return;
     }

     QModelIndex idx = folderList->currentIndex();

     // remove empty folders without prompt else show a yes/abort message box
     if(!folderModel->rmdir(idx)) // folder not empty
     {
         if(QMessageBox::warning(this,tr("Remove Folder"),
                                tr("Do you really want to delete the notebook \"%1\"? All contained notes will be lost?").arg(folderModel->fileName(idx)),
                             QMessageBox::Yes | QMessageBox::Abort) != QMessageBox::Yes)
            return;


        // list all files
        QString path = folderModel->filePath(idx);
        QStringList fileList = QDir(path).entryList(QDir::NoDotAndDotDot | QDir::Dirs | QDir::Files);

        // try to remove each
        foreach(QString name, fileList)
        {
            name = QDir::toNativeSeparators(QString("%1/%2").arg(path).arg(name));
            if(!folderModel->remove(folderModel->index(name)))
            {
                qWarning(qPrintable(QString("could not remove ") + name));
            }
        }

        // try to remove the (now empty?) folder again
        if(!folderModel->rmdir(idx))
        {
            QMessageBox::warning(this,tr("Folder could not be removed"), tr("The folder could not be removed because one or more files inside the folder could not be removed"));
            return;
        }
     }

// // TODO  Important! the following #ifdef code must only be executed if the folder has been removed
#ifdef Q_OS_WIN32
    // gives error QFileSystemWatcher: FindNextChangeNotification failed!! (Zugriff verweigert)
    // and dir deletion is delayed until another dir has been selected or the application is closed

     folderList->setRowHidden(idx.row(),true);
     QModelIndex idxAt = folderList->indexAt(QPoint(0,0));
     if(!idxAt.isValid())
     return;

     folderList->selectionModel()->select(idxAt,QItemSelectionModel::Select);
     setCurrentFolder(idxAt);
#endif

//TODO: check why:
//QInotifyFileSystemWatcherEngine::addPaths: inotify_add_watch failed: Datei oder Verzeichnis nicht gefunden
//QFileSystemWatcher: failed to add paths: /home/hakaishi/.nobleNote/new folder
}

void NobleNote::removeNote(){
     if(QMessageBox::warning(this,tr("Delete note"),
         tr("Do you really want to delete the note \"%1\"?").arg(noteModel->fileName(noteList->currentIndex())),
           QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes)
       return;
     noteModel->remove(noteList->currentIndex());
}

void NobleNote::showContextMenuF(const QPoint &pos){
     QPoint globalPos = this->mapToGlobal(pos);

     QMenu menu;

     if(!folderList->indexAt(pos).isValid()) // if index doesn't exists at position
     {
         QAction* addNewF = new QAction(tr("New &folder"), &menu);
         connect(addNewF, SIGNAL(triggered()), this, SLOT(newFolder()));
         menu.addAction(addNewF);
     }
     if(folderList->indexAt(pos).isValid()) // if index exists at position
     {
         QAction* renameF = new QAction(tr("R&ename folder"), &menu);
         QAction* removeFolder = new QAction(tr("&Remove folder"), &menu);
         connect(renameF, SIGNAL(triggered()), this, SLOT(renameFolder()));
         connect(removeFolder, SIGNAL(triggered()), this, SLOT(removeFolder()));
         menu.addAction(renameF);
         menu.addAction(removeFolder);
     }
     menu.exec(globalPos);
}

void NobleNote::showContextMenuN(const QPoint &pos){
     QPoint globalPos = this->mapToGlobal(pos);

     QMenu menu;

     if(!noteList->indexAt(pos).isValid()) // if index doesn't exists at position
     {
         QAction* addNewN = new QAction(tr("New &note"), &menu);
         connect(addNewN, SIGNAL(triggered()), this, SLOT(newNote()));
         menu.addAction(addNewN);
     }
     if(noteList->indexAt(pos).isValid()) // if index exists at position
     {
         QAction* renameN = new QAction(tr("Ren&ame note"), &menu);
         QAction* removeNote = new QAction(tr("Re&move note"), &menu);
         connect(renameN, SIGNAL(triggered()), this, SLOT(renameNote()));
         connect(removeNote, SIGNAL(triggered()), this, SLOT(removeNote()));
         menu.addAction(renameN);
         menu.addAction(removeNote);
     }
     menu.exec(globalPos);
}
