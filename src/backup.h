/* nobleNote, a note taking application
 * Copyright (C) 2012 Christian Metscher <hakaishi@web.de>,
                      Fabian Deuchler <Taiko000@gmail.com>

 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.

 * nobleNote is licensed under the MIT, see `http://copyfree.org/licenses/mit/license.txt'.
 */

#ifndef BACKUP_H
#define BACKUP_H

#include "ui_backup.h"
#include <QSplitter>
#include <QTreeWidget>
#include <QTextEdit>
#include <QTextDocument>
#include <QLabel>
#include <QPushButton>
#include <QProgressDialog>
#include <QFutureWatcher>
#include <QUuid>

class Backup : public QDialog, public Ui::Backup {
     Q_OBJECT
 
     public:
      Backup(QWidget *parent = 0);

     private:
      QSplitter     *splitter;
      QFrame        *frame;
      QGridLayout   *gridLayout3;
      QTreeWidget   *treeWidget;
      QLabel        *label;
      QTextDocument *document;
      QTextEdit     *textEdit;
      QPushButton   *deleteOldButton;
      QProgressDialog *indexDialog, *progressDialog;
      QFutureWatcher<QUuid> *future1;
      QFutureWatcher<void> *future2;
      QList<QUuid> notesUuids;

     private slots:
      void showPreview(const QModelIndex &idx);
      void deleteOldBackupsAndFileEntries();
      void getNoteUuidList();
      void progressChanges();
      void restoreBackup();
      void handleBackups();
      void setupTreeData();
      QList<QVariant> getFileData(const QString &file);

     signals:
      void handleBackupsSignal();
};

#endif //BACKUP_H