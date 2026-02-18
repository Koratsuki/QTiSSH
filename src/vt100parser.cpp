#include "vt100parser.h"
#include <QDebug>
#include <QRegularExpression>

VT100Parser::VT100Parser(QObject *parent)
    : QObject(parent)
    , m_state(ParserState::Normal)
    , m_currentForeground(TerminalColor::Default)
    , m_currentBackground(TerminalColor::Default)
    , m_currentAttributes(TextAttribute::None)
    , m_currentCharacterSet(0)
{
    // Initialize character sets to default (ASCII)
    for (int i = 0; i < 4; ++i) {
        m_characterSets[i] = 'B';  // ASCII
    }
    
    reset();
}

void VT100Parser::processData(const QByteArray &data)
{
    processData(QString::fromUtf8(data));
}

void VT100Parser::processData(const QString &data)
{
    for (const QChar &ch : data) {
        processCharacter(ch);
    }
    
    // Emit any pending text
    if (!m_textBuffer.isEmpty()) {
        emit textReceived(m_textBuffer);
        m_textBuffer.clear();
    }
}

void VT100Parser::reset()
{
    m_state = ParserState::Normal;
    resetSequence();
    m_textBuffer.clear();
    m_currentForeground = TerminalColor::Default;
    m_currentBackground = TerminalColor::Default;
    m_currentAttributes = TextAttribute::None;
}

void VT100Parser::processCharacter(QChar ch)
{
    switch (m_state) {
    case ParserState::Normal:
        processNormalCharacter(ch);
        break;
    case ParserState::Escape:
        processEscapeSequence(ch);
        break;
    case ParserState::CSI:
        processCSISequence(ch);
        break;
    case ParserState::OSC:
        processOSCSequence(ch);
        break;
    case ParserState::DCS:
        processDCSSequence(ch);
        break;
    default:
        // For SOS, PM, APC - just wait for string terminator
        if (ch == '\x1b' || ch == '\x07' || ch == '\x9c') {
            m_state = ParserState::Normal;
            resetSequence();
        }
        break;
    }
}

void VT100Parser::processNormalCharacter(QChar ch)
{
    if (ch == '\x1b') {  // ESC
        // Emit any pending text before processing escape sequence
        if (!m_textBuffer.isEmpty()) {
            emit textReceived(m_textBuffer);
            m_textBuffer.clear();
        }
        m_state = ParserState::Escape;
        resetSequence();
    } else if (ch.unicode() < 32) {  // Control characters
        // Emit any pending text before processing control character
        if (!m_textBuffer.isEmpty()) {
            emit textReceived(m_textBuffer);
            m_textBuffer.clear();
        }
        handleControlCharacter(ch);
    } else {
        // Regular printable character
        QChar mappedCh = mapCharacter(ch);
        m_textBuffer.append(mappedCh);
    }
}

void VT100Parser::processEscapeSequence(QChar ch)
{
    if (ch == '[') {
        m_state = ParserState::CSI;
    } else if (ch == ']') {
        m_state = ParserState::OSC;
    } else if (ch == 'P') {
        m_state = ParserState::DCS;
    } else if (ch == 'X') {
        m_state = ParserState::SOS;
    } else if (ch == '^') {
        m_state = ParserState::PM;
    } else if (ch == '_') {
        m_state = ParserState::APC;
    } else {
        // Single character escape sequences
        switch (ch.unicode()) {
        case 'D':  // IND - Index
            emit cursorDown(1);
            break;
        case 'E':  // NEL - Next Line
            emit cursorNextLine(1);
            break;
        case 'H':  // HTS - Horizontal Tab Set
            emit setTabStop();
            break;
        case 'M':  // RI - Reverse Index
            emit cursorUp(1);
            break;
        case 'Z':  // DECID - Identify Terminal
            // Should respond with device attributes
            break;
        case '7':  // DECSC - Save Cursor
            emit saveCursor();
            break;
        case '8':  // DECRC - Restore Cursor
            emit restoreCursor();
            break;
        case '=':  // DECKPAM - Keypad Application Mode
            emit setPrivateMode(1, true);
            break;
        case '>':  // DECKPNM - Keypad Numeric Mode
            emit setPrivateMode(1, false);
            break;
        case 'c':  // RIS - Reset to Initial State
            emit resetTextAttributes();
            emit clearScreen();
            emit cursorPosition(0, 0);
            break;
        }
        
        m_state = ParserState::Normal;
        resetSequence();
    }
}

void VT100Parser::processCSISequence(QChar ch)
{
    if (ch.isDigit() || ch == ';' || ch == '?') {
        m_sequence.append(ch);
    } else if (ch.unicode() >= 0x20 && ch.unicode() <= 0x2F) {
        // Intermediate characters
        m_intermediateChars.append(ch);
    } else if (ch.unicode() >= 0x40 && ch.unicode() <= 0x7E) {
        // Final character
        m_finalChar = ch;
        executeCSICommand();
        m_state = ParserState::Normal;
        resetSequence();
    } else {
        // Invalid sequence, reset
        m_state = ParserState::Normal;
        resetSequence();
    }
}

void VT100Parser::processOSCSequence(QChar ch)
{
    if (ch == '\x07' || ch == '\x1b') {  // BEL or ESC
        executeOSCCommand();
        m_state = ParserState::Normal;
        resetSequence();
    } else {
        m_sequence.append(ch);
    }
}

void VT100Parser::processDCSSequence(QChar ch)
{
    if (ch == '\x1b' || ch == '\x9c') {  // ESC or ST
        executeDCSCommand();
        m_state = ParserState::Normal;
        resetSequence();
    } else {
        m_sequence.append(ch);
    }
}

void VT100Parser::executeCSICommand()
{
    parseCSIParameters();
    
    switch (m_finalChar.unicode()) {
    case 'A':  // CUU - Cursor Up
        handleCursorUp();
        break;
    case 'B':  // CUD - Cursor Down
        handleCursorDown();
        break;
    case 'C':  // CUF - Cursor Forward
        handleCursorForward();
        break;
    case 'D':  // CUB - Cursor Backward
        handleCursorBackward();
        break;
    case 'E':  // CNL - Cursor Next Line
        handleCursorNextLine();
        break;
    case 'F':  // CPL - Cursor Previous Line
        handleCursorPreviousLine();
        break;
    case 'G':  // CHA - Cursor Horizontal Absolute
        handleCursorHorizontalAbsolute();
        break;
    case 'H':  // CUP - Cursor Position
    case 'f':  // HVP - Horizontal and Vertical Position
        handleCursorPosition();
        break;
    case 'J':  // ED - Erase in Display
        handleEraseInDisplay();
        break;
    case 'K':  // EL - Erase in Line
        handleEraseInLine();
        break;
    case 'L':  // IL - Insert Lines
        handleInsertLines();
        break;
    case 'M':  // DL - Delete Lines
        handleDeleteLines();
        break;
    case 'P':  // DCH - Delete Characters
        handleDeleteCharacters();
        break;
    case 'S':  // SU - Scroll Up
        handleScrollUp();
        break;
    case 'T':  // SD - Scroll Down
        handleScrollDown();
        break;
    case 'X':  // ECH - Erase Characters
        handleEraseCharacters();
        break;
    case '@':  // ICH - Insert Characters
        handleInsertCharacters();
        break;
    case 'm':  // SGR - Select Graphic Rendition
        handleSelectGraphicRendition();
        break;
    case 'r':  // DECSTBM - Set Top and Bottom Margins
        handleSetScrollingRegion();
        break;
    case 's':  // SCOSC - Save Cursor (SCO)
        handleSaveCursor();
        break;
    case 'u':  // SCORC - Restore Cursor (SCO)
        handleRestoreCursor();
        break;
    case 'n':  // DSR - Device Status Report
        handleDeviceStatusReport();
        break;
    case 'h':  // SM - Set Mode
        if (m_sequence.startsWith('?')) {
            handleSetPrivateMode();
        } else {
            handleSetMode();
        }
        break;
    case 'l':  // RM - Reset Mode
        if (m_sequence.startsWith('?')) {
            handleResetPrivateMode();
        } else {
            handleResetMode();
        }
        break;
    }
}

void VT100Parser::executeOSCCommand()
{
    // OSC sequences are typically for setting window title, etc.
    // For now, we'll just ignore them
    qDebug() << "OSC command:" << m_sequence;
}

void VT100Parser::executeDCSCommand()
{
    // DCS sequences are for device control
    // For now, we'll just ignore them
    qDebug() << "DCS command:" << m_sequence;
}

void VT100Parser::parseCSIParameters()
{
    m_parameters.clear();
    
    QString paramString = m_sequence;
    if (paramString.startsWith('?')) {
        paramString = paramString.mid(1);  // Remove '?' prefix
    }
    
    if (paramString.isEmpty()) {
        return;
    }
    
    QStringList parts = paramString.split(';');
    for (const QString &part : parts) {
        bool ok;
        int value = part.toInt(&ok);
        if (ok) {
            m_parameters.append(value);
        } else {
            m_parameters.append(DEFAULT_PARAMETER);
        }
    }
}

void VT100Parser::parseOSCParameters()
{
    // OSC parameters are typically semicolon-separated
    // Implementation depends on specific OSC command
}

void VT100Parser::resetSequence()
{
    m_sequence.clear();
    m_parameters.clear();
    m_intermediateChars.clear();
    m_finalChar = QChar();
}

void VT100Parser::handleControlCharacter(QChar ch)
{
    switch (ch.unicode()) {
    case 0x07:  // BEL - Bell
        handleBell();
        break;
    case 0x08:  // BS - Backspace
        handleBackspace();
        break;
    case 0x09:  // HT - Horizontal Tab
        handleTab();
        break;
    case 0x0A:  // LF - Line Feed
        handleLineFeed();
        break;
    case 0x0B:  // VT - Vertical Tab
        handleVerticalTab();
        break;
    case 0x0C:  // FF - Form Feed
        handleFormFeed();
        break;
    case 0x0D:  // CR - Carriage Return
        handleCarriageReturn();
        break;
    }
}

void VT100Parser::handleBell()
{
    emit bell();
}

void VT100Parser::handleBackspace()
{
    emit cursorBackward(1);
}

void VT100Parser::handleTab()
{
    emit tabForward(1);
}

void VT100Parser::handleLineFeed()
{
    emit cursorDown(1);
}

void VT100Parser::handleVerticalTab()
{
    emit cursorDown(1);
}

void VT100Parser::handleFormFeed()
{
    emit clearScreen();
    emit cursorPosition(0, 0);
}

void VT100Parser::handleCarriageReturn()
{
    emit cursorHorizontalAbsolute(0);
}

// CSI command handlers
void VT100Parser::handleCursorUp()
{
    int lines = m_parameters.isEmpty() ? 1 : qMax(1, m_parameters[0]);
    emit cursorUp(lines);
}

void VT100Parser::handleCursorDown()
{
    int lines = m_parameters.isEmpty() ? 1 : qMax(1, m_parameters[0]);
    emit cursorDown(lines);
}

void VT100Parser::handleCursorForward()
{
    int columns = m_parameters.isEmpty() ? 1 : qMax(1, m_parameters[0]);
    emit cursorForward(columns);
}

void VT100Parser::handleCursorBackward()
{
    int columns = m_parameters.isEmpty() ? 1 : qMax(1, m_parameters[0]);
    emit cursorBackward(columns);
}

void VT100Parser::handleCursorNextLine()
{
    int lines = m_parameters.isEmpty() ? 1 : qMax(1, m_parameters[0]);
    emit cursorNextLine(lines);
}

void VT100Parser::handleCursorPreviousLine()
{
    int lines = m_parameters.isEmpty() ? 1 : qMax(1, m_parameters[0]);
    emit cursorPreviousLine(lines);
}

void VT100Parser::handleCursorHorizontalAbsolute()
{
    int column = m_parameters.isEmpty() ? 1 : qMax(1, m_parameters[0]);
    emit cursorHorizontalAbsolute(column - 1);  // Convert to 0-based
}

void VT100Parser::handleCursorPosition()
{
    int row = m_parameters.size() > 0 ? qMax(1, m_parameters[0]) : 1;
    int column = m_parameters.size() > 1 ? qMax(1, m_parameters[1]) : 1;
    emit cursorPosition(row - 1, column - 1);  // Convert to 0-based
}

void VT100Parser::handleEraseInDisplay()
{
    int mode = m_parameters.isEmpty() ? 0 : m_parameters[0];
    switch (mode) {
    case 0:  // Clear from cursor to end of screen
        emit clearScreenFromCursor();
        break;
    case 1:  // Clear from beginning of screen to cursor
        emit clearScreenToCursor();
        break;
    case 2:  // Clear entire screen
    case 3:  // Clear entire screen and scrollback
        emit clearScreen();
        break;
    }
}

void VT100Parser::handleEraseInLine()
{
    int mode = m_parameters.isEmpty() ? 0 : m_parameters[0];
    switch (mode) {
    case 0:  // Clear from cursor to end of line
        emit clearLineFromCursor();
        break;
    case 1:  // Clear from beginning of line to cursor
        emit clearLineToCursor();
        break;
    case 2:  // Clear entire line
        emit clearLine();
        break;
    }
}

void VT100Parser::handleInsertLines()
{
    int count = m_parameters.isEmpty() ? 1 : qMax(1, m_parameters[0]);
    emit insertLines(count);
}

void VT100Parser::handleDeleteLines()
{
    int count = m_parameters.isEmpty() ? 1 : qMax(1, m_parameters[0]);
    emit deleteLines(count);
}

void VT100Parser::handleInsertCharacters()
{
    int count = m_parameters.isEmpty() ? 1 : qMax(1, m_parameters[0]);
    emit insertCharacters(count);
}

void VT100Parser::handleDeleteCharacters()
{
    int count = m_parameters.isEmpty() ? 1 : qMax(1, m_parameters[0]);
    emit deleteCharacters(count);
}

void VT100Parser::handleEraseCharacters()
{
    int count = m_parameters.isEmpty() ? 1 : qMax(1, m_parameters[0]);
    emit eraseCharacters(count);
}

void VT100Parser::handleScrollUp()
{
    int lines = m_parameters.isEmpty() ? 1 : qMax(1, m_parameters[0]);
    emit scrollUp(lines);
}

void VT100Parser::handleScrollDown()
{
    int lines = m_parameters.isEmpty() ? 1 : qMax(1, m_parameters[0]);
    emit scrollDown(lines);
}

void VT100Parser::handleSelectGraphicRendition()
{
    if (m_parameters.isEmpty()) {
        // Reset to default
        m_currentForeground = TerminalColor::Default;
        m_currentBackground = TerminalColor::Default;
        m_currentAttributes = TextAttribute::None;
    } else {
        parseGraphicRendition(m_parameters);
    }
    
    emit setTextAttributes(m_currentForeground, m_currentBackground, m_currentAttributes);
}

void VT100Parser::handleSetScrollingRegion()
{
    int top = m_parameters.size() > 0 ? qMax(1, m_parameters[0]) : 1;
    int bottom = m_parameters.size() > 1 ? qMax(1, m_parameters[1]) : 24;
    emit setScrollingRegion(top - 1, bottom - 1);  // Convert to 0-based
}

void VT100Parser::handleSaveCursor()
{
    emit saveCursor();
}

void VT100Parser::handleRestoreCursor()
{
    emit restoreCursor();
}

void VT100Parser::handleDeviceStatusReport()
{
    int mode = m_parameters.isEmpty() ? 0 : m_parameters[0];
    switch (mode) {
    case 5:  // Device status
        emit deviceStatusReport();
        break;
    case 6:  // Cursor position
        emit cursorPositionReport();
        break;
    }
}

void VT100Parser::handleSetMode()
{
    for (int param : m_parameters) {
        emit setMode(param, true);
    }
}

void VT100Parser::handleResetMode()
{
    for (int param : m_parameters) {
        emit setMode(param, false);
    }
}

void VT100Parser::handleSetPrivateMode()
{
    for (int param : m_parameters) {
        if (param == 1049 || param == 47) {
            emit useAlternateScreenBuffer(true);
        }
        emit setPrivateMode(param, true);
    }
}

void VT100Parser::handleResetPrivateMode()
{
    for (int param : m_parameters) {
        if (param == 1049 || param == 47) {
            emit useAlternateScreenBuffer(false);
        }
        emit setPrivateMode(param, false);
    }
}

void VT100Parser::parseGraphicRendition(const QVector<int> &params)
{
    for (int i = 0; i < params.size(); ++i) {
        int param = params[i];
        
        switch (param) {
        case 0:  // Reset
            m_currentForeground = TerminalColor::Default;
            m_currentBackground = TerminalColor::Default;
            m_currentAttributes = TextAttribute::None;
            break;
        case 1:  // Bold
            m_currentAttributes |= TextAttribute::Bold;
            break;
        case 2:  // Dim
            m_currentAttributes |= TextAttribute::Dim;
            break;
        case 3:  // Italic
            m_currentAttributes |= TextAttribute::Italic;
            break;
        case 4:  // Underline
            m_currentAttributes |= TextAttribute::Underline;
            break;
        case 5:  // Blink
            m_currentAttributes |= TextAttribute::Blink;
            break;
        case 7:  // Reverse
            m_currentAttributes |= TextAttribute::Reverse;
            break;
        case 9:  // Strikethrough
            m_currentAttributes |= TextAttribute::Strikethrough;
            break;
        case 22: // Normal intensity (not bold or dim)
            m_currentAttributes &= ~(TextAttribute::Bold | TextAttribute::Dim);
            break;
        case 23: // Not italic
            m_currentAttributes &= ~TextAttribute::Italic;
            break;
        case 24: // Not underlined
            m_currentAttributes &= ~TextAttribute::Underline;
            break;
        case 25: // Not blinking
            m_currentAttributes &= ~TextAttribute::Blink;
            break;
        case 27: // Not reversed
            m_currentAttributes &= ~TextAttribute::Reverse;
            break;
        case 29: // Not strikethrough
            m_currentAttributes &= ~TextAttribute::Strikethrough;
            break;
        default:
            if (param >= 30 && param <= 37) {
                // Foreground colors
                m_currentForeground = parseColor(param - 30);
            } else if (param >= 40 && param <= 47) {
                // Background colors
                m_currentBackground = parseColor(param - 40);
            } else if (param >= 90 && param <= 97) {
                // Bright foreground colors
                m_currentForeground = parseColor(param - 90, true);
            } else if (param >= 100 && param <= 107) {
                // Bright background colors
                m_currentBackground = parseColor(param - 100, true);
            } else if (param == 38 || param == 48) {
                // Extended color sequences (256-color or RGB)
                // This would require parsing additional parameters
                // For now, skip to avoid complexity
            } else if (param == 39) {
                // Default foreground
                m_currentForeground = TerminalColor::Default;
            } else if (param == 49) {
                // Default background
                m_currentBackground = TerminalColor::Default;
            }
            break;
        }
    }
}

TerminalColor VT100Parser::parseColor(int colorCode, bool bright)
{
    if (bright) {
        switch (colorCode) {
        case 0: return TerminalColor::BrightBlack;
        case 1: return TerminalColor::BrightRed;
        case 2: return TerminalColor::BrightGreen;
        case 3: return TerminalColor::BrightYellow;
        case 4: return TerminalColor::BrightBlue;
        case 5: return TerminalColor::BrightMagenta;
        case 6: return TerminalColor::BrightCyan;
        case 7: return TerminalColor::BrightWhite;
        }
    } else {
        switch (colorCode) {
        case 0: return TerminalColor::Black;
        case 1: return TerminalColor::Red;
        case 2: return TerminalColor::Green;
        case 3: return TerminalColor::Yellow;
        case 4: return TerminalColor::Blue;
        case 5: return TerminalColor::Magenta;
        case 6: return TerminalColor::Cyan;
        case 7: return TerminalColor::White;
        }
    }
    
    return TerminalColor::Default;
}
QChar VT100Parser::mapCharacter(QChar ch)
{
    // If not using the graphics character set, return as is
    if (m_characterSets[m_currentCharacterSet] != '0') {
        return ch;
    }
    
    // Map VT100 Special Graphics characters to Unicode
    // Reference: http://vt100.net/docs/vt100-ug/table3-9.html
    switch (ch.unicode()) {
    case '_': return QChar(0x00A0); // Blank
    case '`': return QChar(0x25C6); // Diamond
    case 'a': return QChar(0x2591); // Checkerboard (using light shade)
    case 'b': return QChar(0x2409); // HT
    case 'c': return QChar(0x240C); // FF
    case 'd': return QChar(0x240D); // CR
    case 'e': return QChar(0x240A); // LF
    case 'f': return QChar(0x00B0); // Degree
    case 'g': return QChar(0x00B1); // Plus/Minus
    case 'h': return QChar(0x2424); // NL
    case 'i': return QChar(0x240B); // VT
    case 'j': return QChar(0x2518); // Lower Right Corner
    case 'k': return QChar(0x2510); // Upper Right Corner
    case 'l': return QChar(0x250C); // Upper Left Corner
    case 'm': return QChar(0x2514); // Lower Left Corner
    case 'n': return QChar(0x253C); // Crossing Lines
    case 'o': return QChar(0x23BA); // Horizontal Line Scan 1
    case 'p': return QChar(0x23BB); // Horizontal Line Scan 3
    case 'q': return QChar(0x2500); // Horizontal Line
    case 'r': return QChar(0x23BC); // Horizontal Line Scan 7
    case 's': return QChar(0x23BD); // Horizontal Line Scan 9
    case 't': return QChar(0x251C); // Left T
    case 'u': return QChar(0x2524); // Right T
    case 'v': return QChar(0x2534); // Bottom T
    case 'w': return QChar(0x252C); // Top T
    case 'x': return QChar(0x2502); // Vertical Line
    case 'y': return QChar(0x2264); // Less Than or Equal
    case 'z': return QChar(0x2265); // Greater Than or Equal
    case '{': return QChar(0x03C0); // Pi
    case '|': return QChar(0x2260); // Not Equal
    case '}': return QChar(0x00A3); // British Pound
    case '~': return QChar(0x00B7); // Middle Dot
    }
    
    return ch;
}
