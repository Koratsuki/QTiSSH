Pending tasks:
==



- Parse file from .ssh/config into a data structure that fits

-Subscribe to file to check for changes

- Update datastructure in memory whenever the user makes changes

- Save a copy of file whenever you write. Keep track of changes (git?) and allow the user to rollback 

- Translate from that data structure to a string

- Representation that will be printed to file on change

- On main thread: Make user choose if they want to keep this program's config in sync with .ssh/config or on a separate .local/share/QTiSSH folder

- There will be a folder that would be a perfect ~/.ssh/ folder if copied

- Keep API to main and data structures well documented 

- SSH Tunneling/Port Forwarding - Local/remote port forwarding with GUI management
- Jump Host Support - Connect through bastion/jump servers
- Custom SSH Options - Support for advanced SSH parameters (compression, ciphers, etc.)

#### File Management

- SFTP Browser - Built-in file transfer with drag-and-drop and progress bar
- Remote File Editing - Edit remote files locally with auto-sync

#### UI/UX Improvements

- Dark/Light Themes - Theme switching with system preference detection
- Custom Terminal Colors - Configurable color schemes
- Font Customization - Terminal font size and family selection
- Tab Management - Tab reordering, grouping, session saving
- Split Panes - Multiple terminals in same tab

#### Productivity Features

- Command History - Per-server command history with search
- Snippets/Macros - Save and execute common command sequences
- Connection Logs - Session logging with timestamps
- Server Monitoring - Basic system stats (CPU, memory, disk)
- Quick Commands - Predefined buttons for common tasks

#### Configuration & Management

- Import/Export Configs - Backup and share server configurations
- Server Groups/Folders - Hierarchical organization
- Connection Profiles - Different settings per environment
- Auto-reconnect - Automatic reconnection on connection loss
- Session Restoration - Restore tabs on application restart

#### Cross-Platform & Integration
- Windows Terminal Integration - Use Windows Terminal as backend
- iTerm2 Integration - macOS iTerm2 protocol support
- System Tray - Minimize to system tray with quick access
- Global Hotkeys - System-wide shortcuts to open connections

#### Search & Discovery
- Server Search - Quick search/filter servers by name, tags, groups
- Network Discovery - Scan local network for SSH servers
- Recent Connections - Quick access to recently used servers
