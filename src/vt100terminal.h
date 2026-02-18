#ifndef VT100TERMINAL_H
#define VT100TERMINAL_H

#include <QWidget>
#include <QFont>
#include <QTimer>
#include <QScrollBar>
#include "terminalscreen.h"
#include "vt100parser.h"

class QPaintEvent;
class QKeyEvent;
class QMouseEvent;
class QWheelEvent;
class QResizeEvent;

/**
 * @brief VT100-compatible terminal widget
 * 
 * This widget provides a complete VT100 terminal emulator with:
 * - VT100/ANSI escape sequence support
 * - Color and text attribute rendering
 * - Cursor display and blinking
 * - Keyboard input handling
 * - Mouse support
 * - Scrollback buffer
 */
class VT100Terminal : public QWidget
{
    Q_OBJECT

public:
    enum CursorStyle {
        Block,
        Underline,
        IBeam
    };

    explicit VT100Terminal(QWidget *parent = nullptr);
    ~VT100Terminal();
    
    // Terminal configuration
    void setFont(const QFont &font);
    QFont terminalFont() const { return m_font; }
    
    void setTerminalSize(int rows, int columns);
    int terminalRows() const;
    int terminalColumns() const;
    
    // Terminal operations
    void writeData(const QByteArray &data);
    void writeData(const QString &data);
    void clear();
    void reset();
    
    // Cursor configuration
    void setCursorBlinking(bool blink);
    bool isCursorBlinking() const { return m_cursorBlinking; }
    
    void setCursorStyle(CursorStyle style);
    CursorStyle cursorStyle() const { return m_cursorStyle; }
    // Scrollback
    void setScrollbackLines(int lines);
    int scrollbackLines() const;
    void scrollToBottom();
    void scrollToTop();
    void scrollUp(int lines = 1);
    void scrollDown(int lines = 1);
    
    // Selection
    bool hasSelection() const { return m_hasSelection; }
    QString selectedText() const;
    void clearSelection();
    void selectAll();
    void copy();
    void paste();
    
    // Colors
    void setColorScheme(const QColor &foreground, const QColor &background);
    void setDefaultColors(const QColor &foreground, const QColor &background);

signals:
    void keyPressed(const QByteArray &data);
    void terminalSizeChanged(int rows, int columns);
    void bell();

protected:
    // Qt event handlers
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void focusInEvent(QFocusEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;

private slots:
    void onTextReceived(const QString &text);
    void onCursorPositionChanged(const CursorPosition &position);
    void onCursorVisibilityChanged(bool visible);
    void onScreenChanged(const QRect &region);
    void onScreenResized(int rows, int columns);
    void onCursorBlink();
    void onScrollBarValueChanged(int value);
    
    // VT100Parser signal handlers
    void onCursorUp(int lines);
    void onCursorDown(int lines);
    void onCursorForward(int columns);
    void onCursorBackward(int columns);
    void onCursorPosition(int row, int column);
    void onCursorNextLine(int lines);
    void onCursorPreviousLine(int lines);
    void onCursorHorizontalAbsolute(int column);
    void onSaveCursor();
    void onRestoreCursor();
    void onHideCursor();
    void onShowCursor();
    void onUseAlternateScreenBuffer(bool use);
    void onClearScreen();
    void onClearScreenFromCursor();
    void onClearScreenToCursor();
    void onClearLine();
    void onClearLineFromCursor();
    void onClearLineToCursor();
    void onInsertLines(int count);
    void onDeleteLines(int count);
    void onInsertCharacters(int count);
    void onDeleteCharacters(int count);
    void onEraseCharacters(int count);
    void onScrollUp(int lines);
    void onScrollDown(int lines);
    void onSetTextAttributes(TerminalColor foreground, TerminalColor background, TextAttributes attributes);
    void onResetTextAttributes();
    void onSetScrollingRegion(int top, int bottom);
    void onSetTabStop();
    void onClearTabStop();
    void onClearAllTabStops();
    void onTabForward(int count);
    void onTabBackward(int count);
    void onSetPrivateMode(int mode, bool enabled);
    void onBell();

private:
    void setupUI();
    void setupConnections();
    void calculateCharacterSize();
    void updateTerminalSize();
    void updateScrollBar();
    
    // Rendering
    void drawCharacter(QPainter &painter, int row, int column, const TerminalChar &ch, const QRect &charRect);
    void drawCursor(QPainter &painter);
    void drawSelection(QPainter &painter);
    QRect getCharacterRect(int row, int column) const;
    QColor getCharacterColor(TerminalColor color, bool isForeground) const;
    QFont getCharacterFont(TextAttributes attributes) const;
    
    // Coordinate conversion
    CursorPosition pixelToPosition(const QPoint &pixel) const;
    QPoint positionToPixel(const CursorPosition &position) const;
    
    // Selection
    void updateSelection(const QPoint &pos);
    void normalizeSelection();
    bool isPositionSelected(int row, int column) const;
    
    // Keyboard handling
    QByteArray keyEventToSequence(QKeyEvent *event);
    QByteArray functionKeyToSequence(int key);
    QByteArray cursorKeyToSequence(int key);
    
    // Scrolling
    void setScrollOffset(int offset);
    int maxScrollOffset() const;
    
    // Components
    TerminalScreen *m_screen;
    VT100Parser *m_parser;
    QScrollBar *m_scrollBar;
    
    // Display properties
    QFont m_font;
    int m_charWidth;
    int m_charHeight;
    int m_charBaseline;
    
    // Colors
    QColor m_defaultForeground;
    QColor m_defaultBackground;
    
    // Cursor
    bool m_cursorVisible;
    CursorStyle m_cursorStyle;
    bool m_cursorBlinking;
    bool m_cursorBlinkState;
    QTimer *m_cursorBlinkTimer;
    
    // Selection
    bool m_hasSelection;
    bool m_selecting;
    CursorPosition m_selectionStart;
    CursorPosition m_selectionEnd;
    
    // Scrolling
    int m_scrollOffset;  // Number of lines scrolled up from bottom
    
    // Terminal state
    bool m_hasFocus;
    bool m_appCursorKeys;
    
    // Constants
    static const int CURSOR_BLINK_INTERVAL = 500;  // milliseconds
    static const int SCROLL_LINES_PER_WHEEL = 3;
};

#endif // VT100TERMINAL_H
