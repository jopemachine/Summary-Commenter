#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// ==============================+===============================================================
// Import

#include <stack>
#include <queue>
#include <memory>
#include <unordered_map>
#include <utility>
#include <functional>

#include <QDirIterator>
#include <QMessageBox>
#include <QMainWindow>
#include <QDateTime>
#include "constant.h"

// ==============================+===============================================================
// Forward declare

namespace Ui {
class MainWindow;
}

QT_FORWARD_DECLARE_CLASS( QLineEdit );
QT_FORWARD_DECLARE_CLASS( QTableWidget );
QT_FORWARD_DECLARE_CLASS( QTableWidgetItem );
QT_FORWARD_DECLARE_CLASS( QFile );
QT_FORWARD_DECLARE_CLASS( QFileDialog );
QT_FORWARD_DECLARE_CLASS( QTextStream );

// ==============================+===============================================================
// Type define

using ExistLineFeed = bool;
using TblPos        = int ;

template<class T>
using s_ptr         = std::shared_ptr<T>;

template<class T>
using queue         = std::queue<T>;

typedef struct{
    QString     filePath;
    QString     fileName;
    QDateTime   lastModified;
    QDateTime   created;
} FileInfo;

typedef struct{
    QString     key;
    bool        existLineFeed;
} FlagType;

typedef struct{

    std::unordered_map<TblPos, FlagType>                    map;

    void    init(){
                int hit = 0;
                map.insert({hit++,  {"Sup_Div_Line",   false}});
                map.insert({hit++,  {"Project_Name",   false}});
                map.insert({hit++,  {"File_Name",      false}});
                map.insert({hit++,  {"Author",         false}});
                map.insert({hit++,  {"Team",           false}});
                map.insert({hit++,  {"Created_Date",   false}});
                map.insert({hit++,  {"Last_Edited",    false}});
                map.insert({hit++,  {"Desc",           true }});
                map.insert({hit++,  {"Issue",          true }});
                map.insert({hit++,  {"Email",          false}});
                map.insert({hit++,  {"Telephone",      false}});
                map.insert({hit++,  {"Github_Account", false}});
                map.insert({hit++,  {"Ref_URLs",       true }});
                map.insert({hit++,  {"Memo",           true }});
                map.insert({hit++,  {"Sub_Div_Line",   false}});
    }

    void    swap(int key1, int key2){
                auto temp = map.at(key1);
                map.at(key1) = map.at(key2);
                map.at(key2) = temp;
    }

} FlagTypeTbl;

typedef struct{
    TblPos      pos;
    FlagType    type;
    QString&    value;
} Flag;

typedef struct{
    QString      div;
    QStringList  workedFiles;
    CommentStyle style;
} Scproj;

typedef struct{
    QString     fileName;
    QString     content;
} TblRecord;

// ==============================+===============================================================

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit MainWindow(char *argv[], QWidget *parent = nullptr);

    ~MainWindow() override;

    void ShowMessageBox(const QMessageBox::Icon, const QString&, const QString&);

    auto Open() -> bool;

    void OpenRecent(const int);

    void Run();

    void NewFile();

private slots:

    void on_descAddBtn_clicked();

    void on_descRemoveBtn_clicked();

    void on_issueAddBtn_clicked();

    void on_issueRemoveBtn_clicked();

    void on_tabWidget_currentChanged(int);

    void on_actionExit_triggered();

    void on_actionSave_triggered();

    void on_actionOpen_triggered();

    void on_actionExecute_triggered();

    void on_actionGithub_triggered();

    void on_actionSave_as_triggered();

    void on_copyBtn_clicked();

    void on_executeBtn_clicked();

    void on_addReferenceBtn_clicked();

    void on_rmReferenceBtn_clicked();

    void on_actionOpen_and_run_triggered();

    void on_actionRemove_Comments_triggered();

    void on_actionrecent1_triggered();

    void on_actionrecent2_triggered();

    void on_actionrecent3_triggered();

    void on_actionrecent4_triggered();

    void on_actionrecent5_triggered();

    void on_descSortBtn_clicked();

    void on_issueSortBtn_clicked();

    void on_referenceSortBtn_clicked();

    void on_addExcludeBtn_clicked();

    void on_rmExcludeBtn_clicked();

    void on_sortExcludeBtn_clicked();

    void on_actionOpen_Project_Path_triggered();

    void on_actionRefresh_triggered();

    void on_actionSave_md_triggered();

    void on_actionRemove_Comments_From_All_File_triggered();

    void on_actionOpen_setting_file_triggered();

    void on_FlagUpBtn_clicked();

    void on_FlagDownBtn_clicked();

    void on_actionNew_triggered();

    void on_descAutoImtBtn_clicked();

    void on_issueAutoImtBtn_clicked();

    void on_refAutoImtBtn_clicked();

    void on_tableItemClickedked (int,int);

    void on_actionHelp_triggered();

private:

    // member vars
    Ui::MainWindow*                                 ui;
    QString                                         openedFile;
    QFile*                                          sclately;
    std::deque<QString>*                            pathQue;
    FlagTypeTbl*                                    flagTypeTbl;
    QAction*                                        divBySeparator;
    QAction*                                        divByStartEndTag;
    char*                                           programPath;
    char*                                           execPath;

    // event process
    bool                        eventFilter         (QObject*, QEvent*) override;
    void                        addAllItemBtnClicked();
    void                        dragEnterEvent      (QDragEnterEvent*) override;
    void                        dropEvent           (QDropEvent*     ) override;
    void                        handleDrop          (const QStringList&);

    // init, set program
    void                        setTables           ();
    void                        setToolbar          ();
    void                        applyPalette        (QPalette& palete);
    bool                        applyFonts          ();

    // setting table
    void                        sortTbl             (QTableWidget*);
    void                        insertTbl           (QTableWidget*, const QString&, const QString& value = nullptr, bool isMultiline = false);
    void                        setShortCut         ();
    void                        itemChange          (QTableWidget*, int prev, int dest);
    void                        clearAllTbls        ();

    // handle tables
    void                        insertItem          (QTableWidget* tbl, bool keyEditable, const QString& item);
    void                        insertItem          (QTableWidget* tbl, bool keyEditable, const QString& key, const QString& value);
    void                        insertMultiLineItem (QTableWidget* tbl, bool keyEditable, const QString& key, const QString& value);
    void                        removeSelectedItems (QTableWidget* tbl);
    bool                        isDuplicateItem     (QTableWidget* tbl, const QString& item);

    // handle SCPS file
    void                        setSCPSFile         (const QString& path);
    void                        saveSCPSFile        (const QString& path);
    void                        addGlobalVars       (const QString& key, const QString& value);
    void                        setSettingFlags     (const QString& flagName, bool flag);

    // handle SClately file
    bool                        openRecentSCPS      ();
    void                        applyRecentBar      (QStringList& list);
    QStringList                 readSClately        ();

    // handle SCProj file
    s_ptr<Scproj>               readSCProjFile      (const QString& path);
    void                        writeSCProjFile     (const QString& path, queue<FileInfo>&);

    // handle other data I/O
    void                        exportTblDataToMD   (QTextStream& ts, const QTableWidget* tbl, const QString& tblName);

    // directory traversal recursively and get all file info (except link file)
    s_ptr<queue<FileInfo>>      getAllTargetFiles   (const QString& dirName);

    // make and prepend comment
    void                        prependComment      (FileInfo fileInfo);
    void                        processFlag         (QTextStream& ts, Flag& flag, bool previewMode);
    void                        makeComment         (QTextStream& ts, const FileInfo& fileName);
    s_ptr<QString>              makeFromTbl         (QTableWidget* tbl, bool numbering, const FileInfo& fileInfo);

    // remove comment
    void                        removeComment              (QStringList &strList, const QString& div, const CommentStyle style);
    void                        removeCommentByStartEndTag (QStringList &strList, const QString& startTag, const QString& endTag, const QDirIterator::IteratorFlag&);
    void                        removeCommentBySeparator   (QStringList &strList, const QString& separator, const QDirIterator::IteratorFlag&);

    // search table and show
    s_ptr<std::list<TblRecord>> searchTbl           (QTableWidget*, QString target);
    void                        showSearchResDial   (s_ptr<std::list<TblRecord>>);

};

#endif // MAINWINDOW_H
