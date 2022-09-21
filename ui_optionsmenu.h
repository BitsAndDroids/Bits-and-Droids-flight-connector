/********************************************************************************
** Form generated from reading UI file 'optionsmenu.ui'
**
** Created by: Qt User Interface Compiler version 6.2.0
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
    QWidget *sensitivityWidget;
    QButtonGroup *buttonGroup;

    void setupUi(QWidget *optionsMenu)
    {
        if (optionsMenu->objectName().isEmpty())
            optionsMenu->setObjectName(QString::fromUtf8("OptionsMenu"));
        optionsMenu->resize(1094, 857);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(optionsMenu->sizePolicy().hasHeightForWidth());
        optionsMenu->setSizePolicy(sizePolicy);
        optionsMenu->setStyleSheet(QString::fromUtf8("QSlider::groove:horizontal {\n"
"    border: 1px solid #999999;\n"
"    height: 8px; /* the groove expands to the size of the slider by default. by giving it a height, it has a fixed size */\n"
"	width: 250px;\n"
"    text-align:left;\n"
"    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #B1B1B1, stop:1 #c4c4c4);\n"
"    margin: 2px 0;\n"
"}\n"
"\n"
"QSlider::handle:horizontal {\n"
"    background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #00409F, stop:1 #64C0FB);\n"
"    border: 1px solid #5c5c5c;\n"
"    width: 18px;\n"
"    margin: -2px 0; /* handle is placed by default on the contents rect of the groove. Expand outside the groove */\n"
"    border-radius: 3px;\n"
"}"));
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
        saveSettingsBtn->setGeometry(QRect(20, 390, 151, 23));
        pushButton_3 = new QPushButton(optionsMenu);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setGeometry(QRect(20, 420, 151, 23));
        sensitivityWidget = new QWidget(optionsMenu);
        sensitivityWidget->setObjectName(QString::fromUtf8("sensitivityWidget"));
        sensitivityWidget->setGeometry(QRect(660, 70, 481, 571));

        retranslateUi(optionsMenu);

        QMetaObject::connectSlotsByName(optionsMenu);
    } // setupUi

    void retranslateUi(QWidget *optionsMenu)
    {
        optionsMenu->setWindowTitle(QCoreApplication::translate("OptionsMenu", "Settings", nullptr));
        waitXMsBeforeSendingLabel->setText(QCoreApplication::translate("OptionsMenu", "Wait X ms before sending", nullptr));
        waitXMsBeforeSendingLineEdit->setText(QCoreApplication::translate("OptionsMenu", "5", nullptr));
        updateEveryXFramesLabel->setText(QCoreApplication::translate("OptionsMenu", "Update every X frames", nullptr));
        updateEveryXFramesLineEdit->setText(QCoreApplication::translate("OptionsMenu", "3", nullptr));
        label->setText(QCoreApplication::translate("OptionsMenu", "Baudrate:", nullptr));
        baudComboBox->setItemText(0, QCoreApplication::translate("OptionsMenu", "115200", nullptr));
        baudComboBox->setItemText(1, QCoreApplication::translate("OptionsMenu", "9600", nullptr));
        baudComboBox->setItemText(2, QCoreApplication::translate("OptionsMenu", "256000", nullptr));

        checkBox->setText(QCoreApplication::translate("OptionsMenu", "Advanced options (can impact performance)", nullptr));
        label_2->setText(QCoreApplication::translate("OptionsMenu", "Select the max reverse throttle range", nullptr));
        rb15->setText(QCoreApplication::translate("OptionsMenu", "15% (TBM, Beechcraft)", nullptr));
        rb20->setText(QCoreApplication::translate("OptionsMenu", "20% (787, A320, Caravan)", nullptr));
        rb25->setText(QCoreApplication::translate("OptionsMenu", "25% (747)", nullptr));
        saveSettingsBtn->setText(QCoreApplication::translate("OptionsMenu", "Save", nullptr));
        pushButton_3->setText(QCoreApplication::translate("OptionsMenu", "Revert to default", nullptr));
    } // retranslateUi

};

namespace Ui {
    class optionsMenu: public Ui_optionsMenu {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OPTIONSMENU_H
