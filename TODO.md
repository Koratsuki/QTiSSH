Pending tasks:
==

#### Security & Authentication
- [x] SSH Agent Integration - Support for ssh-agent forwarding
- [x] Encrypted Password Storage - Master-password file encryption + OS keychain (Linux Secret Service via D-Bus) with graceful fallback
- [x] Host Key Verification - Strict mode with optional "accept all" per server
- [x] Application Lock - Global password (PBKDF2 verifier) required to open the app

#### Advanced SSH Features

- [x] SSH Tunneling/Port Forwarding - Local/remote/dynamic forwarding (L/R/D) with GUI management
- [x] Jump Host Support - Connect through bastion/jump servers
- [x] Custom SSH Options - Per-server options plus reusable named profiles (compression, ciphers, etc.)

#### File Management

- [x] SFTP Browser - Built-in file transfer with drag-and-drop and progress bar
- [x] Remote File Editing - Edit remote files locally with auto-sync (download/upload, Ctrl+S, external-change detection)

#### UI/UX Improvements

- [x] Add a tree view where you can create folders and within them create servers to have better organization in the left panel
- [x] Dark/Light Themes - Theme switching with system preference detection
- [x] Custom Terminal Colors - Configurable color schemes
- [x] Font Customization - Terminal font size and family selection
- [x] Tab Management - Tab reordering, grouping, session saving (session restore on restart)
- [x] Split Panes - Multiple terminals in same tab (horizontal/vertical split)
- [x] Interface Language - English/Spanish selector (`Edit → Language`), saved in settings.conf

#### Productivity Features

- [x] Command History - Per-server command history with search
- [x] Snippets/Macros - Save and execute common command sequences
- [x] Connection Logs - Session logging with timestamps and log viewer
- [x] Server Monitoring - Basic system stats (CPU, memory, disk)
- [x] Quick Commands - Predefined buttons for common tasks

#### Configuration & Management

- [x] Import/Export Configs - Backup and share server configurations
- [x] Server Groups/Folders - Hierarchical organization
- [x] Connection Profiles - Different settings per environment
- [x] Auto-reconnect - Automatic reconnection on connection loss (max 3 attempts)
- [x] Session Restoration - Restore tabs on application restart

#### Cross-Platform & Integration
- [x] Windows Terminal Integration - Use Windows Terminal as backend
- [x] iTerm2 Integration - macOS iTerm2 protocol support
- [x] System Tray - Minimize to system tray with quick access
- [x] Global Hotkeys - System-wide shortcuts (Quick Connect / Toggle Window, X11 + QShortcut fallback)

#### Search & Discovery
- [x] Server Search - Quick search/filter servers by name, tags, groups
- [x] Network Discovery - Scan local network for SSH servers (port 22)
- [x] Recent Connections - Quick access to recently used servers (last 15)

