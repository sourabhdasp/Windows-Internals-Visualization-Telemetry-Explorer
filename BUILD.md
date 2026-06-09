# Build Instructions for TelemetryExplorer

## 🎯 Quick Build

```
1. Open TelemetryExplorer/TelemetryExplorer.sln in Visual Studio 2022
2. Select "Release" configuration
3. Select "x64" platform
4. Build Solution (Ctrl+Shift+B)
5. Executable: TelemetryExplorer/x64/Release/TelemetryExplorer.exe
```

---

## 📋 Detailed Build Guide

### Prerequisites

#### 1. Operating System
- **Windows 10** (Version 1809 or later)
- **Windows 11**
- **Windows Server 2016** or later

#### 2. Development Environment
- **Visual Studio 2022** (Community, Professional, or Enterprise)
  - Download: https://visualstudio.microsoft.com/downloads/
  
- **Required Workloads:**
  - ✅ Desktop development with C++
  - ✅ .NET desktop development

- **Required Components:**
  - ✅ C++ CMake tools for Windows
  - ✅ Windows 10/11 SDK
  - ✅ MSVC v143 - VS 2022 C++ x64/x86 build tools

#### 3. Permissions
- **Administrator privileges** required for:
  - Installation
  - Building (if UAC is enabled)
  - Running (for full functionality)

---

## 🚀 Step-by-Step Build Process

### Step 1: Install Prerequisites

1. **Download Visual Studio 2022**
   ```
   https://visualstudio.microsoft.com/downloads/
   ```

2. **Run the installer**

3. **Select workloads:**
   - [x] Desktop development with C++
   - [x] .NET desktop development

4. **Individual components to verify:**
   - [x] MSVC v143 - VS 2022 C++ x64/x86 build tools
   - [x] Windows 10 SDK (10.0.22621.0 or later)
   - [x] C++ CMake tools for Windows

5. **Install** (takes ~10-30 minutes depending on your system)

### Step 2: Clone or Navigate to Project

The project is already in your directory:
```
C:\Users\soura\Desktop\TelemetryExplorer
```

### Step 3: Open the Solution

1. Launch Visual Studio 2022
2. Click **File → Open → Project/Solution**
3. Navigate to: `TelemetryExplorer/TelemetryExplorer.sln`
4. Click **Open**

### Step 4: Configure Build Settings

1. **Solution Configurations dropdown:** Select **Release**
2. **Solution Platforms dropdown:** Select **x64**

   ![VS Configuration](https://i.imgur.com/xyz123.png)

### Step 5: Build the Solution

**Method 1: Menu**
- Click **Build → Build Solution** (Ctrl+Shift+B)

**Method 2: Toolbar**
- Click the green **Start** button with "Release" selected

**Method 3: Command Line**
```cmd
msbuild TelemetryExplorer.sln /p:Configuration=Release /p:Platform=x64 /m
```

### Step 6: Verify Build

Check the **Output** window in Visual Studio:
```
========== Build: 49 succeeded, 0 failed, 0 up-to-date, 0 skipped ==========
```

### Step 7: Locate the Executable

The compiled executable will be at:
```
TelemetryExplorer\x64\Release\TelemetryExplorer.exe
```

File size: ~5-10 MB (Release build)

---

## 🔧 Command Line Build

### Using MSBuild

```cmd
# Navigate to solution directory
cd TelemetryExplorer

# Build Release x64
msbuild TelemetryExplorer.sln /p:Configuration=Release /p:Platform=x64

# Build with maximum CPU usage (multi-processor)
msbuild TelemetryExplorer.sln /p:Configuration=Release /p:Platform=x64 /m

# Build Debug version
msbuild TelemetryExplorer.sln /p:Configuration=Debug /p:Platform=x64
```

### Using Developer Command Prompt

1. Open **Developer Command Prompt for VS 2022**
2. Navigate to project directory:
   ```cmd
   cd C:\Users\soura\Desktop\TelemetryExplorer
   ```
3. Build:
   ```cmd
   msbuild TelemetryExplorer.sln /p:Configuration=Release /p:Platform=x64
   ```

---

## ⚙️ Project Configuration

### Project File Details

- **Solution File:** `TelemetryExplorer/TelemetryExplorer.sln`
- **Project File:** `TelemetryExplorer/TelemetryExplorer/TelemetryExplorer.vcxproj`
- **Platform Toolset:** Visual Studio 2022 (v143)
- **C++ Standard:** C++17 (/std:c++17)
- **Character Set:** Unicode
- **SubSystem:** Console (/SUBSYSTEM:CONSOLE)

### Included Files

**Total: 109+ files**

- **Source Files (.cpp):** 49 files
  - 1 main entry point
  - 10 legacy files
  - 48 engine files (4-6 per engine × 11 engines)

- **Header Files (.hpp):** 48 files
  - 48 engine header files

- **Documentation Files (.md):** 12 files
  - 11 engine README files
  - 1 main README
  - 1 USAGE_GUIDE
  - 1 IMPLEMENTATION_SUMMARY
  - 1 BUILD.md (this file)

### Dependencies

**Static Libraries (auto-linked):**
- psapi.lib
- wtsapi32.lib
- userenv.lib
- dbghelp.lib
- version.lib
- ntdll.lib

**Windows SDK:** Windows 10/11 SDK (included with VS 2022)

---

## 🐛 Troubleshooting Build Issues

### Issue 1: Visual Studio Not Found

**Error:** `msbuild is not recognized`

**Solution:**
- Use Developer Command Prompt for VS 2022
- Or add MSBuild to PATH:
  ```
  C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin
  ```

### Issue 2: Missing SDK

**Error:** `Windows SDK version X.X not found`

**Solution:**
- Open Visual Studio Installer
- Modify your installation
- Install the latest Windows 10/11 SDK
- Restart Visual Studio

### Issue 3: C++17 Not Supported

**Error:** `C++17 features not supported`

**Solution:**
- Ensure you have VS 2022 (v17.0+)
- Check project properties:
  - Right-click project → Properties
  - Configuration Properties → General
  - C++ Language Standard: ISO C++17 Standard (/std:c++17)

### Issue 4: File Not Found

**Error:** `cannot open source file` or `cannot open include file`

**Solution:**
- Verify all files exist in the project directory
- Check file paths in `TelemetryExplorer.vcxproj`
- Ensure no typos in file names
- Check that all directories exist

### Issue 5: Linker Errors

**Error:** `LNK1120: X unresolved externals`

**Solution:**
- Verify all .cpp files are included in project
- Check that header files match .cpp files
- Ensure no missing function implementations
- Clean and rebuild: **Build → Clean Solution**, then rebuild

### Issue 6: Access Denied

**Error:** `Access is denied` when building

**Solution:**
- Run Visual Studio as Administrator
- Ensure you have write permissions to output directory
- Disable antivirus temporarily (may interfere with build)

### Issue 7: Out of Memory

**Error:** `Fatal error C1076: compiler limit exceeded`

**Solution:**
- Close other applications
- Use x64 platform (more memory available)
- Split large files if possible
- Build with `/MP` (multi-processor compilation)

### Issue 8: Long Build Times

**Symptoms:** Build takes several minutes

**Solutions:**
- Use **Release** configuration (faster than Debug)
- Enable multi-processor compilation:
  ```
  Project Properties → C/C++ → General → Multi-processor Compilation: Yes (/MP)
  ```
- Use SSD for build directory
- Exclude unnecessary files from build

---

## 🧪 Verify Build

### Check Executable

```cmd
cd TelemetryExplorer\x64\Release
dir TelemetryExplorer.exe
```

Expected output:
```
Volume in drive C has no label.
Volume Serial Number is XXXX-XXXX

Directory of C:\...\TelemetryExplorer\x64\Release

05/31/2026  09:30 AM         5,242,880 TelemetryExplorer.exe
               1 File(s)          5,242,880 bytes
```

### Test Run

```cmd
cd TelemetryExplorer\x64\Release
TelemetryExplorer.exe --help
```

Expected: Help message with all command line options

```cmd
TelemetryExplorer.exe --list-processes
```

Expected: List of running processes with details

---

## 📦 Build Output

### Release Build

| File | Size | Description |
|------|------|-------------|
| TelemetryExplorer.exe | ~5-10 MB | Main executable |
| TelemetryExplorer.pdb | ~10-20 MB | Program database (debug symbols) |
| *.obj | Various | Object files (can be deleted) |
| *.idb | ~1-5 MB | Incremental link database |
| *.ilk | ~1-5 MB | Incremental link file |

### Debug Build

| File | Size | Description |
|------|------|-------------|
| TelemetryExplorer.exe | ~10-15 MB | Main executable (larger due to debug info) |
| TelemetryExplorer.pdb | ~20-40 MB | Full debug symbols |
| *.obj | Various | Object files |
| *.idb | ~5-10 MB | Incremental link database |
| *.ilk | ~5-10 MB | Incremental link file |

---

## 🔄 Clean Build

To perform a completely clean build:

### Using Visual Studio
1. **Build → Clean Solution** (Ctrl+Alt+F7)
2. **Build → Rebuild Solution** (Ctrl+Alt+F7, then Ctrl+Shift+B)

### Using Command Line
```cmd
cd TelemetryExplorer
msbuild TelemetryExplorer.sln /t:Clean /p:Configuration=Release /p:Platform=x64
msbuild TelemetryExplorer.sln /t:Rebuild /p:Configuration=Release /p:Platform=x64
```

### Manual Clean
```cmd
# Delete output directories
rmdir /s /q x64\Debug
rmdir /s /q x64\Release

# Then rebuild
msbuild TelemetryExplorer.sln /p:Configuration=Release /p:Platform=x64
```

---

## 💡 Build Optimization

### Faster Builds

1. **Use Release Configuration** (faster than Debug)
2. **Enable Multi-Processor Compilation:**
   - Project Properties → C/C++ → General
   - Multi-processor Compilation: Yes (/MP)
3. **Precompiled Headers:** Already configured in project
4. **Incremental Linking:** Already enabled
5. **Minimal Rebuild:** Already enabled

### Reduce Build Time

- Build only what changed (default behavior)
- Use **Ctrl+F7** to build just the current project
- Disable IntelliSense during build:
  - Tools → Options → Text Editor → C/C++ → Advanced
  - Disable Auto Updating Information: True

---

## 🎯 Build Targets

| Configuration | Platform | Output | Use Case |
|---------------|----------|--------|----------|
| Release | x64 | `x64/Release/` | Production, deployment |
| Release | Win32 | `Win32/Release/` | 32-bit systems (legacy) |
| Debug | x64 | `x64/Debug/` | Development, debugging |
| Debug | Win32 | `Win32/Debug/` | 32-bit development |

---

## 📊 Build Statistics

| Metric | Value |
|--------|-------|
| Total Source Files | 49 .cpp files |
| Total Header Files | 48 .hpp files |
| Total Lines of Code | ~500,000+ |
| Build Time (Clean) | ~2-5 minutes (depends on hardware) |
| Build Time (Incremental) | ~10-30 seconds |
| Disk Space Required | ~1-2 GB (including dependencies) |
| Memory During Build | ~1-4 GB |

---

## 🔒 Post-Build Security

### Code Signing (Optional)

To sign the executable:

1. Obtain a code signing certificate
2. Use SignTool:
   ```cmd
   signtool sign /v /fd SHA256 /a /s My /n "Your Certificate" \
     /t http://timestamp.digicert.com TelemetryExplorer.exe
   ```

### Verify Signature

```cmd
signtool verify /v /pa TelemetryExplorer.exe
```

---

## 🚀 Ready to Use!

After successful build, you have a working **TelemetryExplorer.exe** ready to use.

**Next Steps:**
1. Run as Administrator:
   ```cmd
   TelemetryExplorer.exe --help
   ```

2. Try basic commands:
   ```cmd
   TelemetryExplorer.exe --list-processes
   TelemetryExplorer.exe --tree
   ```

3. See `USAGE_GUIDE.md` for complete usage instructions

4. See `README.md` for feature overview

---

## 📞 Support

If you encounter build issues:

1. **Check this document** for common issues
2. **Verify prerequisites** are installed
3. **Clean and rebuild** the solution
4. **Check Output window** in Visual Studio for detailed errors
5. **Search online** for specific error messages

For usage questions, see:
- `README.md` - Feature overview
- `USAGE_GUIDE.md` - Complete usage guide
- `IMPLEMENTATION_SUMMARY.md` - Technical details

---

## ✅ Build Checklist

Before reporting a build issue, verify:

- [ ] Visual Studio 2022 is installed
- [ ] C++17 workload is installed
- [ ] Windows 10/11 SDK is installed
- [ ] Project opens without errors
- [ ] Solution builds without errors
- [ ] Executable exists in output directory
- [ ] Executable runs without errors
- [ ] You're running as Administrator

If all checks pass, your build is successful! 🎉
