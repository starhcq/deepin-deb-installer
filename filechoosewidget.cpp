/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     sbw <sbw@sbw.so>
 *
 * Maintainer: sbw <sbw@sbw.so>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "filechoosewidget.h"
#include "utils.h"

#include <QDebug>
#include <QDir>
#include <QDragEnterEvent>
#include <QFileDialog>
#include <QFileInfo>
#include <QLabel>
#include <QMimeData>
#include <QPainter>
#include <QPixmap>
#include <QUrl>
#include <QVBoxLayout>

DWIDGET_USE_NAMESPACE

FileChooseWidget::FileChooseWidget(QWidget *parent)
    : QWidget(parent)
    , m_settings("deepin", "deepin-deb-install") {
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    const auto ratio = devicePixelRatioF();
    setFocusPolicy(Qt::ClickFocus);
    setFixedSize(480, 380);
    // icon
    QPixmap iconPix = Utils::renderSVG(":/images/icon.svg", QSize(123, 109));
    iconPix.setDevicePixelRatio(ratio);
    QLabel *iconImage = new QLabel;
    iconImage->setFixedSize(123, 109);
    iconImage->setPixmap(iconPix);

    // Label 拖拽文字
    QLabel *dndTips = new QLabel;
    dndTips->setText(tr("Drag and drop file here"));
    dndTips->setAlignment(Qt::AlignCenter);
    dndTips->setObjectName("DNDTips");

    QVBoxLayout *centerWrapLayout = new QVBoxLayout;
    centerWrapLayout->addWidget(iconImage);
    centerWrapLayout->setAlignment(iconImage, Qt::AlignTop | Qt::AlignHCenter);
    centerWrapLayout->addSpacing(32);
    centerWrapLayout->addWidget(dndTips);
    centerWrapLayout->setSpacing(0);
    centerWrapLayout->setContentsMargins(0, 0, 0, 15);

    QWidget *centerWidget = new QFrame;
    centerWidget->setFixedWidth(270);
    centerWidget->setLayout(centerWrapLayout);
    centerWidget->setObjectName("CenterWidget");

    // Label 虚线
    QLabel *split_line = new QLabel;
    split_line->setObjectName("SplitLine");
    split_line->setPixmap(QPixmap(":/images/split_line.svg"));
    split_line->setAlignment(Qt::AlignCenter);

    // button 选择文件
    m_fileChooseBtn = new DLinkButton;
    m_fileChooseBtn->setObjectName("FileChooseButton");
    m_fileChooseBtn->setText(tr("Select File"));

    QVBoxLayout *centralLayout = new QVBoxLayout;
    centralLayout->addStretch();
    centralLayout->addWidget(centerWidget);
    centralLayout->setAlignment(centerWidget, Qt::AlignTop | Qt::AlignCenter);
    centralLayout->addSpacing(16);  //文字和虚线的空隙
    centralLayout->addWidget(split_line);
    centralLayout->addSpacing(17);
    centralLayout->addWidget(m_fileChooseBtn);
    centralLayout->setAlignment(m_fileChooseBtn, Qt::AlignCenter);
    centralLayout->setSpacing(0);
    centralLayout->setContentsMargins(0, 0, 0, 60);

    setLayout(centralLayout);
    connect(m_fileChooseBtn, &QPushButton::clicked, this, &FileChooseWidget::chooseFiles);
}

void FileChooseWidget::chooseFiles() {
    QString historyDir = m_settings.value("history_dir").toString();

    if (historyDir.isEmpty()) {
        historyDir = QDir::homePath();
    }

    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::ExistingFiles);
    dialog.setNameFilter("Debian Package Files (*.deb)");
    dialog.setDirectory(historyDir);

    const int mode = dialog.exec();

    // save the directory string to config file.
    m_settings.setValue("history_dir", dialog.directoryUrl().toLocalFile());

    if (mode != QDialog::Accepted) return;

    const QStringList selected_files = dialog.selectedFiles();

    emit packagesSelected(selected_files);
}
