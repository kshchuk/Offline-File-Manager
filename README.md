# Offline File Manager

Offline File Manager is a Qt-based application designed to manage files and directories on external media. It retains information about these files and directories even after the media is disconnected, providing a read-only view. The information remains available even after a software restart.

## Features

### Basic Features

- **Offline Information Storage:** Stores information about files and directories on external media such as flash drives, external drives, and CD/DVDs. This information remains available in read-only mode even after the media is disconnected.
- **Persistent Data:** The application retains the saved information even after a software restart.
- **Custom Metadata:** Users can add custom metadata to files/directories.
- **System File Viewer Integration:** When the media is connected, the application can open the system file viewer for a given file or directory.

### Additional Features

- **Virtual Directories:** Users can create virtual directories that combine data from different sources.
- **Metadata Search:** The application supports searching by metadata, including file/directory name, type, creation/editing time, hash/checksum, and custom metadata.
- **Duplicate Information:** The application provides information about the number of exact copies of a file or directory on known media.
- **Data Synchronization:** The application can generate and run commands to copy or synchronize data between different media, using tool robocopy
- **Cloud Storage Support:** The application supports cloud storage service Google Drive
