/********************************************************************************
** Form generated from reading UI file 'optionsmenu.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OPTIONSMENU_H
#define UI_OPTIONSMENU_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_optionsMenu
{
public:
    QWidget *formLayoutWidget;
    QVBoxLayout *vlOptions;
    QLabel *waitXMsBeforeSendingLabel;
    QLineEdit *waitXMsBeforeSendingLineEdit;
    QLabel *updateEveryXFramesLabel;
    QLineEdit *updateEveryXFramesLineEdit;
    QLabel *label;
    QComboBox *baudComboBox;
    QSpacerItem *horizontalSpacer;
    QCheckBox *checkBox;
    QFrame *widgetRanges;
    QVBoxLayout *vlEngineRanges;
    QVBoxLayout *vlEngineRange;
    QLabel *label_2;
    QRadioButton *rb15;
    QRadioButton *rb20;
    QRadioButton *rb25;
    QPushButton *saveSettingsBtn;
    QPushButton *pushButton_3;
    QButtonGroup *buttonGroup;

    void setupUi(QWidget *optionsMenu)
    {
        if (optionsMenu->objectName().isEmpty())
            optionsMenu->setObjectName(QString::fromUtf8("optionsMenu"));
        optionsMenu->resize(896, 857);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(optionsMenu->sizePolicy().hasHeightForWidth());
        optionsMenu->setSizePolicy(sizePolicy);
        formLayoutWidget = new QWidget(optionsMenu);
        formLayoutWidget->setObjectName(QString::fromUtf8("formLayoutWidget"));
        formLayoutWidget->setGeometry(QRect(20, 70, 302, 311));
        vlOptions = new QVBoxLayout(formLayoutWidget);
        vlOptions->setObjectName(QString::fromUtf8("vlOptions"));
        vlOptions->setSizeConstraint(QLayout::SetMinimumSize);
        vlOptions->setContentsMargins(0, 0, 0, 0);
        waitXMsBeforeSendingLabel = new QLabel(formLayoutWidget);
        waitXMsBeforeSendingLabel->setObjectName(QString::fromUtf8("waitXMsBeforeSendingLabel"));

        vlOptions->addWidget(waitXMsBeforeSendingLabel);

        waitXMsBeforeSendingLineEdit = new QLineEdit(formLayoutWidget);
        waitXMsBeforeSendingLineEdit->setObjectName(QString::fromUtf8("waitXMsBeforeSendingLineEdit"));
        waitXMsBeforeSendingLineEdit->setEnabled(false);
        waitXMsBeforeSendingLineEdit->setMinimumSize(QSize(50, 0));
        waitXMsBeforeSendingLineEdit->setMaximumSize(QSize(50, 16777215));

        vlOptions->addWidget(waitXMsBeforeSendingLineEdit);

        updateEveryXFramesLabel = new QLabel(formLayoutWidget);
        updateEveryXFramesLabel->setObjectName(QString::fromUtf8("updateEveryXFramesLabel"));

        vlOptions->addWidget(updateEveryXFramesLabel);

        updateEveryXFramesLineEdit = new QLineEdit(formLayoutWidget);
        updateEveryXFramesLineEdit->setObjectName(QString::fromUtf8("updateEveryXFramesLineEdit"));
        updateEveryXFramesLineEdit->setEnabled(false);
        updateEveryXFramesLineEdit->setMinimumSize(QSize(50, 0));
        updateEveryXFramesLineEdit->setMaximumSize(QSize(50, 16777215));

        vlOptions->addWidget(updateEveryXFramesLineEdit);

        label = new QLabel(formLayoutWidget);
        label->setObjectName(QString::fromUtf8("label"));

        vlOptions->addWidget(label);

        baudComboBox = new QComboBox(formLayoutWidget);
        baudComboBox->addItem(QString());
        baudComboBox->addItem(QString());
        baudComboBox->addItem(QString());
        baudComboBox->setObjectName(QString::fromUtf8("baudComboBox"));
        baudComboBox->setEnabled(false);
        baudComboBox->setMaximumSize(QSize(100, 16777215));

        vlOptions->addWidget(baudComboBox);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        vlOptions->addItem(horizontalSpacer);

        checkBox = new QCheckBox(optionsMenu);
        checkBox->setObjectName(QString::fromUtf8("checkBox"));
        checkBox->setGeometry(QRect(10, 30, 350, 17));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(checkBox->sizePolicy().hasHeightForWidth());
        checkBox->setSizePolicy(sizePolicy1);
        checkBox->setMinimumSize(QSize(350, 0));
        checkBox->setMaximumSize(QSize(350, 16777215));
        QFont font;
        font.setKerning(true);
        checkBox->setFont(font);
        widgetRanges = new QFrame(optionsMenu);
        widgetRanges->setObjectName(QString::fromUtf8("widgetRanges"));
        widgetRanges->setGeometry(QRect(340, 60, 300, 400));
        widgetRanges->setMinimumSize(QSize(300, 400));
        vlEngineRanges = new QVBoxLayout(widgetRanges);
        vlEngineRanges->setObjectName(QString::fromUtf8("vlEngineRanges"));
        vlEngineRanges->setSizeConstraint(QLayout::SetMinimumSize);
        vlEngineRange = new QVBoxLayout();
        vlEngineRange->setObjectName(QString::fromUtf8("vlEngineRange"));
        vlEngineRange->setSizeConstraint(QLayout::SetDefaultConstraint);

        vlEngineRanges->addLayout(vlEngineRange);

        label_2 = new QLabel(widgetRanges);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        vlEngineRanges->addWidget(label_2);

        rb15 = new QRadioButton(widgetRanges);
        buttonGroup = new QButtonGroup(optionsMenu);
        buttonGroup->setObjectName(QString::fromUtf8("buttonGroup"));
        buttonGroup->addButton(rb15);
        rb15->setObjectName(QString::fromUtf8("rb15"));

        vlEngineRanges->addWidget(rb15);

        rb20 = new QRadioButton(widgetRanges);
        buttonGroup->addButton(rb20);
        rb20->setObjectName(QString::fromUtf8("rb20"));

        vlEngineRanges->addWidget(rb20);

        rb25 = new QRadioButton(widgetRanges);
        buttonGroup->addButton(rb25);
        rb25->setObjectName(QString::fromUtf8("rb25"));

        vlEngineRanges->addWidget(rb25);

        saveSettingsBtn = new QPushButton(optionsMenu);
        saveSettingsBtn->setObjectName(QString::fromUtf8("saveSettingsBtn"));
        saveSettingsBtn->setGeometry(QRect(710, 100, 151, 23));
        pushButton_3 = new QPushButton(optionsMenu);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setGeometry(QRect(710, 70, 151, 23));

        retranslateUi(optionsMenu);

        QMetaObject::connectSlotsByName(optionsMenu);
    } // setupUi

    void retranslateUi(QWidget *optionsMenu)
    {
        optionsMenu->setWindowTitle(QCoreApplication::translate("optionsMenu", "Settings", nullptr));
        waitXMsBeforeSendingLabel->setText(QCoreApplication::translate("optionsMenu", "Wait X ms before sending", nullptr));
        waitXMsBeforeSendingLineEdit->setText(QCoreApplication::translate("optionsMenu", "100", nullptr));
        updateEveryXFramesLabel->setText(QCoreApplication::translate("optionsMenu", "Update every X frames", nullptr));
        updateEveryXFramesLineEdit->setText(QCoreApplication::translate("optionsMenu", "15", nullptr));
        label->setText(QCoreApplication::translate("optionsMenu", "Baudrate:", nullptr));
        baudComboBox->setItemText(0, QCoreApplication::translate("optionsMenu", "115200", nullptr));
        baudComboBox->setItemText(1, QCoreApplication::translate("optionsMenu", "9600", nullptr));
        baudComboBox->setItemText(2, QCoreApplication::translate("optionsMenu", "256000", nullptr));

        checkBox->setText(QCoreApplication::translate("optionsMenu", "Advanced options (altering these values can impact performance)", nullptr));
        label_2->setText(QCoreApplication::translate("optionsMenu", "Select the max reverse throttle range", nullptr));
        rb15->setText(QCoreApplication::translate("optionsMenu", "15% (TBM, Beechcraft)", nullptr));
        rb20->setText(QCoreApplication::translate("optionsMenu", "20% (787, A320, Caravan)", nullptr));
        rb25->setText(QCoreApplication::translate("optionsMenu", "25% (747)", nullptr));
        saveSettingsBtn->setText(QCoreApplication::translate("optionsMenu", "Save", nullptr));
        pushButton_3->setText(QCoreApplication::translate("optionsMenu", "Revert to default", nullptr));
    } // retranslateUi

};

namespace Ui {
    class optionsMenu: public Ui_optionsMenu {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OPTIONSMENU_H
