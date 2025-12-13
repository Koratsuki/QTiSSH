#ifndef TERMINALCHAR_H
#define TERMINALCHAR_H

#include <QChar>
#include <QColor>
#include <QFont>

/**
 * @brief Terminal color enumeration for standard VT100 colors
 */
enum class TerminalColor : int {
    Black = 0,
    Red = 1,
    Green = 2,
    Yellow = 3,
    Blue = 4,
    Magenta = 5,
    Cyan = 6,
    White = 7,
    BrightBlack = 8,
    BrightRed = 9,
    BrightGreen = 10,
    BrightYellow = 11,
    BrightBlue = 12,
    BrightMagenta = 13,
    BrightCyan = 14,
    BrightWhite = 15,
    Default = 256  // Use default terminal colors
};

/**
 * @brief Text attribute flags for terminal characters
 */
enum class TextAttribute : int {
    None = 0x00,
    Bold = 0x01,
    Dim = 0x02,
    Italic = 0x04,
    Underline = 0x08,
    Blink = 0x10,
    Reverse = 0x20,
    Strikethrough = 0x40
};

Q_DECLARE_FLAGS(TextAttributes, TextAttribute)
Q_DECLARE_OPERATORS_FOR_FLAGS(TextAttributes)

// Define bitwise NOT operator for TextAttribute
inline TextAttributes operator~(TextAttribute attr) {
    return static_cast<TextAttributes>(~static_cast<int>(attr));
}

/**
 * @brief Represents a single character in the terminal with its attributes
 */
struct TerminalChar {
    QChar character;
    TerminalColor foregroundColor;
    TerminalColor backgroundColor;
    TextAttributes attributes;
    
    TerminalChar()
        : character(' ')
        , foregroundColor(TerminalColor::Default)
        , backgroundColor(TerminalColor::Default)
        , attributes(TextAttribute::None)
    {
    }
    
    TerminalChar(QChar ch)
        : character(ch)
        , foregroundColor(TerminalColor::Default)
        , backgroundColor(TerminalColor::Default)
        , attributes(TextAttribute::None)
    {
    }
    
    TerminalChar(QChar ch, TerminalColor fg, TerminalColor bg, TextAttributes attr = TextAttribute::None)
        : character(ch)
        , foregroundColor(fg)
        , backgroundColor(bg)
        , attributes(attr)
    {
    }
    
    bool operator==(const TerminalChar &other) const {
        return character == other.character &&
               foregroundColor == other.foregroundColor &&
               backgroundColor == other.backgroundColor &&
               attributes == other.attributes;
    }
    
    bool operator!=(const TerminalChar &other) const {
        return !(*this == other);
    }
    
    /**
     * @brief Check if this character is empty (space with default attributes)
     */
    bool isEmpty() const {
        return character == ' ' &&
               foregroundColor == TerminalColor::Default &&
               backgroundColor == TerminalColor::Default &&
               attributes == TextAttribute::None;
    }
    
    /**
     * @brief Reset character to default state
     */
    void reset() {
        character = ' ';
        foregroundColor = TerminalColor::Default;
        backgroundColor = TerminalColor::Default;
        attributes = TextAttribute::None;
    }
};

/**
 * @brief Cursor position in the terminal
 */
struct CursorPosition {
    int row;
    int column;
    
    CursorPosition(int r = 0, int c = 0) : row(r), column(c) {}
    
    bool operator==(const CursorPosition &other) const {
        return row == other.row && column == other.column;
    }
    
    bool operator!=(const CursorPosition &other) const {
        return !(*this == other);
    }
};

/**
 * @brief Terminal color palette for converting TerminalColor to QColor
 */
class TerminalColorPalette {
public:
    static QColor getColor(TerminalColor color, bool bright = false);
    static QColor getDefaultForeground();
    static QColor getDefaultBackground();
    
private:
    static const QColor standardColors[16];
    static const QColor defaultForeground;
    static const QColor defaultBackground;
};

#endif // TERMINALCHAR_H
