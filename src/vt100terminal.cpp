#include "vt100terminal.h"
#include <QPainter>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QResizeEvent>
#include <QFocusEvent>
#include <QScrollBar>
#include <QApplication>
#include <QClipboard>
#include <QFontMetrics>
#include <QDebug>

VT100Terminal::VT100Terminal(QWidget *parent)
    : QWidget(parent)
    , m_screen(nullptr)
    , m_parser(nullptr)
    , m_scrollBar(nullptr)
    , m_font("Courier New", 10)
    , m_charWidth(8)
    , m_charHeight(12)
    , m_charBaseline(10)
    , m_defaultForeground(192, 192, 192)
    , m_defaultBackground(0, 0, 0)
    , m_cursorVisible(true)
    , m_cursorBlinking(true)
    , m_cursorBlinkState(true)
    , m_cursorBlinkTimer(nullptr)
    , m_hasSelection(false)
    , m_selecting(false)
    , m_scrollOffset(0)
    , m_hasFocus(false)
{
    setupUI();
    setupConnections();
    
    // Set focus policy to receive keyboard events
    setFocusPolicy(Qt::StrongFocus);
    
    // Enable mouse tracking for selection
    setMouseTracking(true);
    
    // Set default terminal size
    setTerminalSize(24, 80);
}

VT100Terminal::~VT100Terminal()
{
    delete m_screen;
    delete m_parser;
}

void VT100Terminal::setupUI()
{
    // Create terminal screen
    m_screen = new TerminalScreen(24, 80, this);
    
    // Create VT100 parser
    m_parser = new VT100Parser(this);
    
    // Create scroll bar
    m_scrollBar = new QScrollBar(Qt::Vertical, this);
    m_scrollBar->setVisible(true);
    
    // Create cursor blink timer
    m_cursorBlinkTimer = new QTimer(this);
    m_cursorBlinkTimer->setInterval(CURSOR_BLINK_INTERVAL);
    
    // Calculate character dimensions
    calculateCharacterSize();
    
    // Set background color
    setAutoFillBackground(true);
    QPalette palette = this->palette();
    palette.setColor(QPalette::Window, m_defaultBackground);
    setPalette(palette);
}

void VT100Terminal::setupConnections()
{
    // Connect screen signals
    connect(m_screen, &TerminalScreen::screenChanged, this, &VT100Terminal::onScreenChanged);
    connect(m_screen, &TerminalScreen::cursorPositionChanged, this, &VT100Terminal::onCursorPositionChanged);
    connect(m_screen, &TerminalScreen::cursorVisibilityChanged, this, &VT100Terminal::onCursorVisibilityChanged);
    connect(m_screen, &TerminalScreen::screenResized, this, &VT100Terminal::onScreenResized);
    
    // Connect parser signals to screen operations
    connect(m_parser, &VT100Parser::textReceived, this, &VT100Terminal::onTextReceived);
    connect(m_parser, &VT100Parser::cursorUp, this, &VT100Terminal::onCursorUp);
    connect(m_parser, &VT100Parser::cursorDown, this, &VT100Terminal::onCursorDown);
    connect(m_parser, &VT100Parser::cursorForward, this, &VT100Terminal::onCursorForward);
    connect(m_parser, &VT100Parser::cursorBackward, this, &VT100Terminal::onCursorBackward);
    connect(m_parser, &VT100Parser::cursorPosition, this, &VT100Terminal::onCursorPosition);
    connect(m_parser, &VT100Parser::cursorNextLine, this, &VT100Terminal::onCursorNextLine);
    connect(m_parser, &VT100Parser::cursorPreviousLine, this, &VT100Terminal::onCursorPreviousLine);
    connect(m_parser, &VT100Parser::cursorHorizontalAbsolute, this, &VT100Terminal::onCursorHorizontalAbsolute);
    connect(m_parser, &VT100Parser::saveCursor, this, &VT100Terminal::onSaveCursor);
    connect(m_parser, &VT100Parser::restoreCursor, this, &VT100Terminal::onRestoreCursor);
    connect(m_parser, &VT100Parser::hideCursor, this, &VT100Terminal::onHideCursor);
    connect(m_parser, &VT100Parser::showCursor, this, &VT100Terminal::onShowCursor);
    connect(m_parser, &VT100Parser::clearScreen, this, &VT100Terminal::onClearScreen);
    connect(m_parser, &VT100Parser::clearScreenFromCursor, this, &VT100Terminal::onClearScreenFromCursor);
    connect(m_parser, &VT100Parser::clearScreenToCursor, this, &VT100Terminal::onClearScreenToCursor);
    connect(m_parser, &VT100Parser::clearLine, this, &VT100Terminal::onClearLine);
    connect(m_parser, &VT100Parser::clearLineFromCursor, this, &VT100Terminal::onClearLineFromCursor);
    connect(m_parser, &VT100Parser::clearLineToCursor, this, &VT100Terminal::onClearLineToCursor);
    connect(m_parser, &VT100Parser::insertLines, this, &VT100Terminal::onInsertLines);
    connect(m_parser, &VT100Parser::deleteLines, this, &VT100Terminal::onDeleteLines);
    connect(m_parser, &VT100Parser::insertCharacters, this, &VT100Terminal::onInsertCharacters);
    connect(m_parser, &VT100Parser::deleteCharacters, this, &VT100Terminal::onDeleteCharacters);
    connect(m_parser, &VT100Parser::eraseCharacters, this, &VT100Terminal::onEraseCharacters);
    connect(m_parser, &VT100Parser::scrollUp, this, &VT100Terminal::onScrollUp);
    connect(m_parser, &VT100Parser::scrollDown, this, &VT100Terminal::onScrollDown);
    connect(m_parser, &VT100Parser::setTextAttributes, this, &VT100Terminal::onSetTextAttributes);
    connect(m_parser, &VT100Parser::resetTextAttributes, this, &VT100Terminal::onResetTextAttributes);
    connect(m_parser, &VT100Parser::setScrollingRegion, this, &VT100Terminal::onSetScrollingRegion);
    connect(m_parser, &VT100Parser::setTabStop, this, &VT100Terminal::onSetTabStop);
    connect(m_parser, &VT100Parser::clearTabStop, this, &VT100Terminal::onClearTabStop);
    connect(m_parser, &VT100Parser::clearAllTabStops, this, &VT100Terminal::onClearAllTabStops);
    connect(m_parser, &VT100Parser::tabForward, this, &VT100Terminal::onTabForward);
    connect(m_parser, &VT100Parser::tabBackward, this, &VT100Terminal::onTabBackward);
    connect(m_parser, &VT100Parser::bell, this, &VT100Terminal::onBell);
    
    // Connect cursor blink timer
    connect(m_cursorBlinkTimer, &QTimer::timeout, this, &VT100Terminal::onCursorBlink);
    
    // Connect scroll bar
    connect(m_scrollBar, &QScrollBar::valueChanged, this, &VT100Terminal::onScrollBarValueChanged);
    
    // Start cursor blinking if enabled
    if (m_cursorBlinking) {
        m_cursorBlinkTimer->start();
    }
}

void VT100Terminal::calculateCharacterSize()
{
    QFontMetrics metrics(m_font);
    m_charWidth = metrics.horizontalAdvance('M');  // Use 'M' as it's typically the widest character
    m_charHeight = metrics.height();
    m_charBaseline = metrics.ascent();
}

void VT100Terminal::setFont(const QFont &font)
{
    m_font = font;
    calculateCharacterSize();
    updateTerminalSize();
    update();
}

void VT100Terminal::setTerminalSize(int rows, int columns)
{
    if (m_screen) {
        m_screen->resize(rows, columns);
        updateTerminalSize();
        updateScrollBar();
        emit terminalSizeChanged(rows, columns);
    }
}

int VT100Terminal::terminalRows() const
{
    return m_screen ? m_screen->rows() : 0;
}

int VT100Terminal::terminalColumns() const
{
    return m_screen ? m_screen->columns() : 0;
}

void VT100Terminal::updateTerminalSize()
{
    if (!m_screen) return;
    
    // Calculate required widget size
    int requiredWidth = m_screen->columns() * m_charWidth + m_scrollBar->sizeHint().width();
    int requiredHeight = m_screen->rows() * m_charHeight;
    
    // Update minimum size
    setMinimumSize(requiredWidth, requiredHeight);
    
    // Position scroll bar
    m_scrollBar->setGeometry(width() - m_scrollBar->sizeHint().width(), 0, 
                            m_scrollBar->sizeHint().width(), height());
}

void VT100Terminal::updateScrollBar()
{
    if (!m_screen) return;
    
    int historySize = m_screen->historySize();
    int screenRows = m_screen->rows();
    
    m_scrollBar->setRange(0, historySize);
    m_scrollBar->setPageStep(screenRows);
    m_scrollBar->setSingleStep(1);
    m_scrollBar->setValue(historySize - m_scrollOffset);
}

void VT100Terminal::writeData(const QByteArray &data)
{
    if (m_parser) {
        m_parser->processData(data);
    }
}

void VT100Terminal::writeData(const QString &data)
{
    if (m_parser) {
        m_parser->processData(data);
    }
}

void VT100Terminal::clear()
{
    if (m_screen) {
        m_screen->clear();
        m_scrollOffset = 0;
        updateScrollBar();
        update();
    }
}

void VT100Terminal::reset()
{
    if (m_parser) {
        m_parser->reset();
    }
    clear();
}

void VT100Terminal::setCursorBlinking(bool blink)
{
    m_cursorBlinking = blink;
    if (blink && m_hasFocus) {
        m_cursorBlinkTimer->start();
    } else {
        m_cursorBlinkTimer->stop();
        m_cursorBlinkState = true;
    }
    update();
}

void VT100Terminal::setScrollbackLines(int lines)
{
    if (m_screen) {
        m_screen->setMaxHistorySize(lines);
        updateScrollBar();
    }
}

int VT100Terminal::scrollbackLines() const
{
    return m_screen ? m_screen->historySize() : 0;
}

void VT100Terminal::scrollToBottom()
{
    setScrollOffset(0);
}

void VT100Terminal::scrollToTop()
{
    setScrollOffset(maxScrollOffset());
}

void VT100Terminal::scrollUp(int lines)
{
    setScrollOffset(m_scrollOffset + lines);
}

void VT100Terminal::scrollDown(int lines)
{
    setScrollOffset(m_scrollOffset - lines);
}

void VT100Terminal::setScrollOffset(int offset)
{
    int maxOffset = maxScrollOffset();
    m_scrollOffset = qBound(0, offset, maxOffset);
    updateScrollBar();
    update();
}

int VT100Terminal::maxScrollOffset() const
{
    return m_screen ? m_screen->historySize() : 0;
}

void VT100Terminal::setColorScheme(const QColor &foreground, const QColor &background)
{
    setDefaultColors(foreground, background);
}

void VT100Terminal::setDefaultColors(const QColor &foreground, const QColor &background)
{
    m_defaultForeground = foreground;
    m_defaultBackground = background;
    
    // Update widget background
    QPalette palette = this->palette();
    palette.setColor(QPalette::Window, background);
    setPalette(palette);
    
    update();
}

// Event handlers will be implemented in the next part due to size constraints
void VT100Terminal::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    
    if (!m_screen) return;
    
    QPainter painter(this);
    painter.setFont(m_font);
    
    // Fill background
    painter.fillRect(rect(), m_defaultBackground);
    
    // Calculate visible area
    int startRow = qMax(0, -m_scrollOffset);
    int endRow = qMin(m_screen->rows() - 1, startRow + height() / m_charHeight);
    
    // Draw characters
    for (int row = startRow; row <= endRow; ++row) {
        for (int col = 0; col < m_screen->columns(); ++col) {
            TerminalChar ch = m_screen->getChar(row, col);
            QRect charRect = getCharacterRect(row, col);
            
            if (charRect.intersects(event->rect())) {
                drawCharacter(painter, row, col, ch, charRect);
            }
        }
    }
    
    // Draw selection
    if (m_hasSelection) {
        drawSelection(painter);
    }
    
    // Draw cursor
    if (m_cursorVisible && m_cursorBlinkState && m_hasFocus && m_scrollOffset == 0) {
        drawCursor(painter);
    }
}

QRect VT100Terminal::getCharacterRect(int row, int column) const
{
    int x = column * m_charWidth;
    int y = (row + m_scrollOffset) * m_charHeight;
    return QRect(x, y, m_charWidth, m_charHeight);
}

void VT100Terminal::drawCharacter(QPainter &painter, int row, int column, const TerminalChar &ch, const QRect &charRect)
{
    Q_UNUSED(row)
    Q_UNUSED(column)
    // Get colors
    QColor fg = getCharacterColor(ch.foregroundColor, true);
    QColor bg = getCharacterColor(ch.backgroundColor, false);
    
    // Handle reverse attribute
    if (ch.attributes & TextAttribute::Reverse) {
        qSwap(fg, bg);
    }
    
    // Draw background
    if (bg != m_defaultBackground) {
        painter.fillRect(charRect, bg);
    }
    
    // Draw character
    if (ch.character != ' ') {
        painter.setPen(fg);
        painter.setFont(getCharacterFont(ch.attributes));
        painter.drawText(charRect, Qt::AlignLeft | Qt::AlignTop, ch.character);
    }
    
    // Draw underline
    if (ch.attributes & TextAttribute::Underline) {
        painter.setPen(fg);
        painter.drawLine(charRect.bottomLeft(), charRect.bottomRight());
    }
    
    // Draw strikethrough
    if (ch.attributes & TextAttribute::Strikethrough) {
        painter.setPen(fg);
        int y = charRect.top() + charRect.height() / 2;
        painter.drawLine(charRect.left(), y, charRect.right(), y);
    }
}

void VT100Terminal::drawCursor(QPainter &painter)
{
    if (!m_screen) return;
    
    CursorPosition pos = m_screen->cursorPosition();
    QRect cursorRect = getCharacterRect(pos.row, pos.column);
    
    painter.setPen(m_defaultForeground);
    painter.drawRect(cursorRect);
}

void VT100Terminal::drawSelection(QPainter &painter)
{
    // Selection drawing implementation
    painter.setCompositionMode(QPainter::CompositionMode_Difference);
    painter.fillRect(rect(), QColor(255, 255, 255, 128));
}

QColor VT100Terminal::getCharacterColor(TerminalColor color, bool isForeground) const
{
    if (color == TerminalColor::Default) {
        return isForeground ? m_defaultForeground : m_defaultBackground;
    }
    return TerminalColorPalette::getColor(color);
}

QFont VT100Terminal::getCharacterFont(TextAttributes attributes) const
{
    QFont font = m_font;
    
    if (attributes & TextAttribute::Bold) {
        font.setBold(true);
    }
    if (attributes & TextAttribute::Italic) {
        font.setItalic(true);
    }
    
    return font;
}

// Slot implementations
void VT100Terminal::onTextReceived(const QString &text)
{
    if (m_screen) {
        m_screen->insertText(text);
        scrollToBottom();  // Auto-scroll to bottom when new text arrives
    }
}

void VT100Terminal::onCursorPositionChanged(const CursorPosition &position)
{
    Q_UNUSED(position)
    update();  // Redraw to show cursor in new position
}

void VT100Terminal::onCursorVisibilityChanged(bool visible)
{
    m_cursorVisible = visible;
    update();
}

void VT100Terminal::onScreenChanged(const QRect &region)
{
    Q_UNUSED(region)
    update();  // For now, just update the entire widget
}

void VT100Terminal::onScreenResized(int rows, int columns)
{
    updateTerminalSize();
    updateScrollBar();
    emit terminalSizeChanged(rows, columns);
}

void VT100Terminal::onCursorBlink()
{
    m_cursorBlinkState = !m_cursorBlinkState;
    update();
}

void VT100Terminal::onScrollBarValueChanged(int value)
{
    if (m_screen) {
        int historySize = m_screen->historySize();
        setScrollOffset(historySize - value);
    }
}

// VT100Parser signal handlers - delegate to screen
void VT100Terminal::onCursorUp(int lines)
{
    if (m_screen) m_screen->moveCursor(-lines, 0);
}

void VT100Terminal::onCursorDown(int lines)
{
    if (m_screen) m_screen->moveCursor(lines, 0);
}

void VT100Terminal::onCursorForward(int columns)
{
    if (m_screen) m_screen->moveCursor(0, columns);
}

void VT100Terminal::onCursorBackward(int columns)
{
    if (m_screen) m_screen->moveCursor(0, -columns);
}

void VT100Terminal::onCursorPosition(int row, int column)
{
    if (m_screen) m_screen->setCursorPosition(row, column);
}

void VT100Terminal::onCursorNextLine(int lines)
{
    if (m_screen) {
        CursorPosition pos = m_screen->cursorPosition();
        m_screen->setCursorPosition(pos.row + lines, 0);
    }
}

void VT100Terminal::onCursorPreviousLine(int lines)
{
    if (m_screen) {
        CursorPosition pos = m_screen->cursorPosition();
        m_screen->setCursorPosition(pos.row - lines, 0);
    }
}

void VT100Terminal::onCursorHorizontalAbsolute(int column)
{
    if (m_screen) m_screen->moveCursorToColumn(column);
}

void VT100Terminal::onSaveCursor()
{
    if (m_screen) m_screen->saveCursor();
}

void VT100Terminal::onRestoreCursor()
{
    if (m_screen) m_screen->restoreCursor();
}

void VT100Terminal::onHideCursor()
{
    if (m_screen) m_screen->setCursorVisible(false);
}

void VT100Terminal::onShowCursor()
{
    if (m_screen) m_screen->setCursorVisible(true);
}

void VT100Terminal::onClearScreen()
{
    if (m_screen) m_screen->clear();
}

void VT100Terminal::onClearScreenFromCursor()
{
    if (m_screen) m_screen->clearFromCursor();
}

void VT100Terminal::onClearScreenToCursor()
{
    if (m_screen) m_screen->clearToCursor();
}

void VT100Terminal::onClearLine()
{
    if (m_screen) {
        CursorPosition pos = m_screen->cursorPosition();
        m_screen->clearLine(pos.row);
    }
}

void VT100Terminal::onClearLineFromCursor()
{
    if (m_screen) m_screen->clearLineFromCursor();
}

void VT100Terminal::onClearLineToCursor()
{
    if (m_screen) m_screen->clearLineToCursor();
}

void VT100Terminal::onInsertLines(int count)
{
    if (m_screen) {
        CursorPosition pos = m_screen->cursorPosition();
        for (int i = 0; i < count; ++i) {
            m_screen->insertLine(pos.row);
        }
    }
}

void VT100Terminal::onDeleteLines(int count)
{
    if (m_screen) {
        CursorPosition pos = m_screen->cursorPosition();
        for (int i = 0; i < count; ++i) {
            m_screen->deleteLine(pos.row);
        }
    }
}

void VT100Terminal::onInsertCharacters(int count)
{
    Q_UNUSED(count)
    // Character insertion implementation would go here
}

void VT100Terminal::onDeleteCharacters(int count)
{
    Q_UNUSED(count)
    // Character deletion implementation would go here
}

void VT100Terminal::onEraseCharacters(int count)
{
    Q_UNUSED(count)
    // Character erasure implementation would go here
}

void VT100Terminal::onScrollUp(int lines)
{
    if (m_screen) m_screen->scrollUp(lines);
}

void VT100Terminal::onScrollDown(int lines)
{
    if (m_screen) m_screen->scrollDown(lines);
}

void VT100Terminal::onSetTextAttributes(TerminalColor foreground, TerminalColor background, TextAttributes attributes)
{
    if (m_screen) {
        m_screen->setCurrentAttributes(foreground, background, attributes);
    }
}

void VT100Terminal::onResetTextAttributes()
{
    if (m_screen) m_screen->resetAttributes();
}

void VT100Terminal::onSetScrollingRegion(int top, int bottom)
{
    if (m_screen) m_screen->setScrollingRegion(top, bottom);
}

void VT100Terminal::onSetTabStop()
{
    if (m_screen) {
        CursorPosition pos = m_screen->cursorPosition();
        m_screen->setTabStop(pos.column);
    }
}

void VT100Terminal::onClearTabStop()
{
    if (m_screen) {
        CursorPosition pos = m_screen->cursorPosition();
        m_screen->clearTabStop(pos.column);
    }
}

void VT100Terminal::onClearAllTabStops()
{
    if (m_screen) m_screen->clearAllTabStops();
}

void VT100Terminal::onTabForward(int count)
{
    if (m_screen) {
        for (int i = 0; i < count; ++i) {
            m_screen->tabToNextStop();
        }
    }
}

void VT100Terminal::onTabBackward(int count)
{
    if (m_screen) {
        for (int i = 0; i < count; ++i) {
            m_screen->tabToPreviousStop();
        }
    }
}

void VT100Terminal::onBell()
{
    emit bell();
}

// Event handlers - basic implementations
void VT100Terminal::keyPressEvent(QKeyEvent *event)
{
    QByteArray sequence = keyEventToSequence(event);
    if (!sequence.isEmpty()) {
        emit keyPressed(sequence);
    }
}

void VT100Terminal::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    // Mouse handling implementation
}

void VT100Terminal::mouseMoveEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    // Mouse handling implementation
}

void VT100Terminal::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    // Mouse handling implementation
}

void VT100Terminal::wheelEvent(QWheelEvent *event)
{
    int delta = event->angleDelta().y();
    if (delta > 0) {
        scrollUp(SCROLL_LINES_PER_WHEEL);
    } else if (delta < 0) {
        scrollDown(SCROLL_LINES_PER_WHEEL);
    }
}

void VT100Terminal::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    updateTerminalSize();
    
    // Calculate new terminal size based on widget size
    int newColumns = (width() - m_scrollBar->sizeHint().width()) / m_charWidth;
    int newRows = height() / m_charHeight;
    
    if (newColumns > 0 && newRows > 0) {
        setTerminalSize(newRows, newColumns);
    }
}

void VT100Terminal::focusInEvent(QFocusEvent *event)
{
    QWidget::focusInEvent(event);
    m_hasFocus = true;
    if (m_cursorBlinking) {
        m_cursorBlinkTimer->start();
    }
    update();
}

void VT100Terminal::focusOutEvent(QFocusEvent *event)
{
    QWidget::focusOutEvent(event);
    m_hasFocus = false;
    m_cursorBlinkTimer->stop();
    m_cursorBlinkState = true;
    update();
}

QByteArray VT100Terminal::keyEventToSequence(QKeyEvent *event)
{
    // Basic key handling - convert Qt key events to terminal sequences
    QString text = event->text();
    if (!text.isEmpty() && text[0].isPrint()) {
        return text.toUtf8();
    }
    
    // Handle special keys
    switch (event->key()) {
    case Qt::Key_Return:
    case Qt::Key_Enter:
        return "\r";
    case Qt::Key_Backspace:
        return "\x08";
    case Qt::Key_Tab:
        return "\t";
    case Qt::Key_Escape:
        return "\x1b";
    case Qt::Key_Up:
        return "\x1b[A";
    case Qt::Key_Down:
        return "\x1b[B";
    case Qt::Key_Right:
        return "\x1b[C";
    case Qt::Key_Left:
        return "\x1b[D";
    case Qt::Key_Home:
        return "\x1b[H";
    case Qt::Key_End:
        return "\x1b[F";
    case Qt::Key_PageUp:
        return "\x1b[5~";
    case Qt::Key_PageDown:
        return "\x1b[6~";
    case Qt::Key_Delete:
        return "\x1b[3~";
    case Qt::Key_Insert:
        return "\x1b[2~";
    }
    
    return QByteArray();
}

// Placeholder implementations for remaining methods
QString VT100Terminal::selectedText() const { return QString(); }
void VT100Terminal::clearSelection() { m_hasSelection = false; update(); }
void VT100Terminal::selectAll() { /* Implementation */ }
CursorPosition VT100Terminal::pixelToPosition(const QPoint &pixel) const { Q_UNUSED(pixel) return CursorPosition(); }
QPoint VT100Terminal::positionToPixel(const CursorPosition &position) const { Q_UNUSED(position) return QPoint(); }
void VT100Terminal::updateSelection(const QPoint &pos) { Q_UNUSED(pos) }
void VT100Terminal::normalizeSelection() { /* Implementation */ }
bool VT100Terminal::isPositionSelected(int row, int column) const { Q_UNUSED(row) Q_UNUSED(column) return false; }
QByteArray VT100Terminal::functionKeyToSequence(int key) { Q_UNUSED(key) return QByteArray(); }
QByteArray VT100Terminal::cursorKeyToSequence(int key) { Q_UNUSED(key) return QByteArray(); }
