#!/bin/bash

# IDTech Bundle Copy Script
# This script automatically copies IDTech.bundle from the SPM package to your app bundle
# 
# Usage: Add this as a "Run Script" build phase in your Xcode project
# Make sure to run it BEFORE "Copy Bundle Resources" phase

set -e

echo "🔍 Searching for IDTech.bundle in SPM dependencies..."

# Find the bundle in SPM checkouts
BUNDLE_NAME="IDTech.bundle"
SPM_CHECKOUTS="${BUILD_ROOT}/../../SourcePackages/checkouts"

# Search in common SPM locations
BUNDLE_PATH=""

# Location 1: SPM checkouts (most common)
if [ -d "$SPM_CHECKOUTS/Fattmerchant-iOS-SDK/Sources/IDTechResources/$BUNDLE_NAME" ]; then
    BUNDLE_PATH="$SPM_CHECKOUTS/Fattmerchant-iOS-SDK/Sources/IDTechResources/$BUNDLE_NAME"
# Location 2: Alternative SPM structure
elif [ -d "$SPM_CHECKOUTS/fattmerchant-ios-sdk/Sources/IDTechResources/$BUNDLE_NAME" ]; then
    BUNDLE_PATH="$SPM_CHECKOUTS/fattmerchant-ios-sdk/Sources/IDTechResources/$BUNDLE_NAME"
# Location 3: Search in derived data
else
    DERIVED_DATA="${BUILD_ROOT}/../.."
    FOUND_PATH=$(find "$DERIVED_DATA" -name "$BUNDLE_NAME" -path "*/SourcePackages/checkouts/*" -type d 2>/dev/null | head -n 1)
    if [ -n "$FOUND_PATH" ]; then
        BUNDLE_PATH="$FOUND_PATH"
    fi
fi

# Check if bundle was found
if [ -z "$BUNDLE_PATH" ] || [ ! -d "$BUNDLE_PATH" ]; then
    echo "❌ Error: IDTech.bundle not found in SPM dependencies"
    echo "   Make sure Fattmerchant SDK is properly added to your project"
    echo "   Searched in: $SPM_CHECKOUTS"
    exit 1
fi

echo "✅ Found IDTech.bundle at: $BUNDLE_PATH"

# Destination in app bundle
DEST_PATH="${BUILT_PRODUCTS_DIR}/${PRODUCT_NAME}.app/$BUNDLE_NAME"

# Remove existing bundle if present
if [ -d "$DEST_PATH" ]; then
    echo "🗑️  Removing existing bundle..."
    rm -rf "$DEST_PATH"
fi

# Copy the bundle
echo "📦 Copying IDTech.bundle to app bundle..."
cp -R "$BUNDLE_PATH" "$DEST_PATH"

# Verify the copy
if [ -d "$DEST_PATH" ]; then
    echo "✅ Successfully copied IDTech.bundle to app bundle"
    echo "   Destination: $DEST_PATH"
    
    # List bundle contents for verification
    BUNDLE_FILES=$(find "$DEST_PATH" -type f | wc -l | tr -d ' ')
    echo "   Bundle contains $BUNDLE_FILES files"
else
    echo "❌ Error: Failed to copy IDTech.bundle"
    exit 1
fi

echo "✨ IDTech.bundle setup complete!"
