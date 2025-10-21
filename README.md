# 🌐 HTTP Protocol Project

A simple **HTTP Client and Server** implemented in **C**.
This project demonstrates how to:

- 📥 Build a basic HTTP client to download files from URLs.
- 📤 Build a basic HTTP server to serve files from directories.
- 📂 Organize downloaded files by type in specific folders.

---

## ✨ Features

### 🖥️ Client
- Parse HTTP URLs (http://host:port/path/file.ext).
- Download files and save them in a structured folder system:
  - music/ → .mp3, .wav, .flac
  - pictures/ → .jpg, .png, .gif, etc.
  - videos/ → .mp4, .mkv, .avi, etc.
  - texts/ → .txt, .md, .csv, .log
  - archives/ → .zip, .tar, .rar, .gz
  - code/ → .c, .cpp, .h, .py, .js, .java
  - media/ → .webm, .ogg
  - others/ → unknown file types
- Handles HTTP responses properly:
  - Prints the HTTP header received from the server in the terminal.
  - Only saves the file if the response is HTTP 200 OK.
- Maintains folder structure automatically and avoids committing downloaded files to Git.
- Supports both IPv4 and IPv6 addresses.
- Properly handles connection errors, invalid URLs, and socket failures.

### 🖧 Server
- Serves files over HTTP from the files/ directory.
- Supports both direct file requests and directory requests:
  - If a URL points to a folder, serves index.html if it exists.
  - If index.html does not exist, the server generates dynamic indexes listing all files in the folder (via generate_indexes.py).
- Prints each client request in the terminal, including client IP, port, method, and requested path.
- Only supports the GET method; other HTTP methods return 405 Method Not Allowed.
- Handles concurrent connections sequentially (one at a time).
- Automatically updates index.html for subfolders every time a client accesses the server.
- Compatible with common web browsers and HTTP clients.

---

## 🗂️ Folder Structure
```
project-root/
├── client/
│   ├── files/
│   │   ├── music/README.md
│   │   ├── pictures/README.md
│   │   ├── texts/README.md
│   │   ├── videos/README.md
│   │   ├── archives/README.md
│   │   ├── code/README.md
│   │   ├── media/README.md
│   │   └── others/README.md
│   ├── include/
│   │   ├── file_utils.h
│   │   └── url.h
│   └── src/
│       ├── client.c
│       ├── file_utils.c
│       ├── makefile
│       └── url.c
│
├── server/
│   ├── files/
│   │   ├── music/README.md
│   │   ├── pictures/README.md
│   │   ├── texts/README.md
│   │   ├── videos/README.md
│   │   ├── archives/README.md
│   │   ├── code/README.md
│   │   ├── media/README.md
│   │   └── others/README.md
│   └── src/
│       ├── generate_indexes.py
│       ├── makefile
│       └── server.c
├── .gitignore
├── LICENSE
└── README.md
```

---

## 🛠️ How to Compile
```bash
# Compile the client
cd client/src
make

# Compile the server
cd ../../server/src
make
```

---

# ▶️ How to Run
```bash
# Start the server
./server

# Run the client
./client http://host:port/path_to_file
```

Downloaded files will be saved in the appropriate folder under client/files/.

---

## 📝 License
This project is open-source under the MIT License.
