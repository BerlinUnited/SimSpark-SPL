#
# soccerbindings.rb sets up a mapping from input constants used by the
# inputserver to soccer command constants
#
inputServer = get('/sys/server/input');

if (inputServer == nil)
  print "(soccerbindings.rb) no InputServer present\n";
else
  print "(soccerbindings.rb) setting up bindings\n";

  inputServer.bindCommand('k',      Command.KickOff);
end
