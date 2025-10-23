Two Payment Device SDK variants are now supplied in the iOS release. 
ChipDnaMobile/Library which contains static libraries and header files.
ChipDnaMobile/Framework which contains the XCFrameworks.
Important: Do not include both variants in the same project. Choose the one that best fits your integration approach:

Library: A static library bundle supporting Architectures: x86_84 for Simulator, as well as arm64 for iOS Devices.

Framework: A framework bundle for Apple Silicon, supporting x86_64 and arm64 simulators, as well as arm64 iOS devices.

