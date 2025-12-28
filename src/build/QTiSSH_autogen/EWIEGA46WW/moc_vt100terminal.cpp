/****************************************************************************
** Meta object code from reading C++ file 'vt100terminal.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../vt100terminal.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'vt100terminal.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_VT100Terminal_t {
    uint offsetsAndSizes[132];
    char stringdata0[14];
    char stringdata1[11];
    char stringdata2[1];
    char stringdata3[5];
    char stringdata4[20];
    char stringdata5[5];
    char stringdata6[8];
    char stringdata7[5];
    char stringdata8[15];
    char stringdata9[5];
    char stringdata10[24];
    char stringdata11[15];
    char stringdata12[9];
    char stringdata13[26];
    char stringdata14[8];
    char stringdata15[16];
    char stringdata16[7];
    char stringdata17[16];
    char stringdata18[14];
    char stringdata19[24];
    char stringdata20[6];
    char stringdata21[11];
    char stringdata22[6];
    char stringdata23[13];
    char stringdata24[16];
    char stringdata25[17];
    char stringdata26[17];
    char stringdata27[4];
    char stringdata28[7];
    char stringdata29[17];
    char stringdata30[21];
    char stringdata31[27];
    char stringdata32[13];
    char stringdata33[16];
    char stringdata34[13];
    char stringdata35[13];
    char stringdata36[14];
    char stringdata37[24];
    char stringdata38[22];
    char stringdata39[12];
    char stringdata40[22];
    char stringdata41[20];
    char stringdata42[14];
    char stringdata43[6];
    char stringdata44[14];
    char stringdata45[19];
    char stringdata46[19];
    char stringdata47[18];
    char stringdata48[11];
    char stringdata49[13];
    char stringdata50[20];
    char stringdata51[14];
    char stringdata52[11];
    char stringdata53[11];
    char stringdata54[15];
    char stringdata55[11];
    char stringdata56[22];
    char stringdata57[21];
    char stringdata58[4];
    char stringdata59[7];
    char stringdata60[13];
    char stringdata61[15];
    char stringdata62[19];
    char stringdata63[13];
    char stringdata64[14];
    char stringdata65[7];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_VT100Terminal_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_VT100Terminal_t qt_meta_stringdata_VT100Terminal = {
    {
        QT_MOC_LITERAL(0, 13),  // "VT100Terminal"
        QT_MOC_LITERAL(14, 10),  // "keyPressed"
        QT_MOC_LITERAL(25, 0),  // ""
        QT_MOC_LITERAL(26, 4),  // "data"
        QT_MOC_LITERAL(31, 19),  // "terminalSizeChanged"
        QT_MOC_LITERAL(51, 4),  // "rows"
        QT_MOC_LITERAL(56, 7),  // "columns"
        QT_MOC_LITERAL(64, 4),  // "bell"
        QT_MOC_LITERAL(69, 14),  // "onTextReceived"
        QT_MOC_LITERAL(84, 4),  // "text"
        QT_MOC_LITERAL(89, 23),  // "onCursorPositionChanged"
        QT_MOC_LITERAL(113, 14),  // "CursorPosition"
        QT_MOC_LITERAL(128, 8),  // "position"
        QT_MOC_LITERAL(137, 25),  // "onCursorVisibilityChanged"
        QT_MOC_LITERAL(163, 7),  // "visible"
        QT_MOC_LITERAL(171, 15),  // "onScreenChanged"
        QT_MOC_LITERAL(187, 6),  // "region"
        QT_MOC_LITERAL(194, 15),  // "onScreenResized"
        QT_MOC_LITERAL(210, 13),  // "onCursorBlink"
        QT_MOC_LITERAL(224, 23),  // "onScrollBarValueChanged"
        QT_MOC_LITERAL(248, 5),  // "value"
        QT_MOC_LITERAL(254, 10),  // "onCursorUp"
        QT_MOC_LITERAL(265, 5),  // "lines"
        QT_MOC_LITERAL(271, 12),  // "onCursorDown"
        QT_MOC_LITERAL(284, 15),  // "onCursorForward"
        QT_MOC_LITERAL(300, 16),  // "onCursorBackward"
        QT_MOC_LITERAL(317, 16),  // "onCursorPosition"
        QT_MOC_LITERAL(334, 3),  // "row"
        QT_MOC_LITERAL(338, 6),  // "column"
        QT_MOC_LITERAL(345, 16),  // "onCursorNextLine"
        QT_MOC_LITERAL(362, 20),  // "onCursorPreviousLine"
        QT_MOC_LITERAL(383, 26),  // "onCursorHorizontalAbsolute"
        QT_MOC_LITERAL(410, 12),  // "onSaveCursor"
        QT_MOC_LITERAL(423, 15),  // "onRestoreCursor"
        QT_MOC_LITERAL(439, 12),  // "onHideCursor"
        QT_MOC_LITERAL(452, 12),  // "onShowCursor"
        QT_MOC_LITERAL(465, 13),  // "onClearScreen"
        QT_MOC_LITERAL(479, 23),  // "onClearScreenFromCursor"
        QT_MOC_LITERAL(503, 21),  // "onClearScreenToCursor"
        QT_MOC_LITERAL(525, 11),  // "onClearLine"
        QT_MOC_LITERAL(537, 21),  // "onClearLineFromCursor"
        QT_MOC_LITERAL(559, 19),  // "onClearLineToCursor"
        QT_MOC_LITERAL(579, 13),  // "onInsertLines"
        QT_MOC_LITERAL(593, 5),  // "count"
        QT_MOC_LITERAL(599, 13),  // "onDeleteLines"
        QT_MOC_LITERAL(613, 18),  // "onInsertCharacters"
        QT_MOC_LITERAL(632, 18),  // "onDeleteCharacters"
        QT_MOC_LITERAL(651, 17),  // "onEraseCharacters"
        QT_MOC_LITERAL(669, 10),  // "onScrollUp"
        QT_MOC_LITERAL(680, 12),  // "onScrollDown"
        QT_MOC_LITERAL(693, 19),  // "onSetTextAttributes"
        QT_MOC_LITERAL(713, 13),  // "TerminalColor"
        QT_MOC_LITERAL(727, 10),  // "foreground"
        QT_MOC_LITERAL(738, 10),  // "background"
        QT_MOC_LITERAL(749, 14),  // "TextAttributes"
        QT_MOC_LITERAL(764, 10),  // "attributes"
        QT_MOC_LITERAL(775, 21),  // "onResetTextAttributes"
        QT_MOC_LITERAL(797, 20),  // "onSetScrollingRegion"
        QT_MOC_LITERAL(818, 3),  // "top"
        QT_MOC_LITERAL(822, 6),  // "bottom"
        QT_MOC_LITERAL(829, 12),  // "onSetTabStop"
        QT_MOC_LITERAL(842, 14),  // "onClearTabStop"
        QT_MOC_LITERAL(857, 18),  // "onClearAllTabStops"
        QT_MOC_LITERAL(876, 12),  // "onTabForward"
        QT_MOC_LITERAL(889, 13),  // "onTabBackward"
        QT_MOC_LITERAL(903, 6)   // "onBell"
    },
    "VT100Terminal",
    "keyPressed",
    "",
    "data",
    "terminalSizeChanged",
    "rows",
    "columns",
    "bell",
    "onTextReceived",
    "text",
    "onCursorPositionChanged",
    "CursorPosition",
    "position",
    "onCursorVisibilityChanged",
    "visible",
    "onScreenChanged",
    "region",
    "onScreenResized",
    "onCursorBlink",
    "onScrollBarValueChanged",
    "value",
    "onCursorUp",
    "lines",
    "onCursorDown",
    "onCursorForward",
    "onCursorBackward",
    "onCursorPosition",
    "row",
    "column",
    "onCursorNextLine",
    "onCursorPreviousLine",
    "onCursorHorizontalAbsolute",
    "onSaveCursor",
    "onRestoreCursor",
    "onHideCursor",
    "onShowCursor",
    "onClearScreen",
    "onClearScreenFromCursor",
    "onClearScreenToCursor",
    "onClearLine",
    "onClearLineFromCursor",
    "onClearLineToCursor",
    "onInsertLines",
    "count",
    "onDeleteLines",
    "onInsertCharacters",
    "onDeleteCharacters",
    "onEraseCharacters",
    "onScrollUp",
    "onScrollDown",
    "onSetTextAttributes",
    "TerminalColor",
    "foreground",
    "background",
    "TextAttributes",
    "attributes",
    "onResetTextAttributes",
    "onSetScrollingRegion",
    "top",
    "bottom",
    "onSetTabStop",
    "onClearTabStop",
    "onClearAllTabStops",
    "onTabForward",
    "onTabBackward",
    "onBell"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_VT100Terminal[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
      44,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,  278,    2, 0x06,    1 /* Public */,
       4,    2,  281,    2, 0x06,    3 /* Public */,
       7,    0,  286,    2, 0x06,    6 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       8,    1,  287,    2, 0x08,    7 /* Private */,
      10,    1,  290,    2, 0x08,    9 /* Private */,
      13,    1,  293,    2, 0x08,   11 /* Private */,
      15,    1,  296,    2, 0x08,   13 /* Private */,
      17,    2,  299,    2, 0x08,   15 /* Private */,
      18,    0,  304,    2, 0x08,   18 /* Private */,
      19,    1,  305,    2, 0x08,   19 /* Private */,
      21,    1,  308,    2, 0x08,   21 /* Private */,
      23,    1,  311,    2, 0x08,   23 /* Private */,
      24,    1,  314,    2, 0x08,   25 /* Private */,
      25,    1,  317,    2, 0x08,   27 /* Private */,
      26,    2,  320,    2, 0x08,   29 /* Private */,
      29,    1,  325,    2, 0x08,   32 /* Private */,
      30,    1,  328,    2, 0x08,   34 /* Private */,
      31,    1,  331,    2, 0x08,   36 /* Private */,
      32,    0,  334,    2, 0x08,   38 /* Private */,
      33,    0,  335,    2, 0x08,   39 /* Private */,
      34,    0,  336,    2, 0x08,   40 /* Private */,
      35,    0,  337,    2, 0x08,   41 /* Private */,
      36,    0,  338,    2, 0x08,   42 /* Private */,
      37,    0,  339,    2, 0x08,   43 /* Private */,
      38,    0,  340,    2, 0x08,   44 /* Private */,
      39,    0,  341,    2, 0x08,   45 /* Private */,
      40,    0,  342,    2, 0x08,   46 /* Private */,
      41,    0,  343,    2, 0x08,   47 /* Private */,
      42,    1,  344,    2, 0x08,   48 /* Private */,
      44,    1,  347,    2, 0x08,   50 /* Private */,
      45,    1,  350,    2, 0x08,   52 /* Private */,
      46,    1,  353,    2, 0x08,   54 /* Private */,
      47,    1,  356,    2, 0x08,   56 /* Private */,
      48,    1,  359,    2, 0x08,   58 /* Private */,
      49,    1,  362,    2, 0x08,   60 /* Private */,
      50,    3,  365,    2, 0x08,   62 /* Private */,
      56,    0,  372,    2, 0x08,   66 /* Private */,
      57,    2,  373,    2, 0x08,   67 /* Private */,
      60,    0,  378,    2, 0x08,   70 /* Private */,
      61,    0,  379,    2, 0x08,   71 /* Private */,
      62,    0,  380,    2, 0x08,   72 /* Private */,
      63,    1,  381,    2, 0x08,   73 /* Private */,
      64,    1,  384,    2, 0x08,   75 /* Private */,
      65,    0,  387,    2, 0x08,   77 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QByteArray,    3,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    5,    6,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void, 0x80000000 | 11,   12,
    QMetaType::Void, QMetaType::Bool,   14,
    QMetaType::Void, QMetaType::QRect,   16,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    5,    6,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   20,
    QMetaType::Void, QMetaType::Int,   22,
    QMetaType::Void, QMetaType::Int,   22,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   27,   28,
    QMetaType::Void, QMetaType::Int,   22,
    QMetaType::Void, QMetaType::Int,   22,
    QMetaType::Void, QMetaType::Int,   28,
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
    QMetaType::Void, QMetaType::Int,   43,
    QMetaType::Void, QMetaType::Int,   43,
    QMetaType::Void, QMetaType::Int,   43,
    QMetaType::Void, QMetaType::Int,   43,
    QMetaType::Void, QMetaType::Int,   43,
    QMetaType::Void, QMetaType::Int,   22,
    QMetaType::Void, QMetaType::Int,   22,
    QMetaType::Void, 0x80000000 | 51, 0x80000000 | 51, 0x80000000 | 54,   52,   53,   55,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   58,   59,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   43,
    QMetaType::Void, QMetaType::Int,   43,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject VT100Terminal::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_VT100Terminal.offsetsAndSizes,
    qt_meta_data_VT100Terminal,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_VT100Terminal_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<VT100Terminal, std::true_type>,
        // method 'keyPressed'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QByteArray &, std::false_type>,
        // method 'terminalSizeChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'bell'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onTextReceived'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'onCursorPositionChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const CursorPosition &, std::false_type>,
        // method 'onCursorVisibilityChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'onScreenChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QRect &, std::false_type>,
        // method 'onScreenResized'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onCursorBlink'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onScrollBarValueChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onCursorUp'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onCursorDown'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onCursorForward'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onCursorBackward'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onCursorPosition'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onCursorNextLine'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onCursorPreviousLine'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onCursorHorizontalAbsolute'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onSaveCursor'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onRestoreCursor'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onHideCursor'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onShowCursor'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onClearScreen'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onClearScreenFromCursor'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onClearScreenToCursor'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onClearLine'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onClearLineFromCursor'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onClearLineToCursor'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onInsertLines'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onDeleteLines'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onInsertCharacters'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onDeleteCharacters'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onEraseCharacters'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onScrollUp'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onScrollDown'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onSetTextAttributes'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<TerminalColor, std::false_type>,
        QtPrivate::TypeAndForceComplete<TerminalColor, std::false_type>,
        QtPrivate::TypeAndForceComplete<TextAttributes, std::false_type>,
        // method 'onResetTextAttributes'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onSetScrollingRegion'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onSetTabStop'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onClearTabStop'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onClearAllTabStops'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onTabForward'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onTabBackward'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onBell'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void VT100Terminal::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<VT100Terminal *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->keyPressed((*reinterpret_cast< std::add_pointer_t<QByteArray>>(_a[1]))); break;
        case 1: _t->terminalSizeChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 2: _t->bell(); break;
        case 3: _t->onTextReceived((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 4: _t->onCursorPositionChanged((*reinterpret_cast< std::add_pointer_t<CursorPosition>>(_a[1]))); break;
        case 5: _t->onCursorVisibilityChanged((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 6: _t->onScreenChanged((*reinterpret_cast< std::add_pointer_t<QRect>>(_a[1]))); break;
        case 7: _t->onScreenResized((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 8: _t->onCursorBlink(); break;
        case 9: _t->onScrollBarValueChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 10: _t->onCursorUp((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 11: _t->onCursorDown((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 12: _t->onCursorForward((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 13: _t->onCursorBackward((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 14: _t->onCursorPosition((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 15: _t->onCursorNextLine((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 16: _t->onCursorPreviousLine((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 17: _t->onCursorHorizontalAbsolute((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 18: _t->onSaveCursor(); break;
        case 19: _t->onRestoreCursor(); break;
        case 20: _t->onHideCursor(); break;
        case 21: _t->onShowCursor(); break;
        case 22: _t->onClearScreen(); break;
        case 23: _t->onClearScreenFromCursor(); break;
        case 24: _t->onClearScreenToCursor(); break;
        case 25: _t->onClearLine(); break;
        case 26: _t->onClearLineFromCursor(); break;
        case 27: _t->onClearLineToCursor(); break;
        case 28: _t->onInsertLines((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 29: _t->onDeleteLines((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 30: _t->onInsertCharacters((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 31: _t->onDeleteCharacters((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 32: _t->onEraseCharacters((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 33: _t->onScrollUp((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 34: _t->onScrollDown((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 35: _t->onSetTextAttributes((*reinterpret_cast< std::add_pointer_t<TerminalColor>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<TerminalColor>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<TextAttributes>>(_a[3]))); break;
        case 36: _t->onResetTextAttributes(); break;
        case 37: _t->onSetScrollingRegion((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 38: _t->onSetTabStop(); break;
        case 39: _t->onClearTabStop(); break;
        case 40: _t->onClearAllTabStops(); break;
        case 41: _t->onTabForward((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 42: _t->onTabBackward((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 43: _t->onBell(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (VT100Terminal::*)(const QByteArray & );
            if (_t _q_method = &VT100Terminal::keyPressed; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (VT100Terminal::*)(int , int );
            if (_t _q_method = &VT100Terminal::terminalSizeChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (VT100Terminal::*)();
            if (_t _q_method = &VT100Terminal::bell; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject *VT100Terminal::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *VT100Terminal::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_VT100Terminal.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int VT100Terminal::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 44)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 44;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 44)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 44;
    }
    return _id;
}

// SIGNAL 0
void VT100Terminal::keyPressed(const QByteArray & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void VT100Terminal::terminalSizeChanged(int _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void VT100Terminal::bell()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
