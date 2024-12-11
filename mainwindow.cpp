#include "mainwindow.h"
#include <QAction>
#include <QApplication>
#include <QFileDialog>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QSplitter>
#include <QTextStream>
#include <QVBoxLayout>

// 预览控件
#include <QCheckBox>
#include <QComboBox>
#include <QDial>
#include <QLabel>
#include <QLineEdit>
#include <QProgressBar>
#include <QPushButton>
#include <QRadioButton>
#include <QSlider>
#include <QSpinBox>
#include <QTextEdit>

#include <Qsci/qsciapis.h>
#include <Qsci/qscilexercss.h>
#include <Qsci/qsciscintilla.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // 创建中心部件
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    // 创建分割器
    splitter = new QSplitter(Qt::Horizontal);
    layout->addWidget(splitter);

    // 创建编辑器
    editor = new QsciScintilla(this);
    splitter->addWidget(editor);

    // 设置编辑器
    setupEditor();
    setupAutoCompletion();

    // 创建预览区域
    setupPreviewArea();

    // 创建菜单
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));

    QAction *openAction = fileMenu->addAction(tr("&Open"));
    connect(openAction, &QAction::triggered, this, &MainWindow::loadStyleSheet);

    QAction *saveAction = fileMenu->addAction(tr("&Save"));
    connect(saveAction, &QAction::triggered, this, &MainWindow::saveStyleSheet);

    // 连接编辑器内容变化信号
    // QScintilla使用SCN_MODIFIED通知来处理文本变化
    //    connect(editor, SIGNAL(SCN_MODIFIED(int, int, const char*, int, int, int, int, int, int, int)),
    //            this, SLOT(updatePreview()));
    connect(editor, SIGNAL(textChanged()), this, SLOT(updatePreview()));

    // 设置窗口属性
    resize(1200, 800);
    setWindowTitle(tr("QStyle Generator"));

    // 设置分割器初始大小
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 1);

    // 设置默认样式
    editor->setText("/* 默认样式 */\n"
                    "QWidget {\n"
                    "    background-color: #f0f0f0;\n"
                    "    color: #333333;\n"
                    "}\n"
                    "\n"
                    "QPushButton {\n"
                    "    background-color: #4a90e2;\n"
                    "    color: white;\n"
                    "    border: none;\n"
                    "    padding: 5px 15px;\n"
                    "    border-radius: 3px;\n"
                    "}\n"
                    "\n"
                    "QPushButton:hover {\n"
                    "    background-color: #357abd;\n"
                    "}\n");
}

MainWindow::~MainWindow()
{
    delete lexer;
    delete apis;
}

void MainWindow::setupEditor()
{
    // 设置词法分析器
    lexer = new QsciLexerCSS(this);
    editor->setLexer(lexer);

    // 显示行号
    editor->setMarginType(0, QsciScintilla::NumberMargin);
    editor->setMarginWidth(0, 35);

    // 显示折叠标记
    editor->setMarginType(1, QsciScintilla::SymbolMargin);
    editor->setMarginWidth(1, 15);
    editor->setFolding(QsciScintilla::BoxedTreeFoldStyle);

    // 显示当前行高亮
    editor->setCaretLineVisible(true);
    editor->setCaretLineBackgroundColor(QColor("#e8e8ff"));

    // 设置字体
    QFont font("Consolas", 10);
    editor->setFont(font);
    lexer->setFont(font);

    // 启用UTF-8编码
    editor->setUtf8(true);

    // 设置制表符宽度
    editor->setIndentationsUseTabs(false);
    editor->setTabWidth(4);

    // 启用自动缩进
    editor->setAutoIndent(true);
    editor->setIndentationGuides(true);
}

void MainWindow::setupAutoCompletion()
{
    // 创建API对象
    apis = new QsciAPIs(lexer);

    // 添加Qt控件选择器
    QStringList selectors = {// 基础控件
                             "QWidget",
                             "QPushButton",
                             "QLineEdit",
                             "QTextEdit",
                             "QPlainTextEdit",
                             "QComboBox",
                             "QSpinBox",
                             "QDoubleSpinBox",
                             "QTimeEdit",
                             "QDateEdit",
                             "QDateTimeEdit",
                             "QCheckBox",
                             "QRadioButton",
                             "QSlider",
                             "QScrollBar",
                             "QDial",
                             "QProgressBar",
                             "QLabel",
                             "QToolButton",

                             // 容器控件
                             "QGroupBox",
                             "QFrame",
                             "QTabWidget",
                             "QTabBar",
                             "QStackedWidget",
                             "QDockWidget",
                             "QMainWindow",
                             "QMdiArea",
                             "QWorkspace",

                             // 菜单和工具栏
                             "QMenu",
                             "QMenuBar",
                             "QStatusBar",
                             "QToolBar",
                             "QToolBox",

                             // 项视图
                             "QTableView",
                             "QTableWidget",
                             "QListView",
                             "QListWidget",
                             "QTreeView",
                             "QTreeWidget",
                             "QColumnView",
                             "QHeaderView",

                             // 日期时间
                             "QCalendarWidget",

                             // 对话框
                             "QDialog",
                             "QMessageBox",
                             "QInputDialog",
                             "QFileDialog",
                             "QFontDialog",
                             "QColorDialog",

                             // 滚动区域
                             "QScrollArea",
                             "QScrollBar",

                             // 特殊控件
                             "QLCDNumber",
                             "QSplitter",
                             "QSizeGrip",

                             // 伪状态选择器
                             ":hover",
                             ":pressed",
                             ":checked",
                             ":unchecked",
                             ":disabled",
                             ":enabled",
                             ":focus",
                             ":selected",
                             ":indeterminate",
                             ":on",
                             ":off",
                             ":top",
                             ":bottom",
                             ":middle",
                             ":first",
                             ":last",
                             ":only-one",
                             ":horizontal",
                             ":vertical",

                             // 子控件选择器
                             "::down-arrow",
                             "::up-arrow",
                             "::down-button",
                             "::up-button",
                             "::drop-down",
                             "::indicator",
                             "::handle",
                             "::groove",
                             "::menu-indicator",
                             "::item",
                             "::tab",
                             "::title",
                             "::close-button",
                             "::float-button",
                             "::decoration",
                             "::add-line",
                             "::sub-line",
                             "::chunk",
                             "::separator"};

    // 添加CSS属性
    QStringList properties = {// 背景
                              "background",
                              "background-color",
                              "background-image",
                              "background-repeat",
                              "background-position",
                              "background-attachment",
                              "background-clip",

                              // 边框
                              "border",
                              "border-color",
                              "border-style",
                              "border-width",
                              "border-radius",
                              "border-top",
                              "border-right",
                              "border-bottom",
                              "border-left",
                              "border-top-color",
                              "border-right-color",
                              "border-bottom-color",
                              "border-left-color",
                              "border-top-style",
                              "border-right-style",
                              "border-bottom-style",
                              "border-left-style",
                              "border-top-width",
                              "border-right-width",
                              "border-bottom-width",
                              "border-left-width",

                              // 外边距
                              "margin",
                              "margin-top",
                              "margin-right",
                              "margin-bottom",
                              "margin-left",

                              // 内边距
                              "padding",
                              "padding-top",
                              "padding-right",
                              "padding-bottom",
                              "padding-left",

                              // 定位
                              "position",
                              "top",
                              "right",
                              "bottom",
                              "left",

                              // 尺寸
                              "width",
                              "min-width",
                              "max-width",
                              "height",
                              "min-height",
                              "max-height",

                              // 文本
                              "color",
                              "font",
                              "font-family",
                              "font-size",
                              "font-style",
                              "font-weight",
                              "text-align",
                              "text-decoration",
                              "text-transform",
                              "line-height",
                              "letter-spacing",
                              "text-indent",
                              "word-spacing",

                              // 布局
                              "alignment",
                              "spacing",
                              "min-width",
                              "max-width",
                              "min-height",
                              "max-height",

                              // 图标
                              "image",
                              "icon-size",

                              // 其他
                              "opacity",
                              "outline",
                              "selection-background-color",
                              "selection-color",
                              "alternate-background-color",
                              "gridline-color",
                              "show-decoration-selected",
                              "lineedit-password-character",
                              "button-layout"};

    QStringList values = {// 颜色
                          "transparent",
                          "currentColor",
                          "rgb()",
                          "rgba()",
                          "hsl()",
                          "hsla()",
                          "red",
                          "green",
                          "blue",
                          "yellow",
                          "white",
                          "black",
                          "gray",
                          "lightgray",
                          "darkgray",
                          "cyan",
                          "magenta",
                          "darkred",
                          "darkgreen",
                          "darkblue",
                          "darkcyan",
                          "darkmagenta",
                          "darkyellow",

                          // 尺寸单位
                          "px",
                          "pt",
                          "em",
                          "ex",
                          "%",

                          // 边框样式
                          "none",
                          "hidden",
                          "dotted",
                          "dashed",
                          "solid",
                          "double",
                          "groove",
                          "ridge",
                          "inset",
                          "outset",

                          // 定位
                          "static",
                          "relative",
                          "absolute",
                          "fixed",

                          // 显示
                          "block",
                          "inline",
                          "inline-block",
                          "list-item",
                          "none",

                          // 对齐
                          "left",
                          "right",
                          "center",
                          "justify",

                          // 字体样式
                          "normal",
                          "italic",
                          "oblique",
                          "bold",
                          "lighter",
                          "bolder",
                          "underline",
                          "overline",
                          "line-through",

                          // 背景重复
                          "repeat",
                          "repeat-x",
                          "repeat-y",
                          "no-repeat",

                          // 背景位置
                          "top",
                          "bottom",
                          "left",
                          "right",
                          "center"};

    // 添加到API
    for (const QString &selector : selectors) {
        apis->add(selector);
    }
    for (const QString &property : properties) {
        apis->add(property);
    }
    for (const QString &value : values) {
        apis->add(value);
    }

    // 准备API
    apis->prepare();

    // 配置自动补全
    editor->setAutoCompletionSource(QsciScintilla::AcsAll);
    editor->setAutoCompletionThreshold(2);
    editor->setAutoCompletionCaseSensitivity(false);
    editor->setAutoCompletionReplaceWord(true);
    editor->setAutoCompletionUseSingle(QsciScintilla::AcusNever);
}

void MainWindow::setupPreviewArea()
{
    // 创建预览区域容器
    previewWidget = new QWidget;
    previewWidget->setObjectName("previewWidget");
    previewLayout = new QVBoxLayout(previewWidget);
    splitter->addWidget(previewWidget);

    // 添加标题
    QLabel *titleLabel = new QLabel(tr("Preview Area"));
    titleLabel->setObjectName("previewTitle");
    previewLayout->addWidget(titleLabel);

    // 创建标签页
    QTabWidget *tabWidget = new QTabWidget;
    tabWidget->setObjectName("previewTabWidget");
    previewLayout->addWidget(tabWidget);

    // 基础控件页
    QWidget     *basicTab = new QWidget;
    QVBoxLayout *basicLayout = new QVBoxLayout(basicTab);
    setupBasicControls(basicLayout);
    tabWidget->addTab(basicTab, tr("Basic Controls"));

    // 表格列表页
    QWidget     *tableTab = new QWidget;
    QVBoxLayout *tableLayout = new QVBoxLayout(tableTab);
    setupTableControls(tableLayout);
    tabWidget->addTab(tableTab, tr("Tables && Lists"));

    // 其他控件页
    QWidget     *otherTab = new QWidget;
    QVBoxLayout *otherLayout = new QVBoxLayout(otherTab);
    setupOtherControls(otherLayout);
    tabWidget->addTab(otherTab, tr("Other Controls"));
}

void MainWindow::setupBasicControls(QVBoxLayout *layout)
{
    // 按钮组
    QGroupBox   *buttonGroup = new QGroupBox(tr("Buttons"));
    QVBoxLayout *buttonLayout = new QVBoxLayout;

    QPushButton *normalBtn = new QPushButton(tr("Normal Button"));
    normalBtn->setObjectName("previewNormalButton");
    buttonLayout->addWidget(normalBtn);

    QPushButton *toggleBtn = new QPushButton(tr("Toggle Button"));
    toggleBtn->setObjectName("previewToggleButton");
    toggleBtn->setCheckable(true);
    buttonLayout->addWidget(toggleBtn);

    QToolButton *toolBtn = new QToolButton;
    toolBtn->setObjectName("previewToolButton");
    toolBtn->setText(tr("Tool Button"));
    buttonLayout->addWidget(toolBtn);

    buttonGroup->setLayout(buttonLayout);
    layout->addWidget(buttonGroup);

    // 输入控件组
    QGroupBox   *inputGroup = new QGroupBox(tr("Input Controls"));
    QVBoxLayout *inputLayout = new QVBoxLayout;

    QLineEdit *lineEdit = new QLineEdit(tr("Single Line Input"));
    lineEdit->setObjectName("previewLineEdit");
    inputLayout->addWidget(lineEdit);

    QSpinBox *spinBox = new QSpinBox;
    spinBox->setObjectName("previewSpinBox");
    inputLayout->addWidget(spinBox);

    QDoubleSpinBox *doubleSpinBox = new QDoubleSpinBox;
    doubleSpinBox->setObjectName("previewDoubleSpinBox");
    inputLayout->addWidget(doubleSpinBox);

    QComboBox *combo = new QComboBox;
    combo->setObjectName("previewComboBox");
    combo->addItems({tr("Option 1"), tr("Option 2"), tr("Option 3")});
    inputLayout->addWidget(combo);

    inputGroup->setLayout(inputLayout);
    layout->addWidget(inputGroup);

    // 选择控件组
    QGroupBox   *selectionGroup = new QGroupBox(tr("Selection Controls"));
    QVBoxLayout *selectionLayout = new QVBoxLayout;

    QCheckBox *checkBox = new QCheckBox(tr("Checkbox"));
    checkBox->setObjectName("previewCheckBox");
    selectionLayout->addWidget(checkBox);

    QRadioButton *radioBtn1 = new QRadioButton(tr("Radio 1"));
    radioBtn1->setObjectName("previewRadioButton1");
    QRadioButton *radioBtn2 = new QRadioButton(tr("Radio 2"));
    radioBtn2->setObjectName("previewRadioButton2");
    selectionLayout->addWidget(radioBtn1);
    selectionLayout->addWidget(radioBtn2);

    selectionGroup->setLayout(selectionLayout);
    layout->addWidget(selectionGroup);

    // 进度指示组
    QGroupBox   *progressGroup = new QGroupBox(tr("Progress Controls"));
    QVBoxLayout *progressLayout = new QVBoxLayout;

    QProgressBar *progressBar = new QProgressBar;
    progressBar->setObjectName("previewProgressBar");
    progressBar->setValue(75);
    progressLayout->addWidget(progressBar);

    QSlider *hSlider = new QSlider(Qt::Horizontal);
    hSlider->setObjectName("previewHSlider");
    hSlider->setValue(50);
    progressLayout->addWidget(hSlider);

    QDial *dial = new QDial;
    dial->setObjectName("previewDial");
    dial->setValue(30);
    progressLayout->addWidget(dial);

    progressGroup->setLayout(progressLayout);
    layout->addWidget(progressGroup);

    layout->addStretch();
}

void MainWindow::setupTableControls(QVBoxLayout *layout)
{
    // 表格视图
    QGroupBox   *tableGroup = new QGroupBox(tr("Table Views"));
    QVBoxLayout *tableLayout = new QVBoxLayout;

    QTableWidget *table = new QTableWidget(4, 3);
    table->setObjectName("previewTableWidget");
    table->setHorizontalHeaderLabels({tr("Column 1"), tr("Column 2"), tr("Column 3")});
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 3; ++j) {
            table->setItem(i, j, new QTableWidgetItem(QString("Item %1,%2").arg(i).arg(j)));
        }
    }
    tableLayout->addWidget(table);

    tableGroup->setLayout(tableLayout);
    layout->addWidget(tableGroup);

    // 列表视图
    QGroupBox   *listGroup = new QGroupBox(tr("List Views"));
    QVBoxLayout *listLayout = new QVBoxLayout;

    QListWidget *list = new QListWidget;
    list->setObjectName("previewListWidget");
    for (int i = 1; i <= 5; ++i) {
        list->addItem(QString(tr("List Item %1")).arg(i));
    }
    listLayout->addWidget(list);

    listGroup->setLayout(listLayout);
    layout->addWidget(listGroup);

    // 树形视图
    QGroupBox   *treeGroup = new QGroupBox(tr("Tree View"));
    QVBoxLayout *treeLayout = new QVBoxLayout;

    QTreeWidget *tree = new QTreeWidget;
    tree->setObjectName("previewTreeWidget");
    tree->setHeaderLabel(tr("Tree Items"));
    for (int i = 1; i <= 3; ++i) {
        QTreeWidgetItem *item = new QTreeWidgetItem(QStringList()
                                                    << QString(tr("Parent %1")).arg(i));
        for (int j = 1; j <= 2; ++j) {
            item->addChild(
                new QTreeWidgetItem(QStringList() << QString(tr("Child %1.%2")).arg(i).arg(j)));
        }
        tree->addTopLevelItem(item);
    }
    tree->expandAll();
    treeLayout->addWidget(tree);

    treeGroup->setLayout(treeLayout);
    layout->addWidget(treeGroup);

    layout->addStretch();
}

void MainWindow::setupOtherControls(QVBoxLayout *layout)
{
    // 文本编辑组
    QGroupBox   *textGroup = new QGroupBox(tr("Text Editing"));
    QVBoxLayout *textLayout = new QVBoxLayout;

    QTextEdit *textEdit = new QTextEdit;
    textEdit->setObjectName("previewTextEdit");
    textEdit->setPlainText(tr("Multi-line Text Editor\nSupports rich text and plain text"));
    textEdit->setMaximumHeight(100);
    textLayout->addWidget(textEdit);

    textGroup->setLayout(textLayout);
    layout->addWidget(textGroup);

    // 日期时间组
    QGroupBox   *dateGroup = new QGroupBox(tr("Date && Time"));
    QVBoxLayout *dateLayout = new QVBoxLayout;

    QDateEdit *dateEdit = new QDateEdit(QDate::currentDate());
    dateEdit->setObjectName("previewDateEdit");
    dateLayout->addWidget(dateEdit);

    QTimeEdit *timeEdit = new QTimeEdit(QTime::currentTime());
    timeEdit->setObjectName("previewTimeEdit");
    dateLayout->addWidget(timeEdit);

    QCalendarWidget *calendar = new QCalendarWidget;
    calendar->setObjectName("previewCalendarWidget");
    calendar->setMaximumHeight(200);
    dateLayout->addWidget(calendar);

    dateGroup->setLayout(dateLayout);
    layout->addWidget(dateGroup);

    // 容器组
    QGroupBox   *containerGroup = new QGroupBox(tr("Containers"));
    QVBoxLayout *containerLayout = new QVBoxLayout;

    QGroupBox *nestedGroup = new QGroupBox(tr("Nested GroupBox"));
    nestedGroup->setObjectName("previewNestedGroupBox");
    QVBoxLayout *nestedLayout = new QVBoxLayout;
    nestedLayout->addWidget(new QPushButton(tr("Button in Group")));
    nestedGroup->setLayout(nestedLayout);
    containerLayout->addWidget(nestedGroup);

    containerGroup->setLayout(containerLayout);
    layout->addWidget(containerGroup);

    layout->addStretch();
}

void MainWindow::updatePreview()
{
    if (!previewWidget)
        return;

    QString styleSheet = editor->text();

    // 只应用样式到预览区域
    previewWidget->setStyleSheet(styleSheet);

    // 确保预览区域的控件都更新
    QList<QWidget *> widgets = previewWidget->findChildren<QWidget *>();
    for (QWidget *widget : widgets) {
        widget->style()->unpolish(widget);
        widget->style()->polish(widget);
        widget->update();
    }

    // 更新预览区域本身
    previewWidget->style()->unpolish(previewWidget);
    previewWidget->style()->polish(previewWidget);
    previewWidget->update();
}

void MainWindow::loadStyleSheet()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open Style Sheet"),
                                                    "",
                                                    tr("Style Sheets (*.qss);;All Files (*)"));

    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this,
                             tr("Error"),
                             tr("Cannot open file %1:\n%2.").arg(fileName).arg(file.errorString()));
        return;
    }

    QTextStream in(&file);
    editor->setText(in.readAll());
}

void MainWindow::saveStyleSheet()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save Style Sheet"),
                                                    "",
                                                    tr("Style Sheets (*.qss);;All Files (*)"));

    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this,
                             tr("Error"),
                             tr("Cannot write file %1:\n%2.").arg(fileName).arg(file.errorString()));
        return;
    }

    QTextStream out(&file);
    out << editor->text();
}
