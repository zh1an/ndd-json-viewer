//
// Created by Zh1an on 2023/2/14.
//

// You may need to build the project (run Qt uic code generator) to get "ui_DockTitleWidget.h" resolved

#include "docktitlewidget.h"
#include "ui_DockTitleWidget.h"

DockTitleWidget::DockTitleWidget(QWidget *parent) : QWidget(parent), ui(new Ui::DockTitleWidget)
{
    ui->setupUi(this);

    ui->pb_refresh->setIcon(QIcon(tr(":/res/Refresh.ico")));
    ui->pb_validate->setIcon(QIcon(tr(":/res/validate.ico")));
    ui->pb_format->setIcon(QIcon(tr(":/res/format.ico")));
    ui->pb_search->setIcon(QIcon(tr(":/res/search.ico")));

    ui->pb_search->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    //    ui->pb_search->sizePolicy().setVerticalStretch(20);
    //    ui->pb_search->sizePolicy().setHorizontalStretch(20);
    ui->pb_search->setFixedSize(20, 20);
    ui->pb_search->setMinimumWidth(20);
    ui->pb_search->setMaximumWidth(20);

    connect(ui->pb_refresh, &QPushButton::clicked, this, [this] { emit sigRefreshClicked(); });
    connect(ui->pb_validate, &QPushButton::clicked, this, [this] { emit sigValidateClicked(); });
    connect(ui->pb_format, &QPushButton::clicked, this, [this] { emit sigFormatClicked(); });
    connect(ui->pb_search, &QPushButton::clicked, this, [this] {
        ui->lineEdit->selectAll();
        emit sigFindClicked(ui->lineEdit->text());
    });
    connect(ui->lineEdit, &QLineEdit::editingFinished, this, [this] {
        ui->lineEdit->selectAll();
        emit sigFindClicked(ui->lineEdit->text());
    });
}

DockTitleWidget::~DockTitleWidget()
{
    delete ui;
}
