/********************************************************************************
** Form generated from reading UI file 'mplayercall.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MPLAYERCALL_H
#define UI_MPLAYERCALL_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MPlayerCall
{
public:

    void setupUi(QWidget *MPlayerCall)
    {
        if (MPlayerCall->objectName().isEmpty())
            MPlayerCall->setObjectName(QStringLiteral("MPlayerCall"));
        MPlayerCall->resize(800, 480);
        MPlayerCall->setMinimumSize(QSize(800, 480));
        MPlayerCall->setMaximumSize(QSize(800, 480));

        retranslateUi(MPlayerCall);

        QMetaObject::connectSlotsByName(MPlayerCall);
    } // setupUi

    void retranslateUi(QWidget *MPlayerCall)
    {
        MPlayerCall->setWindowTitle(QApplication::translate("MPlayerCall", "Form", 0));
    } // retranslateUi

};

namespace Ui {
    class MPlayerCall: public Ui_MPlayerCall {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MPLAYERCALL_H
