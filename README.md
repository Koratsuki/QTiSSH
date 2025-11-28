# QTiSSH - Qt SSH Connection Manager

A modern, user-friendly SSH connection manager built with Qt. Manage multiple SSH connections with ease using a tabbed interface.

![Qt](https://img.shields.io/badge/Qt-5%2F6-green.svg)
![C++](https://img.shields.io/badge/C++-17-blue.svg)
![License](https://img.shields.io/badge/license-MIT-blue.svg)

## Features

✨ **Multiple Simultaneous Connections** - Open multiple SSH sessions in tabs  
🔐 **Flexible Authentication** - Support for both password and public key authentication  
💾 **Server Management** - Save and organize your server configurations  
🎨 **Clean Interface** - Modern Qt-based UI with intuitive controls  
📋 **Server Organization** - Group servers and add tags for easy management  
🚀 **Quick Connect** - Double-click to connect instantly  

## Screenshots

### Main Window
The main window features a server list on the left and tabbed SSH terminals on the right.

### Add Server Dialog
Easy-to-use dialog for adding new servers with all necessary connection parameters.

## Requirements

- Qt 5.12+ or Qt 6.x
- CMake 3.16+
- C++17 compatible compiler
- OpenSSH client (`ssh` command must be available in PATH)

## Building from Source

### Linux

```bash
cd src
mkdir build
cd build
cmake ..
make
./QTiSSH
```

### Windows

```bash
cd src
mkdir build
cd build
cmake -G "MinGW Makefiles" ..
mingw32-make
QTiSSH.exe
```

Or use Qt Creator:
1. Open `src/CMakeLists.txt` in Qt Creator
2. Configure the project
3. Build and run

## Usage

### Adding a Server

1. Click the **Add** button or use `File → Add Server` (Ctrl+N)
2. Fill in the server details:
   - **Alias**: A friendly name for the server
   - **Host**: IP address or hostname
   - **Port**: SSH port (default: 22)
   - **Username**: SSH username
   - **Authentication Type**:
     - **Password**: Enter your password
     - **Public Key**: Browse to your private key file (e.g., `~/.ssh/id_rsa`)
   - **Group** (optional): Organize servers into groups
   - **Tags** (optional): Add tags for filtering

3. Click **Add Server**

### Connecting to a Server

- **Double-click** a server in the list, or
- Select a server and click **Connect**

A new tab will open with the SSH terminal session.

### Managing Servers

- **Edit**: Select a server and click **Edit** to modify its configuration
- **Delete**: Select a server and click **Delete** to remove it
- **Multiple Connections**: Open multiple tabs to the same or different servers

### Closing Connections

Click the **X** button on a tab to close the connection. If the connection is still active, you'll be prompted to confirm.

## Configuration

Server configurations are stored in:
- **Linux**: `~/.local/share/qtissh/servers.json`
- **Windows**: `%APPDATA%/qtissh/servers.json`
- **macOS**: `~/Library/Application Support/qtissh/servers.json`

## Security Notes

- Passwords are stored in plaintext in the configuration file. Consider using public key authentication for better security.
- The application disables strict host key checking for convenience. For production use, you may want to enable it.
- SSH keys are referenced by path and not copied or modified by the application.

## Troubleshooting

### "Failed to start SSH" Error

Ensure that the `ssh` command is available in your system PATH:

```bash
# Linux/macOS
which ssh

# Windows
where ssh
```

If SSH is not installed:
- **Linux**: `sudo apt-get install openssh-client` (Debian/Ubuntu) or equivalent
- **Windows**: Use OpenSSH that comes with Windows 10/11 or install it via Windows Features
- **macOS**: SSH is pre-installed

### Connection Hangs

If a connection hangs:
1. Close the tab (you'll be prompted)
2. Check your network connectivity
3. Verify the server address and port are correct
4. Ensure the server's SSH service is running

### Authentication Fails

- **Password Authentication**: Verify username and password are correct
- **Key Authentication**: 
  - Ensure the key file has proper permissions (600 on Linux/macOS)
  - Verify the corresponding public key is in the server's `~/.ssh/authorized_keys`
  - Try the key manually: `ssh -i /path/to/key user@host`

## Development

### Project Structure

```
QTiSSH/
├── src/
│   ├── main.cpp              # Application entry point
│   ├── mainwindow.h/cpp/ui   # Main window with server list and tabs
│   ├── add_server.h/cpp/ui   # Add/Edit server dialog
│   ├── serverconfig.h/cpp    # Server configuration data model
│   ├── servermanager.h/cpp   # Server storage and management
│   ├── sshterminal.h/cpp     # SSH terminal widget
│   └── CMakeLists.txt        # Build configuration
└── README.md
```

### Adding Features

Some ideas for future enhancements:
- SSH tunneling/port forwarding
- SFTP file browser
- Connection profiles with custom SSH options
- Import/export server configurations
- Dark/light theme toggle
- Terminal color scheme customization
- Command history
- Connection logs

## Contributing

Contributions are welcome! Please feel free to submit pull requests or open issues for bugs and feature requests.

## License

This project is open source. See LICENSE file for details.

## Acknowledgments

- Built with [Qt](https://www.qt.io/)
- Uses OpenSSH for SSH connectivity

---

**Note**: This is a work in progress. If you encounter any issues or have suggestions, please open an issue on GitHub.

