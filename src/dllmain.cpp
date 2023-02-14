//
// Created by Zh1an on 2023/2/11.
//

#include <pluginGl.h>

#include <Qsci/qsciscintilla.h>
#include <QWidget>

#include <nddjsonplugin.h>

#include <functional>

#define NDD_EXPORTDLL

#if defined(Q_OS_WIN)
#if defined(NDD_EXPORTDLL)
#define NDD_EXPORT __declspec(dllexport)
#else
#define NDD_EXPORT __declspec(dllimport)
#endif
#else
#define NDD_EXPORT __attribute__((visibility("default")))
#endif

#ifdef __cplusplus
extern "C"
{
#endif

    NDD_EXPORT bool NDD_PROC_IDENTIFY(NDD_PROC_DATA *pProcData);
    NDD_EXPORT int NDD_PROC_MAIN(QWidget *pNotepad, const QString &strFileName,
                                 std::function<QsciScintilla *()> getCurEdit, NDD_PROC_DATA *procData);

#ifdef __cplusplus
}
#endif

bool NDD_PROC_IDENTIFY(NDD_PROC_DATA *pProcData)
{
    if (pProcData == nullptr)
    {
        return false;
    }
    pProcData->m_strPlugName = QObject::tr("JsonView Plug");
    pProcData->m_strComment = QObject::tr("json format");

    pProcData->m_version = QString("0.0.0.1");
    pProcData->m_auther = QString("Zh1an");

    pProcData->m_menuType = 1;

    return true;
}

NDDJsonPlugin *nddJsonPlugin = nullptr;
static NDD_PROC_DATA s_procData;
static QWidget *s_pMainNotepad = nullptr;
std::function<QsciScintilla *()> s_getCurEdit;

// 插件的入口点函数
int NDD_PROC_MAIN(QWidget *pNotepad, const QString &strFileName, std::function<QsciScintilla *()> getCurEdit,
                  NDD_PROC_DATA *pProcData)
{
    if (pProcData == nullptr)
    {
        return 1;
    }

    s_pMainNotepad = pNotepad;
    s_procData = *pProcData;
    s_getCurEdit = getCurEdit;

    if (!nddJsonPlugin)
    {
        nddJsonPlugin = new NDDJsonPlugin(s_pMainNotepad, strFileName, nullptr, s_pMainNotepad);

        nddJsonPlugin->getJsonViewMenu(s_procData.m_rootMenu);

        nddJsonPlugin->setScintilla(s_getCurEdit);
    }

    return 0;
}
