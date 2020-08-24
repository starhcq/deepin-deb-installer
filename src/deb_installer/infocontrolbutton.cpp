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

#include "infocontrolbutton.h"
#include "utils.h"

#include <DStyleHelper>
#include <DApplicationHelper>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPixmap>
#include <QIcon>
#include <QPen>

InfoControlButton::InfoControlButton(const QString &expandTips, const QString &shrinkTips, QWidget *parent)
    : QWidget(parent)
    , m_tipsText(new DCommandLinkButton("", this))
    , m_expand(false)
    , m_expandTips(expandTips)
    , m_shrinkTips(shrinkTips)
    , m_arrowIcon(new DLabel(this))
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_arrowIcon->setAlignment(Qt::AlignCenter);

    QIcon arrow_up = QIcon::fromTheme("di_arrow_up");
    m_arrowIcon->setPixmap(arrow_up.pixmap(QSize(25, 8)));

    m_arrowIcon->setFixedHeight(13);

    m_tipsText->setText(expandTips);
    QFontInfo fontinfo = m_tipsText->fontInfo();
    int fontsize = fontinfo.pixelSize();
    if (fontsize >= 16) {
        m_tipsText->setFixedHeight(28);
    } else {
        m_tipsText->setFixedHeight(15);
    }
//    m_tipsText->setFixedHeight(15);

    QString normalFontFamily = Utils::loadFontFamilyByType(Utils::SourceHanSansNormal);
    Utils::bindFontBySizeAndWeight(m_tipsText, normalFontFamily, 12, QFont::ExtraLight);

    centralLayout = new QVBoxLayout(this);
    centralLayout->setSpacing(5);
    centralLayout->setContentsMargins(0, 0, 0, 0);
    centralLayout->addWidget(m_arrowIcon);
    centralLayout->addWidget(m_tipsText);

    // keep the tips in the middle
    centralLayout->setAlignment(m_tipsText, Qt::AlignCenter);

    setLayout(centralLayout);

    QObject::connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged,
                     this, &InfoControlButton::themeChanged);

    // add clicked connection fot expand or shrink
    connect(m_tipsText, &DCommandLinkButton::clicked, this, &InfoControlButton::onMouseRelease);


//#define SHOWBGCOLOR
#ifdef SHOWBGCOLOR
    m_tipsText->setStyleSheet("QLabel{background: cyan;}");
    m_arrowIcon->setStyleSheet("QLabel{background: red;}");
#endif
}

void InfoControlButton::mouseReleaseEvent(QMouseEvent *e)
{
    QWidget::mouseReleaseEvent(e);

    onMouseRelease();
}
/**
 * @brief InfoControlButton::onMouseRelease
 * 鼠标点击事件
 */
void InfoControlButton::onMouseRelease()
{
    if (m_expand) {
        emit shrink();
    } else {
        emit expand();
    }

    m_expand = !m_expand;
    centralLayout->removeWidget(m_arrowIcon);
    centralLayout->removeWidget(m_tipsText);
    if (!m_expand) {
        centralLayout->setSpacing(5);
        centralLayout->addWidget(m_arrowIcon);
        centralLayout->addWidget(m_tipsText);
        //fix bug: 33999 keep tips in the middle when install details hidden
        centralLayout->setAlignment(m_tipsText, Qt::AlignCenter);
        QIcon arrow_up = QIcon::fromTheme("di_arrow_up");
        m_arrowIcon->setPixmap(arrow_up.pixmap(QSize(25, 8)));
        m_tipsText->setText(m_expandTips);
    } else {
        centralLayout->setSpacing(0);
        centralLayout->addWidget(m_tipsText);
        //fix bug: 33999 keep tips in the middle when details show
        centralLayout->setAlignment(m_tipsText, Qt::AlignCenter);
        centralLayout->addWidget(m_arrowIcon);
        QIcon arrow_up = QIcon::fromTheme("di_arrow_down");
        m_arrowIcon->setPixmap(arrow_up.pixmap(QSize(25, 8)));
        m_tipsText->setText(m_shrinkTips);
    }
}

void InfoControlButton::setExpandTips(const QString text)
{
    m_expandTips = text;
    m_tipsText->setText(m_expandTips);
}

void InfoControlButton::setShrinkTips(const QString text)
{
    m_shrinkTips = text;
    m_tipsText->setText(m_shrinkTips);
}

void InfoControlButton::themeChanged()
{
    if (m_expand) {
        QIcon arrow_down = QIcon::fromTheme("di_arrow_down");
        m_arrowIcon->setPixmap(arrow_down.pixmap(QSize(25, 8)));
    } else {
        QIcon arrow_up = QIcon::fromTheme("di_arrow_up");
        m_arrowIcon->setPixmap(arrow_up.pixmap(QSize(25, 8)));
    }
}
