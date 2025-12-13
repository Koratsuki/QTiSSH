#include "terminalchar.h"

// Standard VT100 color palette
const QColor TerminalColorPalette::standardColors[16] = {
    // Normal colors (0-7)
    QColor(0, 0, 0),         // Black
    QColor(128, 0, 0),       // Red
    QColor(0, 128, 0),       // Green
    QColor(128, 128, 0),     // Yellow
    QColor(0, 0, 128),       // Blue
    QColor(128, 0, 128),     // Magenta
    QColor(0, 128, 128),     // Cyan
    QColor(192, 192, 192),   // White
    
    // Bright colors (8-15)
    QColor(128, 128, 128),   // Bright Black (Gray)
    QColor(255, 0, 0),       // Bright Red
    QColor(0, 255, 0),       // Bright Green
    QColor(255, 255, 0),     // Bright Yellow
    QColor(0, 0, 255),       // Bright Blue
    QColor(255, 0, 255),     // Bright Magenta
    QColor(0, 255, 255),     // Bright Cyan
    QColor(255, 255, 255)    // Bright White
};

const QColor TerminalColorPalette::defaultForeground = QColor(192, 192, 192);  // Light gray
const QColor TerminalColorPalette::defaultBackground = QColor(0, 0, 0);        // Black

QColor TerminalColorPalette::getColor(TerminalColor color, bool bright)
{
    if (color == TerminalColor::Default) {
        return bright ? defaultBackground : defaultForeground;
    }
    
    int colorIndex = static_cast<int>(color);
    
    // Handle standard colors (0-15)
    if (colorIndex >= 0 && colorIndex < 16) {
        return standardColors[colorIndex];
    }
    
    // For extended colors (16-255), we'll use a simplified mapping
    // In a full implementation, you'd want to support 256-color palette
    if (colorIndex >= 16 && colorIndex < 256) {
        // Simplified: map to nearest standard color
        return standardColors[colorIndex % 16];
    }
    
    // Fallback to default
    return bright ? defaultBackground : defaultForeground;
}

QColor TerminalColorPalette::getDefaultForeground()
{
    return defaultForeground;
}

QColor TerminalColorPalette::getDefaultBackground()
{
    return defaultBackground;
}
