# QTiSSH - Qt SSH Connection Manager

A modern, user-friendly SSH connection manager built with Qt. Manage multiple SSH connections with ease using a tabbed interface.

![Qt](https://img.shields.io/badge/Qt-5%2F6-green.svg)
![C++](https://img.shields.io/badge/C++-17-blue.svg)
![License](https://img.shields.io/badge/license-MIT-blue.svg)

## Features

✨ **Multiple Simultaneous Connections** - Open multiple SSH sessions in tabs  
🔐 **Flexible Authentication** - Password, public key and SSH agent (with agent forwarding)  
🔑 **Encrypted Credentials** - Passwords are encrypted (AES-256) with an optional master password  
💾 **Server Management** - Save and organize your server configurations  
🎨 **Clean Interface** - Modern Qt-based UI with intuitive controls  
📋 **Server Organization** - Group servers and add tags for easy management  
🚀 **Quick Connect** - Double-click to connect instantly  
📁 **SFTP File Browser** - Built-in dual-pane file manager with drag-and-drop  
⬆️ **File Transfers** - Upload and download files with real progress tracking  
🔄 **Transfer Queue** - Manage multiple file transfers with queue system  
🧩 **Snippets** - Save and run frequently used commands  
🕘 **Command History** - Per-server history of executed commands  
🌐 **SSH Tunnels** - Local/Remote/Dynamic port forwarding per connection  
🪂 **Jump Host** - Connect through an intermediate SSH host  
🔀 **Import/Export** - Backup and restore server configurations (JSON)  
🌓 **Light / Dark Theme** - Starts in light mode; switch from `Edit → Appearance` or the 🌙 / ☀️ button  
🎨 **Terminal Colors** - Customizable foreground/background terminal colors  
⚙️ **Custom SSH Options & Profiles** - Per-server options plus reusable named profiles  
🔁 **Auto-reconnect** - Automatic reconnection after connection loss (max 3 attempts)  
✏️ **Remote File Editing** - Edit remote files locally with auto-sync (Ctrl+S, external-change detection)  
📊 **Server Monitoring** - CPU load, memory, disk and top processes via SSH  
🕵️ **Network Discovery** - Scan the local network for SSH servers (port 22)  
📜 **Connection Logs** - Timestamped session logs with viewer  
🧩 **Split Panes** - Multiple terminals to the same server in one tab  
🗂️ **Recent Connections** - Quick access to the last 15 servers  
🔑 **OS Keychain** - Credentials stored in the system Secret Service (Linux) with file-encryption fallback  
🌐 **Global Hotkeys** - System-wide Quick Connect / Toggle Window shortcuts (X11)  
🖥️ **System Tray** - Minimize to tray with quick access menu  
💾 **Session Restoration** - Restore open tabs on restart  
🔒 **Application Lock** - Global password required to open the app  
🌍 **Interface Languages** - English and Spanish, switchable from `Edit → Language` (the app restarts automatically to apply)

## Screenshots

### Main Window
The main window features a server list on the left and tabbed SSH terminals on the right.

### Add Server Dialog
Easy-to-use dialog for adding new servers with all necessary connection parameters.

## Requirements

- Qt 5.12+ or Qt 6.x (DBus and Network modules; global hotkeys require Qt Widgets)
- CMake 3.16+
- C++17 compatible compiler
- OpenSSL (libssl-dev / libcrypto)
- OpenSSH client (`ssh` and `sftp` commands must be available in PATH)
- Linux: X11 development headers (`libx11-dev`) for global hotkeys

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
     - **Password**: Enter your password (optionally encrypted with a master password)
     - **Public Key**: Browse to your private key file (e.g., `~/.ssh/id_rsa`)
     - **SSH Agent**: Use a key loaded in your running SSH agent
   - **Jump Host** (optional): SSH host used to reach this server
   - **Group** (optional): Organize servers into groups
   - **Tags** (optional): Add tags for filtering
   - **Advanced**:
     - **Strict Host Key Verification**: Verify the server's host key
     - **Forward Agent**: Forward your SSH agent to the remote server
     - **Tunnels**: One per line, e.g. `L:8080:localhost:80`, `R:5432:dbhost:5432` or `D:1080`

3. Click **Add Server**

### Connecting to a Server

- **Double-click** a server in the list, or
- Select a server and click **SSH Terminal** for a terminal session
- Select a server and click **SFTP Browser** for file management

A new tab will open with either the SSH terminal or SFTP file browser.

### Managing Servers

- **Edit**: Select a server and click **Edit** to modify its configuration
- **Delete**: Select a server and click **Delete** to remove it
- **Import/Export**: `File → Export Servers` / `File → Import Servers` to back up or restore configurations as JSON. The file format is documented in [Import / Export (JSON format)](#import--export-json-format).
- **Multiple Connections**: Open multiple tabs to the same or different servers

### Appearance (Light / Dark Theme)

- QTiSSH starts in **Light** mode by default. Use the 🌙 / ☀️ button next to the search bar, or
  `Edit → Appearance`, to switch themes.
- `Edit → Appearance` offers:
  - **Toggle Light/Dark Theme** (Ctrl+Shift+D) — flips the current theme.
  - **Light Theme** / **Dark Theme** — pick one directly; the active one shows a check mark.
- The choice is saved in `settings.conf` (`appearance/theme`) and restored on the next launch.

### Import / Export (JSON format)

`File → Export Servers` writes a backup of every saved server as JSON, and
`File → Import Servers` reads one back. The file is the same format used internally by
`servers.json`, so you can hand-edit it, generate it from a script, or move servers between machines.

#### Top-level structure

The document root is a **JSON array with one object per server** — not an object with a wrapper key.
The array is written indented; a compact array is also accepted on import.

```json
[
  {
    "id": "3f1c9a52-7b0e-4a1e-9d2f-5c6b8e0a1d34",
    "alias": "Web Server",
    "host": "web.example.com",
    "port": 22,
    "username": "deploy",
    "password": "",
    "keyPath": "/home/deploy/.ssh/id_ed25519",
    "authType": 1,
    "group": "Production",
    "tags": "web, nginx, tls",
    "strictHostKeyChecking": true,
    "tunnels": "L:8080:localhost:80",
    "jumpHost": "",
    "forwardAgent": true,
    "sshOptions": "ServerAliveInterval=30\nCompression=yes",
    "autoReconnect": true,
    "profileName": ""
  }
]
```

#### Field reference

| Field | Type | Required | Default | Description |
| --- | --- | --- | --- | --- |
| `id` | string | no | new UUID | Unique server identifier. **Regenerated on every import**, so importing the same file twice creates two independent copies. |
| `alias` | string | no | `""` | Friendly display name shown in the server list. |
| `host` | string | **yes** | — | IP address or hostname. An empty host makes the entry unusable for connecting. |
| `port` | number | no | `22` | SSH port. Must be between 1 and 65535. |
| `username` | string | **yes** | — | SSH user. An empty username makes the entry unusable for connecting. |
| `password` | string | no | `""` | See [Password values](#password-values) below. Empty means "no stored password". |
| `keyPath` | string | no | `""` | Absolute path to the private key, used when `authType` is `1`. Never copied or modified. |
| `authType` | number | no | `0` | `0` = Password, `1` = PublicKey, `2` = SSHAgent. See [authType values](#authtype-values). |
| `group` | string | no | `""` | Group the server is filed under. Must already exist as a folder, otherwise the server appears ungrouped. |
| `tags` | string | no | `""` | Free-form labels for filtering, comma separated (e.g. `"web, nginx"`). |
| `strictHostKeyChecking` | boolean | no | `false` | Verify the server's host key. **Disabled by default** — enable it in production. |
| `tunnels` | string | no | `""` | Port forwards, see [Tunnel syntax](#tunnel-syntax). |
| `jumpHost` | string | no | `""` | ProxyJump target, passed to `ssh -J`. Use `user@bastion.example.com`. |
| `forwardAgent` | boolean | no | `false` | Forward your SSH agent to the remote server. |
| `sshOptions` | string | no | `""` | Extra OpenSSH options, one `Key=Value` per line, `#` starts a comment. See [SSH options](#ssh-options-syntax). |
| `autoReconnect` | boolean | no | `false` | Reconnect automatically after a connection loss (up to 3 attempts). |
| `profileName` | string | no | `""` | Name of a saved profile from `profiles.json`. Its options are merged underneath the server's own `sshOptions`. |

Only `host` and `username` matter for the entry to be usable. Every other key may be omitted —
missing keys fall back to the defaults in the table, and unknown keys are ignored, so files
produced by older or newer versions still import.

#### authType values

| Value | Meaning | Uses |
| --- | --- | --- |
| `0` | **Password** | `password` |
| `1` | **PublicKey** | `keyPath` |
| `2` | **SSHAgent** | key already loaded in the running `ssh-agent` |

#### Password values

`password` is a plain string, but QTiSSH uses two prefixes to mark values it cannot read as
plain text. Both are safe to write in a file you generate yourself:

| Value | Meaning | What happens on import |
| --- | --- | --- |
| `"secret123"` | Plain text | Stored as-is and re-encrypted (or sent to the keychain) on the next save. |
| `"enc:<base64>"` | Encrypted with your **master password** (AES-256-CBC) | Kept encrypted, but **only if this QTiSSH install has the same master password set and unlocked**. If no master password is set, the value is **discarded** and you must re-enter the password. |
| `"kc:<id>"` | Marker meaning "the real password lives in the OS keychain" | **Always discarded** on import, because it points at the keychain entry of the *source* machine. Re-enter the password. |
| `""` | No stored password | Nothing to connect with until you add one. |

**Recommended practice:** leave `password` empty in exported/hand-written files and store secrets
in a password manager. The only exception is a backup you are moving to another machine that
shares the same master password — in that case the `enc:` value survives the round trip.

#### Tunnel syntax

`tunnels` is a single string holding one or more specs separated by whitespace, commas or
semicolons. Each spec may be prefixed with its type:

| Form | Result |
| --- | --- |
| `L:8080:localhost:80` | Local forward (`ssh -L 8080:localhost:80`) |
| `R:5432:dbhost:5432` | Remote forward (`ssh -R 5432:dbhost:5432`) |
| `D:1080` | SOCKS proxy (`ssh -D 1080`) |
| `8080:localhost:80` | No prefix — treated as local forward |

#### SSH options syntax

`sshOptions` is a multi-line string of OpenSSH `Key=Value` pairs. Each line becomes one `-o Key=Value`
argument, and the profile options (from `profileName`) are placed *before* the server's own, so a
server-specific line wins on conflict.

```
# keep the session alive through NAT
ServerAliveInterval=30
ServerAliveCountMax=6
Compression=yes
```

#### Minimal example

Only the two mandatory fields, with everything else defaulted:

```json
[
  {
    "host": "192.168.1.10",
    "username": "root"
  },
  {
    "host": "backup.example.org",
    "username": "admin",
    "port": 2222,
    "authType": 1,
    "keyPath": "/home/admin/.ssh/id_ed25519"
  }
]
```

#### Import behaviour

- Imported servers are **added** to the current list; existing entries are never overwritten or deleted.
- Each imported server gets a **fresh `id`**, so imports never collide with your current servers.
- Folders are not part of the file — `group` is only a label, so a group that does not exist
  locally leaves the server ungrouped.
- The dialog reports how many servers were imported (`Imported N server(s).`).
- A file whose root is not an array, or that is not valid JSON, imports 0 servers.

### Password Security

- Set a **master password** via `Edit → Security → Set Master Password`. Once set, stored passwords are encrypted (AES-256-CBC, PBKDF2) on disk.
- You will be prompted to unlock on startup; encrypted passwords are stored as `enc:...` in `servers.json`.
- Use `Edit → Security` to change, remove or lock the master password.

### Application Lock

- Set a **global application password** via `Edit → Security → Application Lock → Set Application Password`. When enabled, QTiSSH asks for it before the main window opens.
- Use `Change Application Password` to update it, `Remove Application Password` to disable the lock, and `Lock Application Now` to re-lock without restarting.
- The password is never stored: only a salted PBKDF2-HMAC-SHA256 verifier is kept in `applock.conf`. It is independent of the master password used for credential encryption.

### Language

- QTiSSH ships with **English** (default) and **Spanish** interfaces. Switch anytime via `Edit → Language → English / Español`.
- The choice is saved in `settings.conf` (`appearance/language`) and the app restarts automatically to apply it.

### Snippets and Command History

- **Snippets** (`Edit → Snippets`): save reusable commands and run them in the active terminal.
- **Command History** (`Edit → Command History`): per-server list of previously executed commands; double-click or select and execute to re-run.

### Using the SFTP Browser

1. **Connect**: Click "SFTP Browser" to open a file management tab
2. **Navigate**: 
   - **Local files** (left pane): Browse your local file system
   - **Remote files** (right pane): Browse server files after connecting
3. **File Operations**:
   - **Upload**: Select local files and click "⬆️ Upload" or drag files to remote pane
   - **Download**: Select remote files and click "⬇️ Download"
   - **Delete**: Select remote files and click "🗑️ Delete"
   - **New Folder**: Click "📁 New Folder" to create directories
4. **Transfer Queue**: Monitor file transfers in the bottom panel with progress bars

### Closing Connections

Click the **X** button on a tab to close the connection. If the connection is still active, you'll be prompted to confirm.

## Configuration

All configuration lives under `~/.config/QTiSSH/`:

- `servers.json` - server configurations (passwords may be encrypted or keychain-marked)
- `settings.conf` - application settings (theme, language, terminal colors, font size, tray, hotkeys)
- `snippets.json` - saved command snippets
- `history.json` - per-server command history
- `master.conf` - master password verifier (only if you set one)
- `applock.conf` - application password verifier (only if the app lock is enabled)
- `profiles.json` - named SSH option profiles
- `recent.json` - recent connections (last 15)
- `logs/` - timestamped SSH session logs
- `session.json` - last open tabs (used for session restoration)

## Security Notes

- Without a master password, passwords are stored in the OS keychain (Linux Secret Service via D-Bus) when available, otherwise as plain text. Enable/disable keychain usage in code via `security/useKeychain`; set a master password (`Edit → Security`) to fall back to file encryption.
- Encrypted passwords use AES-256-CBC with a key derived from your master password via PBKDF2-HMAC-SHA256 (100,000 iterations). The verifier and salt in `master.conf` cannot be used to recover the password.
- Host key verification is **disabled by default** for convenience. Enable **Strict Host Key Verification** in the server's Advanced options for production use.
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
│   ├── main.cpp                    # Application entry point
│   ├── mainwindow.h/cpp/ui         # Main window with server list and tabs
│   ├── add_server.h/cpp/ui         # Add/Edit server dialog
│   ├── serverconfig.h/cpp          # Server configuration data model
│   ├── servermanager.h/cpp         # Server storage and management
│   ├── sshterminal.h/cpp           # SSH terminal widget
│   ├── sftpbrowser.h/cpp           # SFTP file browser widget
│   ├── sftpconnection.h/cpp        # SFTP connection handler
│   ├── filetransfer.h/cpp          # Individual file transfer
│   ├── filetransfermanager.h/cpp   # File transfer queue management
│   └── CMakeLists.txt              # Build configuration
└── README.md
```

### Adding Features

Some ideas for future enhancements:
- Tab reordering and grouping
- SCP quick actions (right-click upload/download)
- Extended SFTP operations (rename, chmod, symlinks)

## Contributing

Contributions are welcome! Please feel free to submit pull requests or open issues for bugs and feature requests.

## License

This project is open source. See LICENSE file for details.

## Acknowledgments

- Built with [Qt](https://www.qt.io/)
- Uses OpenSSH for SSH connectivity

---

**Note**: This is a work in progress. If you encounter any issues or have suggestions, please open an issue on GitHub.
