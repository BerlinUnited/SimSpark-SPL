importBundle "bundle/filesystemstd"
#importBundle "bundle/filesystemzip"
#importBundle "bundle/filesystemrar"

# mount a standard file system
create "kerosin/FileServer", "/sys/server/file"
cd "sys/server/file"
mount "FileSystemSTD", "data/"

def testSoundSystem
	importBundle "bundle/soundsystemfmod"
	importBundle "bundle/soundsystembass"
	print "SoundServer Test\n"
	print "Creating SoundServer\n"
	create "kerosin/SoundServer", "/sys/server/sound"

	print "  Adding SoundSystem\n"
	cd "/sys/server/sound"
	init "SoundSystemBASS"
	#init "SoundSystemFMOD"
	playEffect "shotgun.wav"
	playModule "BTGATESF.XM"
	#playModule 'F:\Audio\Modules\Donna.xm'
	#playStream "sinch.mp3"
end

def testFileServer
	print "FileServer Test\n"
	print "  Adding FileSystemZIP\n"
	so "/sys/server/file"
	mount "FileSystemZIP", "./testfile.zip"
	print "  Does the file testit.cpp exist?\n"
	exist "testit.cpp"
end

testSoundSystem
