print "start inputtest.rb\n"

####
# Input Server
#
inputServer = new ('kerosin/InputServer', '/sys/server/input');
# inputServer.init ('InputSystemSDL');
# load german keyboard layout
inputServer.importScanCodeMapping ('sys/script/german.scan.rb');

print "end inputtest.rb\n"
