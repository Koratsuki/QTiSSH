#!/bin/bash

echo "🧹 Cleaning Qt build cache..."

# Remove all generated UI headers
find . -name "ui_*.h" -delete
echo "✅ Removed generated UI headers"

# Remove all MOC files
find . -name "moc_*" -delete
echo "✅ Removed MOC files"

# Remove all Makefiles
find . -name "Makefile*" -delete
echo "✅ Removed Makefiles"

# Remove build directories
rm -rf build*
rm -rf debug
rm -rf release
echo "✅ Removed build directories"

# Remove object files
find . -name "*.o" -delete
echo "✅ Removed object files"

echo ""
echo "🎯 Build cache cleaned successfully!"
echo ""
echo "📋 Next steps in Qt Creator:"
echo "1. Build → Clean All"
echo "2. Build → Rebuild All"
echo ""
echo "This will force Qt Creator to regenerate all files with the correct UI connections."

