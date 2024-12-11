#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <Qsci/qsciscintilla.h>
#include <Qsci/qscilexercss.h>
#include <Qsci/qsciapis.h>

// 预览控件头文件
#include <QCalendarWidget>
#include <QCheckBox>
#include <QComboBox>
#include <QDateEdit>
#include <QDial>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QProgressBar>
#include <QPushButton>
#include <QRadioButton>
#include <QSlider>
#include <QSpinBox>
#include <QSplitter>
#include <QTableWidget>
#include <QTextEdit>
#include <QTimeEdit>
#include <QToolButton>
#include <QTreeWidget>

class QVBoxLayout;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void updatePreview();

private:
    void setupEditor();
    void setupAutoCompletion();
    void setupPreviewArea();
    void setupBasicControls(QVBoxLayout *layout);
    void setupTableControls(QVBoxLayout *layout);
    void setupOtherControls(QVBoxLayout *layout);
    void loadStyleSheet();
    void saveStyleSheet();

private:
    QsciScintilla *editor;
    QsciLexerCSS *lexer;
    QsciAPIs *apis;
    
    QWidget *previewWidget;
    QVBoxLayout *previewLayout;
    QSplitter *splitter;
};
#endif // MAINWINDOW_H
