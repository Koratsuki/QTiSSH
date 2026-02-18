#include "terminalscreen.h"
#include <QDebug>

TerminalScreen::TerminalScreen(int rows, int columns, QObject *parent)
    : QObject(parent)
    , m_rows(rows)
    , m_columns(columns)
    , m_maxHistorySize(1000)
    , m_cursorPos(0, 0)
    , m_savedCursorPos(0, 0)
    , m_cursorVisible(true)
    , m_currentForeground(TerminalColor::Default)
    , m_currentBackground(TerminalColor::Default)
    , m_currentAttributes(TextAttribute::None)
    , m_scrollTop(0)
    , m_scrollBottom(rows - 1)
    , m_useAlternateBuffer(false)
{
    initializeScreen();
    m_currentScreen = &m_mainScreen;
}

void TerminalScreen::initializeScreen()
{
    // Initialize main screen buffer
    m_mainScreen.resize(m_rows);
    m_alternateScreen.resize(m_rows);
    for (int i = 0; i < m_rows; ++i) {
        m_mainScreen[i].resize(m_columns);
        m_alternateScreen[i].resize(m_columns);
        for (int j = 0; j < m_columns; ++j) {
            m_mainScreen[i][j] = TerminalChar();
            m_alternateScreen[i][j] = TerminalChar();
        }
    }
    
    // Initialize tab stops (every 8 columns by default)
    m_tabStops.resize(m_columns);
    for (int i = 0; i < m_columns; ++i) {
        m_tabStops[i] = (i % DEFAULT_TAB_SIZE == 0);
    }
}

void TerminalScreen::resize(int rows, int columns)
{
    if (rows == m_rows && columns == m_columns) {
        return;
    }
    
    // Save current screen content
    QVector<QVector<TerminalChar>> oldScreen = (*m_currentScreen);
    int oldRows = m_rows;
    int oldColumns = m_columns;
    
    // Update dimensions
    m_rows = rows;
    m_columns = columns;
    m_scrollBottom = rows - 1;
    
    // Reinitialize screen
    initializeScreen();
    
    // Copy old content to new screen
    int copyRows = qMin(oldRows, m_rows);
    int copyColumns = qMin(oldColumns, m_columns);
    
    for (int i = 0; i < copyRows; ++i) {
        for (int j = 0; j < copyColumns; ++j) {
            (*m_currentScreen)[i][j] = oldScreen[i][j];
        }
    }
    
    // Ensure cursor is in bounds
    ensureCursorInBounds();
    
    emit screenResized(m_rows, m_columns);
    emit screenChanged(QRect(0, 0, m_columns, m_rows));
}

void TerminalScreen::setCursorPosition(int row, int column)
{
    CursorPosition oldPos = m_cursorPos;
    m_cursorPos.row = qBound(0, row, m_rows - 1);
    m_cursorPos.column = qBound(0, column, m_columns - 1);
    
    if (m_cursorPos != oldPos) {
        emit cursorPositionChanged(m_cursorPos);
    }
}

void TerminalScreen::setCursorPosition(const CursorPosition &pos)
{
    setCursorPosition(pos.row, pos.column);
}

void TerminalScreen::moveCursor(int deltaRow, int deltaColumn)
{
    setCursorPosition(m_cursorPos.row + deltaRow, m_cursorPos.column + deltaColumn);
}

void TerminalScreen::moveCursorToColumn(int column)
{
    setCursorPosition(m_cursorPos.row, column);
}

void TerminalScreen::moveCursorToRow(int row)
{
    setCursorPosition(row, m_cursorPos.column);
}

void TerminalScreen::saveCursor()
{
    m_savedCursorPos = m_cursorPos;
}

void TerminalScreen::restoreCursor()
{
    setCursorPosition(m_savedCursorPos);
}

void TerminalScreen::setCursorVisible(bool visible)
{
    if (m_cursorVisible != visible) {
        m_cursorVisible = visible;
        emit cursorVisibilityChanged(visible);
    }
}

TerminalChar TerminalScreen::getChar(int row, int column) const
{
    if (!isValidPosition(row, column)) {
        return TerminalChar();
    }
    return (*m_currentScreen)[row][column];
}

void TerminalScreen::setChar(int row, int column, const TerminalChar &ch)
{
    if (!isValidPosition(row, column)) {
        return;
    }
    
    (*m_currentScreen)[row][column] = ch;
    emit screenChanged(QRect(column, row, 1, 1));
}

void TerminalScreen::setChar(const TerminalChar &ch)
{
    setChar(m_cursorPos.row, m_cursorPos.column, ch);
}

void TerminalScreen::insertChar(const TerminalChar &ch)
{
    // Create character with current attributes if not specified
    TerminalChar charToInsert = ch;
    if (charToInsert.foregroundColor == TerminalColor::Default) {
        charToInsert.foregroundColor = m_currentForeground;
    }
    if (charToInsert.backgroundColor == TerminalColor::Default) {
        charToInsert.backgroundColor = m_currentBackground;
    }
    if (charToInsert.attributes == TextAttribute::None) {
        charToInsert.attributes = m_currentAttributes;
    }
    
    setChar(charToInsert);
    
    // Advance cursor
    if (m_cursorPos.column < m_columns - 1) {
        moveCursor(0, 1);
    } else {
        // Line wrap
        if (m_cursorPos.row < m_rows - 1) {
            setCursorPosition(m_cursorPos.row + 1, 0);
        } else {
            // Scroll up if at bottom
            scrollUp(1);
            setCursorPosition(m_rows - 1, 0);
        }
    }
}

void TerminalScreen::insertText(const QString &text)
{
    for (const QChar &ch : text) {
        if (ch == '\n') {
            // Newline: move to next line, column 0
            if (m_cursorPos.row < m_rows - 1) {
                setCursorPosition(m_cursorPos.row + 1, 0);
            } else {
                scrollUp(1);
                setCursorPosition(m_rows - 1, 0);
            }
        } else if (ch == '\r') {
            // Carriage return: move to column 0
            moveCursorToColumn(0);
        } else if (ch == '\t') {
            // Tab: move to next tab stop
            tabToNextStop();
        } else if (ch.isPrint()) {
            // Regular printable character
            insertChar(TerminalChar(ch, m_currentForeground, m_currentBackground, m_currentAttributes));
        }
        // Ignore other control characters for now
    }
}

QVector<TerminalChar> TerminalScreen::getLine(int row) const
{
    if (row < 0 || row >= m_rows) {
        return QVector<TerminalChar>();
    }
    return (*m_currentScreen)[row];
}

void TerminalScreen::setLine(int row, const QVector<TerminalChar> &line)
{
    if (row < 0 || row >= m_rows) {
        return;
    }
    
    int copyLength = qMin(line.size(), m_columns);
    for (int i = 0; i < copyLength; ++i) {
        (*m_currentScreen)[row][i] = line[i];
    }
    
    // Clear remaining columns if line is shorter
    for (int i = copyLength; i < m_columns; ++i) {
        (*m_currentScreen)[row][i] = TerminalChar();
    }
    
    emit screenChanged(QRect(0, row, m_columns, 1));
}

void TerminalScreen::clearLine(int row)
{
    if (row < 0 || row >= m_rows) {
        return;
    }
    
    for (int i = 0; i < m_columns; ++i) {
        (*m_currentScreen)[row][i] = TerminalChar();
    }
    
    emit screenChanged(QRect(0, row, m_columns, 1));
}

void TerminalScreen::clearLineFromCursor()
{
    for (int i = m_cursorPos.column; i < m_columns; ++i) {
        (*m_currentScreen)[m_cursorPos.row][i] = TerminalChar();
    }
    
    emit screenChanged(QRect(m_cursorPos.column, m_cursorPos.row, 
                            m_columns - m_cursorPos.column, 1));
}

void TerminalScreen::clearLineToCursor()
{
    for (int i = 0; i <= m_cursorPos.column && i < m_columns; ++i) {
        (*m_currentScreen)[m_cursorPos.row][i] = TerminalChar();
    }
    
    emit screenChanged(QRect(0, m_cursorPos.row, m_cursorPos.column + 1, 1));
}

void TerminalScreen::insertLine(int row)
{
    if (row < m_scrollTop || row > m_scrollBottom) {
        return;
    }
    
    // Save the bottom line for history if we're scrolling the whole screen
    if (m_scrollTop == 0 && m_scrollBottom == m_rows - 1) {
        addLineToHistory((*m_currentScreen)[m_scrollBottom]);
    }
    
    // Shift lines down
    for (int i = m_scrollBottom; i > row; --i) {
        (*m_currentScreen)[i] = (*m_currentScreen)[i - 1];
    }
    
    // Clear the inserted line
    clearLine(row);
    
    emitScreenChanged(row, m_scrollBottom);
}

void TerminalScreen::deleteLine(int row)
{
    if (row < m_scrollTop || row > m_scrollBottom) {
        return;
    }
    
    // Shift lines up
    for (int i = row; i < m_scrollBottom; ++i) {
        (*m_currentScreen)[i] = (*m_currentScreen)[i + 1];
    }
    
    // Clear the bottom line
    clearLine(m_scrollBottom);
    
    emitScreenChanged(row, m_scrollBottom);
}

void TerminalScreen::clear()
{
    for (int i = 0; i < m_rows; ++i) {
        clearLine(i);
    }
    setCursorPosition(0, 0);
}

void TerminalScreen::clearFromCursor()
{
    // Clear from cursor to end of screen
    clearLineFromCursor();
    
    for (int i = m_cursorPos.row + 1; i < m_rows; ++i) {
        clearLine(i);
    }
}

void TerminalScreen::clearToCursor()
{
    // Clear from beginning of screen to cursor
    for (int i = 0; i < m_cursorPos.row; ++i) {
        clearLine(i);
    }
    
    clearLineToCursor();
}

void TerminalScreen::scrollUp(int lines)
{
    scrollUpInRegion(lines);
}

void TerminalScreen::scrollDown(int lines)
{
    scrollDownInRegion(lines);
}

void TerminalScreen::scrollUpInRegion(int lines)
{
    if (lines <= 0) return;
    
    lines = qMin(lines, m_scrollBottom - m_scrollTop + 1);
    
    // Add scrolled lines to history
    for (int i = 0; i < lines; ++i) {
        addLineToHistory((*m_currentScreen)[m_scrollTop + i]);
    }
    
    // Shift lines up
    for (int i = m_scrollTop; i <= m_scrollBottom - lines; ++i) {
        (*m_currentScreen)[i] = (*m_currentScreen)[i + lines];
    }
    
    // Clear bottom lines
    for (int i = m_scrollBottom - lines + 1; i <= m_scrollBottom; ++i) {
        clearLine(i);
    }
    
    emitScreenChanged(m_scrollTop, m_scrollBottom);
}

void TerminalScreen::scrollDownInRegion(int lines)
{
    if (lines <= 0) return;
    
    lines = qMin(lines, m_scrollBottom - m_scrollTop + 1);
    
    // Shift lines down
    for (int i = m_scrollBottom; i >= m_scrollTop + lines; --i) {
        (*m_currentScreen)[i] = (*m_currentScreen)[i - lines];
    }
    
    // Clear top lines
    for (int i = m_scrollTop; i < m_scrollTop + lines; ++i) {
        clearLine(i);
    }
    
    emitScreenChanged(m_scrollTop, m_scrollBottom);
}

void TerminalScreen::setCurrentAttributes(TerminalColor fg, TerminalColor bg, TextAttributes attr)
{
    m_currentForeground = fg;
    m_currentBackground = bg;
    m_currentAttributes = attr;
}

void TerminalScreen::setCurrentForegroundColor(TerminalColor color)
{
    m_currentForeground = color;
}

void TerminalScreen::setCurrentBackgroundColor(TerminalColor color)
{
    m_currentBackground = color;
}

void TerminalScreen::setCurrentTextAttributes(TextAttributes attr)
{
    m_currentAttributes = attr;
}

void TerminalScreen::resetAttributes()
{
    m_currentForeground = TerminalColor::Default;
    m_currentBackground = TerminalColor::Default;
    m_currentAttributes = TextAttribute::None;
}

void TerminalScreen::setScrollingRegion(int top, int bottom)
{
    m_scrollTop = qBound(0, top, m_rows - 1);
    m_scrollBottom = qBound(m_scrollTop, bottom, m_rows - 1);
}

void TerminalScreen::resetScrollingRegion()
{
    m_scrollTop = 0;
    m_scrollBottom = m_rows - 1;
}

void TerminalScreen::setTabStop(int column)
{
    if (column >= 0 && column < m_columns) {
        m_tabStops[column] = true;
    }
}

void TerminalScreen::clearTabStop(int column)
{
    if (column >= 0 && column < m_columns) {
        m_tabStops[column] = false;
    }
}

void TerminalScreen::clearAllTabStops()
{
    m_tabStops.fill(false);
}

void TerminalScreen::tabToNextStop()
{
    int nextTab = m_cursorPos.column + 1;
    while (nextTab < m_columns && !m_tabStops[nextTab]) {
        nextTab++;
    }
    
    if (nextTab < m_columns) {
        moveCursorToColumn(nextTab);
    } else {
        moveCursorToColumn(m_columns - 1);
    }
}

void TerminalScreen::tabToPreviousStop()
{
    int prevTab = m_cursorPos.column - 1;
    while (prevTab >= 0 && !m_tabStops[prevTab]) {
        prevTab--;
    }
    
    if (prevTab >= 0) {
        moveCursorToColumn(prevTab);
    } else {
        moveCursorToColumn(0);
    }
}

QVector<TerminalChar> TerminalScreen::getHistoryLine(int index) const
{
    if (index < 0 || index >= m_history.size()) {
        return QVector<TerminalChar>();
    }
    return m_history[index];
}

void TerminalScreen::setMaxHistorySize(int size)
{
    m_maxHistorySize = qMax(0, size);
    
    // Trim history if necessary
    while (m_history.size() > m_maxHistorySize) {
        m_history.removeFirst();
    }
}

bool TerminalScreen::isValidPosition(int row, int column) const
{
    return row >= 0 && row < m_rows && column >= 0 && column < m_columns;
}

void TerminalScreen::ensureCursorInBounds()
{
    setCursorPosition(m_cursorPos.row, m_cursorPos.column);
}

void TerminalScreen::addLineToHistory(const QVector<TerminalChar> &line)
{
    if (m_maxHistorySize <= 0) {
        return;
    }
    
    m_history.append(line);
    
    // Trim history if it exceeds maximum size
    while (m_history.size() > m_maxHistorySize) {
        m_history.removeFirst();
    }
}

void TerminalScreen::setUseAlternateBuffer(bool use)
{
    if (m_useAlternateBuffer == use) return;
    
    m_useAlternateBuffer = use;
    m_currentScreen = use ? &m_alternateScreen : &m_mainScreen;
    
    // Clear alternate buffer when switching to it
    if (use) {
        for (int i = 0; i < m_rows; ++i) {
            for (int j = 0; j < m_columns; ++j) {
                m_alternateScreen[i][j] = TerminalChar();
            }
        }
    }
    
    emit screenChanged(QRect(0, 0, m_columns, m_rows));
}

void TerminalScreen::emitScreenChanged(int startRow, int endRow)
{
    emit screenChanged(QRect(0, startRow, m_columns, endRow - startRow + 1));
}
