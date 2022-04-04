//
// Created by Dave Riedel on 04-Apr-22.
//

#include <QMessageBox>
#include "MessageCaster.h"

void MessageCaster::showWarningMessage(const QString& error) {
    auto errorMessageBox = new QMessageBox();
    errorMessageBox->setIcon(QMessageBox::Warning);
    errorMessageBox->setText(error);
    errorMessageBox->exec();
}
