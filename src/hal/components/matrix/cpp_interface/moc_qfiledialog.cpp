/****************************************************************************
** Meta object code from reading C++ file 'qfiledialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "libqt/gui/dialogs/qfiledialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qfiledialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QFileDialog_t {
    QByteArrayData data[73];
    char stringdata0[945];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QFileDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QFileDialog_t qt_meta_stringdata_QFileDialog = {
    {
QT_MOC_LITERAL(0, 0, 11), // "QFileDialog"
QT_MOC_LITERAL(1, 12, 12), // "fileSelected"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 4), // "file"
QT_MOC_LITERAL(4, 31, 13), // "filesSelected"
QT_MOC_LITERAL(5, 45, 5), // "files"
QT_MOC_LITERAL(6, 51, 14), // "currentChanged"
QT_MOC_LITERAL(7, 66, 4), // "path"
QT_MOC_LITERAL(8, 71, 16), // "directoryEntered"
QT_MOC_LITERAL(9, 88, 9), // "directory"
QT_MOC_LITERAL(10, 98, 14), // "filterSelected"
QT_MOC_LITERAL(11, 113, 6), // "filter"
QT_MOC_LITERAL(12, 120, 14), // "_q_pathChanged"
QT_MOC_LITERAL(13, 135, 19), // "_q_navigateBackward"
QT_MOC_LITERAL(14, 155, 18), // "_q_navigateForward"
QT_MOC_LITERAL(15, 174, 19), // "_q_navigateToParent"
QT_MOC_LITERAL(16, 194, 18), // "_q_createDirectory"
QT_MOC_LITERAL(17, 213, 15), // "_q_showListView"
QT_MOC_LITERAL(18, 229, 18), // "_q_showDetailsView"
QT_MOC_LITERAL(19, 248, 18), // "_q_showContextMenu"
QT_MOC_LITERAL(20, 267, 16), // "_q_renameCurrent"
QT_MOC_LITERAL(21, 284, 16), // "_q_deleteCurrent"
QT_MOC_LITERAL(22, 301, 13), // "_q_showHidden"
QT_MOC_LITERAL(23, 315, 17), // "_q_updateOkButton"
QT_MOC_LITERAL(24, 333, 17), // "_q_currentChanged"
QT_MOC_LITERAL(25, 351, 11), // "QModelIndex"
QT_MOC_LITERAL(26, 363, 5), // "index"
QT_MOC_LITERAL(27, 369, 17), // "_q_enterDirectory"
QT_MOC_LITERAL(28, 387, 16), // "_q_goToDirectory"
QT_MOC_LITERAL(29, 404, 16), // "_q_useNameFilter"
QT_MOC_LITERAL(30, 421, 19), // "_q_selectionChanged"
QT_MOC_LITERAL(31, 441, 10), // "_q_goToUrl"
QT_MOC_LITERAL(32, 452, 3), // "url"
QT_MOC_LITERAL(33, 456, 9), // "_q_goHome"
QT_MOC_LITERAL(34, 466, 13), // "_q_showHeader"
QT_MOC_LITERAL(35, 480, 8), // "QAction*"
QT_MOC_LITERAL(36, 489, 23), // "_q_autoCompleteFileName"
QT_MOC_LITERAL(37, 513, 4), // "text"
QT_MOC_LITERAL(38, 518, 15), // "_q_rowsInserted"
QT_MOC_LITERAL(39, 534, 6), // "parent"
QT_MOC_LITERAL(40, 541, 14), // "_q_fileRenamed"
QT_MOC_LITERAL(41, 556, 7), // "oldName"
QT_MOC_LITERAL(42, 564, 7), // "newName"
QT_MOC_LITERAL(43, 572, 8), // "viewMode"
QT_MOC_LITERAL(44, 581, 8), // "ViewMode"
QT_MOC_LITERAL(45, 590, 8), // "fileMode"
QT_MOC_LITERAL(46, 599, 8), // "FileMode"
QT_MOC_LITERAL(47, 608, 10), // "acceptMode"
QT_MOC_LITERAL(48, 619, 10), // "AcceptMode"
QT_MOC_LITERAL(49, 630, 8), // "readOnly"
QT_MOC_LITERAL(50, 639, 15), // "resolveSymlinks"
QT_MOC_LITERAL(51, 655, 16), // "confirmOverwrite"
QT_MOC_LITERAL(52, 672, 13), // "defaultSuffix"
QT_MOC_LITERAL(53, 686, 24), // "nameFilterDetailsVisible"
QT_MOC_LITERAL(54, 711, 7), // "options"
QT_MOC_LITERAL(55, 719, 7), // "Options"
QT_MOC_LITERAL(56, 727, 6), // "Detail"
QT_MOC_LITERAL(57, 734, 4), // "List"
QT_MOC_LITERAL(58, 739, 7), // "AnyFile"
QT_MOC_LITERAL(59, 747, 12), // "ExistingFile"
QT_MOC_LITERAL(60, 760, 9), // "Directory"
QT_MOC_LITERAL(61, 770, 13), // "ExistingFiles"
QT_MOC_LITERAL(62, 784, 13), // "DirectoryOnly"
QT_MOC_LITERAL(63, 798, 10), // "AcceptOpen"
QT_MOC_LITERAL(64, 809, 10), // "AcceptSave"
QT_MOC_LITERAL(65, 820, 6), // "Option"
QT_MOC_LITERAL(66, 827, 12), // "ShowDirsOnly"
QT_MOC_LITERAL(67, 840, 19), // "DontResolveSymlinks"
QT_MOC_LITERAL(68, 860, 20), // "DontConfirmOverwrite"
QT_MOC_LITERAL(69, 881, 12), // "DontUseSheet"
QT_MOC_LITERAL(70, 894, 19), // "DontUseNativeDialog"
QT_MOC_LITERAL(71, 914, 8), // "ReadOnly"
QT_MOC_LITERAL(72, 923, 21) // "HideNameFilterDetails"

    },
    "QFileDialog\0fileSelected\0\0file\0"
    "filesSelected\0files\0currentChanged\0"
    "path\0directoryEntered\0directory\0"
    "filterSelected\0filter\0_q_pathChanged\0"
    "_q_navigateBackward\0_q_navigateForward\0"
    "_q_navigateToParent\0_q_createDirectory\0"
    "_q_showListView\0_q_showDetailsView\0"
    "_q_showContextMenu\0_q_renameCurrent\0"
    "_q_deleteCurrent\0_q_showHidden\0"
    "_q_updateOkButton\0_q_currentChanged\0"
    "QModelIndex\0index\0_q_enterDirectory\0"
    "_q_goToDirectory\0_q_useNameFilter\0"
    "_q_selectionChanged\0_q_goToUrl\0url\0"
    "_q_goHome\0_q_showHeader\0QAction*\0"
    "_q_autoCompleteFileName\0text\0"
    "_q_rowsInserted\0parent\0_q_fileRenamed\0"
    "oldName\0newName\0viewMode\0ViewMode\0"
    "fileMode\0FileMode\0acceptMode\0AcceptMode\0"
    "readOnly\0resolveSymlinks\0confirmOverwrite\0"
    "defaultSuffix\0nameFilterDetailsVisible\0"
    "options\0Options\0Detail\0List\0AnyFile\0"
    "ExistingFile\0Directory\0ExistingFiles\0"
    "DirectoryOnly\0AcceptOpen\0AcceptSave\0"
    "Option\0ShowDirsOnly\0DontResolveSymlinks\0"
    "DontConfirmOverwrite\0DontUseSheet\0"
    "DontUseNativeDialog\0ReadOnly\0"
    "HideNameFilterDetails"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QFileDialog[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      28,   14, // methods
       9,  218, // properties
       5,  245, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,  154,    2, 0x06 /* Public */,
       4,    1,  157,    2, 0x06 /* Public */,
       6,    1,  160,    2, 0x06 /* Public */,
       8,    1,  163,    2, 0x06 /* Public */,
      10,    1,  166,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      12,    1,  169,    2, 0x08 /* Private */,
      13,    0,  172,    2, 0x08 /* Private */,
      14,    0,  173,    2, 0x08 /* Private */,
      15,    0,  174,    2, 0x08 /* Private */,
      16,    0,  175,    2, 0x08 /* Private */,
      17,    0,  176,    2, 0x08 /* Private */,
      18,    0,  177,    2, 0x08 /* Private */,
      19,    1,  178,    2, 0x08 /* Private */,
      20,    0,  181,    2, 0x08 /* Private */,
      21,    0,  182,    2, 0x08 /* Private */,
      22,    0,  183,    2, 0x08 /* Private */,
      23,    0,  184,    2, 0x08 /* Private */,
      24,    1,  185,    2, 0x08 /* Private */,
      27,    1,  188,    2, 0x08 /* Private */,
      28,    1,  191,    2, 0x08 /* Private */,
      29,    1,  194,    2, 0x08 /* Private */,
      30,    0,  197,    2, 0x08 /* Private */,
      31,    1,  198,    2, 0x08 /* Private */,
      33,    0,  201,    2, 0x08 /* Private */,
      34,    1,  202,    2, 0x08 /* Private */,
      36,    1,  205,    2, 0x08 /* Private */,
      38,    1,  208,    2, 0x08 /* Private */,
      40,    3,  211,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QStringList,    5,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void, QMetaType::QString,   11,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QPoint,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 25,   26,
    QMetaType::Void, 0x80000000 | 25,   26,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void, QMetaType::Int,   26,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QUrl,   32,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 35,    2,
    QMetaType::Void, QMetaType::QString,   37,
    QMetaType::Void, 0x80000000 | 25,   39,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString,    7,   41,   42,

 // properties: name, type, flags
      43, 0x80000000 | 44, 0x0009510b,
      45, 0x80000000 | 46, 0x0009510b,
      47, 0x80000000 | 48, 0x0009510b,
      49, QMetaType::Bool, 0x00094103,
      50, QMetaType::Bool, 0x00094103,
      51, QMetaType::Bool, 0x00094103,
      52, QMetaType::QString, 0x00095103,
      53, QMetaType::Bool, 0x00094103,
      54, 0x80000000 | 55, 0x0009510b,

 // enums: name, alias, flags, count, data
      44,   44, 0x0,    2,  270,
      46,   46, 0x0,    5,  274,
      48,   48, 0x0,    2,  284,
      65,   65, 0x0,    7,  288,
      55,   65, 0x1,    7,  302,

 // enum data: key, value
      56, uint(QFileDialog::Detail),
      57, uint(QFileDialog::List),
      58, uint(QFileDialog::AnyFile),
      59, uint(QFileDialog::ExistingFile),
      60, uint(QFileDialog::Directory),
      61, uint(QFileDialog::ExistingFiles),
      62, uint(QFileDialog::DirectoryOnly),
      63, uint(QFileDialog::AcceptOpen),
      64, uint(QFileDialog::AcceptSave),
      66, uint(QFileDialog::ShowDirsOnly),
      67, uint(QFileDialog::DontResolveSymlinks),
      68, uint(QFileDialog::DontConfirmOverwrite),
      69, uint(QFileDialog::DontUseSheet),
      70, uint(QFileDialog::DontUseNativeDialog),
      71, uint(QFileDialog::ReadOnly),
      72, uint(QFileDialog::HideNameFilterDetails),
      66, uint(QFileDialog::ShowDirsOnly),
      67, uint(QFileDialog::DontResolveSymlinks),
      68, uint(QFileDialog::DontConfirmOverwrite),
      69, uint(QFileDialog::DontUseSheet),
      70, uint(QFileDialog::DontUseNativeDialog),
      71, uint(QFileDialog::ReadOnly),
      72, uint(QFileDialog::HideNameFilterDetails),

       0        // eod
};

void QFileDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<QFileDialog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->fileSelected((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->filesSelected((*reinterpret_cast< const QStringList(*)>(_a[1]))); break;
        case 2: _t->currentChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->directoryEntered((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: _t->filterSelected((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 5: _t->d_func()->_q_pathChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 6: _t->d_func()->_q_navigateBackward(); break;
        case 7: _t->d_func()->_q_navigateForward(); break;
        case 8: _t->d_func()->_q_navigateToParent(); break;
        case 9: _t->d_func()->_q_createDirectory(); break;
        case 10: _t->d_func()->_q_showListView(); break;
        case 11: _t->d_func()->_q_showDetailsView(); break;
        case 12: _t->d_func()->_q_showContextMenu((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 13: _t->d_func()->_q_renameCurrent(); break;
        case 14: _t->d_func()->_q_deleteCurrent(); break;
        case 15: _t->d_func()->_q_showHidden(); break;
        case 16: _t->d_func()->_q_updateOkButton(); break;
        case 17: _t->d_func()->_q_currentChanged((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 18: _t->d_func()->_q_enterDirectory((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 19: _t->d_func()->_q_goToDirectory((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 20: _t->d_func()->_q_useNameFilter((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 21: _t->d_func()->_q_selectionChanged(); break;
        case 22: _t->d_func()->_q_goToUrl((*reinterpret_cast< const QUrl(*)>(_a[1]))); break;
        case 23: _t->d_func()->_q_goHome(); break;
        case 24: _t->d_func()->_q_showHeader((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 25: _t->d_func()->_q_autoCompleteFileName((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 26: _t->d_func()->_q_rowsInserted((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 27: _t->d_func()->_q_fileRenamed((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (QFileDialog::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&QFileDialog::fileSelected)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (QFileDialog::*)(const QStringList & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&QFileDialog::filesSelected)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (QFileDialog::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&QFileDialog::currentChanged)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (QFileDialog::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&QFileDialog::directoryEntered)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (QFileDialog::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&QFileDialog::filterSelected)) {
                *result = 4;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<QFileDialog *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< ViewMode*>(_v) = _t->viewMode(); break;
        case 1: *reinterpret_cast< FileMode*>(_v) = _t->fileMode(); break;
        case 2: *reinterpret_cast< AcceptMode*>(_v) = _t->acceptMode(); break;
        case 3: *reinterpret_cast< bool*>(_v) = _t->isReadOnly(); break;
        case 4: *reinterpret_cast< bool*>(_v) = _t->resolveSymlinks(); break;
        case 5: *reinterpret_cast< bool*>(_v) = _t->confirmOverwrite(); break;
        case 6: *reinterpret_cast< QString*>(_v) = _t->defaultSuffix(); break;
        case 7: *reinterpret_cast< bool*>(_v) = _t->isNameFilterDetailsVisible(); break;
        case 8: *reinterpret_cast<int*>(_v) = QFlag(_t->options()); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        auto *_t = static_cast<QFileDialog *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setViewMode(*reinterpret_cast< ViewMode*>(_v)); break;
        case 1: _t->setFileMode(*reinterpret_cast< FileMode*>(_v)); break;
        case 2: _t->setAcceptMode(*reinterpret_cast< AcceptMode*>(_v)); break;
        case 3: _t->setReadOnly(*reinterpret_cast< bool*>(_v)); break;
        case 4: _t->setResolveSymlinks(*reinterpret_cast< bool*>(_v)); break;
        case 5: _t->setConfirmOverwrite(*reinterpret_cast< bool*>(_v)); break;
        case 6: _t->setDefaultSuffix(*reinterpret_cast< QString*>(_v)); break;
        case 7: _t->setNameFilterDetailsVisible(*reinterpret_cast< bool*>(_v)); break;
        case 8: _t->setOptions(QFlag(*reinterpret_cast<int*>(_v))); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

QT_INIT_METAOBJECT const QMetaObject QFileDialog::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_QFileDialog.data,
    qt_meta_data_QFileDialog,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *QFileDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QFileDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QFileDialog.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int QFileDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 28)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 28;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 28)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 28;
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 9;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 9;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 9;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 9;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 9;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void QFileDialog::fileSelected(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QFileDialog::filesSelected(const QStringList & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void QFileDialog::currentChanged(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void QFileDialog::directoryEntered(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void QFileDialog::filterSelected(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
