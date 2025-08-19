/****************************************************************************
** Meta object code from reading C++ file 'test_engine.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../../tests/test_engine.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'test_engine.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.4.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
namespace {
struct qt_meta_stringdata_TestEngine_t {
    uint offsetsAndSizes[16];
    char stringdata0[11];
    char stringdata1[5];
    char stringdata2[1];
    char stringdata3[8];
    char stringdata4[18];
    char stringdata5[13];
    char stringdata6[20];
    char stringdata7[15];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_TestEngine_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_TestEngine_t qt_meta_stringdata_TestEngine = {
    {
        QT_MOC_LITERAL(0, 10),  // "TestEngine"
        QT_MOC_LITERAL(11, 4),  // "init"
        QT_MOC_LITERAL(16, 0),  // ""
        QT_MOC_LITERAL(17, 7),  // "cleanup"
        QT_MOC_LITERAL(25, 17),  // "testResponse_data"
        QT_MOC_LITERAL(43, 12),  // "testResponse"
        QT_MOC_LITERAL(56, 19),  // "testReflection_data"
        QT_MOC_LITERAL(76, 14)   // "testReflection"
    },
    "TestEngine",
    "init",
    "",
    "cleanup",
    "testResponse_data",
    "testResponse",
    "testReflection_data",
    "testReflection"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_TestEngine[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   50,    2, 0x08,    1 /* Private */,
       3,    0,   51,    2, 0x08,    2 /* Private */,
       4,    0,   52,    2, 0x08,    3 /* Private */,
       5,    0,   53,    2, 0x08,    4 /* Private */,
       6,    0,   54,    2, 0x08,    5 /* Private */,
       7,    0,   55,    2, 0x08,    6 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject TestEngine::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_TestEngine.offsetsAndSizes,
    qt_meta_data_TestEngine,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_TestEngine_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<TestEngine, std::true_type>,
        // method 'init'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'cleanup'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'testResponse_data'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'testResponse'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'testReflection_data'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'testReflection'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void TestEngine::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<TestEngine *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->init(); break;
        case 1: _t->cleanup(); break;
        case 2: _t->testResponse_data(); break;
        case 3: _t->testResponse(); break;
        case 4: _t->testReflection_data(); break;
        case 5: _t->testReflection(); break;
        default: ;
        }
    }
    (void)_a;
}

const QMetaObject *TestEngine::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TestEngine::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TestEngine.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int TestEngine::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 6;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
