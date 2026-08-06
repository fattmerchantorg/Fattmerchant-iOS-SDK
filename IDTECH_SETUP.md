# IDTech Framework Setup Guide

## Issue: `IdTechBundleRequired` Error

If you see the error `IdTechBundleRequired` in the device status, it means the IDTech binary framework cannot find its required resource bundle (`IDTech.bundle`).

## Root Cause

Binary frameworks (`.xcframework`) in Swift Package Manager cannot access resources from the SPM module's bundle at runtime. They look for resources in the **main application's bundle**.

## Solution

Add a build phase script that automatically copies the `IDTech.bundle` to your app bundle during each build.

### Setup Instructions

1. **Open Build Phases**
   - In Xcode, select your app target
   - Go to "Build Phases" tab

2. **Add Run Script Phase**
   - Click "+" → "New Run Script Phase"
   - **Important**: Drag it to run BEFORE "Copy Bundle Resources"
   - Paste this script:

```bash
# Copy IDTech.bundle from SPM package
SCRIPT_PATH="${BUILD_ROOT}/../../SourcePackages/checkouts/Fattmerchant-iOS-SDK/Scripts/copy_idtech_bundle.sh"

if [ -f "$SCRIPT_PATH" ]; then
    bash "$SCRIPT_PATH"
else
    echo "⚠️  Warning: IDTech bundle copy script not found at $SCRIPT_PATH"
    echo "   The script will be available after the first SPM dependency resolution"
fi
```

3. **Clean and Build**
   - Product → Clean Build Folder (⇧⌘K)
   - Build your project
   - Check the build log for "✨ IDTech.bundle setup complete!"

## Verification

After adding the script, the error should be resolved and you'll see proper device information instead of the `IdTechBundleRequired` error.

## Why This Is Necessary

Unlike regular Swift frameworks, binary frameworks (pre-compiled `.xcframework` files) cannot dynamically load resources from Swift Package Manager's module bundles. The IDTech framework specifically calls `[IDTUtility frameworkBundle]` to locate its resources, which searches only in the main application bundle at runtime.

This is a limitation of how Apple's binary frameworks work with SPM, not a bug in the Fattmerchant SDK.
