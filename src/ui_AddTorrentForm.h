/********************************************************************************
** Form generated from reading UI file 'AddTorrentForm.ui'
**
** Created by: Qt User Interface Compiler version 6.2.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADDTORRENTFORM_H
#define UI_ADDTORRENTFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

namespace uTorrent {

class Ui_AddTorrentForm
{
public:
    QVBoxLayout *vboxLayout;
    QGroupBox *_mGroupBox;
    QWidget *_mWidget;
    QGridLayout *gridLayout;
    QLabel *_mLabel4;
    QLineEdit *_mTorrentFile;
    QLabel *_mLabel2;
    QPushButton *_mBrowseTorrents;
    QLabel *_mLabel5;
    QLabel *_mLabel3;
    QLabel *_mLabel6;
    QTextEdit *_mTorrentContents;
    QLineEdit *_mDestinationFolder;
    QLabel *_mAnnounceUrl;
    QLabel *_mLabel;
    QPushButton *_mBrowseDestination;
    QLabel *_mLabel7;
    QLabel *_mCommentLabel;
    QLabel *_mCreatorLabel;
    QLabel *_mSizeLabel;
    QHBoxLayout *hboxLayout;
    QSpacerItem *spacerItem;
    QPushButton *_mOkButton;
    QPushButton *_mCancelButton;

    void setupUi(QDialog *uTorrent__AddTorrentFile)
    {
        if (uTorrent__AddTorrentFile->objectName().isEmpty())
            uTorrent__AddTorrentFile->setObjectName(QString::fromUtf8("uTorrent__AddTorrentFile"));
        uTorrent__AddTorrentFile->resize(464, 385);
        uTorrent__AddTorrentFile->setSizeGripEnabled(false);
        uTorrent__AddTorrentFile->setModal(true);
        vboxLayout = new QVBoxLayout(uTorrent__AddTorrentFile);
#ifndef Q_OS_MAC
        vboxLayout->setSpacing(6);
#endif
        vboxLayout->setContentsMargins(8, 8, 8, 8);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        _mGroupBox = new QGroupBox(uTorrent__AddTorrentFile);
        _mGroupBox->setObjectName(QString::fromUtf8("_mGroupBox"));
        _mWidget = new QWidget(_mGroupBox);
        _mWidget->setObjectName(QString::fromUtf8("_mWidget"));
        _mWidget->setGeometry(QRect(10, 40, 364, 33));
        gridLayout = new QGridLayout(_mGroupBox);
#ifndef Q_OS_MAC
        gridLayout->setSpacing(6);
#endif
        gridLayout->setContentsMargins(8, 8, 8, 8);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        _mLabel4 = new QLabel(_mGroupBox);
        _mLabel4->setObjectName(QString::fromUtf8("_mLabel4"));

        gridLayout->addWidget(_mLabel4, 6, 0, 1, 1);

        _mTorrentFile = new QLineEdit(_mGroupBox);
        _mTorrentFile->setObjectName(QString::fromUtf8("_mTorrentFile"));

        gridLayout->addWidget(_mTorrentFile, 0, 1, 1, 2);

        _mLabel2 = new QLabel(_mGroupBox);
        _mLabel2->setObjectName(QString::fromUtf8("_mLabel2"));

        gridLayout->addWidget(_mLabel2, 1, 0, 1, 1);

        _mBrowseTorrents = new QPushButton(_mGroupBox);
        _mBrowseTorrents->setObjectName(QString::fromUtf8("_mBrowseTorrents"));

        gridLayout->addWidget(_mBrowseTorrents, 0, 3, 1, 1);

        _mLabel5 = new QLabel(_mGroupBox);
        _mLabel5->setObjectName(QString::fromUtf8("_mLabel5"));
        _mLabel5->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);

        gridLayout->addWidget(_mLabel5, 5, 0, 1, 1);

        _mLabel3 = new QLabel(_mGroupBox);
        _mLabel3->setObjectName(QString::fromUtf8("_mLabel3"));

        gridLayout->addWidget(_mLabel3, 4, 0, 1, 1);

        _mLabel6 = new QLabel(_mGroupBox);
        _mLabel6->setObjectName(QString::fromUtf8("_mLabel6"));

        gridLayout->addWidget(_mLabel6, 2, 0, 1, 1);

        _mTorrentContents = new QTextEdit(_mGroupBox);
        _mTorrentContents->setObjectName(QString::fromUtf8("_mTorrentContents"));
        _mTorrentContents->setFocusPolicy(Qt::NoFocus);
        _mTorrentContents->setTabChangesFocus(true);
        _mTorrentContents->setLineWrapMode(QTextEdit::NoWrap);
        _mTorrentContents->setReadOnly(true);

        gridLayout->addWidget(_mTorrentContents, 5, 1, 1, 3);

        _mDestinationFolder = new QLineEdit(_mGroupBox);
        _mDestinationFolder->setObjectName(QString::fromUtf8("_mDestinationFolder"));
        _mDestinationFolder->setFocusPolicy(Qt::StrongFocus);

        gridLayout->addWidget(_mDestinationFolder, 6, 1, 1, 2);

        _mAnnounceUrl = new QLabel(_mGroupBox);
        _mAnnounceUrl->setObjectName(QString::fromUtf8("_mAnnounceUrl"));

        gridLayout->addWidget(_mAnnounceUrl, 1, 1, 1, 3);

        _mLabel = new QLabel(_mGroupBox);
        _mLabel->setObjectName(QString::fromUtf8("_mLabel"));

        gridLayout->addWidget(_mLabel, 0, 0, 1, 1);

        _mBrowseDestination = new QPushButton(_mGroupBox);
        _mBrowseDestination->setObjectName(QString::fromUtf8("_mBrowseDestination"));

        gridLayout->addWidget(_mBrowseDestination, 6, 3, 1, 1);

        _mLabel7 = new QLabel(_mGroupBox);
        _mLabel7->setObjectName(QString::fromUtf8("_mLabel7"));

        gridLayout->addWidget(_mLabel7, 3, 0, 1, 1);

        _mCommentLabel = new QLabel(_mGroupBox);
        _mCommentLabel->setObjectName(QString::fromUtf8("_mCommentLabel"));

        gridLayout->addWidget(_mCommentLabel, 3, 1, 1, 3);

        _mCreatorLabel = new QLabel(_mGroupBox);
        _mCreatorLabel->setObjectName(QString::fromUtf8("_mCreatorLabel"));

        gridLayout->addWidget(_mCreatorLabel, 2, 1, 1, 3);

        _mSizeLabel = new QLabel(_mGroupBox);
        _mSizeLabel->setObjectName(QString::fromUtf8("_mSizeLabel"));

        gridLayout->addWidget(_mSizeLabel, 4, 1, 1, 3);


        vboxLayout->addWidget(_mGroupBox);

        hboxLayout = new QHBoxLayout();
#ifndef Q_OS_MAC
        hboxLayout->setSpacing(6);
#endif
        hboxLayout->setContentsMargins(0, 0, 0, 0);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        spacerItem = new QSpacerItem(131, 31, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(spacerItem);

        _mOkButton = new QPushButton(uTorrent__AddTorrentFile);
        _mOkButton->setObjectName(QString::fromUtf8("_mOkButton"));
        _mOkButton->setEnabled(false);

        hboxLayout->addWidget(_mOkButton);

        _mCancelButton = new QPushButton(uTorrent__AddTorrentFile);
        _mCancelButton->setObjectName(QString::fromUtf8("_mCancelButton"));

        hboxLayout->addWidget(_mCancelButton);


        vboxLayout->addLayout(hboxLayout);

        QWidget::setTabOrder(_mTorrentFile, _mBrowseTorrents);
        QWidget::setTabOrder(_mBrowseTorrents, _mTorrentContents);
        QWidget::setTabOrder(_mTorrentContents, _mDestinationFolder);
        QWidget::setTabOrder(_mDestinationFolder, _mBrowseDestination);
        QWidget::setTabOrder(_mBrowseDestination, _mOkButton);
        QWidget::setTabOrder(_mOkButton, _mCancelButton);

        retranslateUi(uTorrent__AddTorrentFile);

        _mBrowseTorrents->setDefault(true);


        QMetaObject::connectSlotsByName(uTorrent__AddTorrentFile);
    } // setupUi

    void retranslateUi(QDialog *uTorrent_AddTorrentFile)
    {
        uTorrent_AddTorrentFile->setWindowTitle(QCoreApplication::translate("uTorrent::AddTorrentForm", "Add a torrent", nullptr));
        _mGroupBox->setTitle(QCoreApplication::translate("uTorrent::AddTorrentForm", "Select a torrent source", nullptr));
        _mLabel4->setText(QCoreApplication::translate("uTorrent::AddTorrentForm", "Destination:", nullptr));
        _mLabel2->setText(QCoreApplication::translate("uTorrent::AddTorrentForm", "Tracker URL:", nullptr));
        _mBrowseTorrents->setText(QCoreApplication::translate("uTorrent::AddTorrentForm", "Browse", nullptr));
        _mLabel5->setText(QCoreApplication::translate("uTorrent::AddTorrentForm", "File(s):", nullptr));
        _mLabel3->setText(QCoreApplication::translate("uTorrent::AddTorrentForm", "Size:", nullptr));
        _mLabel6->setText(QCoreApplication::translate("uTorrent::AddTorrentForm", "Creator:", nullptr));
        _mAnnounceUrl->setText(QCoreApplication::translate("uTorrent::AddTorrentForm", "<none>", nullptr));
        _mLabel->setText(QCoreApplication::translate("uTorrent::AddTorrentForm", "Torrent file:", nullptr));
        _mBrowseDestination->setText(QCoreApplication::translate("uTorrent::AddTorrentForm", "Browse", nullptr));
        _mLabel7->setText(QCoreApplication::translate("uTorrent::AddTorrentForm", "Comment:", nullptr));
        _mCommentLabel->setText(QCoreApplication::translate("uTorrent::AddTorrentForm", "<none>", nullptr));
        _mCreatorLabel->setText(QCoreApplication::translate("uTorrent::AddTorrentForm", "<none>", nullptr));
        _mSizeLabel->setText(QCoreApplication::translate("uTorrent::AddTorrentForm", "0", nullptr));
        _mOkButton->setText(QCoreApplication::translate("uTorrent::AddTorrentForm", "&OK", nullptr));
        _mCancelButton->setText(QCoreApplication::translate("uTorrent::AddTorrentForm", "&Cancel", nullptr));
    } // retranslateUi

};

} // namespace uTorrent

namespace uTorrent {
namespace Ui {
    class AddTorrentDialog: public Ui_AddTorrentForm {};
} // namespace Ui
} // namespace uTorrent

#endif // UI_ADDTORRENTFORM_H
