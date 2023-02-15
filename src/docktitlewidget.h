//
// Created by Zh1an on 2023/2/14.
//

#ifndef NDD_JSON_VIEWER_DOCKTITLEWIDGET_H
#define NDD_JSON_VIEWER_DOCKTITLEWIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui
{
class DockTitleWidget;
}
QT_END_NAMESPACE

class DockTitleWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DockTitleWidget(QWidget *parent = nullptr);
    ~DockTitleWidget() override;

signals:
    void sigRefreshClicked();
    void sigValidateClicked();
    void sigFormatClicked();
    void sigCloseClicked();
    void sigFindClicked(const QString &str);

private:
    Ui::DockTitleWidget *ui;
};

#endif  // NDD_JSON_VIEWER_DOCKTITLEWIDGET_H
