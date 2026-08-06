# Optional Frameworks

This directory contains optional frameworks that are **not** included in the standard SDK distribution via CocoaPods or Swift Package Manager.

## CloudCommerce.xcframework

**Purpose**: Required for Tap to Pay on iPhone functionality. Also provides advanced transaction management features including SOAP-based transaction history, receipt management, and extended transaction details.

**Required for**:
- ✅ Tap to Pay on iPhone (iOS 17.4+)

**Optional for**:
- SOAP transaction history retrieval
- Extended transaction details and statistics
- Advanced receipt management features

**NOT required for**:
- Basic payment processing
- Mobile reader transactions (swipe/chip/contactless readers)
- Payment tokenization

### Download

CloudCommerce.xcframework is stored using **Git LFS** (Large File Storage) due to its size.

#### For Users WITH Git LFS Enabled

If you have Git LFS installed and enabled, the framework will be downloaded automatically when you clone or pull the repository.

#### For Users WITHOUT Git LFS

If you don't have Git LFS enabled, you'll see a small pointer file instead of the actual framework. You have two options:

**Option 1: Enable Git LFS (Recommended if you need Tap to Pay)**

```bash
# Install Git LFS (if not already installed)
brew install git-lfs  # macOS
# or download from https://git-lfs.github.com/

# Enable Git LFS in this repository
git lfs install

# Pull the actual framework files
git lfs pull
```

### Installation

#### Swift Package Manager

1. Place the downloaded `CloudCommerce.xcframework` in your project directory
2. Drag `CloudCommerce.xcframework` into your Xcode project
3. In your target's **General** tab, add `CloudCommerce.xcframework` to **Frameworks, Libraries, and Embedded Content**
4. Set the embed option to **Embed & Sign**

#### CocoaPods

1. Place the downloaded `CloudCommerce.xcframework` in your project's frameworks folder
2. Add the following to your Podfile:

```ruby
post_install do |installer|
  installer.pods_project.targets.each do |target|
    target.build_configurations.each do |config|
      config.build_settings['FRAMEWORK_SEARCH_PATHS'] ||= ['$(inherited)']
      config.build_settings['FRAMEWORK_SEARCH_PATHS'] << '$(PROJECT_DIR)/Frameworks'
    end
  end
end
```

3. Ensure the framework is added to your target's **Frameworks, Libraries, and Embedded Content**

### License

CloudCommerce is a proprietary framework provided by Stax. By using this framework, you agree to the Stax terms of service.
