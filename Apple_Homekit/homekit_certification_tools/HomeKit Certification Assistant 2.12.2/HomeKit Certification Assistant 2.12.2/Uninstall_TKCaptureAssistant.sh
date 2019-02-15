#!/usr/bin/env bash

sudo launchctl unload /Library/LaunchDaemons/com.apple.TKCaptureAssistant.plist
sudo rm /Library/LaunchDaemons/com.apple.TKCaptureAssistant.plist
sudo rm /Library/PrivilegedHelperTools/com.apple.TKCaptureAssistant
sudo security -q authorizationdb remove "com.apple.TKCaptureAssistant"

