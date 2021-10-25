#!/usr/bin/osascript
tell application "Safari"
    activate
    tell application "System Events"
        keystroke "f" using {command down, control down}
    end tell
end tell