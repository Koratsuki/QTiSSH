#ifndef TERMINALSCREEN_H
#define TERMINALSCREEN_H

#include <QObject>
#include <QVector>
#include <QRect>
#include "terminalchar.h"

/**
 * @brief Manages the terminal screen buffer, cursor, and scrolling
 */
class TerminalScreen : public QObject
{
    Q_OBJECT

public:
    explicit TerminalScreen(int rows = 24, int columns = 80, QObject *parent = nullptr);
    
    // Screen dimensions
    int rows() const { return m_rows; }
    int columns() const { return m_columns; }
    void resize(int rows, int columns);
    
    // Cursor operations
    CursorPosition cursorPosition() const { return m_cursorPos; }
    void setCursorPosition(int row, int column);
    void setCursorPosition(const CursorPosition &pos);
    void moveCursor(int deltaRow, int deltaColumn);
    void moveCursorToColumn(int column);
    void moveCursorToRow(int row);
    void saveCursor();
    void restoreCursor();
    bool isCursorVisible() const { return m_cursorVisible; }
    void setCursorVisible(bool visible);
    
    // Character operations
    TerminalChar getChar(int row, int column) const;
    void setChar(int row, int column, const TerminalChar &ch);
    void setChar(const TerminalChar &ch); // At current cursor position
    void insertChar(const TerminalChar &ch); // Insert and advance cursor
    void insertText(const QString &text);
    
    // Line operations
    QVector<TerminalChar> getLine(int row) const;
    void setLine(int row, const QVector<TerminalChar> &line);
    void clearLine(int row);
    void clearLineFromCursor();
    void clearLineToCursor();
    void insertLine(int row);
    void deleteLine(int row);
    
    // Screen operations
    void clear();
    void clearFromCursor();
    void clearToCursor();
    void scrollUp(int lines = 1);
    void scrollDown(int lines = 1);
    
    // Attributes
    void setCurrentAttributes(TerminalColor fg, TerminalColor bg, TextAttributes attr);
    void setCurrentForegroundColor(TerminalColor color);
    void setCurrentBackgroundColor(TerminalColor color);
    void setCurrentTextAttributes(TextAttributes attr);
    void resetAttributes();
    
    // Scrolling region
    void setScrollingRegion(int top, int bottom);
    void resetScrollingRegion();
    int scrollingRegionTop() const { return m_scrollTop; }
    int scrollingRegionBottom() const { return m_scrollBottom; }
    
    // Tab stops
    void setTabStop(int column);
    void clearTabStop(int column);
    void clearAllTabStops();
    void tabToNextStop();
    void tabToPreviousStop();
    
    // History/scrollback
    int historySize() const { return m_history.size(); }
    QVector<TerminalChar> getHistoryLine(int index) const;
    void setMaxHistorySize(int size);
    
    // Utility
    bool isValidPosition(int row, int column) const;
    void ensureCursorInBounds();
    
signals:
    void screenChanged(const QRect &region);
    void cursorPositionChanged(const CursorPosition &position);
    void cursorVisibilityChanged(bool visible);
    void screenResized(int rows, int columns);

private:
    void initializeScreen();
    void scrollUpInRegion(int lines);
    void scrollDownInRegion(int lines);
    void addLineToHistory(const QVector<TerminalChar> &line);
    void emitScreenChanged(int startRow, int endRow);
    
    // Screen buffer
    QVector<QVector<TerminalChar>> m_screen;
    QVector<QVector<TerminalChar>> m_history;
    
    // Dimensions
    int m_rows;
    int m_columns;
    int m_maxHistorySize;
    
    // Cursor state
    CursorPosition m_cursorPos;
    CursorPosition m_savedCursorPos;
    bool m_cursorVisible;
    
    // Current text attributes
    TerminalColor m_currentForeground;
    TerminalColor m_currentBackground;
    TextAttributes m_currentAttributes;
    
    // Scrolling region
    int m_scrollTop;
    int m_scrollBottom;
    
    // Tab stops
    QVector<bool> m_tabStops;
    
    // Default tab stop interval
    static const int DEFAULT_TAB_SIZE = 8;
};

#endif // TERMINALSCREEN_H
