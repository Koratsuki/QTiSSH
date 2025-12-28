/****************************************************************************
** Meta object code from reading C++ file 'vt100parser.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../vt100parser.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'vt100parser.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_VT100Parser_t {
    uint offsetsAndSizes[126];
    char stringdata0[12];
    char stringdata1[13];
    char stringdata2[1];
    char stringdata3[5];
    char stringdata4[9];
    char stringdata5[6];
    char stringdata6[11];
    char stringdata7[14];
    char stringdata8[8];
    char stringdata9[15];
    char stringdata10[15];
    char stringdata11[4];
    char stringdata12[7];
    char stringdata13[15];
    char stringdata14[19];
    char stringdata15[25];
    char stringdata16[11];
    char stringdata17[14];
    char stringdata18[11];
    char stringdata19[11];
    char stringdata20[12];
    char stringdata21[22];
    char stringdata22[20];
    char stringdata23[10];
    char stringdata24[20];
    char stringdata25[18];
    char stringdata26[12];
    char stringdata27[6];
    char stringdata28[12];
    char stringdata29[17];
    char stringdata30[17];
    char stringdata31[16];
    char stringdata32[9];
    char stringdata33[11];
    char stringdata34[18];
    char stringdata35[14];
    char stringdata36[11];
    char stringdata37[11];
    char stringdata38[15];
    char stringdata39[11];
    char stringdata40[20];
    char stringdata41[19];
    char stringdata42[4];
    char stringdata43[7];
    char stringdata44[11];
    char stringdata45[13];
    char stringdata46[17];
    char stringdata47[11];
    char stringdata48[12];
    char stringdata49[16];
    char stringdata50[11];
    char stringdata51[8];
    char stringdata52[19];
    char stringdata53[4];
    char stringdata54[8];
    char stringdata55[5];
    char stringdata56[7];
    char stringdata57[15];
    char stringdata58[5];
    char stringdata59[19];
    char stringdata60[21];
    char stringdata61[17];
    char stringdata62[3];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_VT100Parser_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_VT100Parser_t qt_meta_stringdata_VT100Parser = {
    {
        QT_MOC_LITERAL(0, 11),  // "VT100Parser"
        QT_MOC_LITERAL(12, 12),  // "textReceived"
        QT_MOC_LITERAL(25, 0),  // ""
        QT_MOC_LITERAL(26, 4),  // "text"
        QT_MOC_LITERAL(31, 8),  // "cursorUp"
        QT_MOC_LITERAL(40, 5),  // "lines"
        QT_MOC_LITERAL(46, 10),  // "cursorDown"
        QT_MOC_LITERAL(57, 13),  // "cursorForward"
        QT_MOC_LITERAL(71, 7),  // "columns"
        QT_MOC_LITERAL(79, 14),  // "cursorBackward"
        QT_MOC_LITERAL(94, 14),  // "cursorPosition"
        QT_MOC_LITERAL(109, 3),  // "row"
        QT_MOC_LITERAL(113, 6),  // "column"
        QT_MOC_LITERAL(120, 14),  // "cursorNextLine"
        QT_MOC_LITERAL(135, 18),  // "cursorPreviousLine"
        QT_MOC_LITERAL(154, 24),  // "cursorHorizontalAbsolute"
        QT_MOC_LITERAL(179, 10),  // "saveCursor"
        QT_MOC_LITERAL(190, 13),  // "restoreCursor"
        QT_MOC_LITERAL(204, 10),  // "hideCursor"
        QT_MOC_LITERAL(215, 10),  // "showCursor"
        QT_MOC_LITERAL(226, 11),  // "clearScreen"
        QT_MOC_LITERAL(238, 21),  // "clearScreenFromCursor"
        QT_MOC_LITERAL(260, 19),  // "clearScreenToCursor"
        QT_MOC_LITERAL(280, 9),  // "clearLine"
        QT_MOC_LITERAL(290, 19),  // "clearLineFromCursor"
        QT_MOC_LITERAL(310, 17),  // "clearLineToCursor"
        QT_MOC_LITERAL(328, 11),  // "insertLines"
        QT_MOC_LITERAL(340, 5),  // "count"
        QT_MOC_LITERAL(346, 11),  // "deleteLines"
        QT_MOC_LITERAL(358, 16),  // "insertCharacters"
        QT_MOC_LITERAL(375, 16),  // "deleteCharacters"
        QT_MOC_LITERAL(392, 15),  // "eraseCharacters"
        QT_MOC_LITERAL(408, 8),  // "scrollUp"
        QT_MOC_LITERAL(417, 10),  // "scrollDown"
        QT_MOC_LITERAL(428, 17),  // "setTextAttributes"
        QT_MOC_LITERAL(446, 13),  // "TerminalColor"
        QT_MOC_LITERAL(460, 10),  // "foreground"
        QT_MOC_LITERAL(471, 10),  // "background"
        QT_MOC_LITERAL(482, 14),  // "TextAttributes"
        QT_MOC_LITERAL(497, 10),  // "attributes"
        QT_MOC_LITERAL(508, 19),  // "resetTextAttributes"
        QT_MOC_LITERAL(528, 18),  // "setScrollingRegion"
        QT_MOC_LITERAL(547, 3),  // "top"
        QT_MOC_LITERAL(551, 6),  // "bottom"
        QT_MOC_LITERAL(558, 10),  // "setTabStop"
        QT_MOC_LITERAL(569, 12),  // "clearTabStop"
        QT_MOC_LITERAL(582, 16),  // "clearAllTabStops"
        QT_MOC_LITERAL(599, 10),  // "tabForward"
        QT_MOC_LITERAL(610, 11),  // "tabBackward"
        QT_MOC_LITERAL(622, 15),  // "setCharacterSet"
        QT_MOC_LITERAL(638, 10),  // "designator"
        QT_MOC_LITERAL(649, 7),  // "charset"
        QT_MOC_LITERAL(657, 18),  // "selectCharacterSet"
        QT_MOC_LITERAL(676, 3),  // "set"
        QT_MOC_LITERAL(680, 7),  // "setMode"
        QT_MOC_LITERAL(688, 4),  // "mode"
        QT_MOC_LITERAL(693, 6),  // "enable"
        QT_MOC_LITERAL(700, 14),  // "setPrivateMode"
        QT_MOC_LITERAL(715, 4),  // "bell"
        QT_MOC_LITERAL(720, 18),  // "deviceStatusReport"
        QT_MOC_LITERAL(739, 20),  // "cursorPositionReport"
        QT_MOC_LITERAL(760, 16),  // "processCharacter"
        QT_MOC_LITERAL(777, 2)   // "ch"
    },
    "VT100Parser",
    "textReceived",
    "",
    "text",
    "cursorUp",
    "lines",
    "cursorDown",
    "cursorForward",
    "columns",
    "cursorBackward",
    "cursorPosition",
    "row",
    "column",
    "cursorNextLine",
    "cursorPreviousLine",
    "cursorHorizontalAbsolute",
    "saveCursor",
    "restoreCursor",
    "hideCursor",
    "showCursor",
    "clearScreen",
    "clearScreenFromCursor",
    "clearScreenToCursor",
    "clearLine",
    "clearLineFromCursor",
    "clearLineToCursor",
    "insertLines",
    "count",
    "deleteLines",
    "insertCharacters",
    "deleteCharacters",
    "eraseCharacters",
    "scrollUp",
    "scrollDown",
    "setTextAttributes",
    "TerminalColor",
    "foreground",
    "background",
    "TextAttributes",
    "attributes",
    "resetTextAttributes",
    "setScrollingRegion",
    "top",
    "bottom",
    "setTabStop",
    "clearTabStop",
    "clearAllTabStops",
    "tabForward",
    "tabBackward",
    "setCharacterSet",
    "designator",
    "charset",
    "selectCharacterSet",
    "set",
    "setMode",
    "mode",
    "enable",
    "setPrivateMode",
    "bell",
    "deviceStatusReport",
    "cursorPositionReport",
    "processCharacter",
    "ch"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_VT100Parser[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
      42,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      41,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,  266,    2, 0x06,    1 /* Public */,
       4,    1,  269,    2, 0x06,    3 /* Public */,
       6,    1,  272,    2, 0x06,    5 /* Public */,
       7,    1,  275,    2, 0x06,    7 /* Public */,
       9,    1,  278,    2, 0x06,    9 /* Public */,
      10,    2,  281,    2, 0x06,   11 /* Public */,
      13,    1,  286,    2, 0x06,   14 /* Public */,
      14,    1,  289,    2, 0x06,   16 /* Public */,
      15,    1,  292,    2, 0x06,   18 /* Public */,
      16,    0,  295,    2, 0x06,   20 /* Public */,
      17,    0,  296,    2, 0x06,   21 /* Public */,
      18,    0,  297,    2, 0x06,   22 /* Public */,
      19,    0,  298,    2, 0x06,   23 /* Public */,
      20,    0,  299,    2, 0x06,   24 /* Public */,
      21,    0,  300,    2, 0x06,   25 /* Public */,
      22,    0,  301,    2, 0x06,   26 /* Public */,
      23,    0,  302,    2, 0x06,   27 /* Public */,
      24,    0,  303,    2, 0x06,   28 /* Public */,
      25,    0,  304,    2, 0x06,   29 /* Public */,
      26,    1,  305,    2, 0x06,   30 /* Public */,
      28,    1,  308,    2, 0x06,   32 /* Public */,
      29,    1,  311,    2, 0x06,   34 /* Public */,
      30,    1,  314,    2, 0x06,   36 /* Public */,
      31,    1,  317,    2, 0x06,   38 /* Public */,
      32,    1,  320,    2, 0x06,   40 /* Public */,
      33,    1,  323,    2, 0x06,   42 /* Public */,
      34,    3,  326,    2, 0x06,   44 /* Public */,
      40,    0,  333,    2, 0x06,   48 /* Public */,
      41,    2,  334,    2, 0x06,   49 /* Public */,
      44,    0,  339,    2, 0x06,   52 /* Public */,
      45,    0,  340,    2, 0x06,   53 /* Public */,
      46,    0,  341,    2, 0x06,   54 /* Public */,
      47,    1,  342,    2, 0x06,   55 /* Public */,
      48,    1,  345,    2, 0x06,   57 /* Public */,
      49,    2,  348,    2, 0x06,   59 /* Public */,
      52,    1,  353,    2, 0x06,   62 /* Public */,
      54,    2,  356,    2, 0x06,   64 /* Public */,
      57,    2,  361,    2, 0x06,   67 /* Public */,
      58,    0,  366,    2, 0x06,   70 /* Public */,
      59,    0,  367,    2, 0x06,   71 /* Public */,
      60,    0,  368,    2, 0x06,   72 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
      61,    1,  369,    2, 0x08,   73 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   11,   12,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void, QMetaType::Int,   12,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   27,
    QMetaType::Void, QMetaType::Int,   27,
    QMetaType::Void, QMetaType::Int,   27,
    QMetaType::Void, QMetaType::Int,   27,
    QMetaType::Void, QMetaType::Int,   27,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void, 0x80000000 | 35, 0x80000000 | 35, 0x80000000 | 38,   36,   37,   39,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   42,   43,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   27,
    QMetaType::Void, QMetaType::Int,   27,
    QMetaType::Void, QMetaType::Char, QMetaType::Char,   50,   51,
    QMetaType::Void, QMetaType::Int,   53,
    QMetaType::Void, QMetaType::Int, QMetaType::Bool,   55,   56,
    QMetaType::Void, QMetaType::Int, QMetaType::Bool,   55,   56,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::QChar,   62,

       0        // eod
};

Q_CONSTINIT const QMetaObject VT100Parser::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_VT100Parser.offsetsAndSizes,
    qt_meta_data_VT100Parser,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_VT100Parser_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<VT100Parser, std::true_type>,
        // method 'textReceived'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'cursorUp'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'cursorDown'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'cursorForward'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'cursorBackward'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'cursorPosition'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'cursorNextLine'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'cursorPreviousLine'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'cursorHorizontalAbsolute'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'saveCursor'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'restoreCursor'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'hideCursor'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'showCursor'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'clearScreen'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'clearScreenFromCursor'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'clearScreenToCursor'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'clearLine'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'clearLineFromCursor'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'clearLineToCursor'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'insertLines'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'deleteLines'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'insertCharacters'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'deleteCharacters'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'eraseCharacters'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'scrollUp'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'scrollDown'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'setTextAttributes'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<TerminalColor, std::false_type>,
        QtPrivate::TypeAndForceComplete<TerminalColor, std::false_type>,
        QtPrivate::TypeAndForceComplete<TextAttributes, std::false_type>,
        // method 'resetTextAttributes'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'setScrollingRegion'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'setTabStop'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'clearTabStop'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'clearAllTabStops'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'tabForward'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'tabBackward'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'setCharacterSet'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<char, std::false_type>,
        QtPrivate::TypeAndForceComplete<char, std::false_type>,
        // method 'selectCharacterSet'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'setMode'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'setPrivateMode'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'bell'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'deviceStatusReport'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'cursorPositionReport'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'processCharacter'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QChar, std::false_type>
    >,
    nullptr
} };

void VT100Parser::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<VT100Parser *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->textReceived((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 1: _t->cursorUp((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 2: _t->cursorDown((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 3: _t->cursorForward((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 4: _t->cursorBackward((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 5: _t->cursorPosition((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 6: _t->cursorNextLine((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 7: _t->cursorPreviousLine((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 8: _t->cursorHorizontalAbsolute((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 9: _t->saveCursor(); break;
        case 10: _t->restoreCursor(); break;
        case 11: _t->hideCursor(); break;
        case 12: _t->showCursor(); break;
        case 13: _t->clearScreen(); break;
        case 14: _t->clearScreenFromCursor(); break;
        case 15: _t->clearScreenToCursor(); break;
        case 16: _t->clearLine(); break;
        case 17: _t->clearLineFromCursor(); break;
        case 18: _t->clearLineToCursor(); break;
        case 19: _t->insertLines((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 20: _t->deleteLines((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 21: _t->insertCharacters((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 22: _t->deleteCharacters((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 23: _t->eraseCharacters((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 24: _t->scrollUp((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 25: _t->scrollDown((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 26: _t->setTextAttributes((*reinterpret_cast< std::add_pointer_t<TerminalColor>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<TerminalColor>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<TextAttributes>>(_a[3]))); break;
        case 27: _t->resetTextAttributes(); break;
        case 28: _t->setScrollingRegion((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 29: _t->setTabStop(); break;
        case 30: _t->clearTabStop(); break;
        case 31: _t->clearAllTabStops(); break;
        case 32: _t->tabForward((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 33: _t->tabBackward((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 34: _t->setCharacterSet((*reinterpret_cast< std::add_pointer_t<char>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<char>>(_a[2]))); break;
        case 35: _t->selectCharacterSet((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 36: _t->setMode((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[2]))); break;
        case 37: _t->setPrivateMode((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[2]))); break;
        case 38: _t->bell(); break;
        case 39: _t->deviceStatusReport(); break;
        case 40: _t->cursorPositionReport(); break;
        case 41: _t->processCharacter((*reinterpret_cast< std::add_pointer_t<QChar>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (VT100Parser::*)(const QString & );
            if (_t _q_method = &VT100Parser::textReceived; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (VT100Parser::*)(int );
            if (_t _q_method = &VT100Parser::cursorUp; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (VT100Parser::*)(int );
            if (_t _q_method = &VT100Parser::cursorDown; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (VT100Parser::*)(int );
            if (_t _q_method = &VT100Parser::cursorForward; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (VT100Parser::*)(int );
            if (_t _q_method = &VT100Parser::cursorBackward; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (VT100Parser::*)(int , int );
            if (_t _q_method = &VT100Parser::cursorPosition; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (VT100Parser::*)(int );
            if (_t _q_method = &VT100Parser::cursorNextLine; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (VT100Parser::*)(int );
            if (_t _q_method = &VT100Parser::cursorPreviousLine; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (VT100Parser::*)(int );
            if (_t _q_method = &VT100Parser::cursorHorizontalAbsolute; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 8;
                return;
            }
        }
        {
            using _t = void (VT100Parser::*)();
            if (_t _q_method = &VT100Parser::saveCursor; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 9;
                return;
            }
        }
        {
            using _t = void (VT100Parser::*)();
            if (_t _q_method = &VT100Parser::restoreCursor; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 10;
                return;
            }
        }
        {
            using _t = void (VT100Parser::*)();
            if (_t _q_method = &VT100Parser::hideCursor; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 11;
                return;
            }
        }
        {
            using _t = void (VT100Parser::*)();
            if (_t _q_method = &VT100Parser::showCursor; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 12;
                return;
            }
        }
        {
            using _t = void (VT100Parser::*)();
            if (_t _q_method = &VT100Parser::clearScreen; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 13;
                return;
            }
        }
        {
            using _t = void (VT100Parser::*)();
            if (_t _q_method = &VT100Parser::clearScreenFromCursor; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 14;
                return;
            }
        }
        {
            using _t = void (VT100Parser::*)();
            if (_t _q_method = &VT100Parser::clearScreenToCursor; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 15;
                return;
            }
        }
        {
            using _t = void (VT100Parser::*)();
            if (_t _q_method = &VT100Parser::clearLine; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 16;
                return;
            }
        }
        {
            using _t = void (VT100Parser::*)();
            if (_t _q_method = &VT100Parser::clearLineFromCursor; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 17;
                return;
            }
        }
        {
            using _t = void (VT100Parser::*)();
            if (_t _q_method = &VT100Parser::clearLineToCursor; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 18;
                return;
            }
        }
        {
            using _t = void (VT100Parser::*)(int );
            if (_t _q_method = &VT100Parser::insertLines; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 19;
                return;
            }
        }
        {
            using _t = void (VT100Parser::*)(int );
            if (_t _q_method = &VT100Parser::deleteLines; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 20;
                return;
            }
        }
        {
            using _t = void (VT100Parser::*)(int );
            if (_t _q_method = &VT100Parser::insertCharacters; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 21;
                return;
            }
        }
        {
            using _t = void (VT100Parser::*)(int );
            if (_t _q_method = &VT100Parser::deleteCharacters; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 22;
                return;
            }
        }
        {
            using _t = void (VT100Parser::*)(int );
            if (_t _q_method = &VT100Parser::eraseCharacters; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 23;
                return;
            }
        }
        {
            using _t = void (VT100Parser::*)(int );
            if (_t _q_method = &VT100Parser::scrollUp; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 24;
                return;
            }
        }
        {
            using _t = void (VT100Parser::*)(int );
            if (_t _q_method = &VT100Parser::scrollDown; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 25;
                return;
            }
        }
        {
            using _t = void (VT100Parser::*)(TerminalColor , TerminalColor , TextAttributes );
            if (_t _q_method = &VT100Parser::setTextAttributes; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 26;
                return;
            }
        }
        {
            using _t = void (VT100Parser::*)();
            if (_t _q_method = &VT100Parser::resetTextAttributes; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 27;
                return;
            }
        }
        {
            using _t = void (VT100Parser::*)(int , int );
            if (_t _q_method = &VT100Parser::setScrollingRegion; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 28;
                return;
            }
        }
        {
            using _t = void (VT100Parser::*)();
            if (_t _q_method = &VT100Parser::setTabStop; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 29;
                return;
            }
        }
        {
            using _t = void (VT100Parser::*)();
            if (_t _q_method = &VT100Parser::clearTabStop; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 30;
                return;
            }
        }
        {
            using _t = void (VT100Parser::*)();
            if (_t _q_method = &VT100Parser::clearAllTabStops; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 31;
                return;
            }
        }
        {
            using _t = void (VT100Parser::*)(int );
            if (_t _q_method = &VT100Parser::tabForward; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 32;
                return;
            }
        }
        {
            using _t = void (VT100Parser::*)(int );
            if (_t _q_method = &VT100Parser::tabBackward; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 33;
                return;
            }
        }
        {
            using _t = void (VT100Parser::*)(char , char );
            if (_t _q_method = &VT100Parser::setCharacterSet; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 34;
                return;
            }
        }
        {
            using _t = void (VT100Parser::*)(int );
            if (_t _q_method = &VT100Parser::selectCharacterSet; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 35;
                return;
            }
        }
        {
            using _t = void (VT100Parser::*)(int , bool );
            if (_t _q_method = &VT100Parser::setMode; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 36;
                return;
            }
        }
        {
            using _t = void (VT100Parser::*)(int , bool );
            if (_t _q_method = &VT100Parser::setPrivateMode; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 37;
                return;
            }
        }
        {
            using _t = void (VT100Parser::*)();
            if (_t _q_method = &VT100Parser::bell; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 38;
                return;
            }
        }
        {
            using _t = void (VT100Parser::*)();
            if (_t _q_method = &VT100Parser::deviceStatusReport; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 39;
                return;
            }
        }
        {
            using _t = void (VT100Parser::*)();
            if (_t _q_method = &VT100Parser::cursorPositionReport; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 40;
                return;
            }
        }
    }
}

const QMetaObject *VT100Parser::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *VT100Parser::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_VT100Parser.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int VT100Parser::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 42)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 42;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 42)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 42;
    }
    return _id;
}

// SIGNAL 0
void VT100Parser::textReceived(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void VT100Parser::cursorUp(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void VT100Parser::cursorDown(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void VT100Parser::cursorForward(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void VT100Parser::cursorBackward(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void VT100Parser::cursorPosition(int _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void VT100Parser::cursorNextLine(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void VT100Parser::cursorPreviousLine(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void VT100Parser::cursorHorizontalAbsolute(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void VT100Parser::saveCursor()
{
    QMetaObject::activate(this, &staticMetaObject, 9, nullptr);
}

// SIGNAL 10
void VT100Parser::restoreCursor()
{
    QMetaObject::activate(this, &staticMetaObject, 10, nullptr);
}

// SIGNAL 11
void VT100Parser::hideCursor()
{
    QMetaObject::activate(this, &staticMetaObject, 11, nullptr);
}

// SIGNAL 12
void VT100Parser::showCursor()
{
    QMetaObject::activate(this, &staticMetaObject, 12, nullptr);
}

// SIGNAL 13
void VT100Parser::clearScreen()
{
    QMetaObject::activate(this, &staticMetaObject, 13, nullptr);
}

// SIGNAL 14
void VT100Parser::clearScreenFromCursor()
{
    QMetaObject::activate(this, &staticMetaObject, 14, nullptr);
}

// SIGNAL 15
void VT100Parser::clearScreenToCursor()
{
    QMetaObject::activate(this, &staticMetaObject, 15, nullptr);
}

// SIGNAL 16
void VT100Parser::clearLine()
{
    QMetaObject::activate(this, &staticMetaObject, 16, nullptr);
}

// SIGNAL 17
void VT100Parser::clearLineFromCursor()
{
    QMetaObject::activate(this, &staticMetaObject, 17, nullptr);
}

// SIGNAL 18
void VT100Parser::clearLineToCursor()
{
    QMetaObject::activate(this, &staticMetaObject, 18, nullptr);
}

// SIGNAL 19
void VT100Parser::insertLines(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 19, _a);
}

// SIGNAL 20
void VT100Parser::deleteLines(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 20, _a);
}

// SIGNAL 21
void VT100Parser::insertCharacters(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 21, _a);
}

// SIGNAL 22
void VT100Parser::deleteCharacters(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 22, _a);
}

// SIGNAL 23
void VT100Parser::eraseCharacters(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 23, _a);
}

// SIGNAL 24
void VT100Parser::scrollUp(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 24, _a);
}

// SIGNAL 25
void VT100Parser::scrollDown(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 25, _a);
}

// SIGNAL 26
void VT100Parser::setTextAttributes(TerminalColor _t1, TerminalColor _t2, TextAttributes _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 26, _a);
}

// SIGNAL 27
void VT100Parser::resetTextAttributes()
{
    QMetaObject::activate(this, &staticMetaObject, 27, nullptr);
}

// SIGNAL 28
void VT100Parser::setScrollingRegion(int _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 28, _a);
}

// SIGNAL 29
void VT100Parser::setTabStop()
{
    QMetaObject::activate(this, &staticMetaObject, 29, nullptr);
}

// SIGNAL 30
void VT100Parser::clearTabStop()
{
    QMetaObject::activate(this, &staticMetaObject, 30, nullptr);
}

// SIGNAL 31
void VT100Parser::clearAllTabStops()
{
    QMetaObject::activate(this, &staticMetaObject, 31, nullptr);
}

// SIGNAL 32
void VT100Parser::tabForward(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 32, _a);
}

// SIGNAL 33
void VT100Parser::tabBackward(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 33, _a);
}

// SIGNAL 34
void VT100Parser::setCharacterSet(char _t1, char _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 34, _a);
}

// SIGNAL 35
void VT100Parser::selectCharacterSet(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 35, _a);
}

// SIGNAL 36
void VT100Parser::setMode(int _t1, bool _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 36, _a);
}

// SIGNAL 37
void VT100Parser::setPrivateMode(int _t1, bool _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 37, _a);
}

// SIGNAL 38
void VT100Parser::bell()
{
    QMetaObject::activate(this, &staticMetaObject, 38, nullptr);
}

// SIGNAL 39
void VT100Parser::deviceStatusReport()
{
    QMetaObject::activate(this, &staticMetaObject, 39, nullptr);
}

// SIGNAL 40
void VT100Parser::cursorPositionReport()
{
    QMetaObject::activate(this, &staticMetaObject, 40, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
