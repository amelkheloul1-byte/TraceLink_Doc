/****************************************************************************
** Meta object code from reading C++ file 'GestionnaireFiltresGUI.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../headers/GestionnaireFiltresGUI.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'GestionnaireFiltresGUI.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.11.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN22GestionnaireFiltresGUIE_t {};
} // unnamed namespace

template <> constexpr inline auto GestionnaireFiltresGUI::qt_create_metaobjectdata<qt_meta_tag_ZN22GestionnaireFiltresGUIE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "GestionnaireFiltresGUI",
        "filtreModifie",
        "",
        "FiltreGUI",
        "filtre",
        "reinitialiser",
        "appliquerFiltres"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'filtreModifie'
        QtMocHelpers::SignalData<void(const FiltreGUI &)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Slot 'reinitialiser'
        QtMocHelpers::SlotData<void()>(5, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'appliquerFiltres'
        QtMocHelpers::SlotData<void()>(6, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<GestionnaireFiltresGUI, qt_meta_tag_ZN22GestionnaireFiltresGUIE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject GestionnaireFiltresGUI::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN22GestionnaireFiltresGUIE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN22GestionnaireFiltresGUIE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN22GestionnaireFiltresGUIE_t>.metaTypes,
    nullptr
} };

void GestionnaireFiltresGUI::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<GestionnaireFiltresGUI *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->filtreModifie((*reinterpret_cast<std::add_pointer_t<FiltreGUI>>(_a[1]))); break;
        case 1: _t->reinitialiser(); break;
        case 2: _t->appliquerFiltres(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (GestionnaireFiltresGUI::*)(const FiltreGUI & )>(_a, &GestionnaireFiltresGUI::filtreModifie, 0))
            return;
    }
}

const QMetaObject *GestionnaireFiltresGUI::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GestionnaireFiltresGUI::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN22GestionnaireFiltresGUIE_t>.strings))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int GestionnaireFiltresGUI::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void GestionnaireFiltresGUI::filtreModifie(const FiltreGUI & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}
QT_WARNING_POP
