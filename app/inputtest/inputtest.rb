importBundle "bundle/filesystemstd"
importBundle "bundle/inputsdl"

# mount a standard file system
create "kerosin/FileServer", "/sys/server/file"
cd "/sys/server/file"
mount "FileSystemSTD", "data/"

cd "/sys/server/log"
addStream "zeitgeist.log", "eAll"
addStream ":cout", "eAll"

create "kerosin/InputServer", "/sys/server/input"
cd "/sys/server/input"
init "InputSystemSDL"
createDevice 'Keyboard'
createDevice 'Mouse'

