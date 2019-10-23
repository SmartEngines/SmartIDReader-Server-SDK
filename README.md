# SmartIDReader-Server-SDK - Demo version

This is a DEMO version of Smart IDReader Server SDK by Smart Engines which demonstrates the usage of Smart IDReader library/SDK in ypur Python or PHP projects without actually providing any recognition functionality. Instead, it outputs fake results for document search, field segmentation and recognition.

If you'd like to obtain a trial or full version of Smart IDReader please contact us via:

support@smartengines.com
http://smartengines.com/contacts
http://smartengines.ru/contacts
Test and trial/full versions are only different in static library + configuration files so you wouldn't have to rewrite any code after you're finished integrating Smart IDReader SDK into your application.

## Smart IDReader overview
Smart IDReader technology allows you to recognize identity and property rights documents while using video/photo cameras and scanners in mobile, desktop, server and terminal solutions. With this tecnhology you only need to present the document to the camera to let Smart IDReader recognize all required data in 1-3 seconds and then fill them in any file, form or a work sheet.

## Key features:

Real time document recognition in video stream on mobile devices
Recognition of documents in various lighting conditions
White label license
Security: only device RAM is used, no personal data is being copied or sent over the internet (e.g. for processing on servers)
Supported platforms: iOS, Android, Windows, Linux, MacOS, Solaris and others
Supported programming languages: C++, C, C#, Objective-C, Java, PHP 5/7, Python 2/3 and others
Supported architectures: armv7-v8, aarch64, x86, x86_64, SPARC, E2K and others


## Build wrapper:

To build wrapper for your version of interpreter use autobild scripts from /samples/<target_language> with following arguments: 

### For Python:
```
bash build_python.sh<PATH_TO_SMARTIDENGINE_LIBRARY_DIRECTORY> <VERSION_OF_PYTHON>

e.g. bash build_python.sh ../../bin 3 # Python
```

### For PHP:
```
bash build_php.sh <PATH_TO_SMARTIDENGINE_LIBRARY_DIRECTORY> <PATH_TO_YOUR_php-config_BINARY>

e.g bash build_php.sh ../../bin /usr/bin/php56-config

```
