# hx Client User Manual

This manual describes how to build, configure and use the **hx** client (also known as Synapse Hotline X or *shx*).  The client implements the Hotline protocol and may be compiled for either a terminal (TTY) interface or a graphical GTK+ interface.

## 1. Building the Client

### TTY Version
1. Run `./configure` in the source directory.
2. Run `make` to compile the client.
3. Launch with `./hx`.

The TTY build requires GNU readline. If the library is missing, obtain it from the GNU FTP site.

### GTK+ Version
1. Run `./configure --enable-gtk` to enable the graphical frontend.
2. Optionally add `--enable-dulled` for dimmed icons when users are idle.
3. Compile with `make`.

Additional build options include SOCKS support via `--with-socks` and `--with-socks-include` as noted in the `INSTALL` file.

## 2. Startup Configuration

At launch the client reads commands from a startup script `~/.hxrc`.  A sample is provided in the repository.  The file can enable sounds and associate audio files with events:

```shell
snd -O
snd -s ~/hxsnds/logged-in.wav login off
snd -s ~/hxsnds/message.wav message on
snd -s ~/hxsnds/chatpost.wav chat off
snd -s ~/hxsnds/newspost.wav news off
snd -s ~/hxsnds/join.wav join off
snd -s ~/hxsnds/part.wav part off
snd -s ~/hxsnds/chatinvite.wav chat_invite on
snd -s ~/hxsnds/filedone.wav file_done off
snd -s ~/hxsnds/error.wav error on
```

The above entries load sound files and control whether each event will play audio.

Persistent settings such as window positions and icon resources are stored in `~/.hxvars`.  The following fragment shows typical variables:

```text
chat_font[0][0]='shine'
user_icon_files[0]='zombie_icons.rsrc'
user_icon_files[1]='bm_icons.rsrc'
user_icon_files[2]='icons.rsrc'
icon_files[0]='icons.rsrc'
window_geometry[0][chat]='628x405+83+81'
window_geometry[0][toolbar]='362x33+303+0'
window_geometry[0][users]='254x842+0+0'
window_geometry[0][tasks]='444x118+763+442'
window_geometry[0][news]='485x437+39+512'
window_geometry[0][tracker]='914x432+49+301'
window_geometry[0][post]='453x190+422+493'
window_geometry[0][options]='337x373+370+322'
window_geometry[0][useredit]='250x637+652+184'
window_geometry[0][connect]='359x239+460+392'
window_geometry[0][files]='0x0+0+0'
tty_show_user_parts='1'
tty_show_user_joins='1'
tty_show_user_changes='1'
tty_chat_pretty='1'
strip_ansi='0'
infoprefix=' '
```

Values are loaded at startup and updated when you issue the `savevars` command or exit the client.

## 3. Using the Client

### Connecting to a Server
Use `/server` to connect to a Hotline server.  The command accepts many options:

```
/server [OPTIONS] <address>[:][port]
  -l, --login <login>
  -p, --password <password>
  -n, --nickname <nickname>
  -i, --icon <icon>
  -c, --cipher {RC4, BLOWFISH, IDEA, NONE}
  -z, --zip {GZIP, NONE}
  -o, --old    [not secure]
  -s, --secure
```

For GTK users the "Connect" window provides the same fields.

### Command Reference
The built‑in `/help` command lists every available command.  Important ones include:

| Command    | Purpose                                   |
|------------|-------------------------------------------|
| `away`     | Toggle away status                        |
| `broadcast`| Message all connected users               |
| `cd`       | Change server directory                   |
| `chat`     | Invite a user to a new chat               |
| `chats`    | List active chat sessions                 |
| `close`    | Disconnect from the server                |
| `colorz`   | Toggle multicolored chat text             |
| `exec`     | Execute a shell command                   |
| `get`      | Download files from the server            |
| `icon`     | Change your user icon                    |
| `ignore`   | Ignore a user                             |
| `info`     | Display information about a user          |
| `join`     | Join an existing chat                     |
| `kick`     | Kick or ban a user                        |
| `ls`       | List directory contents                   |
| `me`       | Post a chat action                        |
| `mkdir`    | Make a new folder on the server           |
| `msg`      | Send a private message                    |
| `news`     | Read server news                          |
| `nick`     | Change your nickname and optionally icon  |
| `part`     | Leave a chat session                      |
| `post`     | Post a news item                          |
| `put`      | Upload a file                             |
| `pwd`      | Show current server directory             |
| `quit`     | Quit the client                           |
| `savevars` | Save variables to ~/.hxvars               |
| `server`   | Connect to a server                       |
| `set`      | Set a variable                            |
| `snd`      | Configure sound options                   |
| `tracker`  | Retrieve a list of servers from a tracker |
| `version`  | Display client version                    |
| `who`      | List users in the active chat             |

Run `/help <command>` inside hx for usage details.

### Sound and Notifications
Use the `snd` command to load sound sets, enable or disable events, and specify external players.  Example:

```
/snd -S my_sounds.sndset
```

To toggle sound globally:

```
/snd -O      # enable sound
/snd -o      # disable sound
```

### Managing Transfers
Commands such as `get`, `put`, `xfers`, `xfgo` and `xfkill` handle file transfers.  The GTK interface also exposes buttons for uploads and downloads.

### Exiting
Use `/quit` to disconnect and exit.  Your variables are automatically saved.

## 4. The GTK Interface

When built with GTK, hx provides windows for chat, file browsing, user lists and task monitoring.  The "Connect" dialog prompts for server address, login and password and can save bookmarks.  Window geometry is restored from `~/.hxvars`.

GTK builds display toolbar icons and support custom icon resources via the variables `icon_files[i]` and `user_icon_files[i]`.

## 5. Further Information

Refer to the `INSTALL` file for more detailed build instructions and optional features.  Additional notes and troubleshooting tips are located under `info/`.

For the latest updates visit [http://hx.fortyoz.org/](http://hx.fortyoz.org/).

