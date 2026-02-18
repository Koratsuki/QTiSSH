#ifndef VT100PARSER_H
#define VT100PARSER_H

#include <QObject>
#include <QString>
#include <QVector>
#include "terminalchar.h"

/**
 * @brief Parser state for VT100 escape sequence processing
 */
enum class ParserState {
    Normal,          // Normal text processing
    Escape,          // ESC received, waiting for next character
    CSI,             // Control Sequence Introducer (ESC[)
    OSC,             // Operating System Command (ESC])
    DCS,             // Device Control String (ESC P)
    SOS,             // Start of String (ESC X)
    PM,              // Privacy Message (ESC ^)
    APC              // Application Program Command (ESC _)
};

/**
 * @brief VT100 escape sequence parser
 * 
 * This class parses VT100/ANSI escape sequences and emits signals
 * for terminal operations like cursor movement, text formatting, etc.
 */
class VT100Parser : public QObject
{
    Q_OBJECT

public:
    explicit VT100Parser(QObject *parent = nullptr);
    
    /**
     * @brief Process input data and parse escape sequences
     * @param data Input data to process
     */
    void processData(const QByteArray &data);
    void processData(const QString &data);
    
    /**
     * @brief Reset parser to initial state
     */
    void reset();

signals:
    // Text output
    void textReceived(const QString &text);
    
    // Cursor operations
    void cursorUp(int lines);
    void cursorDown(int lines);
    void cursorForward(int columns);
    void cursorBackward(int columns);
    void cursorPosition(int row, int column);
    void cursorNextLine(int lines);
    void cursorPreviousLine(int lines);
    void cursorHorizontalAbsolute(int column);
    void saveCursor();
    void restoreCursor();
    void hideCursor();
    void showCursor();
    
    // Screen operations
    void clearScreen();
    void clearScreenFromCursor();
    void clearScreenToCursor();
    void clearLine();
    void clearLineFromCursor();
    void clearLineToCursor();
    void insertLines(int count);
    void deleteLines(int count);
    void insertCharacters(int count);
    void deleteCharacters(int count);
    void eraseCharacters(int count);
    void scrollUp(int lines);
    void scrollDown(int lines);
    
    // Text attributes
    void setTextAttributes(TerminalColor foreground, TerminalColor background, TextAttributes attributes);
    void resetTextAttributes();
    
    // Scrolling region
    void setScrollingRegion(int top, int bottom);
    
    // Tab operations
    void setTabStop();
    void clearTabStop();
    void clearAllTabStops();
    void tabForward(int count);
    void tabBackward(int count);
    
    // Character set operations
    void setCharacterSet(char designator, char charset);
    void selectCharacterSet(int set);
    
    // Mode operations
    void setMode(int mode, bool enable);
    void setPrivateMode(int mode, bool enable);
    void useAlternateScreenBuffer(bool use);
    
    // Bell
    void bell();
    
    // Device status
    void deviceStatusReport();
    void cursorPositionReport();

private slots:
    void processCharacter(QChar ch);

private:
    void processNormalCharacter(QChar ch);
    void processEscapeSequence(QChar ch);
    void processCSISequence(QChar ch);
    void processOSCSequence(QChar ch);
    void processDCSSequence(QChar ch);
    
    void executeCSICommand();
    void executeOSCCommand();
    void executeDCSCommand();
    
    void parseCSIParameters();
    void parseOSCParameters();
    
    void resetSequence();
    
    // Control character handlers
    void handleControlCharacter(QChar ch);
    void handleBell();
    void handleBackspace();
    void handleTab();
    void handleLineFeed();
    void handleVerticalTab();
    void handleFormFeed();
    void handleCarriageReturn();
    
    // CSI command handlers
    void handleCursorUp();
    void handleCursorDown();
    void handleCursorForward();
    void handleCursorBackward();
    void handleCursorNextLine();
    void handleCursorPreviousLine();
    void handleCursorHorizontalAbsolute();
    void handleCursorPosition();
    void handleEraseInDisplay();
    void handleEraseInLine();
    void handleInsertLines();
    void handleDeleteLines();
    void handleInsertCharacters();
    void handleDeleteCharacters();
    void handleEraseCharacters();
    void handleScrollUp();
    void handleScrollDown();
    void handleSelectGraphicRendition();
    void handleSetScrollingRegion();
    void handleSaveCursor();
    void handleRestoreCursor();
    void handleDeviceStatusReport();
    void handleSetMode();
    void handleResetMode();
    void handleSetPrivateMode();
    void handleResetPrivateMode();
    
    // Attribute parsing
    void parseGraphicRendition(const QVector<int> &params);
    TerminalColor parseColor(int colorCode, bool bright = false);
    
    // Parser state
    ParserState m_state;
    QString m_sequence;
    QVector<int> m_parameters;
    QString m_intermediateChars;
    QChar m_finalChar;
    
    // Current text attributes
    TerminalColor m_currentForeground;
    TerminalColor m_currentBackground;
    TextAttributes m_currentAttributes;
    
    // Character sets
    char m_characterSets[4];
    int m_currentCharacterSet;
    
    // Pending text buffer
    QString m_textBuffer;

    QChar mapCharacter(QChar ch);
    
    // Constants
    static const int DEFAULT_PARAMETER = -1;
    static const int MAX_PARAMETERS = 16;
};

#endif // VT100PARSER_H
