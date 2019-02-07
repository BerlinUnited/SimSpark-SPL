importBundle "filesystemstd"
importBundle "sexpparser"
# importBundle "survival"

# mount a standard file system
fileServer = get('/sys/server/file');
fileServer.mount('FileSystemSTD', 'data/');

def testSoundSystem
  importBundle "soundsystemfmod"
  #importBundle "soundsystembass"
  print "SoundServer Test\n"
  print "Creating SoundServer\n"
  soundServer = new('kerosin/SoundServer', '/sys/server/sound');

  print "  Adding SoundSystem\n"
  soundServer.init('SoundSystemFMOD');
  #soundServer.init('SoundSystemBASS');
  #playModule "BTGATESF.XM"
  #soundServer.playStream('sinch.mp3');
  soundServer.playStream('monkey.wav');
end

testSoundSystem
