# HTTP Protocol Project

A simple **HTTP Client and Server** implemented in **C**.
This project demonstrates how to:

- Build a basic HTTP client to download files from URLs.
- Build a basic HTTP server to serve files from directories.
- Organize downloaded files by type in specific folders.

---

## Features

### Client
- Parse HTTP URLs (`http://host:port/path/file.ext`).
- Download files and save them in a structured folder system:
  - `music/` → `.mp3`, `.wav`, `.flac`
  - `pictures/` → `.jpg`, `.png`, `.gif`, etc.
  - `videos/` → `.mp4`, `.mkv`, `.avi`, etc.
  - `texts/` → `.txt`, `.md`, `.csv`, `.log`
  - `archives/` → `.zip`, `.tar`, `.rar`, `.gz`
  - `code/` → `.c`, `.cpp`, `.h`, `.py`, `.js`, `.java`
  - `media/` → `.webm`, `.ogg`
  - `others/` → unknown file types
- Skips HTTP headers and only writes the file content.
- Maintains folder structure and avoids versioning downloaded files in Git.

### Server
- Serve files from a given directory over HTTP.
- Supports default `index.html` or directory listing if no index exists.
- Compatible with Python's simple HTTP server or custom C server.

---

## Folder Structure
```
project-root/
├── client/
│ ├── files/
│ │ ├── music/README.md
│ │ ├── pictures/README.md
│ │ ├── texts/README.md
│ │ ├── videos/README.md
│ │ ├── archives/README.md
│ │ ├── code/README.md
│ │ ├── media/README.md
│ │ └── others/README.md
│ ├── include/
│ │ ├── file_utils.h
│ │ └── url.h
│ └── src/
│   ├── client.c
│   ├── file_utils.c
│   ├── makefile
│   └── url.c
│
├── server/
│ ├── files/
│ │ ├── music/README.md
│ │ ├── pictures/README.md
│ │ ├── texts/README.md
│ │ ├── videos/README.md
│ │ ├── archives/README.md
│ │ ├── code/README.md
│ │ ├── media/README.md
│ │ └── others/README.md
│ ├── include/
│ │ ├──
│ │ └──
│ └── src/
│   ├── server.c
│   ├──
│   ├── makefile
│   └──
├── LICENSE
└── README.md
```

---

## How to Compile
```bash
# Compile the client
cd client/src
make

# Compile the server
cd ../../server/src
make
```

---

# How to Run
```bash
# Start the server

# Run the client
./client http://host:port/path_to_file # Download a file from the server
```

Downloaded files will be saved in the appropriate folder under client/files/.

---

## License
This project is open-source under the MIT License.
